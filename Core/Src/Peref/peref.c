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
    g_Peref.TS_Enable = GPIO_PIN_RESET;  //ToDo менять если надо 0 enable
    
    // конфигурируем ТУ 
    MCP23S17_init();
}
extern uint16_t adc[6];

uint32_t hui, murovei, gavno = 0, temp666, temp777 = 0;

void peref_18KHzCalc(TPeref *p)//
{
    temp666 =  (31000 * adc[0]) / 65535;
  if (temp666 > murovei)
  {
    murovei = temp666;
    temp777 = 0;}
      else
      {
        if (temp777 > 10)
{
         
        temp777 = 0;
         if (gavno <= 40)
          {
            
            hui += (31000 * adc[0]) / 65535;
            gavno ++;
          }
            else 
            {
              gavno = 0;
              hui = hui / 40;
                g_Ram.Status.Ur = hui; 
                hui = 0;
               // murovei = 0;
            }
}
else temp777++;
      
      }
}


void peref_2KHzCalc(TPeref *p)
{
 
      
}

void peref_200HzCalc(TPeref *p)
{

         switch (memtemp)
        {
          case 0:  break;
          case 1: 
          {     
            if (IsMemParReady())
            { 
                addr = GetAdr(FactoryParam.ProductYear);
                count = 1;
               ReadWriteEeprom(&Eeprom1,F_WRITE,addr,&Data,1);
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
                ReadWriteEeprom(&Eeprom1,F_READ,addr,&Data,1);
                  memtemp = 0;

            }
          }
          break; 
        }
}


void peref_50HzCalc(TPeref *p)
{ 
  uint8_t DAC_tmp[2];
 
   // TC ----------------------------------------------------------------------
    /*  g_Ram.Status.StateTs.all = (1<<TmpTC);  // перебор ТС раз в 2 секунды
   if (TmpTC> 7) TmpTC = 0;
   if (TimerTC++ >= PRD_50HZ*2) 
   {
      TmpTC++; 
      TimerTC = 0;
   }*/
  HAL_GPIO_WritePin(ENB_TC_GPIO_Port, ENB_TC_Pin, p->TS_Enable);
  peref_74HC595D(p);
  
  // TU------------------------------------------------------------------------
  MCP23S17_update(p);
   // DAC----------------------------------------------------------------------
  HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_RESET);    
  HAL_SPI_Receive(&hspi6,(uint8_t*)DAC_tmp, 2, 100);    
  HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_SET);
  
  p->DAC_data = DAC_tmp[0];
  p->DAC_data |= DAC_tmp[1]<<8;
    
  // ADC----------------------------------------------------------------------
      
   
}

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
     ADT7301_Update(&p->Temper);
     p->BlockTemper = *p->Temper.Temper;  
       
     // ten control
     if (p->BlockTemper >= 30) //ToDo подвязять параметр в 
     {
        HAL_GPIO_WritePin(TEN_OFF_GPIO_Port, TEN_OFF_Pin, GPIO_PIN_SET);
     }
     else HAL_GPIO_WritePin(TEN_OFF_GPIO_Port, TEN_OFF_Pin, GPIO_PIN_RESET);
       
     // volt on control
     p->VoltOn = HAL_GPIO_ReadPin(VOLT_ON_GPIO_Port, VOLT_ON_Pin);
}

void ADT7301_Update(ADT7301 *p)
{
	uint8_t Data[2];
        Int data16;
          
            Uns TempVal,ADC_Temp_Code_dec;
	
	HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_RESET); 
	
	HAL_SPI_Receive(&hspi1, Data, 2, 10); 
          
        DELAY_US(3);
        HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_SET); 
	
        data16 = Data[0]<<8;
        data16 |= Data[1];
       
          if ((0x2000 & data16) == 0x2000) //Check sign bit for negative value.
          {
           *p->Temper = (data16 - 16384)/32; //Conversion formula if negative temperature.
           }
          else
           {
           *p->Temper = (data16/32); //Conversion formula if positive temperature.
           }  
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

    p->TS_outData = g_Ram.Status.StateTs.all & 0xFF;

    
      
    HAL_SPI_Transmit(&hspi6, &p->TS_outData, 1, 100);
    HAL_GPIO_WritePin(CS_TC_GPIO_Port, CS_TC_Pin, GPIO_PIN_SET); 
    HAL_GPIO_WritePin(CS_TC_GPIO_Port, CS_TC_Pin, GPIO_PIN_RESET);  
    
    
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
    p->ADC_Out_Config.bit.MODE = 0;
}
