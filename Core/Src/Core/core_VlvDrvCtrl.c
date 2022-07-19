/*======================================================================
��� �����:          core_VlvDrvCtrl.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ���������
======================================================================*/

#include "g_Core.h"

 void GetActiveControls(TCoreVlvDrvCtrl *);	// �������� ������ �� ���� ������������ �������
 void MpuControl(TCoreVlvDrvCtrl *);		// ������������ ������ ������
 void TeleControl(TCoreVlvDrvCtrl *);		// ������������ ������� ��/������
 void UnitControl(TCoreVlvDrvCtrl *);		// �������� �������� � ����������� �� ���������� ���������
 void DriveStop(TCoreVlvDrvCtrl *);			// �������� ��� ������� ����
 void DriveClose(TCoreVlvDrvCtrl *);		// ... ��� �������
 void DriveOpen(TCoreVlvDrvCtrl *);			// ... ��� �������
 void UpdateComm(TCoreVlvDrvCtrl *);		// ��������� ���������� ������� ��� ��������� �������
static   void ReverseDrive(TCoreVlvDrvCtrl *);		// �������� ��� ��������

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
	
        GetActiveControls(p);	        // ��������� ������ �� ��������������� ���� ���������� (�������/���������)
	TeleControl(p);			// ������ ������ �������������� ����������
	MpuControl(p);			// ������ ������ �������� ����������
        if (p->ActiveControls & CMD_SRC_ANALOG)
        Core_ValveDriveMove(&g_Core.VlvDrvCtrl, g_Ram.UserParam.SetPosition);
        
        UnitControl(p);			// �������� � ����������� �� ������� (�������/�������/����)
        
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

// ��������� ������ � ���
 void MpuControl(TCoreVlvDrvCtrl *p)
{
                        Uns Key = 0;
			Uns  Active = 0;

		p->Mpu.Enable = p->Status->bit.MuDu ;

		if(!p->Mpu.Enable) return;				// ��������� �������

		if (*p->Mpu.BtnKey)						// ������ ������� � �����
		{
			Key = *p->Mpu.BtnKey;				// ��������� ��������� �������
			Active = (p->ActiveControls & CMD_SRC_MPU); // ��������� ����������� �� ����� ���
			p->EvLog.Source = CMD_SRC_MPU;				// �������� ������� ��� �������
			*p->Mpu.BtnKey = 0;					// �������� �������
		}

		switch (Key)							// ��������� ��������� �������
		{
			case KEY_STOP:						// ���� ����
				*p->ControlWord = vcwStop;		// ������ ������� �� ���������, ��� ����������� �� ��������� ������ ���
				break;
			case KEY_CLOSE:
				if (Active) *p->ControlWord = vcwClose;	// ���� ����� ��� ����������� �� �������� �������
				else p->Mpu.CancelFlag = True;			// ���� ����� ��� �� ������� �� ������ ���� ������ �������
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
		p->IgnorComFlag = 1;										// ���� ��� ������� �� ���������� ���� ������������� �������
	 	return;
	}
	if(p->Status->bit.Closing) return;								// ���� ���� �������� �� ������
	if (p->Status->bit.Opening) {/*ReverseDrive(p);*/ return;}

	p->Valve.BreakFlag = False;										//
	FreeRun = (*p->ControlWord == vcwTestClose) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);		// ����� ���������� �������� - ���� �������� �������� ��� �� ��������������
	if (FreeRun) p->Valve.Position = POS_UNDEF;						//
	else if (!((*p->Valve.BreakMode == vtClose) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = 0;				// ���� �� �������� � ������ ���������� - ������� ��������� 0
	}
	else															// ���� �������� � ������ ����������
	{
		p->Valve.BreakFlag = True;									// ��������� ���� ������ � ����������
		p->Valve.Position  = -(LgInt)p->Valve.BreakDelta;			// �������� ������� ��������� �� ���������� ��� ��������� ����� ��� ����������
	}
	p->EvLog.Value = CMD_CLOSE;	// �������� ������� ��� �������

	p->StartControl(*p->ControlWord); // ������ ������� ���������� ��� ������ ������
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

 void UpdateComm(TCoreVlvDrvCtrl *p)		// ������� �������������� ������ ������� ��� ������� ()
{
	if (p->Command != vcwNone)					// ���� ������ ����������
	{
		if (p->Command == vcwStop) *p->ControlWord = vcwStop;	// ���� ������� ��� ������� ����, ������ ���������� ������� ����
		else if (!p->Status->bit.Stop) return;					// ���� ���� �����������
		else *p->ControlWord = p->Command;						// ������ ������� � ��������������� ������������
		p->Command = vcwNone;									// ���������� ���������� �������
	}
}

void ReverseDrive(TCoreVlvDrvCtrl *p)				// ������� �������
{
	switch (*p->ReverseType)					// � ����������� �� ���� �������
	{
		case rvtStop:  p->Command = vcwStop; break;	// ���� ��� �������
		case rvtAuto: {
						p->Command = *p->ControlWord; Core_ValveDriveStop(p);	// ����� ���� ������� � ������ �������
						break;
					  }
		case rvtNone: *p->ControlWord = vcwNone; break; // ������������� ������� ����������
	}
}




