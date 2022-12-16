/*======================================================================
Имя файла:         	aci.c
Автор:             	
				   	  	
Версия файла:      	01.00
Дата изменения:		09/12/22
======================================================================*/

#include "vector.h"

TVector	                g_Vector;


void Vector_Init(TVector *p)
{
  p->bMCBootCompleted = 0;  // 
  

  p->CircleLimitationM1.MaxModule  = MAX_MODULE,
  p->CircleLimitationM1.MaxVd      = (uint16_t)(MAX_MODULE * 950 / 1000),
  
  
  /**********************************************************/
    /*    PWM and current sensing component initialization    */
    /**********************************************************/
   
  
  
  //  pwmcHandle[M1] = &PWM_Handle_M1._Super;
  //  R3_2_Init(&PWM_Handle_M1);
  //  ASPEP_start(&aspepOverUartA); -->>> хз что это за интерфейс и для чего он нужен

    /**************************************/
    /*    Start timers synchronously      */
    /**************************************/
  //  startTimers();

    /******************************************************/
    /*   PID component initialization: speed regulation   */
    /******************************************************/
  p->PIDSpeedHandle_M1.hDefKpGain          = (int16_t)PID_SPEED_KP_DEFAULT;
  p->PIDSpeedHandle_M1.hDefKiGain          = (int16_t)PID_SPEED_KI_DEFAULT;
  p->PIDSpeedHandle_M1.wUpperIntegralLimit = (int32_t)IQMAX * (int32_t)SP_KIDIV;
  p->PIDSpeedHandle_M1.wLowerIntegralLimit = -(int32_t)IQMAX * (int32_t)SP_KIDIV;
  p->PIDSpeedHandle_M1.hUpperOutputLimit       = (int16_t)IQMAX;
  p->PIDSpeedHandle_M1.hLowerOutputLimit       = -(int16_t)IQMAX;
  p->PIDSpeedHandle_M1.hKpDivisor          = (uint16_t)SP_KPDIV;
  p->PIDSpeedHandle_M1.hKiDivisor          = (uint16_t)SP_KIDIV;
  p->PIDSpeedHandle_M1.hKpDivisorPOW2      = (uint16_t)SP_KPDIV_LOG;
  p->PIDSpeedHandle_M1.hKiDivisorPOW2      = (uint16_t)SP_KIDIV_LOG;
  p->PIDSpeedHandle_M1.hDefKdGain           = 0x0000U;
  p->PIDSpeedHandle_M1.hKdDivisor           = 0x0000U;
  p->PIDSpeedHandle_M1.hKdDivisorPOW2       = 0x0000U;
  
  PID_HandleInit(&p->PIDSpeedHandle_M1);
    /*s*****************************************************/
    /*   Main speed sensor component initialization       */
    /******************************************************/
/*
    p->STO_PLL_M1._Super.bElToMecRatio                     =	POLE_PAIR_NUM;
    p->STO_PLL_M1._Super.SpeedUnit                         =    SPEED_UNIT;
    p->STO_PLL_M1._Super.hMaxReliableMecSpeedUnit          =	(uint16_t)(1.15*MAX_APPLICATION_SPEED_UNIT);
    p->STO_PLL_M1._Super.hMinReliableMecSpeedUnit          =	(uint16_t)(MIN_APPLICATION_SPEED_UNIT);
    p->STO_PLL_M1._Super.bMaximumSpeedErrorsNumber         =	MEAS_ERRORS_BEFORE_FAULTS;
    p->STO_PLL_M1._Super.hMaxReliableMecAccelUnitP         =	65535;
    p->STO_PLL_M1._Super.hMeasurementFrequency             =	TF_REGULATION_RATE_SCALED;
    p->STO_PLL_M1._Super.DPPConvFactor                     =    DPP_CONV_FACTOR;

 p->STO_PLL_M1.hC1                         =	C1;
 p->STO_PLL_M1.hC2                         =	C2;
 p->STO_PLL_M1.hC3                         =	C3;
 p->STO_PLL_M1.hC4                         =	C4;
 p->STO_PLL_M1.hC5                         =	C5;
 p->STO_PLL_M1.hF1                         =	F1;
 p->STO_PLL_M1.hF2                         =	F2;
 
     p->STO_PLL_M1.PIRegulator.hDefKpGain = PLL_KP_GAIN;
    p->STO_PLL_M1.PIRegulator .hDefKiGain = PLL_KI_GAIN;
	 p->STO_PLL_M1.PIRegulator.hDefKdGain = 0x0000U;
     p->STO_PLL_M1.PIRegulator.hKpDivisor = PLL_KPDIV;
     p->STO_PLL_M1.PIRegulator.hKiDivisor = PLL_KIDIV;
	 p->STO_PLL_M1.PIRegulator.hKdDivisor = 0x0000U;
     p->STO_PLL_M1.PIRegulator.wUpperIntegralLimit = INT32_MAX;
     p->STO_PLL_M1.PIRegulator.wLowerIntegralLimit = -INT32_MAX;
     p->STO_PLL_M1.PIRegulator.hUpperOutputLimit = INT16_MAX;
     p->STO_PLL_M1.PIRegulator.hLowerOutputLimit = -INT16_MAX;
     p->STO_PLL_M1.PIRegulator.hKpDivisorPOW2 = PLL_KPDIV_LOG;
     p->STO_PLL_M1.PIRegulator.hKiDivisorPOW2 = PLL_KIDIV_LOG;
     p->STO_PLL_M1.PIRegulator.hKdDivisorPOW2       = 0x0000U;

 p->STO_PLL_M1.SpeedBufferSizeUnit                =	STO_FIFO_DEPTH_UNIT;
 p->STO_PLL_M1.SpeedBufferSizeDpp                 =	STO_FIFO_DEPTH_DPP;
 p->STO_PLL_M1.VariancePercentage                 =	PERCENTAGE_FACTOR;
 p->STO_PLL_M1.SpeedValidationBand_H              =	SPEED_BAND_UPPER_LIMIT;
 p->STO_PLL_M1.SpeedValidationBand_L              =	SPEED_BAND_LOWER_LIMIT;
 p->STO_PLL_M1.MinStartUpValidSpeed               =	OBS_MINIMUM_SPEED_UNIT;
p->STO_PLL_M1.StartUpConsistThreshold            =	NB_CONSECUTIVE_TESTS;
 p->STO_PLL_M1.Reliability_hysteresys             =	OBS_MEAS_ERRORS_BEFORE_FAULTS;
 p->STO_PLL_M1.BemfConsistencyCheck               =	BEMF_CONSISTENCY_TOL;
 p->STO_PLL_M1.BemfConsistencyGain                =	BEMF_CONSISTENCY_GAIN;
 p->STO_PLL_M1.MaxAppPositiveMecSpeedUnit         =	(uint16_t)(MAX_APPLICATION_SPEED_UNIT*1.15);
 p->STO_PLL_M1.F1LOG                              =	F1_LOG;
 p->STO_PLL_M1.F2LOG                              =	F2_LOG;
 p->STO_PLL_M1.SpeedBufferSizeDppLOG              =	STO_FIFO_DEPTH_DPP_LOG;
 p->STO_PLL_M1.hForcedDirection                   =  0x0000U;

  STO_PLL_Init (&p->STO_PLL_M1);*/

   /******************************************************/
    /*   Speed & torque component initialization          */
    /******************************************************/
 //   STC_Init(pSTC[M1],&PIDSpeedHandle_M1, &STO_PLL_M1._Super);

    /****************************************************/
    /*   Virtual speed sensor component initialization  */
    /****************************************************/
 //   VSS_Init(&VirtualSpeedSensorM1);

    /**************************************/
    /*   Rev-up component initialization  */
    /**************************************/
 //   RUC_Init(&RevUpControlM1, pSTC[M1], &VirtualSpeedSensorM1, &STO_M1, pwmcHandle[M1]);

    /********************************************************/
    /*   PID component initialization: current regulation   */
    /********************************************************/
 //   PID_HandleInit(&PIDIqHandle_M1);
 //   PID_HandleInit(&PIDIdHandle_M1);

    /********************************************************/
    /*   Bus voltage sensor component initialization        */
    /********************************************************/
  //  RVBS_Init(&BusVoltageSensor_M1);

    /*************************************************/
    /*   Power measurement component initialization  */
    /*************************************************/
  //  pMPM[M1]->pVBS = &(BusVoltageSensor_M1._Super);
  //  pMPM[M1]->pFOCVars = &FOCVars[M1];

    /*******************************************************/
    /*   Temperature measurement component initialization  */
    /*******************************************************/
   /* NTC_Init(&TempSensor_M1);

    pREMNG[M1] = &RampExtMngrHFParamsM1;
    REMNG_Init(pREMNG[M1]);

    FOC_Clear(M1);
    FOCVars[M1].bDriveInput = EXTERNAL;
    FOCVars[M1].Iqdref = STC_GetDefaultIqdref(pSTC[M1]);
    FOCVars[M1].UserIdref = STC_GetDefaultIqdref(pSTC[M1]).d;
    MCI_Init(&Mci[M1], pSTC[M1], &FOCVars[M1],pwmcHandle[M1] );
    MCI_ExecSpeedRamp(&Mci[M1],
    STC_GetMecSpeedRefUnitDefault(pSTC[M1]),0); /*First command to STC*/

    pMCIList[M1] = &Mci[M1];

    ICL_Init(&ICL_M1, &(BusVoltageSensor_M1._Super), &ICLDOUTParamsM1);
    Mci[M1].State = ICLWAIT;

    DAC_Init(&DAC_Handle);*/
  
  
  
  p->bMCBootCompleted = 1; //
}

    
void Vector_18KHZ(TVector *p)
{

}

void Vector_2KHZ(TVector *p)
{
  if (0U == p->bMCBootCompleted)
  {
  
  }
  else
  {
   
    MC_Scheduler();
    TSK_SafetyTask();

  }
}

void Vector_200HZ(TVector *p)
{
 
}

void Vector_50HZ(TVector *p)
{

}


void MC_Scheduler(void)
{
  if (((uint8_t)1) == g_Vector.bMCBootCompleted)
  {
    if(g_Vector.hMFTaskCounterM1 > 0u)
    {
      g_Vector.hMFTaskCounterM1--;
    }
    else
    {
      TSK_MediumFrequencyTaskM1();

      // хз что за связь и с чем она
    /*  MCP_Over_UartA.rxBuffer = MCP_Over_UartA.pTransportLayer->fRXPacketProcess(MCP_Over_UartA.pTransportLayer,
                                                                                &MCP_Over_UartA.rxLength);
      if ( 0U == MCP_Over_UartA.rxBuffer)
      {
       
      }
      else
      {
      
        if (0U == MCP_Over_UartA.pTransportLayer->fGetBuffer(MCP_Over_UartA.pTransportLayer,
                                                     (void **) &MCP_Over_UartA.txBuffer, //cstat !MISRAC2012-Rule-11.3
                                                     MCTL_SYNC))
        {
         
        }
        else
        {
          MCP_ReceivedPacket(&MCP_Over_UartA);
          MCP_Over_UartA.pTransportLayer->fSendPacket(MCP_Over_UartA.pTransportLayer, MCP_Over_UartA.txBuffer,
                                                      MCP_Over_UartA.txLength, MCTL_SYNC);
          
        }
      }*/
      
      g_Vector.hMFTaskCounterM1 = (uint16_t)MF_TASK_OCCURENCE_TICKS;
    }
  /*  if(hBootCapDelayCounterM1 > 0U)
    {
      hBootCapDelayCounterM1--;
    }
    if(hStopPermanencyCounterM1 > 0U)
    {
      hStopPermanencyCounterM1--;
    }*/
  }
  else
  {
   
  }
}

// защитное выключение системы
void TSK_SafetyTask(void)
{

  if (1U == g_Vector.bMCBootCompleted)
  {
    TSK_SafetyTask_PWMOFF(M1);
    RCM_ExecUserConv();
  }
}

                      




