/*--------------------------
	perf.c

--------------------------*/

#include "peref.h"


TPeref g_Peref;

//----------------------------------ram test
uint16_t memtemp = 0;
uint16_t addr = 0;
uint16_t addrstatus = 0;
uint16_t count = 0;
//-----------------------------------
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

Uns TirTimer = 2*PRD_50HZ;

uint8_t DAC_tmp[3];
// по умолчанию токовый выход выключен, его нужно включить в меню при выборе управления АНАЛОГОВОЕ когда подключают токовый вход и выход (выставить 0)

//данные для ADS1118 ----------------

uint8_t ADC_data[2]; 
uint8_t pBuffOut[2];
uint16_t DAC_tmp_16;

FDCAN_ProtocolStatusTypeDef FDCANStatus = NULL;
FDCAN_ErrorCountersTypeDef FDCANerrCount = NULL;

// входное АЦП в проценты
//----------------------------------------------------------
// Точки 		     проценты.  АЦП.             4-20.    №
TDot dotsADC[DOTS] = {	        0,      744,		//  4		0
                                31,     842,            //  4.5
                                62,     940,		//  5		1
                                93,     1040,           //  5.5
                                125,    1136,		//  6		2
                                156,    1232,           //  6.5
                                187,    1330,		//  7		3
                                218,    1425,           //  7.5
                                250,    1520,		//  8	        4
                                281,    1614,           //  8.5
                                312,    1710,		//  9	        5
                                343,    1803,           //  9.5
                                375,    1898,		//  10	        6
                                406,    1993,           //  10.5
                                437,    2085,		//  11          7
                                468,    2183,           //  11.5
                                500,    2276,		//  12          8
                                531,    2373,           //  12.5
                                562,    2465,		//  13  9
                                593,    2560,           //  13.5
                                625,    2655,		//  14  10
                                656,    2751,           //  14.5
                                687,    2845,		//  15  11
                                718,    2939,           //  15.5
                                750,    3033,		//  16  12
                                781,    3128,           //  16.5
                                812,    3221,		//  17  13
                                843,    3317,           //  17.5
                                875,    3409,		//  18  14
                                906,    3506,           //  18.5
                                937,    3598,		//  19  15
                                968,    3695,           //  19.5
                                1000,   3785};		//  20  16		
//--------------------------------------------------------
// Точки 		  	ЦАП.  Проценты.            4-20.    №
TDot dotsDAC[DOTS] = {	        484,      0,		//  4		0
                                548,      31,
                                611,      62,		//  5		1
                                672,      93,
                                733,      125,		//  6		2
                                794,      156,
                                853,      187,		//  7		3
                                912,      218,
                                972,      250,		//  8	        4
                                1032,     281,
                                1093,     312,		//  9	        5
                                1157,     343,
                                1224,     375,		//  10	        6
                                1290,     406,
                                1357,     437,		//  11          7
                                1424,     468,
                                1489,     500,		//  12          8
                                1552,     531,
                                1616,     562,		//  13  9
                                1680,     593,
                                1743,     625,		//  14  10
                                1806,     656,
                                1870,     687,		//  15  11
                                1932,     718,
                                1996,     750,		//  16  12
                                2059,     781,
                                2123,     812,		//  17  13
                                2186,     843,
                                2248,     875,		//  18  14
                                2310,     906,
                                2373,     937,		//  19  15
                                2436,     968,
                                2498,     1000};	//  20  16		
//--------------------------------------------------------
// Точки 		  темпер.  АЦП.        сопр.    №
TDotTemper dotsTemper[8] = {	-40,   465,		//  525		0
                        0,     585,		//  577		1
                        25,    642,		//  1379	2
                        50,    687,		//  1569	3
                        80,    731,		//  1670	4
                        100,   773,		//  1993	5
                        110,   834,		//  2471	6
                        120,   854};	       //  2535	7


// Точки 		  темпер.  АЦП.        сопр.    №
TDotTemper dotsTemperModule[8] = {	-40,   465,		//  525		0
                                          0,     585,		//  577		1
                                          25,    642,		//  1379	2
                                          50,    687,		//  1569	3
                                          80,    731,		//  1670	4
                                          100,   773,		//  1993	5
                                          110,   834,		//  2471	6
                                          120,   854};	       //  2535	7


char Icons[NUM_ICONS][7] =	{
					0x1F,0x11,0x1F,0x04,0x06,0x04,0x07,
					0x1F,0x11,0x11,0x1F,0x04,0x1F,0x00,
					0x06,0x15,0x0D,0x06,0x0D,0x015,0x06,
					0x0E,0x11,0x11,0x11,0x1F,0x1B,0x1F,
					0x0E,0x11,0x10,0x10,0x1F,0x1B,0x1F,
				};

void SPIReinit(void)
{
    if ((g_Ram.UserParam.DuSource == mdsDigital && hspi6.Init.CLKPhase != SPI_PHASE_1EDGE )||(g_Ram.UserParam.DuSource == mdsDac && hspi6.Init.CLKPhase != SPI_PHASE_2EDGE ))
    {
          HAL_SPI_DeInit(&hspi6);
          
          hspi6.Instance = SPI6;
          hspi6.Init.Mode = SPI_MODE_MASTER;
          hspi6.Init.Direction = SPI_DIRECTION_2LINES;
          hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
          hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
           if (g_Ram.UserParam.DuSource == mdsDigital)
           {
            hspi6.Init.CLKPhase = SPI_PHASE_1EDGE;
           }
           else if (g_Ram.UserParam.DuSource == mdsDac) 
           {
            hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;
           }
          hspi6.Init.NSS = SPI_NSS_SOFT;
          hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
          hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
          hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
          hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
          hspi6.Init.CRCPolynomial = 0x0;
          hspi6.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
          hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
          hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
          hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
          hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
          hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
          hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
          hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
          hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
          hspi6.Init.IOSwap = SPI_IO_SWAP_DISABLE;
          if (HAL_SPI_Init(&hspi6) != HAL_OK)
          {
            Error_Handler();
          }    
    }
}

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
      
     // токи  
  //  g_Core.Mash1 = 1;
   // g_Core.Mash3 = 1;      
            
    g_Peref.Ia.Input =  0;  
    g_Peref.Ia.Signal = 0;
    g_Peref.Ia.Output = 0;
    g_Peref.Ia.Ramp = &g_Core.rg1.Out;
    g_Peref.Ia.RampPrev = 0;
    g_Peref.Ia.Mash1 = &g_Core.Mash1;
   // g_Peref.Ia.Mash2 = 1;
    g_Peref.Ia.Mash3 = &g_Core.Mash3;
    g_Peref.Ia.Sum = 0;
    g_Peref.Ia.Sum1 = 0;
    g_Peref.Ia.Counter = 0;
      
    g_Peref.Ib.Input =  0;  
    g_Peref.Ib.Signal = 0;
    g_Peref.Ib.Output = 0;
    g_Peref.Ib.Ramp = &g_Core.rg1.Out;
    g_Peref.Ib.RampPrev = 0;
    g_Peref.Ib.Mash1 = &g_Core.Mash1;
 //   g_Peref.Ib.Mash2 = 1;
    g_Peref.Ib.Mash3 = &g_Core.Mash3;
    g_Peref.Ib.Sum = 0;
    g_Peref.Ib.Sum1 = 0;
    g_Peref.Ib.Counter = 0;
         
    g_Peref.Ic.Input =  0;  
    g_Peref.Ic.Signal = 0;
    g_Peref.Ic.Output = 0;
    g_Peref.Ic.Ramp = &g_Core.rg1.Out;
    g_Peref.Ic.RampPrev = 0;
    g_Peref.Ic.Mash1 = &g_Core.Mash1;
  //  g_Peref.Ic.Mash2 = 1;
    g_Peref.Ic.Mash3 = &g_Core.Mash3;
    g_Peref.Ic.Sum = 0;
    g_Peref.Ic.Sum1 = 0;
    g_Peref.Ic.Counter = 0;
   
     //---------------------------------------
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
      peref_ApFilter1Init(&g_Peref.ADCToProcfltr, PRD_50HZ, g_Ram.FactoryParam.RmsTf);
      peref_ADCtoPRCObserverInit(&g_Peref);
        peref_ProctoDACObserverInit(&g_Peref);
    //энкодер
          Cms58mInir(&g_Peref.cms58m_1);
        
      Peref_CalibInit(&g_Peref.Position);
    ///  g_Ram.HideParam.Position ///= &g_Peref.cms58m_1.value;

      peref_ApFilter1Init(&g_Peref.TEMPERfltr, PRD_10HZ, g_Ram.FactoryParam.RmsTf); 
      TempObserverInit(&g_Peref.temperDrive);
      TempObserverInit(&g_Peref.temperModule);  
      
        
          g_Peref.DAC_on_off = 0x02; 
}

void peref_ADCtoPRCObserverInit(TPeref *p)
{
  int i = 0;
  int j = DOTS;
    
      if (g_Ram.FactoryParam.ReversADC == 0){
        for (i = 0; i<DOTS; i++)
        {
          p->ADCtoProc.dots[i] = dotsADC[i];
            p->ADCtoProc.dots[i].adc = g_Ram.FactoryParam.ADCdots[i];
        } 
      }else {
         for (i = 0; i<DOTS; i++)
        {
          j--;
          p->ADCtoProc.dots[i] = dotsADC[i];
            p->ADCtoProc.dots[i].adc = g_Ram.FactoryParam.ADCdots[j];
        } 
      }
}

void peref_ProctoDACObserverInit(TPeref *p)
{
  int i = 0;
  int j = DOTS;
      
     if (g_Ram.FactoryParam.ReversDAC == 0)
     {
      for (i = 0; i<DOTS; i++)
      {
        
        p->ProctoDAC.dots[i] = dotsDAC[i];
          p->ProctoDAC.dots[i].proc = g_Ram.FactoryParam.DACdots[i];
      }
     } else{
          for (i = 0; i<DOTS; i++)
      {
        j--;
        p->ProctoDAC.dots[i] = dotsDAC[i];
          p->ProctoDAC.dots[i].proc = g_Ram.FactoryParam.DACdots[j];
      }
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
      
    p->Ia.Input =  p->IUfltr.Output;   
    p->Ib.Input =  p->IVfltr.Output;  
    p->Ic.Input =  p->IWfltr.Output; 
      
        ileg_fq_calc(&p->Ia);
        ileg_fq_calc(&p->Ib);
        ileg_fq_calc(&p->Ic);
    
    
}

void peref_2KHzCalc(TPeref *p)
{
 
      
}

int32  tmpDelta = 0;
int32  tmpDelta2 = 0;
int32  tmpDelta3 = 0;
int32  tmpData = 0;
int32  tmpData2 = 2;
Uns    flag = 0;
int32 max = 100;

void peref_200HzCalc(TPeref *p)
{
  
     HAL_FDCAN_GetErrorCounters(&hfdcan2, &FDCANerrCount);      
      HAL_FDCAN_GetProtocolStatus(&hfdcan2, &FDCANStatus);
      
         Cms58mRxHandler(&p->cms58m_1);
         if (FDCANerrCount.RxErrorPassive)
         g_Core.Protections.outFaults.Dev.bit.PosSens = 1; 
         else 
         g_Ram.HideParam.Position = p->cms58m_1.value;



        
      /*   if (tmpData != p->cms58m_1.value && tmpData2 != tmpData && tmpData2 != p->cms58m_1.value) 
         {
            tmpData = p->cms58m_1.value;
            tmpData2 = p->cms58m_1.value;
         }
         else if (tmpData == tmpData2 && tmpData != p->cms58m_1.value) 
         {
             tmpData = p->cms58m_1.value;
         }*/
           
             tmpDelta = labs(tmpData - p->cms58m_1.value);
              tmpDelta2 = labs(tmpData2 - p->cms58m_1.value);
                tmpDelta3 = labs(tmpData - tmpData2);
             
         if ((tmpDelta > max) && ( tmpDelta2> max)&&(tmpDelta3 > max))
         {
            tmpData = p->cms58m_1.value;
            tmpData2 = p->cms58m_1.value;
         }
         else if (tmpData == tmpData2 && tmpData != p->cms58m_1.value) 
         {
             tmpData = p->cms58m_1.value;
         }
    
}

//-----------------------------------
void peref_50HzCalc(TPeref *p)
{ 
   Int tmpDACData; 
      
     //ADC 4-20 мА ---------------------------------------------------------------
     ADS1118_update(&g_Peref);  // считали ацп
    //--АДЦ в проценты--------------------------------------------------------------
      
  if (p->ADC_Out_data < 2500){
  
    if (g_Core.VlvDrvCtrl.Status->bit.MuDu == 0 && g_Ram.UserParam.DuSource == mdsDac)
      g_Core.Protections.outFaults.Proc.bit.DAC_no_con = 1; 
    
  }
  else if (g_Core.VlvDrvCtrl.MuDuInput == 0 && g_Ram.UserParam.DuSource == mdsDac) {    
    g_Core.Protections.outFaults.Proc.bit.DAC_no_con = 0;
   p->ADCToProcfltr.Input = (float)p->ADC_Out_data;    // отдали в фильтр Uns в float
   peref_ApFilter1Calc(&p->ADCToProcfltr);             // пофильтровали
   p->ADCtoProc.input = (Uns)p->ADCToProcfltr.Output;  // отдали в интерполяцию float в Uns
    peref_ADCDACtoPRCObserverUpdate(&g_Peref.ADCtoProc); //посчитали интерполяцию
   g_Ram.UserParam.SetPosition = g_Peref.ADCtoProc.output;
  }
     //g_Peref.ProctoDAC.input = g_Peref.ADCtoProc.output; //!!!!!!!!!! заглушка
     if (g_Ram.Status.CalibState == csCalib){
        tmpDACData = g_Ram.Status.PositionPr;     
        if (tmpDACData > 1000) tmpDACData = 10000;
        if (tmpDACData < 0) tmpDACData = 0;
        g_Peref.ProctoDAC.input = (Uns)tmpDACData; 
     }
     else{ 
      g_Peref.ProctoDAC.input = 1; 
    }
    //-- проценты в ЦАП--------------------------------------------------------------
    peref_ADCDACtoPRCObserverUpdate(&g_Peref.ProctoDAC);      
   // DAC----------------------------------------------------------------------
   
    p->DAC_data = p->ProctoDAC.output;
      
    DAC_tmp[0] = g_Peref.DAC_on_off; // когда все гуд 0, чтобы выключить и было 1мА надо установить значение 0х02
    DAC_tmp[1] = ((p->DAC_data)>>8);  
    DAC_tmp[2] = (p->DAC_data);  
   
    HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_RESET);    
    HAL_SPI_Transmit(&hspi6,(uint8_t*)DAC_tmp, 3, 100);     
    HAL_GPIO_WritePin(CS_Iout_GPIO_Port, CS_Iout_Pin, GPIO_PIN_SET);
      
    DAC_tmp_16 = (DAC_tmp[2] & 0xFF) | ((DAC_tmp[1] & 0xFF) << 8);
   // TU------------------------------------------------------------------------
  //  MCP23S17_update(p);
     


}

void peref_10HzCalc(TPeref *p)//
{  
   
   if (g_Core.Protections.FaultDelay > 0) return; 
     
   SPIReinit();
  // температура двигателя---------------------------------------------------------------   
   p->TEMPERfltr.Input = (float)p->adcData1[4];
   peref_ApFilter1Calc(&p->ADCToProcfltr);      
   p->temperDrive.inputR = (Uns)p->TEMPERfltr.Output;
   TempObserverUpdate(&p->temperDrive);
   g_Ram.Status.DriveTemper = p->temperDrive.outputT;  
    
   // температура модуля ---------------------------------------------------------------------   
   p->temperModule.inputR =  (float)p->adcData1[5];    
   TempObserverUpdate(&p->temperModule);
   g_Ram.Status.ModuleTempers = p->temperModule.outputT;  
  // LED control--------------------------------------------------------------------------
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
   // передача данных с ручек ---------------------------------------------------------------------
  
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
   //  ADT7301_Update(&p->Temper);
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
    
      MCP23S17_update(&g_Peref);
     
 // PrdElemInit(Peref_Calibration, &g_Peref.Position);
 // PrdElemInit(CalcClbCycle, &g_Peref.Position);
 // PrdElemInit(Peref_SpeedCalc, &g_Peref.Position);
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
    
    HAL_SPI_Transmit(&hspi6, (uint8_t*)pBuff, 3, 10);
        
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_SET);
}

uint8_t MCP23S17_read(uint8_t addr)
{
    uint8_t pBuff[2], data[1];
      
    pBuff[0] = MCPS17_ADDRESS|MCPS17_AAA|MCPS17_READ;
    pBuff[1] = addr;
      
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(&hspi6, (uint8_t*)pBuff, 2, 10);
    HAL_SPI_Receive(&hspi6, (uint8_t*)data, 1, 10);
        
    HAL_GPIO_WritePin(CS_TU_GPIO_Port, CS_TU_Pin, GPIO_PIN_SET);
      
        return data[0]^0x1f;
}

void MCP23S17_init(void)
{
    MCP23S17_write(MCPS17_IODIRA, 0xff); // порт А и В входы
    MCP23S17_write(MCPS17_IODIRB, 0xff);
}

void MCP23S17_update(TPeref *p)
{
    p->TU_data220 = MCP23S17_read(MCPS17_GPIOA) & NUM_OF_INPUT_TU;
    p->TU_data24  = MCP23S17_read(MCPS17_GPIOB) & NUM_OF_INPUT_TU;
   
}

void ADS1118_init(TPeref *p)
{
  
  p->ADC_Out_Config.bit.OS = 0;
    p->ADC_Out_Config.bit.MUX =4;
      p->ADC_Out_Config.bit.PGA = 0;
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

void peref_ADCDACtoPRCObserverUpdate(TLineObserver *p)
{
	static Int i=0;

        if (p->input <= p->dots[0].adc) 
        {
              p->output = p->dots[0].proc; 
              return;
        } 
          

        if (p->input >= p->dots[DOTS-1].adc)
        {
             p->output = p->dots[DOTS-1].proc;
             return;
        } 
          
         
        
	// Определяем, между какими значениями dots находится R_входное
	//while (! ((p->inputR >= p->dots[i].resist)&&(p->inputR < p->dots[i+1].resist)) )	// Для сопротивления (прямая зависимость)
	while (! ((p->input >= p->dots[i].adc)&&(p->input <= p->dots[i+1].adc)) )	// Для АЦП (обратная зависимость)
	{
		if (p->input > p->dots[i].adc)
		{
			i++;	// Движемся по характеристике вверх и вниз
			//if(i > 7) i = 7;
		}
		else
		{
			i--;
		//	if(i < -1) i = -1;
		}							// пока не окажемся между двумя точками
	}
	
	if (i > DOTS) i = DOTS;
	else if (i < 0) i = 0;

	if (p->input == p->dots[i].adc)			// Если четко попали на точку
		p->output = p->dots[i].proc;		// берем значение температуры этой точки
	else// Линейная интерполяция			   в противном случае интерполируем
		p->output = LinearInterpolation(p->dots[i].adc, p->dots[i].proc ,p->dots[i+1].adc ,p->dots[i+1].proc, p->input);
}

void memTest(void)
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

void TempObserverInit(TTempObserver *p)
{
	int i = 0;
	
          for (i = 0; i<8; i++)
          {
            p->dots[i] = dotsTemper[i]; 
          }
           p->maxResist = 921;  // число максимума 
	
	
}

//--------------------------------------------------------
void TempObserverUpdate(TTempObserver *p)
{
	static Int i=0;

	// Значение АЦП меньше минимально-допустимого или больше минимально-допустимого
	if (p->inputR <= p->dots[0].adc)
	{
		p->fault = false;
		p->outputT = -999;
		return;
	}
	else if (p->inputR >= p->maxResist )
	{
		p->fault = true;					// это значит обрыв датчика температуры или его сбой
		p->outputT = 999;
		return;		
	}
	else 									// Значение АЦП в пределах
		p->fault = false;					// - нет аварии 

	// Определяем, между какими значениями dots находится R_входное
	//while (! ((p->inputR >= p->dots[i].resist)&&(p->inputR < p->dots[i+1].resist)) )	// Для сопротивления (прямая зависимость)
	while (! ((p->inputR >= p->dots[i].adc)&&(p->inputR < p->dots[i+1].adc)) )	// Для АЦП (обратная зависимость)
	{
		if (p->inputR > p->dots[i].adc)
		{
			i++;	// Движемся по характеристике вверх и вниз
		//	if(i > 7) i = 7;
		}
		else
		{
			i--;
		//	if(i < -1) i = -1;
		}							// пока не окажемся между двумя точками
	}
	
	if (i > 8) i = 8;
	else if (i < 0) i = 0;

	if (p->inputR == p->dots[i].adc)			// Если четко попали на точку
		p->outputT = p->dots[i].temper;		// берем значение температуры этой точки
	else// Линейная интерполяция			   в противном случае интерполируем
		p->outputT = LinearInterpolation(p->dots[i].adc, p->dots[i].temper ,p->dots[i+1].adc ,p->dots[i+1].temper, p->inputR);
}



