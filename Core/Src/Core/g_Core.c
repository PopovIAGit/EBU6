
#include "g_Core.h"
#include "stat_fm25v10.h"


TFM25V10 Eeprom1;
TCore	g_Core;

float   PwmFreq;				// частота ШИМ
float   PwmDeltat;			// шаг дискретизации токов
Uns     PWMPreScale = 2;
float corr = 1.0; // 5000 = 50гц
float timeFreq;
float SpeedRef, OutVolt;

Uns InvCtrlTimer =0;

void PWM_keys_disable(void);

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


void core18kHZupdate(void)
{
     
   if (++InvCtrlTimer >= PWMPreScale)
    {   
      g_Core.rg1.Freq = (float)SpeedRef; //ToDo задание скорости - убрать
  

      rampgen_calc(&g_Core.rg1);         // пила основной гармоники 1.0 - 50 Гц
      g_Core.vhz.Input = SpeedRef;
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
      // псевдо обратная связь
       g_Core.svgen3ph.dVa = 1.0 - SpeedRef;
       g_Core.svgen3ph.dVb = 1.0 - SpeedRef;
       g_Core.svgen3ph.dVc = 1.0 - SpeedRef;
       
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
        //  HAL_GPIO_TogglePin(TEN_OFF_GPIO_Port, TEN_OFF_Pin);
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
}

void core200HZupdate(void)
{

}

void core50HZupdate(void)
{

}

void core10HZupdate(void)
{

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
                            break;
                    case BTN_CLOSE:
                                   // Mcu.Mpu.BtnKey = KEY_CLOSE;
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
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                break;
                
                case BTN_STOP2:
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



