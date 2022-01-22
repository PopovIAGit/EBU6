/*======================================================================
Имя файла:          core_Menu.c
Автор:
Версия файла:
Дата изменения:
======================================================================*/

#include "add_ValueConv.h"
#include "core_MenuParams.h" 	// Файл параметров
#include "g_Core.h"
#include "peref.h"
#include "g_Ram.h"
#include "stat.h"

TCoreMenu menu;// = MENU_DEFAULT;

#define EXPRESS_MAX_STATE		34

Byte 	showLog = 0,
	startReadLog = 0,
	TimeOutReadLog = 0,
	taskStartShow = 0;
Uns 	grcGear = 0;
Bool ExpNextStateRdy = false;
Byte SelectParamShow = 0;


 Bool SleepMode(TCoreMenu *);
 void StartState(TCoreMenu *);
 void SelectGroup(TCoreMenu *);
 void SelectParam(TCoreMenu *);
 void EditParam(TCoreMenu *);
//__inline void ExpressState(TCoreMenu *);
 void EditDec(TCoreMenu *, Char Key);
 void EditStr(TCoreMenu *, Char Key);
 void EditTime(TCoreMenu *, Char Key);
 void EditDate(TCoreMenu *, Char Key);
 void EditBin(TCoreMenu *);
static   void ReadDcrFull(TCoreMenu *, Uns Addr);
static   void ChangeCoordinate(TCoreMenu *, Char Key, Bool GroupFlag);
 void ShowStart(TCoreMenu *);
 void ShowGroup(TCoreMenu *);
 Byte DefineBit(TCoreMenu *, Uns Value);
static   void ShowValue(TCoreMenu *, Uns Position, Bool Edit);
static   void ShowParam(TCoreMenu *, Bool Edit);
/*__inline void ShowExpress(TCoreMenu *);
__inline void ShowExpressChoice(TCoreMenu *p, Byte YesOne, Byte YesTwo,
									Byte NoOne, Byte NoTwo, Bool Yes);*/
void ReadWriteAllParams(Byte, TCoreMenu *);
static   void PutAddData(Uns Addr, Uns *Value);

//---------------------------------------------------

void Core_MenuInit(TCoreMenu *p)
{
                p->State = MS_START;        		        // Режим меню
		p->Key = 0;          				// Управляющая команда
		p->EditType = 0;     				// Тип режима редактирования
		p->Level = 0;        				// Текущий уровень меню
		p->MaxLevel = 1;     				// Уровень вложенности меню (1 или 2)
		p->Indication = True;   			// Флаг разрешения индикации
		p->Update = True;       			// Флаг обновления данных из структур
		p->ShowReserved = False; 			// Флаг отображения резервов

		p->MinMaxGain = &g_Ram.FactoryParam.MaxTorque;   // Коэффициент диапазона

		p->Value = 0;        				// Текущее значение параметра
		p->Rate = 0;         				// Текущий разряд
		p->EditRate = 0;     				// Максимальный разряд для редактирования
		p->Blink = False;        			// Состояния мигания значения
		p->BlinkTimer = 0;   				// Таймер для мигания
		p->BlinkHiTime = BLINK_HI_TOUT;  	// Время отображения при редактировании
		p->BlinkLoTime = BLINK_LO_TOUT;  	// Время скрытия при редактировании
		p->Bit = 0xFF;          			// Выводимый бита числа в виде бегущей строки
		p->BitTimer = 0;     				// Таймер паузы при выводе бита
		p->BitTime = BIT_TOUT;      		// Пауза при выводе бита

		p->SleepActive = false;  			// Состояние режима
		p->SleepTimer = 0;   				// Таймер режима
		p->SleepTime = 0;    				// Тайм-аут режима

		p->Data = ToPtr(&g_Ram);         		// Указатель на буфер данных
		p->HiString = g_Peref.Display.HiStr;            // Указатель на буфер верхней строчки индикатора
		p->LoString = g_Peref.Display.LoStr;            // Указатель на буфер нижней строчки индикатора

		p->StartAddr = REG_START_IND;   	        // Адрес параметра при выводе в режиме старта
		p->StartOffset = RAM_ADR;  			// Смещение адреса при выводе в режиме старта

		p->EvLogFlag = False;    			// Флаг чтения записи
		p->EvLogSelected = False;			// Флаг выбора записи
		p->EvLogGroup = MENU_EVLOG_GROUP;       // Номер группы журнала
		p->EvLogAdress = REG_LOG_ADDR;  	// Адрес параметра текущей записи в журнал
		p->EvLogTime = REG_LOG_TIME;    	// Адрес параметра времени записи журнала
		p->EvLogDate = REG_LOG_DATE;    	// Адрес параметра даты записи журнала
		p->EvLogCount = &g_Ram.HideParam.LogEvCount;	 // Количество сделанных записей в журнал

		p->Group.Position = 0;
		p->Group.Addr = 0;
		p->Group.Count = MENU_GROUPS_COUNT;

		p->SubGroup.Position = 0;
		p->SubGroup.Addr = 0;
		p->SubGroup.Count = 0;

		p->EvLog.Position = 0;
		p->EvLog.Addr = 0;
		p->EvLog.Count = LOG_EV_CNT;

		p->Param.Position = 0;
		p->Param.Addr = 0;
		p->Param.Count = 0;

		p->Dcr.Min = 0;
		p->Dcr.Max = 0;
		p->Dcr.Def = 0;
		p->Dcr.Config.all = 0;

		p->setDefaultGroupNumber = 0;

		p->Express.Enable = False;
		p->Express.Select = False;
		p->Express.State = 0;
		p->Express.Index = 0;
		p->Express.First = False;
		p->Express.List = 0;

		p->Express.List1.Cnt = 4;		// Параметры экспресс настройки
		p->Express.List1.Buf[0] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List1.Buf[1] = GetAdr(UserParam.DevDate);		// Параметры экспресс настройки
		p->Express.List1.Buf[2] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List1.Buf[3] = GetAdr(UserParam.DevTime);;		// Параметры экспресс настройки
		p->Express.List1.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List2.Cnt = 2;		// Параметры экспресс настройки
		p->Express.List2.Buf[0] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List2.Buf[1] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List2.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List2.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List2.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List3.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List3.Buf[0] = GetAdr(Comands.TaskClose);		// Параметры экспресс настройки
		p->Express.List3.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List3.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List3.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List3.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List4.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List4.Buf[0] = GetAdr(Comands.TaskOpen);		// Параметры экспресс настройки
		p->Express.List4.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List4.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List4.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List4.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List5.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List5.Buf[0] = GetAdr(Comands.RevOpen);		// Параметры экспресс настройки
		p->Express.List5.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List5.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List5.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List5.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List6.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List6.Buf[0] = GetAdr(Comands.RevClose);		// Параметры экспресс настройки
		p->Express.List6.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List6.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List6.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List6.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List7.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List7.Buf[0] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List7.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List7.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List7.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List7.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List8.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List8.Buf[0] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List8.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List8.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List8.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List8.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List9.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List9.Buf[0] = GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List9.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List9.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List9.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List9.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List10.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List10.Buf[0] =GetAdr(UserParam.DevTime);		// Параметры экспресс настройки
		p->Express.List10.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List10.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List10.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List10.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List11.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List11.Buf[0] = 0;		// Параметры экспресс настройки
		p->Express.List11.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List11.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List11.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List11.Buf[4] = 0;		// Параметры экспресс настройки

		p->Express.List12.Cnt = 1;		// Параметры экспресс настройки
		p->Express.List12.Buf[0] = GetAdr(UserParam.DevTime);	// Параметры экспресс настройки
		p->Express.List12.Buf[1] = 0;		// Параметры экспресс настройки
		p->Express.List12.Buf[2] = 0;		// Параметры экспресс настройки
		p->Express.List12.Buf[3] = 0;		// Параметры экспресс настройки
		p->Express.List12.Buf[4] = 0;		// Параметры экспресс настройки

		p->Groups 		= groups;
		p->SubGroups 	        = Null;
		p->Params 		= params;
		p->Values 		= values;
		
                strcpy(p->Rsvd.Str, "RESERV");

		p->StartDispl = StartDispl;
		p->EnableEdit = EnableEdit;
		p->WriteValue = WriteValue;
		/*p->GetExpressText = GetExpressText;
		p->GetExpressState = GetExpressState;*/

	// Выводим момент
		g_Ram.HideParam.StartIndic = 8;

	// Чтение всех параметров из Eeprom
	ReadWriteAllParams(F_READ,p);
	// Проверяем настроен ли ModBus
	SetModBusParams();
	
}

void Core_MenuUpdate(TCoreMenu *p)
{
	if (SleepMode(p))
		return;

	if (!p->EvLogFlag)
	{
		switch (p->State)
		{
		   case MS_START:	StartState(p);  break;
		   case MS_SELGR:   SelectGroup(p); break;
		   case MS_SELPAR:  SelectParam(p); break;
		   case MS_EDITPAR: EditParam(p);   break;
		  // case MS_EXPRESS: ExpressState(p); break;
		}
	}

	p->Key = 0;
}

void Core_MENU_Display(TCoreMenu *p)
{
	if (!p->Indication) {p->Update = TRUE; return;}

	if (p->EvLogFlag) return;

	if (p->Update)
	{
		memset(p->HiString, ' ', 16);
		memset(p->LoString, ' ', 16);
	}

	if (!p->SleepActive)
	{
		switch (p->State)
		{
		   case MS_START:   ShowStart(p);        break;
		   case MS_SELGR:   ShowGroup(p);        break;
		   case MS_SELPAR:  ShowParam(p, FALSE); break;
		   case MS_EDITPAR: ShowParam(p, TRUE);  break;
		  // case MS_EXPRESS: ShowExpress(p);   	 break;
		}
	}
	p->Update = FALSE;
}

__inline Bool SleepMode(TCoreMenu *p)
{
	if (!p->Key)
	{
		if (!p->SleepActive)
		{
			if (!p->SleepTime) p->SleepTimer = 0;
			else if (p->SleepTimer < p->SleepTime) p->SleepTimer++;
			else {p->SleepActive = TRUE; p->Update = TRUE;}
		}
	}
	else
	{
		p->SleepTimer = 0;
		if (p->SleepActive)
		{
			p->State = MS_START;
			p->SleepActive = FALSE;
			p->Update = TRUE;
			p->Key = 0;
		}
	}
	return p->SleepActive;
}

 void StartState(TCoreMenu *p)
{
	if (p->Express.Enable)
	{
		p->State = MS_EXPRESS;
		return;
	}

	if (p->Key == KEY_ENTER)
	{
		p->State = MS_SELGR;
		p->Level = 1;
		p->EvLogSelected = FALSE;
		p->Update = TRUE;
		p->Group.Position  = p->Group.Count - 1;
		ChangeCoordinate(p, KEY_UP, TRUE);
	}
}

 void SelectGroup(TCoreMenu *p)
{
	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   ChangeCoordinate(p, p->Key, TRUE);
		   p->EvLogFlag = p->EvLogSelected;
		   break;
	   case KEY_ESCAPE:
		   if (p->EvLogSelected) p->EvLogSelected = FALSE;
		   else if (p->Level == 2) p->Level = 1;
		   else p->State = MS_START;
		   break;
	   case KEY_ENTER:
		   if ((p->Group.Position == p->EvLogGroup) && !p->EvLogSelected)
		   {
			   p->EvLogSelected = TRUE;
			   if (p->Data[p->EvLogAdress] > 0) p->EvLog.Position = p->Data[p->EvLogAdress];
			   else p->EvLog.Position = *p->EvLogCount;	//!!!
//			   else p->EvLog.Position = p->EvLog.Count;	//!!!

			   p->EvLogFlag = TRUE;
				ChangeCoordinate(p, KEY_DOWN, TRUE);
			   break;
		   }

		   if ((p->Level == 1) && (p->MaxLevel == 2))
		   {
			   p->Level = 2;
			   p->SubGroup.Position = p->SubGroup.Count - 1;
			   ChangeCoordinate(p, KEY_UP, TRUE);
		   }
		   else
		   {
			   p->Param.Position = p->Param.Count - 1;
			   ChangeCoordinate(p, KEY_UP, FALSE);
			   p->State = MS_SELPAR;
		   }
		   break;
	   default:
		   return;
	}

	p->Update = TRUE;
}

 void SelectParam(TCoreMenu *p)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;

	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   ChangeCoordinate(p, p->Key, FALSE);
		   break;
	   case KEY_ESCAPE:
		   p->State = MS_SELGR;
		   break;
	   case KEY_ENTER:
		   if (Val->ReadOnly) return;
		   if (!p->EnableEdit(Val->PaswwPrt)) return;
		   p->Rate  = 0;
		   p->Blink = FALSE;
		   p->State = MS_EDITPAR;
		   break;
	   default:
		   return;
	}

	p->Update = TRUE;
}

 void EditParam(TCoreMenu *p)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;

	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   switch (Val->Type)
		   {
		      case MT_DEC:  EditDec (p, p->Key); break;
		      case MT_STR:  EditStr (p, p->Key); break;
		      case MT_TIME: EditTime(p, p->Key); break;
		      case MT_DATE: EditDate(p, p->Key); break;
		      case MT_BIN:  EditBin (p);         break;
		   }
		   break;
	   case KEY_LEFT:   if (p->Rate < p->EditRate) p->Rate++; else p->Rate = 0; break;
	   case KEY_RIGHT:  if (p->Rate > 0) p->Rate--; else p->Rate = p->EditRate; break;
	   case KEY_ESCAPE: p->State = MS_SELPAR; break;
	   case KEY_ENTER:
		   if (!CheckRange(p->Value, p->Dcr.Min, p->Dcr.Max)) break;
		   if (p->State == MS_EXPRESS)
			{
				if (!p->WriteValue(Val->Memory, p->Express.List->Buf[p->Express.Index], &p->Value)) break;
			}
		   else
		   {
		   		if (!p->WriteValue(Val->Memory, p->Param.Position + p->Param.Addr, &p->Value)) break;
		   }
		   p->State = MS_SELPAR;
		   break;
	}
}


__inline void EditDec(TCoreMenu *p, Char Key)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	Char  i, *pBuffer, SignChange;
	Uns   Step;
	LgInt Data;

	Step = 1;
	for (i=0; i < p->Rate; i++) Step = Step * 10;

	pBuffer = &p->BufValue[5 - p->Rate];
	SignChange = (Val->Signed && (p->Rate == p->EditRate));

	switch (Key)
	{
	   case KEY_UP:
		   if (!p->EditType)
		   {
			   if (Val->Signed)
			   {
				   Data = (LgInt)((Int)p->Value) + (LgInt)Step;
				   if (Data > (LgInt)((Int)p->Dcr.Max)) p->Value = p->Dcr.Max;
				   else p->Value = p->Value + Step;
			   }
			   else
			   {
				   if (Step > (p->Dcr.Max - p->Value)) p->Value = p->Dcr.Max;
				   else p->Value = p->Value + Step;
			   }
			   return;
		   }
		   else if (!SignChange)
		   {
			   if (*pBuffer < '9') *pBuffer = *pBuffer + 1;
			   else *pBuffer = '0';
		   }
		   break;
	   case KEY_DOWN:
		   if (!p->EditType)
		   {
			   if (Val->Signed)
			   {
				   Data = (LgInt)((Int)p->Value) - (LgInt)Step;
				   if (Data < (LgInt)((Int)p->Dcr.Min)) p->Value = p->Dcr.Min;
				   else p->Value = p->Value - Step;
			   }
			   else
			   {
				   if (Step > (p->Value - p->Dcr.Min)) p->Value = p->Dcr.Min;
				   else p->Value = p->Value - Step;
			   }
			   return;
		   }
		   else if (!SignChange)
		   {
			   if (*pBuffer > '0') *pBuffer = *pBuffer - 1;
			   else *pBuffer = '9';
		   }
		   break;
	   default:
		   return;
	}

	pBuffer = &p->BufValue[4 - Val->RateMax];
	if (SignChange)
	{
		if (*pBuffer == '-') *pBuffer = '+';
		else *pBuffer = '-';
	}
	p->Value = StrToDec(pBuffer + 1, Val->RateMax);
	if (*pBuffer == '-') p->Value = -p->Value;
}

__inline void EditStr(TCoreMenu *p, Char Key)
{
	Uns  Value, Count, RsvdLen, MaxLen;
	Char Tmp, *BufAddr, *BufPtr;

	Value   = p->Value;
	Count   = p->Dcr.Max - p->Dcr.Min + 1;
	RsvdLen = strlen(p->Rsvd.Str);
	MaxLen  = 16 - RsvdLen;

	while(1)
	{
		if (Count > 0) Count--; else break;

		switch (Key)
		{
	   	case KEY_UP:   if (Value < p->Dcr.Max) Value++; else Value = p->Dcr.Min; break;
		   case KEY_DOWN: if (Value > p->Dcr.Min) Value--; else Value = p->Dcr.Max; break;
		}

		PFUNC_blkRead((Int *)p->Values[p->Dcr.Config.Str.Addr + Value].Str, (Int *)p->BufTmp, 16);
		if (p->ShowReserved) break;

		BufAddr = p->BufTmp;
		BufAddr[16] = '\0';
		while (*BufAddr) {if (*BufAddr != ' ') break; BufAddr++;}
		if ((Uns)(BufAddr - p->BufTmp) > MaxLen) break;

		BufPtr = &BufAddr[RsvdLen];
		Tmp = *BufPtr; *BufPtr = '\0';
		if (strcmp(p->Rsvd.Str, BufAddr)) {*BufPtr = Tmp; break;}
	}

	p->Value = Value;
}

__inline void EditTime(TCoreMenu *p, Char Key)
{
	TTimeVar *Time = (TTimeVar *)&p->Value;

	switch (Key)
	{
	   case KEY_UP:
		   switch (p->Rate)
		   {
		      case 0: if (Time->bit.Min  < 59) Time->bit.Min++;  else Time->bit.Min  = 0;  break;
		      case 1: if (Time->bit.Hour < 23) Time->bit.Hour++; else Time->bit.Hour = 0;  break;
		   }
		   break;
	   case KEY_DOWN:
		   switch (p->Rate)
		   {
		      case 0: if (Time->bit.Min  > 0)  Time->bit.Min--;  else Time->bit.Min  = 59; break;
		      case 1: if (Time->bit.Hour > 0)  Time->bit.Hour--; else Time->bit.Hour = 23; break;
		   }
		   break;
	}
}

 void EditDate(TCoreMenu *p, Char Key)
{
	TDateVar *Date = (TDateVar *)&p->Value;

	switch (Key)
	{
	   case KEY_UP:
		   switch (p->Rate)
		   {
		      case 0: if (Date->bit.Year  < 99) Date->bit.Year++;  else Date->bit.Year  = 0; break;
		      case 1: if (Date->bit.Month < 12) Date->bit.Month++; else Date->bit.Month = 1; break;
		      case 2:
			      if (Date->bit.Day < RTC_DefMaxDay(Date->bit.Year, Date->bit.Month)) Date->bit.Day++;
			      else Date->bit.Day = 1;
			      break;
		   }
		   break;
	   case KEY_DOWN:
		   switch (p->Rate)
		   {
		      case 0: if (Date->bit.Year  > 0) Date->bit.Year--;  else Date->bit.Year  = 99; break;
		      case 1: if (Date->bit.Month > 1) Date->bit.Month--; else Date->bit.Month = 12; break;
		      case 2:
			      if (Date->bit.Day > 1) Date->bit.Day--;
			      else Date->bit.Day = RTC_DefMaxDay(Date->bit.Year, Date->bit.Month);
			      break;
		   }
		   break;
	}
}

__inline void EditBin(TCoreMenu *p)
{
	Uns Step = 1U << p->Rate;

	if (p->Value & Step) p->Value &= ~Step;
	else p->Value |= Step;
}

static void ChangeCoordinate(TCoreMenu *p, Char Key, Bool GroupFlag)
{
	struct MENU_CRD *Crd;
	Uns Count, RsvdLen, MaxLen;
	Char *Addr, *GrAddr, Name[17];

	if (GroupFlag)
	{
		if (p->EvLogSelected) {Crd = &p->EvLog; Addr = (Char *)p->Groups;}
		else if (p->Level == 1) {Crd = &p->Group; Addr = (Char *)p->Groups;}
		else {Crd = &p->SubGroup; Addr = (Char *)p->SubGroups;}
	}
	else
	{
		Crd = &p->Param;
		Count = p->Param.Count;
		RsvdLen = strlen(p->Rsvd.Str);
		MaxLen = 16 - RsvdLen;
	}

	while(1)
	{
		switch (Key)
		{
		   case KEY_UP:
			   if (Crd->Position < Crd->Count-1) Crd->Position++;
			   else Crd->Position = 0;
			   break;
		   case KEY_DOWN:
			   if (Crd->Position > 0) Crd->Position--;
			   else Crd->Position = Crd->Count-1;
			   break;
		}

		if (GroupFlag)
		{
			if (p->EvLogSelected) GrAddr = Addr + p->Group.Position * sizeof(struct MENU_GROUP);
			else GrAddr = Addr + Crd->Position * sizeof(struct MENU_GROUP);
			Count = PFUNC_wordRead((Int *)&((struct MENU_GROUP *)GrAddr)->Count);
			if (Count || p->ShowReserved) break;
		}
		else
		{
			if (p->ShowReserved) break;
			if (Count > 0) Count--; else break;
			PFUNC_blkRead((Int *)p->Params[Crd->Position + Crd->Addr].Name, (Int *)Name, 16);
			Addr = strchr(Name, (Int)'.');
			if (!Addr) break;
			Addr++;
			if ((Int)(Addr - Name) > MaxLen) break;
			Addr[RsvdLen] = '\0';
			if (strcmp(p->Rsvd.Str, Addr)) break;
			continue;
		}
	}

	if (GroupFlag)
	{
		Crd = (p->Level == 2) ? &p->SubGroup : &p->Param;
		Crd->Addr  = PFUNC_wordRead((Int *)&((struct MENU_GROUP *)GrAddr)->Addr);
		Crd->Count = Count;
	}
	else ReadDcrFull(p, p->Param.Position + p->Param.Addr);
}

 void ShowStart(TCoreMenu *p)
{
#if !IR_IMP_TEST
	Uns Position;
#endif

	memset(p->HiString, ' ', 16);

#if !IR_IMP_TEST
	Position = p->Data[p->StartAddr] + p->StartOffset;
	if (p->Update) ReadDcrFull(p, Position);
	ShowValue(p, Position, FALSE);
#endif

	p->StartDispl(p->HiString);
}

static void ShowGroup(TCoreMenu *p)
{
	const struct MENU_GROUP *GroupTbl;
	Char *Str;

	if (p->Update)
	{
		if (p->EvLogSelected)
		{
			PFUNC_blkRead((Int *)p->Values[p->EvLog.Addr].Str, (Int *)p->HiString, 16);
			Str = &p->HiString[15]; while (*(Str-6) == ' ') Str--;
			DecToStr(p->EvLog.Position + 1, Str, 0, 4, FALSE, FALSE);
			TimeToStr((RTC_TimeTypeDef *)&p->Data[p->EvLogTime], (String)&p->LoString[1]);
			DateToStr((TDateVar *)&p->Data[p->EvLogDate], (String)&p->LoString[7]);
		}
		else
		{
			if (p->Level == 1) GroupTbl = &p->Groups[p->Group.Position];
			if (p->Level == 2) GroupTbl = &p->SubGroups[p->SubGroup.Position + p->SubGroup.Addr];
			PFUNC_blkRead((Int *)GroupTbl->HiString, (Int *)p->HiString, 16);
			PFUNC_blkRead((Int *)GroupTbl->LoString, (Int *)p->LoString, 16);

			if (p->Group.Position == 2)
			  {
			    if (IsPassword2()) menu.HiString[15] = CODEON_ICO;
			    else menu.HiString[15] = CODEOFF_ICO;
			  }
                        if (p->Group.Position == 1)
                          {
                            if (IsPassword1()) menu.HiString[15] = CODEON_ICO;
                            else menu.HiString[15] = CODEOFF_ICO;
                          }

                        if (p->Group.Position == 3)
                          {
                            if (IsPassword1()) menu.HiString[15] = CODEON_ICO;
                            else menu.HiString[15] = CODEOFF_ICO;
                          }
		}
	}
}

__inline Byte DefineBit(TCoreMenu *p, Uns Value)
{
	if (!Value) {p->Bit = 0xFF; return 0;}

	if (p->Bit == 0xFF) p->BitTimer = 0;

	if (p->BitTimer > 0) p->BitTimer--;
	else
	{
		do {if (++p->Bit >= 15) p->Bit = 0;}
		while (!(Value & (1 << p->Bit)));
		p->BitTimer = p->BitTime;
	}

	return (p->Bit + 1);
}

static void ShowValue(TCoreMenu *p, Uns Position, Bool Edit)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	Char i, *Str;
	Uns  Addr, Timeout;

	if (Edit)
	{
		Timeout = !p->Blink ? p->BlinkHiTime : p->BlinkLoTime;
		if (++p->BlinkTimer >= Timeout)
		{
			p->Blink = !p->Blink;
			p->BlinkTimer = 0;
		}
	}
	else
	{
//----- Экспресс настройка -------
		if ((p->State == MS_EXPRESS) && ((p->Express.State == 5) || (p->Express.State == 7)))
				{	p->Value = 1;	}
		else	p->Value = p->Data[Position];
//-----------------
		p->Blink = FALSE;
		p->BlinkTimer = 0;
	}

	memset(p->LoString, ' ', 16);

	switch (Val->Type)
	{
	   case MT_VERS:
		   Str = &p->LoString[10];

		   if (!Edit)
		   {
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, TRUE, Val->Signed);
				p->LoString[4] = p->LoString[5];
				p->LoString[5] = p->LoString[6];
				p->LoString[6] = '.';
		   }

	   break;
	   case MT_DEC:
		   Str = &p->LoString[8];

		   if (!p->EditType)
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, !Edit, Val->Signed);
		   else if (!Edit)
		   {
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, TRUE, Val->Signed);
			   if (Val->Signed) FloatPositive = '+';
			   DecToStr(p->Value, &p->BufValue[5], 0, Val->RateMax, FALSE, Val->Signed);
			   FloatPositive = ' ';
		   }
		   else for (i=0; i <= p->EditRate; i++)
        {
				if (Val->Precision && (i == Val->Precision)) *Str-- = FloatSeparator;
				*Str-- = p->BufValue[5 - i];
		   }

		   if (p->Blink)
		   {
			   Str = &p->LoString[8 - p->Rate];
			   if (Val->Precision && (p->Rate >= Val->Precision)) Str--;
			   *Str = ' ';
		   }

		   if (p->Update) PFUNC_blkRead((Int *)p->Params[Position].Unit, (Int *)p->BufTmp, 4);
		   else memcpy(&p->LoString[10], p->BufTmp, 4);
		   break;

	   case MT_STR:
		   if (p->Blink) break;
			if (!Edit)
			{
				Addr = p->Dcr.Config.Str.Addr + p->Value;
				PFUNC_blkRead((Int *)p->Values[Addr].Str, (Int *)p->BufTmp, 16);
			}
			memcpy(p->LoString, p->BufTmp, 16);
		   break;

	   case MT_RUN:
		   if (p->Blink) break;
		   Addr = p->Dcr.Config.Str.Addr + DefineBit(p, p->Value);
		   PFUNC_blkRead((Int *)p->Values[Addr].Str, (Int *)p->LoString, 16);
		   break;

	   case MT_TIME:
		   TimeToStr((RTC_TimeTypeDef *)&p->Value, (String)&p->LoString[5]);
		   if (p->Blink)
		   {
			   switch (p->Rate)
			   {
			      case 0: p->LoString[8] = ' '; p->LoString[9] = ' '; break;
			      case 1: p->LoString[5] = ' '; p->LoString[6] = ' '; break;
			   }
		   }
		   break;

	   case MT_DATE:
		   DateToStr((TDateVar *)&p->Value, (String)&p->LoString[4]);
		   if (p->Blink)
		   {
			   switch (p->Rate)
			   {
			      case 0: p->LoString[10] = ' '; p->LoString[11] = ' '; break;
			      case 1: p->LoString[7]  = ' '; p->LoString[8]  = ' '; break;
			      case 2: p->LoString[4]  = ' '; p->LoString[5]  = ' '; break;
			   }
		   }
		   break;

	   case MT_BIN:
		   Str = &p->LoString[9 + Val->RateMax / 2];
		   for (i=0; i <= (Char)Val->RateMax; i++)
		   {
			   Str--;
			   if (p->Blink && (i == p->Rate)) continue;
			   *Str = (p->Value & (1 << i)) ? '1' : '0';
		   }
		   break;
	}
}

static void ShowParam(TCoreMenu *p, Bool Edit)
{
	Uns Position = p->Param.Position + p->Param.Addr;

	if (p->Update) PFUNC_blkRead((Int *)p->Params[Position].Name, (Int *)p->HiString, 16);

	ShowValue(p, Position, Edit);
}
/*
__inline void ShowExpress(TCoreMenu *p)
{
	Uns Addr;

	p->Express.List = 0;

	if (p->Express.Enable)							// Включено сообщение о выборе экспресс настройки
	{
		p->GetExpressText(0);						// Выводим первое сообщение
		ShowExpressChoice(p, 3,6,7,11, false);		// Выводим скобочки текущего выбора
		return;
	}

	if (ExpNextStateRdy)	return;

	switch(p->Express.State)							// Шаги экспресс настройки
	{
		case 0: p->Express.List = &p->Express.List1; 								break;
		case 1:	p->GetExpressText(1);	ShowExpressChoice(p, 11,14, 0, 0, true);	break;
		case 2: p->Express.List = &p->Express.List2;								break;
		case 3:	p->GetExpressText(2);	ShowExpressChoice(p,   0, 5, 6,15, true);	break;
		// Переведите в положение закрыто
		case 4: p->GetExpressText(3);	ShowExpressChoice(p,  10,13, 0, 0, true);	break;
		// Задание закрыто
		case 5: p->Express.List = &p->Express.List3;								break;
		// Переведите в положение открыто
		case 6: p->GetExpressText(4);	ShowExpressChoice(p,  10, 13, 0, 0, true);	break;
		// Задание открыто
		case 7: p->Express.List = &p->Express.List4;								break;
		// Переведите в крайнее положение
		case 8: p->GetExpressText(5);	ShowExpressChoice(p,  11, 14, 0, 0, true);	break;
		// Положение открыто или закрыто
		case 9: p->GetExpressText(6);	ShowExpressChoice(p,   2,  7, 8,13, true);	break;
		// Обороты на открытие
		case 10: p->Express.List = &p->Express.List5;								break;
		// Обороты на закрытие
		case 11: p->Express.List = &p->Express.List6;								break;
		// Проверить калибровку
		case 12: p->GetExpressText(7);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// Проверяем текущее положение: открыто или закрыто
		case 13:																	break;
		// Пустите в открыто
		case 14: p->GetExpressText(8);												break;
		// Выводим положение в оборотах
		case 15: p->GetExpressText(9);												break;
		// Остановились по "Стоп". Программно нажимаем "Ввод" для перехода к следующему состоянию
		case 16: 																	break;
		// Сбросить калибровку?
		case 17: p->GetExpressText(10);	ShowExpressChoice(p,   3,  6, 7,11, false);	break;
		// Сброс калибровки и возврат к выбору типа калибровки
		case 18:																	break;
		// Доехали до "Открыто"
		case 19:																	break;
	//---------------------------------------------------------------------------
		// Пустите в закрыто
		case 20: p->GetExpressText(11);												break;
		// Выводим положение в оборотах
		case 21: p->GetExpressText(9);												break;
		// Остановились по "Стоп". Программно нажимаем "Ввод" для перехода к следующему состоянию
		case 22: 																	break;
		// Сбросить калибровку?
		case 23: p->GetExpressText(10);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// Сброс калибровки и возврат к выбору типа калибровки
		case 24:																	break;
		// Доехали до "Открыто"
		case 25:																	break;
		// Вас устраивает калибровка?
		case 26: p->GetExpressText(12);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// Задать рабочие моменты
		case 27: p->GetExpressText(13);	ShowExpressChoice(p,   11,14, 0, 0, true);	break;
		// Задаем моменты
		case 28: p->Express.List = &p->Express.List2;				 				break;
		// Момент уплотнения только для клиновой
		case 29: p->Express.List = &p->Express.List7;								break;
		// Задаем момент
		case 30: p->Express.List = &p->Express.List8;								break;
		// Момент уплотнения только для клиновой
		case 31: p->Express.List = &p->Express.List9;								break;
		// Задаем момент
		case 32: p->Express.List = &p->Express.List10;							 	break;
		// Выбираем тип входного сигнала только для БУР-Т
		case 33: p->Express.List = &p->Express.List11;								break;
		// Выбор режима МУ/ДУ
		case 34: p->Express.List = &p->Express.List12;								break;
	}


	if (p->Express.List)																	// Если выбран List с адресами параметров
	{
		Addr = p->Express.List->Buf[p->Express.Index];										// Формируем адрес текущего параметра
		PFUNC_blkRead((Int *)p->Params[Addr].Name, (Int *)p->HiString, 16);		// Считываем название текущего параметра
		if (p->Update)
		{
			ReadDcrFull(p, Addr);									// Когда флаг update, то считываем описание параметра
			if ((p->Express.State == 5) || (p->Express.State == 7))
				{	p->Dcr.Min = 1;	p->Dcr.Max = 1;	}
		}
		if (p->Express.First)												// Выводим значение параметра на дисплей
			{
				p->Rate = 0;
				ShowValue(p, Addr, FALSE);	p->Express.First = false;
			}
		else	ShowValue(p, Addr, TRUE);									// Выводим редактирование на дисплей
	}
}



__inline void ShowExpressChoice(TCoreMenu *p, Byte YesOne, Byte YesTwo,
									Byte NoOne, Byte NoTwo, Bool Yes)
{
	Uns Timeout;

	Timeout = !p->Blink ? p->BlinkHiTime : p->BlinkLoTime;
	if (++p->BlinkTimer >= Timeout)
	{
		p->Blink = !p->Blink;
		p->BlinkTimer = 0;
	}

	if (p->Blink)	return;

	if (p->Express.First)
	{
		p->Express.Select = Yes;
		p->Express.First = false;
	}

	if (p->Express.Select)				// Выводим скобочки, обозначающие текущий выбор на индикаторе
	{
		p->LoString[YesOne] = '[';
		p->LoString[YesTwo] = ']';
	}
	else
	{
		if ((!NoOne) && (!NoTwo))
		{	p->Express.Select = true;	return;	}

			p->LoString[NoOne] = '[';
			p->LoString[NoTwo] = ']';
	}
}*/

//---------------------------------------------------
// Определяем информацию, которая отображается на верхнем уровне меню
void StartDispl(String Str)
{
    Uns displPosition = 0;
    displPosition = !g_Ram.Status.Faults.Dev.bit.PosSens ? g_Ram.Status.PositionPr : 9999; // Если "сбой датчика положения", выводим 999.9
    DecToStr(displPosition, &menu.HiString[8], 1, 4, True, True);
    menu.HiString[9] = '%';

/*	if (IsPassword1())
		menu.HiString[11] = CODE_ICO;
	if (g_Comm.mbAsu.Stat.Status.bit.NoConnect == 1)
		menu.HiString[12] = CONN_ICO;
	if (g_Comm.Bluetooth.IsConnected)
	{
		menu.HiString[14] = BT_ICO;
		if (g_Comm.Bluetooth.ModeProtocol == 1 && menu.State == MS_START) menu.HiString[15] = 'Д';
		if (g_Comm.Bluetooth.ModeProtocol == 2 && menu.State == MS_START) menu.HiString[15] = 'Ж';
	}*/
}

//------------------------------------------------------------------
//TODO Подтверждение или изменение паролей
Bool UpdateCode(Uns addrPassw, Uns addrCode, Uns value, Uns def)	// в WriteValue()
{
	Uns *password = ToUnsPtr(&g_Ram) + addrPassw;
	Uns *code = ToUnsPtr(&g_Ram) + addrCode;
	Bool writeFlag = false;

	if (!IsMemParReady()) return false;
	if (*password != 0)
	{
		if ((value == *password) || (value == def))
		{
			*code = 0; *password = 0;
			//writeFlag = true;
		}
	}
	else if (value != 0)
	{
		*password = value; *code = 0;
		writeFlag = true;
	}
	else if (value == 0)
	{
		*password = value; *code = 0;
		writeFlag = true;
	}

	if (writeFlag)
	{
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,addrPassw,password,1))
		{
			return true;
		}
	}

	return true;
}
//---------------------------------------------------
// Чтение настроек параметра
void ReadDcrFull(TCoreMenu *p, Uns Addr)	// в ChangeCoordinate()
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;

	Core_MenuReadDcr(p, &p->Dcr, Addr);

	p->Bit = 0xFF;

	// Для знакового значения увеличиваем диапазон
	p->EditRate = Val->RateMax;
	if (p->EditType && (Val->Type == MT_DEC) && Val->Signed) p->EditRate++;
}
//---------------------------------------------------
// Чтение и установка границ значения параметра

Uns addrtmp2 = 0;
void Core_MenuReadDcr(TCoreMenu *p, struct MENU_DCR *Dcr, Uns Addr) // в ReadDcrFull()
{
	Uns Kmm = *p->MinMaxGain;
	Uns tmp =0;
        
          
            
              
        addrtmp2 = Addr;            
	PFUNC_blkRead((Ptr)&p->Params[Addr].Dcr, (Ptr)Dcr, sizeof(struct MENU_DCR));

	if ((Dcr->Config.Val.Type == MT_DEC) && (Dcr->Config.Val.MinMax))
	{

		Dcr->Min = Dcr->Min * Kmm;

		tmp = Dcr->Max*(Kmm/100);///100 * Kmm;
		if (tmp>1200) Dcr->Max = 60000;
		else Dcr->Max = Dcr->Max * Kmm;

		//Dcr->Max = Dcr->Max * Kmm;
		Dcr->Def = Dcr->Def * Kmm;
	}
}
//---------------------------------------------------
// Запись параметра
Bool WriteValue(Uns memory, Uns param, Uns *value) // в EditParam()
{
	if (memory && !IsMemParReady()) return false;

	if (param == REG_CODE)
	{
		if ( UpdateCode(REG_PASSW1, param, *value, DEF_USER_PASS) )
			return true;
	}
	else if (param == REG_FCODE)
	{
		if ( UpdateCode(REG_PASSW2, param, *value, DEF_FACT_PASS) )
			return true;
	}

	*(ToUnsPtr(&g_Ram) + param) = *value;
	if (memory)
	{
		//if ( WriteToEeprom(param, value, 1) )
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,param,value,1))
		{
			RefreshParams(param);

			// Отправляем данные в журнал изменения параметров
		//	g_Stat.LogParam.Addr 	  = param;									// Адрес параметра
		//	g_Stat.LogParam.NewValue = *value;									// Новое значение параметра
		//	g_Stat.LogParam.ExecFlag = true;									// Выставляем флаг формирования журнала

			return true;
		}
	}
	else return true;
	return false;
}
//---------------------------------------------------
// Разрешение редактирования
Bool EnableEdit(Uns password)
{
	switch (password)
	{
		case 1: if (IsPassword1()) return false; break;
		case 2: if (IsPassword2()) return false; break;
	}

	return true;
}
//---------------------------------------------------
void Core_SetDeff(TCoreMenu *p)	// 50 Гц
{
	// Меняем заводские параметры (группа C), защищённые паролем 2
	if (g_Ram.FactoryParam.FactCode)
	{
		p->setDefaultGroupNumber = GROUP_FACT_PAR;
		g_Ram.HideParam.Password2 = 0;
		g_Ram.FactoryParam.FactCode = 0;
	}

	// По умолчанию
	if(!g_Core.DefFlag)
	{
		if (g_Ram.Comands.SetDefaults)
		{
			if (!g_Ram.Status.Status.bit.Stop) g_Core.VlvDrvCtrl.Mpu.CancelFlag = True;
			else
			{
				g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_USER;
				g_Core.DefFlag = 1;
				p->setDefaultGroupNumber = 1;
				g_Ram.Comands.SetDefaults = 0;
			}
		}
		if (g_Ram.FactoryParam.SetDefaults)
		{
			if (!g_Ram.Status.Status.bit.Stop) g_Core.VlvDrvCtrl.Mpu.CancelFlag = True;
			else
				{
					g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_FACT;
					g_Core.DefFlag = 1;
					p->setDefaultGroupNumber = 2;
					g_Ram.FactoryParam.SetDefaults = 0;
				}
		}
	}
	if (p->setDefaultGroupNumber)
		SetDefaultValues(p, &p->setDefaultGroupNumber);
}
//---------------------------------------------------
Uns a = 0;
// Установка значений по умолчанию
void SetDefaultValues(TCoreMenu *p, Byte *groupNumber) // в Core_MenuDisplay()
{
	struct MENU_DCR Dcr;
	static LgUns DefAddr = 0;
	Byte DefCode = 0;

	if (!IsMemParReady()) return;
	 // Меняем заводские параметры (группа B), защищённые паролем 1
	if (*groupNumber == GROUP_EDIT_PAR) DefCode = M_EDIT_PAR;
	 // Меняем заводские параметры (группа C), защищённые паролем 2
	else DefCode = M_FACT_PAR;

	if (DefAddr < RAM_DATA_SIZE)
	{
	    Core_MenuReadDcr(p, &Dcr, DefAddr);

	    a++;
	    g_Ram.HideParam.BusyValue = 100 - ((a * 100)/RAM_DATA_SIZE);//(a * (25/RAM_DATA_SIZE));
	    // Проверяем какие параметры записывать
	    // Если не Время и не Дата или
	    // Если Код доступа групп B и C
	    if ((((Dcr.Config.all & DefCode) == DefCode)
		    && (DefAddr != REG_GEAR_RATIO) // Не тип редуктора
		    && (DefAddr != REG_TASK_TIME)
		    && (DefAddr != REG_MAX_TRQE)
		    && (DefAddr != REG_I_NOM)
		    && (DefAddr != REG_TASK_DATE))) 
			{
			*(ToUnsPtr(&g_Ram) + DefAddr) = Dcr.Def;

			g_Core.DefFlag = 2;
			ReadWriteEeprom(&Eeprom1, F_WRITE, DefAddr,ToUnsPtr(&g_Ram) + DefAddr, 1);

			while (!IsMemParReady())
			{
				FM25V10_Update(&Eeprom1);
			}
			// Инициализация фильтров, масштабов и т.д.
			RefreshParams(DefAddr);

			DefAddr++;
		}
	    else
	    {
			DefAddr++;
			return;
	    }



	}
	else
	{
		a = 0;
		g_Core.DefFlag = 0;
                *groupNumber = 0;
                DefAddr = 0;
                return;
	}



	/*if (IsMemParReady())
	{
		// Запись параметров в Eeprom
		//ReadWriteAllParams(F_WRITE,p);
		//DefAddr = 0;
		*groupNumber = 0;
	}*/
}

//--------------------------------------------------------

Uns addrtmp = 0;
void ReadWriteAllParams(Byte cmd, TCoreMenu *p)	// в Core_MenuInit()
{
	struct MENU_DCR Dcr;
	Uns DefAddr = 0, count;

	while (DefAddr < RAM_DATA_SIZE)
	{
		count = 1;
		if (DefAddr == REG_CALIB_CLOSE) // Для считывания LgUns
			count = 5;
		Core_MenuReadDcr(p,&Dcr,DefAddr);
		// Проверяем какие параметры записывать
		if ( !(Dcr.Config.all & M_NVM)
			&&(DefAddr != REG_TASK_TIME)
			&&(DefAddr != REG_TASK_DATE)
			&&(DefAddr != REG_VER_PO)
			&&(DefAddr != REG_SUBVER_PO) )
		{
			*(ToUnsPtr(&g_Ram) + DefAddr) = 0;
		}
		else {
			if (g_Ram.FactoryParam.DriveType!=0
					&& (DefAddr==REG_MAX_TRQE || DefAddr==REG_I_NOM || DefAddr==REG_GEAR_RATIO
						/*|| (DefAddr>=REG_TORQUE_CURR && DefAddr<REG_TORQUE_ANG_SF)*/
					)
				)
			{

			} else
			{
				ReadWriteEeprom(&Eeprom1,cmd,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,count);
			}
		}
		while (!IsMemParReady()) 
                {
                  FM25V10_Update(&Eeprom1); 
                  __asm("NOP");
                }
		// Инициализация фильтров, масштабов и т.д.
		RefreshParams(DefAddr);
		DefAddr += count;
                addrtmp =  DefAddr;
	}
}
//---------------------------------------------------
// Установка значений ModBus
void SetModBusParams(void)
{
	//для БКД
	/*
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	*/
	//для АСУ
	if ((g_Ram.UserParam.RsBaudRate == 0xFFFF) & (g_Ram.UserParam.RsStation == 0xFFFF) || g_Ram.UserParam.RsStation == 0)
	{
		g_Ram.UserParam.RsBaudRate= br19200;
		g_Ram.UserParam.RsStation	= 1;
		g_Ram.UserParam.RsMode		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	/*
	//для УПП
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	//для Bluetooth
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	*/
}

/*
void GetExpressText(Uns State)
{
   if (State != 9)
    {
        ReadAddStr(menu.HiString, EXPR_STR_ADDR + (State<<1) + 0);
        ReadAddStr(menu.LoString, EXPR_STR_ADDR + (State<<1) + 1);
    }
    else
    {
        // Отображение текущего хода при проверке калибровки
        ReadAddStr(menu.HiString, EXPR_STR_ADDR + (State<<1) + 0);
        DecToStr(g_Ram.ramGroupA.PositionPr, &menu.LoString[8], 1, 4, True, True);
        menu.LoString[9] = '%';
    }
}
*/
/*
Byte GetExpressState(Byte CurrentState, Uns State)
{
    // Не забыть сбросить!
    static Byte ExCycleCount = 0;

   switch (State)
    {
        // Проверяем текущее состояние: открыто или закрыто
        case 0: if      (g_Ram.ramGroupA.Status.bit.Closed)    CurrentState = 14;      // Если закрыто, то следующее состояние
                else if (g_Ram.ramGroupA.Status.bit.Opened)    CurrentState = 20;      // Если открыто, то через два состояния
                break;
        // Двигаемся из состояния "Закрыто"
        case 1: if ((g_Ram.ramGroupA.Status.bit.Closing) || (g_Ram.ramGroupA.Status.bit.Opening))         CurrentState = 15;      // Если зафиксированна команда "Открыто", то переходим на следующий шаг
//      case 1: if (IsOpening())            CurrentState = 15;      // Если зафиксированна команда "Открыто", то переходим на следующий шаг
                break;
        // Останов во время движения
        case 2: if ((g_Ram.ramGroupA.Status.bit.Stop) && (!g_Ram.ramGroupA.Status.bit.Opened)) CurrentState = 16;  // Принудительный стоп
                else if (g_Ram.ramGroupA.Status.bit.Opened)                CurrentState = 19;  // Доехали до открыто
                break;
        // Сброс калибровки
        case 3: g_Ram.ramGroupD.CalibReset = 1;    ExCycleCount = 0;               CurrentState = 3;   // Возвращаемся к выбору типу калибровки
                break;
        // Доехали до "Открыто". Проверяем наличие полного цикла
        case 4: if (++ExCycleCount >= 2)            // Если был проделан полный цикл
                    {   ExCycleCount = 0;   CurrentState = 26;  }
                else                        CurrentState = 20;      // Иначе двигаемся в состояние "Закрыто"
                break;

        // Двигаемся из состояния "Открыто"
        case 5: if ((g_Ram.ramGroupA.Status.bit.Closing) || (g_Ram.ramGroupA.Status.bit.Opening))         CurrentState = 21;      // Если зафиксированна команда "Закрыто", то переходим на следующий шаг
//      case 5: if (IsClosing())            CurrentState = 21;      // Если зафиксированна команда "Закрыто", то переходим на следующий шаг
                break;
        // Останов во время движения
        case 6: if ((g_Ram.ramGroupA.Status.bit.Stop) && (!g_Ram.ramGroupA.Status.bit.Closed)) CurrentState = 22;  // Принудительный стоп
                else if (g_Ram.ramGroupA.Status.bit.Closed)                CurrentState = 25;  // Доехали до закрыто
                break;
        // Доехали до "Открыто". Проверяем наличие полного цикла
        case 7: if (++ExCycleCount >= 2)            // Если был проделан полный цикл
                    {   ExCycleCount = 0;   CurrentState = 26;  }
                else                        CurrentState = 14;      // Иначе двигаемся в состояние "Закрыто"
                break;
        // Проверяем какой был выбран тип задвижки
        case 8: if (g_Ram.ramGroupB.ValveType == vtKlin)   CurrentState = 29;
                else                            CurrentState = 30;
                break;
        // Проверяем какой был выбран тип задвижки
        case 9: if (g_Ram.ramGroupB.ValveType == vtKlin)   CurrentState = 31;
                else                            CurrentState = 32;
                break;
        // Проверяем тип модификации блока
        case 10:
                            CurrentState = 33;

                            menu.Express.List11.Buf[0] = GetAdr(ramGroupB.InputType);

                 break;
    }
    return CurrentState;
}*/



static void PutAddData(Uns Addr, Uns *Value)
{
	PutAddHiStr(Addr);

	if (!Value)
		PutAddLoStr(Addr + 1);
	else
	{
		DISPL_ClrLoStr(&g_Peref.Display);
		DecToStr(*Value, &g_Peref.Display.LoStr[7], 0, 2, True, False);
		g_Peref.Display.LoStr[9] = '%';
	}
}

void AddControl(void)
{
	if(!g_Ram.Status.Status.bit.Test)
	{
		/*if (!g_Core.PowerEnable)				// если выключенно
		 {
		 GrH->BusyValue = 0;	// 		процент выполнения
		 Menu.State = MS_START;		// меню в стартовый режим
		 }
		 else if (ClearLogFlag)
		 {
		 g_Ram.ramGroupH.BusyValue = GetWriteLogPercent();
		 PutAddData(BUSY_STR_ADR, &GrH->BusyValue);
		 if (IsMemLogReady())
		 {
		 g_Core.Mcu.EvLog.Source = g_Stat.ClearLogSource;
		 ClearLogFlag = False;
		 }
		 }
		 else */if(g_Core.DefFlag)	// если происходит сброс параметров к исходным
		{
			if(g_Core.DefFlag == 1) 									//
			g_Ram.HideParam.BusyValue = 100;
			else												//
			{
				//g_Ram.ramGroupH.BusyValue = GetWriteParPercent();
				//if (IsMemParReady())
				//	g_Core.DefFlag = 0;
			}
			PutAddData(BUSY_STR_ADR, &g_Ram.HideParam.BusyValue);
		}
		else if(g_Core.VlvDrvCtrl.Mpu.CancelFlag) //отмена команды пду
		{
			PutAddData(CMD_CANC_ADR, Null);
			if(++g_Core.CancelTimer >= (Uns) CANCEL_TOUT)
			{
				g_Core.VlvDrvCtrl.Mpu.CancelFlag = False;
				g_Core.CancelTimer = 0;
			}
		}
		/*else if (g_Core.VlvDrvCtrl.Mpu.MpuBlockedFlag)	// Команда МУ заблокировано
		 {

		 CancelTimer++;
		 if (CancelTimer <= 20)			// 2 секунды показываем "КОМАНДА ОТМЕНЕНА"
		 {
		 PutAddData(CMD_CANC_ADR, Null);
		 }
		 else
		 {
		 PutAddData(MPU_BLOCKED_ADR, Null);
		 }
		 if (CancelTimer >= 40)		// 2 секунды показываем "МПУ ЗАБЛОКИРОВАНО"
		 {
		 g_Core.Mcu.Mpu.MpuBlockedFlag = False;
		 CancelTimer = 0;
		 }
		 }*/
		else
		{
			g_Ram.HideParam.BusyValue = 0;
			menu.Indication = True;
			g_Core.CancelTimer = 0;
			return;
		}
	}
	else if(!g_Ram.TestParam.DisplShow)
	{
		g_Ram.HideParam.BusyValue = 0;
		menu.Indication = True;
		return;
	}

	/*g_Peref.BtnStatus &= BTN_STOP;	// если был стоп то остается стоп, если было чтото другое то добавляем стоп и команда не обрабатывается
	 g_Peref.Pult.Key = 0;			// сбросили команду с пду*/
	menu.Indication = False;		// хз чтото с меню
}



//---------Конец файла------------------------------------
