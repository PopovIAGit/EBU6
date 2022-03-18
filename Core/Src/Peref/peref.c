/*--------------------------
	perf.c

--------------------------*/

#include "peref.h"


TPeref g_Peref;

// ram test
uint16_t memtemp = 0;
uint16_t addr = 0;
uint16_t addrstatus = 0;
uint16_t count = 0;

Uns Data = 1;

Uns BtnTout   = (Uns)BTN_TIME;
Uns BtnLevel  = BTN_LEVEL;
Bool RtcStart = False;
Uns RtcUpdateFlag = 0;

Uns TimerTC = 0;
Uns TmpTC = 0;

Uns tempertmp = 0;
 Uns btnOpen, btnClose, btnStop1,btnStop2,btnProg1, btnProg2;
extern float SpeedRef;

uint8_t pBuff_ADC [2];
uint8_t pADS1118_ConfRegData[2];

char Icons[NUM_ICONS][7] =	{
					0x1F,0x11,0x1F,0x04,0x06,0x04,0x07,
					0x1F,0x11,0x11,0x1F,0x04,0x1F,0x00,
					0x06,0x15,0x0D,0x06,0x0D,0x015,0x06,
					0x0E,0x11,0x11,0x11,0x1F,0x1B,0x1F,
					0x0E,0x11,0x10,0x10,0x1F,0x1B,0x1F,
				};

void peref_Init(void)
{	 
  // дисплей----------------------------------------------------------------------------------
    DISPL_init(&g_Peref.Display);
    DISPL_AddSymb(&g_Peref.Display,0,ToPtr(Icons), NUM_ICONS);
  //ручка открыть-----------------------------------------------------------------------------
    g_Peref.BtnOpen.LogType = ltAnMin;
    g_Peref.BtnOpen.Enable = TRUE;
    g_Peref.BtnOpen.Input = Null;
   g_Peref.BtnOpen.Output = &g_Peref.BtnStatus;
    g_Peref.BtnOpen.Level = &BtnLevel;
    g_Peref.BtnOpen.Timeout = &BtnTout;
    g_Peref.BtnOpen.BitMask = 1<<0;
    g_Peref.BtnOpen.Scale = 1;
    g_Peref.BtnOpen.Timer = 0;
    g_Peref.BtnOpen.Match = false;
    g_Peref.BtnOpen.Flag = false;
  //ручка закрыть-----------------------------------------------------------------------------
    g_Peref.BtnClose.LogType = ltAnMin;
    g_Peref.BtnClose.Enable = TRUE;
    g_Peref.BtnClose.Input = Null;
   g_Peref.BtnClose.Output = &g_Peref.BtnStatus;
    g_Peref.BtnClose.Level = &BtnLevel;
    g_Peref.BtnClose.Timeout = &BtnTout;
    g_Peref.BtnClose.BitMask = 1<<1;
    g_Peref.BtnClose.Scale = 1;
    g_Peref.BtnClose.Timer = 0;
    g_Peref.BtnClose.Match = false;
    g_Peref.BtnClose.Flag = false;    
  //ручка стоп1-------------------------------------------------------------------------------
    g_Peref.BtnStop1.LogType = ltAnMin;
    g_Peref.BtnStop1.Enable = TRUE;
    g_Peref.BtnStop1.Input = Null;
    g_Peref.BtnStop1.Output = &g_Peref.BtnStatus;
    g_Peref.BtnStop1.Level = &BtnLevel;
    g_Peref.BtnStop1.Timeout = &BtnTout;
    g_Peref.BtnStop1.BitMask = 1<<2;
    g_Peref.BtnStop1.Scale = 1;
    g_Peref.BtnStop1.Timer = 0;
    g_Peref.BtnStop1.Match = false;
    g_Peref.BtnStop1.Flag = false;       
  //ручка стоп2-------------------------------------------------------------------------------
    g_Peref.BtnStop2.LogType = ltAnMin;
    g_Peref.BtnStop2.Enable = TRUE;
    g_Peref.BtnStop2.Input = Null;
    g_Peref.BtnStop2.Output = &g_Peref.BtnStatus;
    g_Peref.BtnStop2.Level = &BtnLevel;
    g_Peref.BtnStop2.Timeout = &BtnTout;
    g_Peref.BtnStop2.BitMask = 1<<3;
    g_Peref.BtnStop2.Scale = 1;
    g_Peref.BtnStop2.Timer = 0;
    g_Peref.BtnStop2.Match = false;
    g_Peref.BtnStop2.Flag = false;         
  //ручка прог1-------------------------------------------------------------------------------
    g_Peref.BtnProg1.LogType = ltAnMin;
    g_Peref.BtnProg1.Enable = TRUE;
    g_Peref.BtnProg1.Input = Null;
    g_Peref.BtnProg1.Output = &g_Peref.BtnStatus;
    g_Peref.BtnProg1.Level = &BtnLevel;
    g_Peref.BtnProg1.Timeout = &BtnTout;
    g_Peref.BtnProg1.BitMask = 1<<4;
    g_Peref.BtnProg1.Scale = 1;
    g_Peref.BtnProg1.Timer = 0;
    g_Peref.BtnProg1.Match = false;
    g_Peref.BtnProg1.Flag = false;          
  //ручка прог2-------------------------------------------------------------------------------
    g_Peref.BtnProg2.LogType = ltAnMin;
    g_Peref.BtnProg2.Enable = TRUE;
    g_Peref.BtnProg2.Input = Null;
    g_Peref.BtnProg2.Output = &g_Peref.BtnStatus;
    g_Peref.BtnProg2.Level = &BtnLevel;
    g_Peref.BtnProg2.Timeout = &BtnTout;
    g_Peref.BtnProg2.BitMask = 1<<5;
    g_Peref.BtnProg2.Scale = 1;
    g_Peref.BtnProg2.Timer = 0;
    g_Peref.BtnProg2.Match = false;
    g_Peref.BtnProg2.Flag = false;              
  
    // включение ТС
  //  g_Peref.TS_Enable = GPIO_PIN_SET;  //ToDo менять если надо 0 enable
    
    // конфигурируем ТУ 
    MCP23S17_init();
     
    peref_ApFilter1Init(&g_Peref.URfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    peref_ApFilter1Init(&g_Peref.USfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    peref_ApFilter1Init(&g_Peref.UTfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    peref_ApFilter1Init(&g_Peref.IUfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    peref_ApFilter1Init(&g_Peref.IVfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    peref_ApFilter1Init(&g_Peref.IWfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    
    peref_ApFilter1Init(&g_Peref.VDCfltr, PRD_18KHZ, g_Ram.FactoryParam.RmsTf);
    
    Peref_SensObserverInit(&g_Peref.sensObserver); // Инициализируем обработку синусойды

    memset(&g_Peref.sinObserver, 0, sizeof(TSinObserver));

    Peref_SinObserverInitFloat(&g_Peref.sinObserver.UR, PRD_18KHZ);
    Peref_SinObserverInitFloat(&g_Peref.sinObserver.US, PRD_18KHZ);
    Peref_SinObserverInitFloat(&g_Peref.sinObserver.UT, PRD_18KHZ);
      
        
    // конфигурируем ADS1118 для токового входа
   
          ADS1118_init(&g_Peref);
   //-------------------------------------------------------
      peref_ADCtoPRCObserverInit1(&g_Peref);
      peref_ADCtoPRCObserverInit1(&g_Peref);
          
}

void peref_ADCtoPRCObserverInit(TPeref *p)
{
                int i = 0;

		 for (i = 0; i<DOTS; i++)
		 {
			p->ADCtoProc.dots[i] = p->dotsADCtoProc[i];
                        p->ADCtoProc.dots[i].adc = g_Ram.FactoryParam.ADCdots[i];
		 }              
}

void peref_ProctoDACObserverInit(TPeref *p)
{
                int i = 0;

		 for (i = 0; i<DOTS; i++)
		 {
			p->ProctoDAC.dots[i] = p->dotsProctoDAC[i];
                        p->ProctoDAC.dots[i].proc = g_Ram.FactoryParam.DACdots[i];
		 }
}

void peref_18KHzCalc(TPeref *p)//
{
   //RST UVW
  
    Peref_SensObserverUpdate(&p->sensObserver);
    //----------------------------------------------------------------------------
    //---отмасштабированные данные с ацп на вход фильтра 1-ого порядка
    p->URfltr.Input = p->sensObserver.URout;
    p->USfltr.Input = p->sensObserver.USout;
    p->UTfltr.Input = p->sensObserver.UTout;
    p->IUfltr.Input = p->sensObserver.IUout;
    p->IVfltr.Input = p->sensObserver.IVout;
    p->IWfltr.Input = p->sensObserver.IWout;
    
    p->VDCfltr.Input = p->sensObserver.VDC_AUout;
    
    peref_ApFilter1Calc(&p->URfltr);
    peref_ApFilter1Calc(&p->USfltr);
    peref_ApFilter1Calc(&p->UTfltr);
    peref_ApFilter1Calc(&p->IUfltr);
    peref_ApFilter1Calc(&p->IVfltr);
    peref_ApFilter1Calc(&p->IWfltr);
    
    peref_ApFilter1Calc(&p->VDCfltr);

    //--------------- RMS угол полярность -----------------------------

    p->sinObserver.UR.Input = p->URfltr.Output;
    p->sinObserver.US.Input = p->USfltr.Output;
    p->sinObserver.UT.Input = p->UTfltr.Output;

    Peref_SinObserverUpdateFloat(&p->sinObserver.UR);
    Peref_SinObserverUpdateFloat(&p->sinObserver.US);
    Peref_SinObserverUpdateFloat(&p->sinObserver.UT);
    
}


void peref_2KHzCalc(TPeref *p)
{
 
      
}

void peref_200HzCalc(TPeref *p)
{
// тест памяти, в основной работе не используеться
         switch (memtemp)
        {
          case 0:  break;
          case 1: 
          {     
            if (IsMemParReady())
            { 
                addr = GetAdr(FactoryParam.ProductYear);
                count = 1;
               ReadWriteEeprom(&Eeprom1,F_WRITE,addr,&g_Ram.FactoryParam.ProductYear,1);
                  memtemp = 0;
            }
          }
          break;   
          case 2: 
          {     
            if (IsMemParReady())
            { 
                addr = GetAdr(FactoryParam.ProductYear);
                count = 1;
                ReadWriteEeprom(&Eeprom1,F_READ,addr,&g_Ram.FactoryParam.ProductYear,1);
                  memtemp = 0;

            }
          }
          break; 
        }
}

Uns TirTimer = 2*PRD_50HZ;
uint32_t hui = 0;

uint8_t DAC_tmp[3];
uint8_t DAC_on_off = 0x02; // по умолчанию токовый выход выключен, его нужно включить в меню при выборе управления АНАЛОГОВОЕ когда подключают токовый вход и выход (выставить 0)

//данные для ADS1118 ----------------

uint8_t ADC_data [2]; 
uint8_t pBuffOut[2];
uint16_t DAC_tmp_16;
//-----------------------------------
void peref_50HzCalc(TPeref *p)
{ 
  
 //TODO переделать на нормальный PowerControl
/*  if (g_Peref.VoltOn == 0) 
  {
    TIM1->CCR4 = 100;
    hui = 0;
  }
  else {
  
    if (g_Peref.VoltOn && TirTimer) 
      TirTimer--;
    if (TirTimer == 0 && TIM1->CCR4 > 0 ) 
      TIM1->CCR4--;
      else 
        if (TirTimer == 0 && TIM1->CCR4 == 0) 
          TIM1->CCR4 = 0;  
    if (hui >= 500) 
        TIM1->CCR4 = 100;
        else 
          hui++;
   }*/
  
  // TU------------------------------------------------------------------------
  //  MCP23S17_update(p);
    
      
   // DAC----------------------------------------------------------------------
        
    DAC_tmp[0] = DAC_on_off; // когда все гуд 0, чтобы выключить и было 1мА надо установить значение 0х02
    DAC_tmp[1] = ((p->DAC_data)>>8);  
    DAC_tmp[2] = (p->DAC_data);  
   
    HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_RESET);    
    HAL_SPI_Transmit(&hspi6,(uint8_t*)DAC_tmp, 3, 100);     
    HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_SET);
      
    DAC_tmp_16 = (DAC_tmp[2] & 0xFF) | ((DAC_tmp[1] & 0xFF) << 8);
     
       //ADC 4-20 мА ---------------------------------------------------------------
     
        
      ADS1118_update(&g_Peref); 
    
 
}

uint8_t murovei = 0, gavno = 0;
void peref_10HzCalc(TPeref *p)//
{
 
  // LED control
  if (g_Ram.TestParam.Mode == 1)
  {
    HAL_GPIO_WritePin(ALARM_SD_GPIO_Port, ALARM_SD_Pin, g_Ram.TestParam.LedsReg.bit.Fault);
    HAL_GPIO_WritePin(MU_DU_SD_GPIO_Port, MU_DU_SD_Pin, g_Ram.TestParam.LedsReg.bit.MuDu);
    HAL_GPIO_WritePin(OPENED_SD_GPIO_Port, OPENED_SD_Pin, g_Ram.TestParam.LedsReg.bit.Opened);
    HAL_GPIO_WritePin(CLOSED_SD_GPIO_Port, CLOSED_SD_Pin, g_Ram.TestParam.LedsReg.bit.Closed);
  }
  else 
  {
    HAL_GPIO_WritePin(ALARM_SD_GPIO_Port, ALARM_SD_Pin, g_Ram.Status.Status.bit.Fault);
    HAL_GPIO_WritePin(MU_DU_SD_GPIO_Port, MU_DU_SD_Pin, g_Ram.Status.Status.bit.MuDu);
    
    if (g_Ram.Status.Status.bit.Opening)
    { 
       HAL_GPIO_TogglePin(OPENED_SD_GPIO_Port, OPENED_SD_Pin);
    }
    else 
    {
      HAL_GPIO_WritePin(OPENED_SD_GPIO_Port, OPENED_SD_Pin, g_Ram.Status.Status.bit.Opened);
    }
      
    if (g_Ram.Status.Status.bit.Closing)
    { 
       HAL_GPIO_TogglePin(CLOSED_SD_GPIO_Port, CLOSED_SD_Pin);
    }
    else 
    {
      HAL_GPIO_WritePin(CLOSED_SD_GPIO_Port, CLOSED_SD_Pin, g_Ram.Status.Status.bit.Closed);
    }
  }
   // передача данных с ручек 
  
    btnOpen  = HAL_GPIO_ReadPin(OPEN_HAL_GPIO_Port, OPEN_HAL_Pin);
    btnClose = HAL_GPIO_ReadPin(CLOSE_HAL_GPIO_Port, CLOSE_HAL_Pin);
    btnStop1 = HAL_GPIO_ReadPin(STOP1_HAL_GPIO_Port, STOP1_HAL_Pin);
    btnStop2 = HAL_GPIO_ReadPin(STOP2_HAL_GPIO_Port, STOP2_HAL_Pin);
    btnProg1 = HAL_GPIO_ReadPin(PROG1_HAL_GPIO_Port, PROG1_HAL_Pin);
    btnProg2 = HAL_GPIO_ReadPin(PROG2_HAL_GPIO_Port, PROG2_HAL_Pin);   
    
    p->BtnOpen.Input  = &btnOpen;
    p->BtnClose.Input = &btnClose;
    p->BtnStop1.Input = &btnStop1;
    p->BtnStop2.Input = &btnStop2;
    p->BtnProg1.Input = &btnProg1;
    p->BtnProg2.Input = &btnProg2;
      
    // temper
  //   ADT7301_Update(&p->Temper);
     p->BlockTemper = *p->Temper.Temper;  
       
     // ten control
     if (p->BlockTemper >= g_Ram.FactoryParam.OnTen) //ToDo подвязять параметр в 
     {
        p->TenControl = 1; // вфключаем иэн
     }
     else if (p->BlockTemper < (g_Ram.FactoryParam.OnTen - 10))
     {
        p->TenControl = 0; // включаем тэн
     }

     HAL_GPIO_WritePin(TEN_OFF_GPIO_Port, TEN_OFF_Pin, p->TenControl);
       
     // volt on control
     p->VoltOn = HAL_GPIO_ReadPin(VOLT_ON_GPIO_Port, VOLT_ON_Pin);
     //
     p->ModFault = HAL_GPIO_ReadPin(Module_Foult_GPIO_Port, Module_Foult_Pin);
     
     p->Modul_Off = 1;
     HAL_GPIO_WritePin(Module_OFF_GPIO_Port, Module_OFF_Pin, p->Modul_Off);
     
     // mcu220
     HAL_GPIO_WritePin(EBU_220_380_MCU_GPIO_Port, EBU_220_380_MCU_Pin, g_Ram.FactoryParam.MCU220380);
     p->Mcu220380 = g_Ram.FactoryParam.MCU220380;
     // ref15
     HAL_GPIO_WritePin(REF1_5_ON_OFF_GPIO_Port, REF1_5_ON_OFF_Pin, g_Ram.FactoryParam.Ref15);
     p->Ref15 = g_Ram.FactoryParam.Ref15;
     
     
      // ADC----------------------------------------------------------------------  
  if (p->Mcu220380 == 1) //220
  {
    g_Ram.Status.Ur  = p->sinObserver.UR.Output;
    g_Ram.Status.Us  = 1;
    g_Ram.Status.Ut  = 1;
  }
  else  //380
  {
    g_Ram.Status.Ur  = p->sinObserver.UR.Output;
    g_Ram.Status.Us  = p->sinObserver.US.Output;
    g_Ram.Status.Ut  = p->sinObserver.UT.Output;
  }
  
  g_Ram.Status.VDC = (Uns)p->VDCfltr.Output;
     
}

void ADT7301_Update(ADT7301 *p)
{
	uint8_t Data[2];
        Uns data16;
          
        Uns TempVal,ADC_Temp_Code_dec;
	
	HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_RESET); 
	
	HAL_SPI_Receive(&hspi1, Data, 2, 10); 
        HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_SET); 
	
        data16 = Data[0]<<8;
        data16 |= Data[1];
       
         /* if ((0x2000 & data16) == 0x2000) //Check sign bit for negative value.
          {
           *p->Temper = (data16 - 16384)/32; //Conversion formula if negative temperature.
           }
          else
           {
           *p->Temper = (data16/32); //Conversion formula if positive temperature.
           }  */
           data16 = data16>>5;
           if (data16 & 0x0100) data16 |= 0xFE00;  
          if (abs(data16) < 100) {*p->Temper = data16; p->ErrTimer = 0;}
          else if (++p->ErrTimer >= p->ErrCount) {p->Error = TRUE; p->ErrTimer = 0;}

}

void LogInputCalc(LOG_INPUT *p)
{
	register Uns Value, Level, Timeout;
	
	if (!p->Enable)
	{
		if (!p->Match) *p->Output &= ~p->BitMask;
		p->Flag = False;
		p->Timer = 0;
		return;
	}

	switch (p->LogType)
	{
	   case ltDigital:
			Level  = ArgToInt(p->Level);
			Value  = *p->Input >> (Level >> 1);
			if ((Value & 0x1) == (Level & 0x1)) goto AINC;
		   break;
	   case ltAnMax:   if (*p->Input > *p->Level) goto AINC; break;
	   case ltAnMin:   if (*p->Input < *p->Level) goto AINC; break;
	   case ltAbs:     if (abs((Int)*p->Input) > *p->Level) goto AINC; break;
	   case ltZone:
			if ((Int)*p->Input >=  (Int)*p->Level) goto AINC;
			if ((Int)*p->Input <= -(Int)*p->Level) break;;
			return;
	   case ltMaxSign: if ((Int)*p->Input > (Int)*p->Level) goto AINC; break;
	   case ltMinSign: if ((Int)*p->Input < (Int)*p->Level) goto AINC; break;
	}
	
	if (p->Timer > 0) p->Timer--;
	else
	{
		if (!p->Match) *p->Output &= ~p->BitMask;
		p->Flag = false;
	}
	return;
	
AINC:
	Timeout = *p->Timeout * p->Scale;
	if (++p->Timer >= Timeout)
	{
		p->Timer = Timeout;
		if (!p->Match || !p->Flag) *p->Output |= p->BitMask;
		p->Flag = true;
	}
}

void RtcControl(void)
{
	static Uns PrevTime;
	static Uns PrevDate;
	TTimeVar *DevTime = &g_Ram.UserParam.DevTime;
	TDateVar *DevDate = &g_Ram.UserParam.DevDate;
	
	if (RtcStart)
	{
            if (DevTime->all != PrevTime)
            {
            //	RTC_SetTime(&Rtc, DevTime, 0);
                    g_Peref.Time.Hours   = DevTime->bit.Hour;
                    g_Peref.Time.Minutes = DevTime->bit.Min;
                    HAL_RTC_SetTime(&hrtc, &g_Peref.Time, RTC_HOURFORMAT_24);
                    RtcUpdateFlag = True;
            }
            if (DevDate->all != PrevDate)
            {
            //	RTC_SetDate(&Rtc, DevDate, 1);
                    g_Peref.Date.Date = DevDate->bit.Day;
                    g_Peref.Date.Month = DevDate->bit.Month;
                    g_Peref.Date.Year = DevDate->bit.Year;
                    HAL_RTC_SetDate(&hrtc, &g_Peref.Date, RTC_FORMAT_BIN);
                    RtcUpdateFlag = True;
            }
            /*if (RTC_TimeCorr(&g_Peref.RtcData, g_Ram.UserParam.TimeCorrection))
            {
                    RtcUpdateFlag = True;
            }*/
	}

	if (!RtcUpdateFlag)
	{
            HAL_RTC_GetTime(&hrtc, &g_Peref.Time, RTC_HOURFORMAT_24);
            HAL_RTC_GetDate(&hrtc, &g_Peref.Date, RTC_FORMAT_BIN);
            g_Ram.HideParam.Seconds = g_Peref.Time.Seconds;
            DevTime->bit.Hour = g_Peref.Time.Hours;
            DevTime->bit.Min = g_Peref.Time.Minutes;
            DevDate->bit.Day = g_Peref.Date.Date;
            DevDate->bit.Year = g_Peref.Date.Year;
            DevDate->bit.Month = g_Peref.Date.Month;
	}

	PrevTime = DevTime->all;
	PrevDate = DevDate->all;
	RtcStart = True;
        RtcUpdateFlag = false;
}

    // 74HC595D Драйвер ТС----------------------------------------------------
void peref_74HC595D(TPeref *p)
{
    static Uns TuEnbReleTimer;
 
    if (g_Ram.HideParam.HideStateTs.all != g_Ram.Status.StateTs.all)
    {
        HAL_GPIO_WritePin(ENB_TC_GPIO_Port, ENB_TC_Pin, GPIO_PIN_RESET);
          
        g_Ram.Status.StateTs.all = g_Ram.HideParam.HideStateTs.all;
        p->TS_outData = g_Ram.HideParam.HideStateTs.all;
        HAL_GPIO_WritePin(CS_TC_GPIO_Port, CS_TC_Pin, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi6, &p->TS_outData, 1, 10);
        HAL_GPIO_WritePin(CS_TC_GPIO_Port, CS_TC_Pin, GPIO_PIN_SET);  
          
        //    TuEnbReleTimer = (0.3 * PRD_10HZ);
    }
    	//if(TuEnbReleTimer > 0) TuEnbReleTimer--;
	//else if(TuEnbReleTimer == 0 && (HAL_GPIO_ReadPin(ENB_TC_GPIO_Port, ENB_TC_Pin) == 0)) HAL_GPIO_WritePin(ENB_TC_GPIO_Port, ENB_TC_Pin, GPIO_PIN_SET);

}
 //MCP23S17 драйвер ТУ --------------------------------------------------------
void MCP23S17_write(uint8_t addr, uint8_t data)
{
    uint8_t pBuff[3];
      
    pBuff[0] = MCPS17_ADDRESS|MCPS17_AAA|MCPS17_WRITE;
    pBuff[1] = addr;
    pBuff[2] = data;
      
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&hspi6, (uint8_t*)pBuff, 3, 100);
        
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_SET);
}

uint8_t MCP23S17_read(uint8_t addr)
{
    uint8_t pBuff[2], data[1];
      
    pBuff[0] = MCPS17_ADDRESS|MCPS17_AAA|MCPS17_READ;
    pBuff[1] = addr;
      
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&hspi6, (uint8_t*)pBuff, 2, 100);
    HAL_SPI_Receive(&hspi6, (uint8_t*)data, 1, 100);
        
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_SET);
      
    return ~data[0] & 0x1F ;
}

void MCP23S17_init(void)
{
    MCP23S17_write(MCPS17_IODIRA, 0x1f); // порт А и В входы
    MCP23S17_write(MCPS17_IODIRB, 0x1f);
}

void MCP23S17_update(TPeref *p)
{
    p->TU_data220 = MCP23S17_read(MCPS17_GPIOA);
    p->TU_data24  = MCP23S17_read(MCPS17_GPIOB);
}

void ADS1118_init(TPeref *p)
{
    
      
  p->ADC_Out_Config.bit.OS = 0;
    p->ADC_Out_Config.bit.MUX =4;
      p->ADC_Out_Config.bit.PGA = 2;
        p->ADC_Out_Config.bit.MODE = 0;
          p->ADC_Out_Config.bit.DR = 7; //111
          p->ADC_Out_Config.bit.TS_MODE = 0;
            p->ADC_Out_Config.bit.PULL_UP_EN = 0;
              p->ADC_Out_Config.bit.NOP = 0x1;
                p->ADC_Out_Config.bit.CNV_RDY_FL = 0;
                  
 pADS1118_ConfRegData[1] = g_Peref.ADC_Out_Config.all & 0xFF;
 pADS1118_ConfRegData[0] = (g_Peref.ADC_Out_Config.all>>8) & 0xFF;
 
  HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi6, (uint8_t*)pADS1118_ConfRegData,  2, 100);
  HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_SET);
                      
}


void ADS1118_update(TPeref *p)
{
   uint8_t pBuffIn[2]; 
   uint8_t pBuffOut[2]; 
   pBuffOut[1] = g_Peref.ADC_Out_Config.all & 0xFF;
   pBuffOut[0] = (g_Peref.ADC_Out_Config.all>>8) & 0xFF;
     
   HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_RESET);
   HAL_SPI_TransmitReceive(&hspi6, (uint8_t*)pBuffOut, (uint8_t*)pBuffIn, 2, 100);
   HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_SET);
  
   g_Peref.ADC_Out_data = (pBuffIn[1] & 0xFF) | ((pBuffIn[0] & 0xFF) << 8);
}


// Чисто моя функция инита--------------------------------
void ADS_init (void)
{
  uint8_t pBuff[2];
    pBuff[1]=0xE2;
      pBuff[0]=0x42;
        
          HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_RESET);
            HAL_SPI_Transmit(&hspi6, (uint8_t*)pBuff, 2, 100);
              HAL_GPIO_WritePin(CS_Iin_GPIO_Port, CS_Iin_Pin, GPIO_PIN_SET);
}

      

