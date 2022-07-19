/*======================================================================
Имя файла:          core_VlvDrvCtrl.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
модуль управления задвижкой
======================================================================*/

#include "g_Core.h"

 void GetActiveControls(TCoreVlvDrvCtrl *);	// Получаем данные от куда обрабатываем команды
 void MpuControl(TCoreVlvDrvCtrl *);		// Обрабатываем местны команы
 void TeleControl(TCoreVlvDrvCtrl *);		// обрабатываем команды ТУ/Модбас
 void UnitControl(TCoreVlvDrvCtrl *);		// Выбераем действие в зависимости от полученной корманлды
 void DriveStop(TCoreVlvDrvCtrl *);			// Действия при команде стоп
 void DriveClose(TCoreVlvDrvCtrl *);		// ... при закрыть
 void DriveOpen(TCoreVlvDrvCtrl *);			// ... при открыть
 void UpdateComm(TCoreVlvDrvCtrl *);		// Обработка внутренней команды при обработке реверса
static   void ReverseDrive(TCoreVlvDrvCtrl *);		// Действия при реверсеы

Bool Flag = False;

void Core_ValveDriveInit(TCoreVlvDrvCtrl *p)
{
	p->Status 			= ToPtr(&g_Core.Status);
	p->ControlWord 			= &g_Ram.Comands.ControlWord;
	p->MuDuSetup			= &g_Ram.UserParam.MuDuSetup;
	p->DuSource			= &g_Ram.UserParam.DuSource;
	p->ReverseType			= &g_Ram.HideParam.ReverseType;
	p->Mpu.Enable			= true;
	p->Mpu.BtnKey			= &g_Ram.HideParam.CmdButton;
	p->Mpu.CancelFlag		= false;
	p->Tu.Enable			= true;
	p->Tu.LocalFlag			= false;
	p->Tu.State			= &g_Ram.HideParam.TuState;
	p->Tu.Ready			= true;
	p->Tu.LockSeal			= &g_Ram.UserParam.TuLockSeal;
	p->Valve.PosRegEnable 	        = true;
	p->Valve.BreakFlag		= false;
	p->Valve.Position		= POS_UNDEF;
	p->Valve.BreakDelta		= 0;
	p->Valve.BreakMode 		= &g_Ram.UserParam.BreakMode;
	p->Valve.CalibStates	        = &g_Ram.Status.CalibState;
	p->Valve.CalibFullStep          = &g_Ram.HideParam.FullStep;
	p->EvLog.Source			= 0;
	p->EvLog.Value			= 0;
	p->Command			= vcwNone;
        p->DacControl.Enable            = true;
        p->DacControl.PrecentData       = 9999;
        p->DacControl.CancelFlag        = false;
	p->MuDuInput			= 0;
	p->ActiveControls		= 0;
	p->StartDelay			= 0;
	p->IgnorComFlag			= 0;
	p->StartControl			= &StartPowerControl;
	p->StopControl			= &StopPowerControl;

}

void Core_ValveDriveStop(TCoreVlvDrvCtrl *p)
{
	p->Valve.BreakFlag = False;

	if (p->Status->bit.Fault || !(*p->Tu.LockSeal)) p->Tu.Ready = False;

	p->Valve.Position = POS_UNDEF;

	p->IgnorComFlag = 0;

	p->StopControl();
}

void Core_ValveDriveUpdate(TCoreVlvDrvCtrl *p)
{
	
        GetActiveControls(p);	        // Получение данных по активированному типу управления (местное/дистанция)
	TeleControl(p);			// Подача команд дистанционного управления
	MpuControl(p);			// Подача команд местного управления
        if (p->ActiveControls & CMD_SRC_ANALOG)
        Core_ValveDriveMove(&g_Core.VlvDrvCtrl, g_Ram.UserParam.SetPosition);
        
        UnitControl(p);			// Дейстаия в зависимости от команды (открыть/закрыть/стоп)
        
}

void Core_ValveDriveMove(TCoreVlvDrvCtrl *p, Uns Percent)
{
    TValveCmd MoveControl = vcwNone;
    LgInt Position;
     LgInt Positiontmp;
      LgInt Positiontmp2;
      LgInt Positiontmp3;
    
    p->DacControl.Enable = p->Valve.PosRegEnable && g_Ram.Status.CalibState == csCalib && g_Ram.Status.Status.bit.Stop && !g_Ram.Status.Status.bit.Fault;
      
      if (!p->DacControl.Enable) return;
    
   // if (!p->Valve.PosRegEnable || g_Ram.Status.CalibState != csCalib || !g_Ram.Status.Status.bit.Stop ) return;
    
    Positiontmp =  g_Peref.Position.FullStep * (LgUns)Percent;
    Positiontmp2 = Positiontmp >>17;
    Positiontmp3 = Positiontmp2 * 131Ul;
    Position = (LgInt)Positiontmp3;
   // Position = (LgInt)((g_Peref.Position.FullStep * (LgUns)Percent * 131Ul)>>17);
    
    if (Position< (g_Peref.Position.LinePos - POS_ERR)) MoveControl = vcwClose;
    if (Position> (g_Peref.Position.LinePos + POS_ERR)) MoveControl = vcwOpen;
    
    if (MoveControl != vcwNone)
    {
      p->Valve.BreakFlag = False;
      p->Valve.Position = Position;
      p->EvLog.Value = CMD_MOVE;
      p->StartControl(MoveControl);
        
    }
}

//
 void GetActiveControls(TCoreVlvDrvCtrl *p)
{
		Uns  DigState = 0;

		p->ActiveControls = 0;

		switch(*p->MuDuSetup)
		{
			case mdDac:    p->Status->bit.MuDu = 0; Flag = True; break;
			case mdSelect:
						{
							p->Status->bit.MuDu = p->MuDuInput;
							Flag = false;
							break;
						}
			case mdMuOnly: p->Status->bit.MuDu = 1; Flag = false;  break;
			case mdDuOnly: p->Status->bit.MuDu = 0; Flag = false;  break;
			default:       return;
		}

		if (!Flag && p->Status->bit.MuDu)
		{
			p->ActiveControls |= (CMD_SRC_PDU|CMD_SRC_MPU);
			if (p->Tu.LocalFlag) p->ActiveControls |= CMD_SRC_DIGITAL;
		}

		if (!Flag && !p->Status->bit.MuDu)
		{
			DigState = p->Tu.LocalFlag ? 0 : CMD_SRC_DIGITAL;
			switch(*p->DuSource)
			{
				case mdsAll:     p->ActiveControls |= (DigState|CMD_SRC_SERIAL); break;
				case mdsDigital: p->ActiveControls |= DigState; break;
				case mdsSerial:  p->ActiveControls |= CMD_SRC_SERIAL; break;
			}
		}
                if (Flag)
                {
                    p->ActiveControls |= CMD_SRC_ANALOG;
                }
}

// обработка команд с МПУ
 void MpuControl(TCoreVlvDrvCtrl *p)
{
                        Uns Key = 0;
			Uns  Active = 0;

		p->Mpu.Enable = p->Status->bit.MuDu ;

		if(!p->Mpu.Enable) return;				// выключено выходим

		if (*p->Mpu.BtnKey)						// Пришла команда с ручек
		{
			Key = *p->Mpu.BtnKey;				// Запомнили пришедшую команду
			Active = (p->ActiveControls & CMD_SRC_MPU); // Запомнили активирован ли режим мпу
			p->EvLog.Source = CMD_SRC_MPU;				// Источник команды для журнала
			*p->Mpu.BtnKey = 0;					// Сбросили команду
		}

		switch (Key)							// Обработка пришедшей команды
		{
			case KEY_STOP:						// если стоп
				*p->ControlWord = vcwStop;		// Подаем команду на остановку, вне зависимости от активации режима мпу
				break;
			case KEY_CLOSE:
				if (Active) *p->ControlWord = vcwClose;	// Если редим мпу активирован то выполяем команду
				else p->Mpu.CancelFlag = True;			// Если режим мпу не активен то подаем флаг отмены команды
				break;
			case KEY_OPEN:
				if (Active) *p->ControlWord = vcwOpen;
				else p->Mpu.CancelFlag = True;
				break;
		}
}

 void TeleControl(TCoreVlvDrvCtrl *p)
{
	TValveCmd TuControl = vcwNone;
	Bool Ready;

	if (!p->Tu.Enable) return;

	if (!(p->ActiveControls & CMD_SRC_DIGITAL))
	{
		*p->Tu.State &= ~(TU_STOP_OPEN|TU_STOP_CLOSE|TU_CLOSE|TU_OPEN);
		p->Tu.Ready = True;
		return;
	}

	Ready = p->Status->bit.Stop && p->Tu.Ready;

	switch(*p->Tu.State & (TU_CLOSE|TU_OPEN))
	{
		case TU_CLOSE: if (Ready) TuControl = vcwClose; break;
		case TU_OPEN:	if (Ready) TuControl = vcwOpen;  break;
		case (TU_CLOSE|TU_OPEN):  TuControl = vcwStop;  break;
		default:
			p->Tu.Ready = True;
	}


	if (*p->Tu.State & (TU_STOP_OPEN | TU_STOP_CLOSE)) TuControl = vcwStop;


	if (TuControl != vcwNone)
	{
		*p->ControlWord = TuControl;
		p->EvLog.Source = CMD_SRC_DIGITAL;
	}
	*p->Tu.State &= ~(TU_STOP_OPEN|TU_STOP_CLOSE|TU_CLOSE|TU_OPEN);
}

 void UnitControl(TCoreVlvDrvCtrl *p)
{
		UpdateComm(p);

		if (p->StartDelay > 0) p->StartDelay--;
		else if (*p->ControlWord)
		{
			switch(*p->ControlWord)
			{
				case vcwStop:      DriveStop(p);  	break;
				case vcwClose:
				case vcwTestClose: DriveClose(p); 	break;
				case vcwOpen:
				case vcwTestOpen:  DriveOpen(p);  	break;
				//case vcwDemo:        				break;
				//case vcwTestEng:
				default: break;//p->StartControl(*p->ControlWord);
			}
			*p->ControlWord = vcwNone;
		}
}

 void DriveStop(TCoreVlvDrvCtrl *p)
{
	Core_ValveDriveStop(p);
	p->EvLog.Value = CMD_STOP;
}

 void DriveClose(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Closed && (*p->ControlWord != vcwTestClose)) // SVS
	{
		p->IgnorComFlag = 1;										// Если уже закрыто то выставляем флаг игнорирования команды
	 	return;
	}
	if(p->Status->bit.Closing) return;								// Если идет закрытие то уходим
	if (p->Status->bit.Opening) {/*ReverseDrive(p);*/ return;}

	p->Valve.BreakFlag = False;										//
	FreeRun = (*p->ControlWord == vcwTestClose) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);		// Режим свободного движения - Если тестовое открытие или не откалиброванно
	if (FreeRun) p->Valve.Position = POS_UNDEF;						//
	else if (!((*p->Valve.BreakMode == vtClose) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = 0;				// Если не работаем в режиме уплотнения - целевое положение 0
	}
	else															// Если работаем в режиме уплотнения
	{
		p->Valve.BreakFlag = True;									// Выставили флаг работы в уплотнении
		p->Valve.Position  = -(LgInt)p->Valve.BreakDelta;			// Сместили целевое положение на допустимый ход выходного звена при уплотнении
	}
	p->EvLog.Value = CMD_CLOSE;	// Значение команды для журнала

	p->StartControl(*p->ControlWord); // Подали команду управления для начала работы
}

 void DriveOpen(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Opened && (*p->ControlWord != vcwTestOpen)) // SVS
	{
		p->IgnorComFlag = 1;
	 	return;
	}
	if (p->Status->bit.Opening) return;
	if (p->Status->bit.Closing) {/*ReverseDrive(p);*/ return;}

	p->Valve.BreakFlag = False;
	FreeRun = (*p->ControlWord == vcwTestOpen) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);
	if (FreeRun) p->Valve.Position = POS_UNDEF;
	else if (!((*p->Valve.BreakMode == vtOpen) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = (LgInt)(*p->Valve.CalibFullStep);
	}
	else
	{
		p->Valve.BreakFlag = True;
		p->Valve.Position  = (LgInt)(*p->Valve.CalibFullStep) + (LgInt)p->Valve.BreakDelta;
	}
	p->EvLog.Value = CMD_OPEN;

	p->StartControl(*p->ControlWord);
}

 void UpdateComm(TCoreVlvDrvCtrl *p)		// Функция автоматической подачи команды при реверсе ()
{
	if (p->Command != vcwNone)					// Если пришла внутренняя
	{
		if (p->Command == vcwStop) *p->ControlWord = vcwStop;	// Если команда для реверса Стоп, подаем нормальную команду стоп
		else if (!p->Status->bit.Stop) return;					// Ждем пока остановимся
		else *p->ControlWord = p->Command;						// Подаем команду в противоположном направленииы
		p->Command = vcwNone;									// Сбрасываем внутреннюю команду
	}
}

void ReverseDrive(TCoreVlvDrvCtrl *p)				// Функция Реверса
{
	switch (*p->ReverseType)					// В зависимости от типа реверса
	{
		case rvtStop:  p->Command = vcwStop; break;	// Стоп при реверсе
		case rvtAuto: {
						p->Command = *p->ControlWord; Core_ValveDriveStop(p);	// Через стоп поехали в другую сторону
						break;
					  }
		case rvtNone: *p->ControlWord = vcwNone; break; // Игнорирование команды управления
	}
}




