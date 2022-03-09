
#include "g_Core.h"
#include "stat_fm25v10.h"


TFM25V10 Eeprom1;
TCore	g_Core;

float   PwmFreq;				// ÷àñòîòà ØÈÌ
float   PwmDeltat;			// øàã äèñêðåòèçàöèè òîêîâ
Uns     PWMPreScale = 2;
float corr = 1.0; // 5000 = 50ãö
float timeFreq;
float SpeedRef, OutVolt;

Uns InvCtrlTimer = 0;

void PWM_keys_disable(void);
void PWM_keys_enable(void);

Uns StartTimer = 1 * PRD_10HZ;          // thyr pause

Uns ShC_Level = 32767;                  // shc level

float speedstart = 0;   // speed start
float SpeedMax = 0;     // to speed
Uns SpeedTime = 0;      // speed time
Uns  SpeedEnable = 0;   // enable
Uns TimerInterp = 0;
float AngleInterp(float StartValue, float EndValue, Uns Time);

void Core_Init(TCore *p)
{	
  p->DisplayTimer = 1 * PRD_10HZ;
  p->Status.bit.Stop = 1;  
  FM25V10_Init(&Eeprom1);   
  Core_MenuInit(&menu);
 
   g_Core.rg1.Gain                 = 1.0;
   g_Core.rg1.Offset               = 0.0;
   
   PwmFreq   = _IQdiv((HZ), 1);
   PwmDeltat = _IQdiv(1, PwmFreq);
   
   g_Core.rg1.StepAngleMax         = 50.0 * PwmDeltat;
   
   g_Core.vhz.NumPoints = 3;
   g_Core.vhz.Points[0].Input = 0 / BASE_FREQ;
   g_Core.vhz.Points[0].Output = 20.0 / BASE_VOLTAGE;
   g_Core.vhz.Points[1].Input = 500.0 / BASE_FREQ;
   g_Core.vhz.Points[1].Output = 38.0 / BASE_VOLTAGE;
   g_Core.vhz.Points[2].Input = 5000.0 / BASE_FREQ;
   g_Core.vhz.Points[2].Output = 380.0 / BASE_VOLTAGE;
   
   g_Core.svgen3ph.Scalar = true;
   
   g_Core.Pwm.Period = _IQdiv((200000000), PwmFreq) - 1;
}

Uns ModuleTemper = 0;
Uns IDC = 0;
Uns ModFault = 1; // 1 norm, 0 - fault
void core18kHZupdate(void)
{
 //  ADC__1                                         0 UL1_ADC   1 UL3_ADC   2 I_brake_A   3 UL2_ADC  4 TMP_DV_A  5 TempModule_A
 //  ADC__3                                         0 VDC_A     1 IU_ADC    2 IV_ADC    3 IW_ADC
    
      
    ModuleTemper = g_Peref.adcData1[5]; // all in 
    IDC  = g_Peref.adcData3[0];
    // SHC Protect Test--------------------------------------------------

    if (( g_Peref.adcData3[1] > ShC_Level) ||  (g_Peref.adcData3[2] > ShC_Level)  || (g_Peref.adcData3[3] > ShC_Level))
    {
      g_Core.Status.bit.Fault = 1;
    }
      ModFault = HAL_GPIO_ReadPin(Module_Foult_GPIO_Port, Module_Foult_Pin);
      if (!ModFault)  
        g_Core.Status.bit.Fault = 1;

    //--------------------------------------------------------------------------

   if (!g_Core.Status.bit.Fault && SpeedRef)
   {     
    PWM_keys_enable();
      
    if (++InvCtrlTimer >= PWMPreScale)
    {   
      g_Core.rg1.Freq = (float)SpeedRef; //ToDo çàäàíèå ñêîðîñòè - óáðàòü
      rampgen_calc(&g_Core.rg1);         // ïèëà îñíîâíîé ãàðìîíèêè 1.0 - 50 Ãö
      g_Core.vhz.Input = fabsf(SpeedRef);
      interp2D_calc(&g_Core.vhz);
      OutVolt = g_Core.vhz.Output;   
      g_Core.ipark.Ds = OutVolt;
      g_Core.ipark.Qs = 0;      
      g_Core.ipark.Angle = g_Core.rg1.Out;
      g_Core.park.Angle = g_Core.rg1.Out;            
      ipark_calc(&g_Core.ipark);
      park_calc(&g_Core.park);
     
      	g_Core.svgen3ph.RampGenOut = g_Core.ipark.Angle;
	g_Core.svgen3ph.Valpha     = g_Core.ipark.Alpha;		
	g_Core.svgen3ph.Vbeta      = g_Core.ipark.Beta;	
       
      svgendq3ph_calc(&g_Core.svgen3ph);
      
      	g_Core.Pwm.MfuncC1 = g_Core.svgen3ph.Ta;
	g_Core.Pwm.MfuncC2 = g_Core.svgen3ph.Tb;
	g_Core.Pwm.MfuncC3 = g_Core.svgen3ph.Tc;
      
        pwm_calc(&g_Core.Pwm);
                             
        TIM1->CCR1 = g_Core.Pwm.Cmpr1;
        TIM1->CCR2 = g_Core.Pwm.Cmpr3;
        TIM1->CCR3 = g_Core.Pwm.Cmpr2;  
                           
        InvCtrlTimer = 0;
    }
   }
     else 
     {
      PWM_keys_disable();
     }
                
}


void svgendq3ph_calc(SVGENDQ_3PH *v)
{
	register Float Sine, Cosine;
	
	if(v->RampGenOut < M_1DIV3)
		v->RampGenOut3 = v->RampGenOut * 3;
	else if ((v->RampGenOut >= M_1DIV3) && (v->RampGenOut < M_2DIV3))
		v->RampGenOut3 = (v->RampGenOut - M_1DIV3) * 3;
	 else
		v->RampGenOut3 = (v->RampGenOut - M_2DIV3) * 3;
	
	Sine   = _IQsinPU(v->RampGenOut3);
	Cosine = _IQcosPU(v->RampGenOut3);
	
	v->VPredMod = (v->PredmodAmp * ((v->Vds * Cosine) - (v->Vqs * Sine)));

	if (v->Scalar)
	{
		v->VaRef = v->Valpha;
                v->VbRef = ((M_SQRT_3 * v->Vbeta) - v->Valpha) / 2;
		v->VcRef = -v->VaRef - v->VbRef;
	}
		
	v->Ta = _IQsat(v->VaRef - v->dVa - v->VPredMod, MAX_Tx_OUT, -MAX_Tx_OUT);
	v->Tb = _IQsat(v->VbRef - v->dVb - v->VPredMod, MAX_Tx_OUT, -MAX_Tx_OUT);
	v->Tc = _IQsat(v->VcRef - v->dVc - v->VPredMod, MAX_Tx_OUT, -MAX_Tx_OUT);     

}

void pwm_calc(PWM *p)
{
	// Saturate inputs
	p->MfuncC1 = _IQsat(p->MfuncC1, 1.0, -1.0);
	p->MfuncC2 = _IQsat(p->MfuncC2, 1.0, -1.0);
	p->MfuncC3 = _IQsat(p->MfuncC3, 1.0, -1.0);
	
	// Compute the compare A (Q0) from the EPWM1AO & EPWM1BO duty cycle ratio
	p->Cmpr1 = _IQmpy((p->MfuncC1 + 1.0) / 2, p->Period);
	
	// Compute the compare B (Q0) from the EPWM2AO & EPWM2BO duty cycle ratio
	p->Cmpr2 = _IQmpy((p->MfuncC2 + 1.0) / 2, p->Period);
	
	// Compute the compare C (Q0) from the EPWM3AO & EPWM3BO duty cycle ratio
	p->Cmpr3 = _IQmpy((p->MfuncC3 + 1.0) / 2, p->Period);
}


void rampgen_calc(RAMPGEN *v)
{
	// Compute the angle rate
	v->Angle += (v->StepAngleMax * v->Freq);
	
	// Saturate the angle rate within (0,1)
	if (v->Angle > corr)     
        {
          v->Angle -= corr;
        }        
        
	else if (v->Angle < 0){ 
          v->Angle += corr;
        }
	
	// Compute the ramp output
	v->Out = (v->Angle * v->Gain) + v->Offset;

	// Saturate the ramp output within (0,1)
	if (v->Out > corr)  {
          v->Out -= corr;   
          
        }
	else if (v->Out < 0) v->Out += corr;
}

void interp2D_calc(INTERP2D *v)
{	
	register float Slope;
	register Uns   Index;
	Pt2D *P0, *P1;
	
	if (v->Input <= v->Points[0].Input)
		v->Output = v->Points[0].Output;
	else if (v->Input >= v->Points[v->NumPoints-1].Input)
		v->Output = v->Points[v->NumPoints-1].Output;
	else
	{
		for (Index=1; Index < v->NumPoints; Index++)
			if (v->Input <= v->Points[Index].Input) break;
		
		P0 = &v->Points[Index-1];
		P1 = &v->Points[Index];
		
		if (P0->Input == P1->Input) v->Output = P0->Output;
		else
		{
			Slope = ((P1->Output - P0->Output) / (P1->Input - P0->Input));
			v->Output = P0->Output + (Slope * (v->Input - P0->Input));
		}
	}
}

void ipark_calc(IPARK *v)
{	
	Float Cosine, Sine;
	
	// Using look-up IQ sine table
	Sine   = _IQsinPU(v->Angle);
	Cosine = _IQcosPU(v->Angle);
	
	v->Alpha = (v->Ds * Cosine) - (v->Qs * Sine);
	v->Beta  = (v->Qs * Cosine) + (v->Ds * Sine);
}

void park_calc(PARK *v)
{	
	LgInt Cosine, Sine;
	
	// Using look-up IQ sine table
	Sine   = _IQsinPU(v->Angle);
	Cosine = _IQcosPU(v->Angle);
	
	v->Ds = (v->Alpha * Cosine) + (v->Beta *  Sine);
	v->Qs = (v->Beta *  Cosine) - (v->Alpha * Sine);
}

void PWM_keys_disable(void)
{
HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

void PWM_keys_enable(void)
{
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void core200HZupdate(void)
{
  
}

void core50HZupdate(void)
{

}

void core10HZupdate(void)
{
    if (StartTimer > 0) StartTimer--;
    else {
    
      HAL_HRTIM_SimplePWMStart(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1);
    }
           
  /*  if (SpeedEnable)  SpeedRef = AngleInterp(speedstart, SpeedMax, SpeedTime);
    else 
    {
      SpeedRef = 0;
      TimerInterp = 0;
    }*/
    
    if (SpeedEnable == 1 && SpeedRef < fabs(SpeedMax))
    {
      SpeedRef = AngleInterp(speedstart, SpeedMax, g_Ram.UserParam.TimeSpeedStart);
    }
    else if (SpeedEnable == 2 && SpeedRef != 0)
    {
      
      SpeedRef = AngleInterp(speedstart, SpeedMax, g_Ram.UserParam.TimeSpeedStop);
    }
    else if (SpeedEnable == 2 && SpeedRef == 0)
    {
      SpeedEnable = 0;
      SpeedMax = 0;
    }
}

void coreTU(TCore *p)
{
  if (g_Ram.UserParam.InputType == it24)
  {
      g_Ram.Status.StateTu.all = g_Peref.TU_data24;
  }
  else if (g_Ram.UserParam.InputType == it220)
  {
        g_Ram.Status.StateTu.all = g_Peref.TU_data220;
  }
}

void coreTS(TCore *p)
{
   g_Ram.HideParam.HideStateTs.bit.Opened       = p->Status.bit.Opened  ^ g_Ram.UserParam.TsInvert.bit.Dout0;
   g_Ram.HideParam.HideStateTs.bit.Closed       = p->Status.bit.Closed  ^ g_Ram.UserParam.TsInvert.bit.Dout1;
   g_Ram.HideParam.HideStateTs.bit.Opening      = p->Status.bit.Opening ^ g_Ram.UserParam.TsInvert.bit.Dout2;
   g_Ram.HideParam.HideStateTs.bit.Closing      = p->Status.bit.Closing ^ g_Ram.UserParam.TsInvert.bit.Dout3;
   g_Ram.HideParam.HideStateTs.bit.Fault        = p->Status.bit.Fault   ^ g_Ram.UserParam.TsInvert.bit.Dout4;
   g_Ram.HideParam.HideStateTs.bit.MuftaOpen    = p->Status.bit.Mufta   ^ g_Ram.UserParam.TsInvert.bit.Dout5;
   g_Ram.HideParam.HideStateTs.bit.MuftaClose   = p->Status.bit.Mufta   ^ g_Ram.UserParam.TsInvert.bit.Dout6;
   g_Ram.HideParam.HideStateTs.bit.MUDU         = p->Status.bit.MuDu    ^ g_Ram.UserParam.TsInvert.bit.Dout7; 
}

void coreTLocalControl(TCore *p)
{
        static Uns prevHalls = 0;
	static Bool readyForNewCmd = true;	// flag ready to new cmd
        
       if (menu.State == 0) g_Core.Status.bit.Program = 0;
       else g_Core.Status.bit.Program = 1;
  
        if (g_Core.Status.bit.Program)
         {
           
           if (prevHalls != g_Peref.BtnStatus) readyForNewCmd = true;
           
            if (readyForNewCmd)
            {
                switch (g_Peref.BtnStatus)
                 {
                   case BTN_OPEN: 
                      menu.Key = KEY_LEFT;
                     break;         
                   case BTN_CLOSE: 
                       menu.Key = KEY_RIGHT;
                     break; 
                   case BTN_STOP1:    //
                        menu.Key = KEY_UP;
                     break;
                   case BTN_STOP2:    //
                        menu.Key = KEY_DOWN;
                     break;
                   case BTN_PROG1:    // Enter
                          menu.Key = KEY_ENTER;
                     break;
                   case BTN_PROG2:    // exit
                          menu.Key = KEY_ESCAPE;
                      break; 
                 }
            }
           readyForNewCmd = false;
           
         }
         else 
         {
             switch(g_Peref.BtnStatus & (~BTN_STOP))
              {
                    case BTN_OPEN: 
                                   // Mcu.Mpu.BtnKey = KEY_OPEN;
                      if (SpeedRef == 0){
                        TimerInterp = 0;
                                    speedstart = 0;
                                    SpeedMax = (((float)g_Ram.UserParam.SpeedStart)/10)/500;
                                    SpeedEnable = 1;}
                            break;
                    case BTN_CLOSE:
                                   // Mcu.Mpu.BtnKey = KEY_CLOSE;
                                   if (SpeedRef == 0){
                                     TimerInterp = 0;
                                    speedstart = 0;
                                    SpeedMax = ((((float)g_Ram.UserParam.SpeedStart)/10)*-1.0)/500;
                                    SpeedEnable = 1;}
                            break;
                    case BTN_PROG1:
                      {
                          if (p->Status.bit.Stop)
                          {
                           menu.Key = KEY_ENTER;
                          }
                      }       
                       break; 
                      case BTN_PROG2:
                      {
                          if (p->Status.bit.Stop)
                          {                       
                          // menu.Key = KEY_ENTER;
                          }
                      }       
                       break; 
              }
              switch(g_Peref.BtnStatus & BTN_STOP)
              {
                case BTN_STOP1:
                  if (SpeedRef != 0){
                    TimerInterp = 0;
                          SpeedEnable = 2;
                          SpeedMax = 0;
                          speedstart = SpeedRef;}
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                break;
                
                case BTN_STOP2:
                          if (SpeedRef != 0){
                            TimerInterp = 0;
                          SpeedEnable = 2;
                          SpeedMax = 0;
                          speedstart = SpeedRef;}
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                break;
              }

         }
  
    prevHalls = g_Peref.BtnStatus;    
        
    g_Ram.FactoryParam.HallBlock.bit.Open  = g_Peref.BtnOpen.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Close = g_Peref.BtnClose.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Stop1 = g_Peref.BtnStop1.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Stop2 = g_Peref.BtnStop2.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Prog1 = g_Peref.BtnProg1.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Prog2 = g_Peref.BtnProg2.Flag;
  

}

float OutputQ15 = 0.0;
float data1 = 0;
float data2 = 0;
float AngleInterp(float StartValue, float EndValue, Uns Time)
{
 
   if (EndValue > 0){
	if (TimerInterp == 0) 
          OutputQ15 = StartValue;
	//else OutputQ15 = OutputQ15 - _IQ15div(((LgInt)(StartValue - EndValue) << 15), _IQ15mpy((LgInt)Time * 3277, _IQ15(200.0)));	// ÐŸÑ€Ð¸ Ð¿ÐµÑ€ÐµÐ½Ð¾ÑÐµ Ñ„ÑƒÐ½ÐºÑ†Ð¸Ð¸ Ð¸Ð· Ð´Ñ€ÑƒÐ³Ð¾Ð³Ð¾ Ð¿Ñ€Ð¾ÐµÐºÑ‚Ð°, Ð±ÑƒÐ´ÑŒÑ‚Ðµ Ð²Ð½Ð¸Ð¼Ð°Ñ‚ÐµÐ»ÑŒÐ½Ñ‹Ð¼Ð¸!!
        else 
        {
          data1 = (EndValue - StartValue); 
          data2 = Time * 1;
          OutputQ15 = OutputQ15 + data1/data2;
        } 

	TimerInterp++;
	
	if (OutputQ15 >= EndValue) 
          return EndValue;
	else if (OutputQ15 <= StartValue ) 
          return StartValue;
		 else 
                   return OutputQ15;
   }
    else if (EndValue < 0)
    {
      if (TimerInterp == 0) 
          OutputQ15 = StartValue;
        else 
        {
          data1 = (EndValue + StartValue); 
          data2 = Time * 1;
          OutputQ15 = OutputQ15 + data1/data2;
        } 

	TimerInterp++;
	
	if (OutputQ15 <= EndValue) 
          return EndValue;
	else if (OutputQ15 >= StartValue ) 
          return StartValue;
		 else 
                   return OutputQ15;
    }
   else if (EndValue == 0 && StartValue > 0)
   {
         if (TimerInterp == 0) 
          OutputQ15 = StartValue;
        else 
        {
          data1 = (EndValue + StartValue); 
          data2 = Time * 1;
          OutputQ15 = OutputQ15 - data1/data2;
        } 

	TimerInterp++;
	
	if (OutputQ15 <= EndValue) 
          return EndValue;
	else if (OutputQ15 >= StartValue ) 
          return StartValue;
		 else 
                   return OutputQ15;
   }
   else if (EndValue == 0 && StartValue <0)
   {
         if (TimerInterp == 0) 
          OutputQ15 = StartValue;
        else 
        {
          data1 = (EndValue + StartValue); 
          data2 = Time * 1;
          OutputQ15 = OutputQ15 - data1/data2;
        } 

	TimerInterp++;
	
	if (OutputQ15 >= EndValue) 
          return EndValue;
	else if (OutputQ15 <= StartValue ) 
          return StartValue;
		 else 
                   return OutputQ15;
   }
} 

