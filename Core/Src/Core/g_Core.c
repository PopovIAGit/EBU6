
#include "g_Core.h"
#include "stat_fm25v10.h"


TFM25V10 Eeprom1;
TCore	g_Core;

float   PwmFreq;			// частота ШИМ
float   PwmDeltat;			// шаг дискретизации токов
Uns     PWMPreScale = 1; // 2
float corr = 1.0; // 5000 = 50гц
float timeFreq;
float SpeedRef, OutVolt;

Uns InvCtrlTimer = 0;

void PWM_keys_disable(void);
void PWM_keys_enable(void);

Uns StartTimer = 1 * PRD_10HZ;          // thyr pause

Uns ShC_Level = 32767;                  // shc level

Uns ModuleTemper = 0;
Uns IDC = 0;
Uns ModFault = 1; // 1 norm, 0 - fault
Uns GoToLocalFLag = 0;
float speedstart = 0;   // speed start
float SpeedMax = 0;     // to speed
Uns SpeedTime = 0;      // speed time
Uns  SpeedEnable = 0;   // enable
Uns TimerInterp = 0;
float AngleInterp(float StartValue, float EndValue, Uns Time);
        Uns TimeSpeedStop = 0;
static void MoveMode (void);
static void StopMode(void);
static void StartMode(void);

static void DynBreakMode(void);
void Core_Init(TCore *p)
{	
  p->DisplayTimer = 1 * PRD_10HZ;
  p->Status.bit.Stop = 1;  
  FM25V10_Init(&Eeprom1);   
  Core_MenuInit(&menu);
  Core_ProtectionsInit(&p->Protections);	// Защиты
   g_Core.rg1.Gain                 = 1.0;
   g_Core.rg1.Offset               = 0.0;
   
   PwmFreq   = _IQdiv((HZ), 1);
   PwmDeltat = _IQdiv(1, PwmFreq);
   
   g_Peref.Ia.Mash2 = PwmDeltat;
   g_Peref.Ib.Mash2 = PwmDeltat;
   g_Peref.Ic.Mash2 = PwmDeltat;
   
   g_Core.Mash1 = (1.0 / (float)g_Ram.FactoryParam.Inom);
 //  g_Core.Mash3 = 50.0;
   
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
   
   Core_ValveDriveInit(&p->VlvDrvCtrl);	// Управление задвижкой
   
   g_Core.pidData.kp = 0.01;
   g_Core.pidData.MAX_OUT = 1.0;
   g_Core.pidData.MIN_OUT = 0.1;
        
     	g_Core.fe.Kp                 = (0.75);
	g_Core.fe.Kir                = (5.994);
        g_Core.fe.Ki                 = (10.0) * PwmDeltat;
   
        aci_fe_init(&g_Core.fe);
}

void SpeedEstemation(TCore *v)
{
	v->volt.DcBusVolt = v->Control.Udc * v->Control.UdcMash; 
	v->volt.MfuncV1   = v->svgen3ph.Ta;
	v->volt.MfuncV2   = v->svgen3ph.Tb;
	v->volt.MfuncV3   = v->svgen3ph.Tc;
	phase_voltage_calc(&v->volt);
	
 	v->fe.UDsS = v->volt.Valpha;
	v->fe.UQsS = v->volt.Vbeta;
 	v->fe.IDsS = v->park.Alpha;
	v->fe.IQsS = v->park.Beta;
 	aci_fe_calc(&v->fe);
	
 	/*v->FiltrLoad.Input = v->fe.Te_est;
	ApFilter3Calc(&v->FiltrLoad);
	v->TorqComp.LoadTorque = v->FiltrLoad.Output;
	v->SleepCompScal.Load = v->FiltrLoad.Output;
	 	
 	v->se.IDsS      = v->park.Alpha;
	v->se.IQsS      = v->park.Beta;
 	v->se.PsiDrS    = v->fe.PsiDrS;
	v->se.PsiQrS    = v->fe.PsiQrS;
	v->se.ThetaFlux = v->fe.ThetaFlux;
 	aci_se_calc(&v->se);
	

	
	v->Status.EstSpeed = v->se.WrHat;*/
}



// Остановка по калибровке
void Core_CalibStop (TCore *p)
{
	Bool StopFlag = False; // внутенний флаг остановки
	LgInt Position = p->VlvDrvCtrl.Valve.Position;
	
		if(p->VlvDrvCtrl.Valve.Position == POS_UNDEF) //Если целевое положение не определено то уходим
		{
			p->MotorControl.TargetPos = POS_UNDEF;
			return;
		}
		p->MotorControl.TargetPos = (g_Peref.Position.LinePos - Position);

		if(p->Status.bit.Stop) return;

		if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= g_Ram.FactoryParam.BreakZone)) StopFlag = True;
		if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= -g_Ram.FactoryParam.BreakZone)) StopFlag = True;

		if (StopFlag)	// Если пора останавливаться
		{
			if(p->VlvDrvCtrl.Valve.BreakFlag) p->MotorControl.OverWayFlag = 1;	//
			else	// Если
				{
					p->MotorControl.CalibStop = 1;
					Core_ValveDriveStop(&p->VlvDrvCtrl);
					p->VlvDrvCtrl.EvLog.Value = CMD_STOP;
					p->VlvDrvCtrl.EvLog.Source = CMD_SRC_BLOCK;
				}
		}
}
Uns  addr1 = 0;
Uns  count1 = 0;
// Управление калибровкой
void Core_CalibControl(TCore *p)
{
	g_Peref.Position.ResetFlag = !p->Status.bit.Stop;

	if(g_Peref.Position.CancelFlag)
	{
		p->VlvDrvCtrl.Mpu.CancelFlag = true;
		g_Peref.Position.CancelFlag = false;
	}

	// Зона смещения передается только когда привод в стопе. В движении зона смещения равна нулю
	g_Ram.HideParam.PositionAccTemp = p->Status.bit.Stop ? g_Ram.UserParam.PositionAcc : 0;

	
	p->Status.bit.Closed =/*  p->Status.bit.Stop && */ ((g_Peref.Position.Zone & CLB_CLOSE) != 0); //ToDo !!! ПИА 13.02.2020 пока не съехали с концевика физически не снимаем сигнал.
	p->Status.bit.Opened =/*  p->Status.bit.Stop &&  */((g_Peref.Position.Zone & CLB_OPEN)  != 0);
	
	if(g_Ram.Comands.CalibReset != 0)
	{
		if (!p->Status.bit.Stop )
		{
			p->VlvDrvCtrl.Mpu.CancelFlag = true;
		}
		else
		{
			g_Ram.Comands.TaskClose = trReset;
			g_Ram.Comands.TaskOpen  = trReset;
			p->VlvDrvCtrl.EvLog.Value = CMD_RES_CLB;
		}
		g_Ram.Comands.CalibReset = 0;
	}
        
        if (IsMemParReady())																	// если есть готовность к записи параметров
	{
		if (g_Ram.HideParam.CycleCnt != p->PrevCycle)										// если счетчик циклов обновился
		{
			WriteToEeprom(REG_CYCLE_CNT, &g_Ram.HideParam.CycleCnt, 1);						// записали параметр счетчик циклов
			p->PrevCycle = g_Ram.HideParam.CycleCnt;										// запомнили записанный параметр, для последующей проверки
		}
		else if (g_Ram.Status.CalibState != g_Ram.HideParam.CalibState)					// если состояние калибровки изменилось
		{
			addr1 = GetAdr(HideParam.CalibState);//REG_CALIB_STATE;
                        count1 = sizeof(ClbIndication);
                        WritePar(addr1, &g_Ram.HideParam.CalibState, count1);// запоминаем записанный параметр, для последующей проверки
                        g_Ram.Status.CalibState = g_Ram.HideParam.CalibState;  
		}
	}
}


void core18kHZupdate(void)
{
  g_Core.Mash3 = fabs(g_Core.rg1.Freq * 50.0);
    // not work
    //  g_Core.Protections.outFaults.Dev.bit.ModuleFault = HAL_GPIO_ReadPin(Module_Foult_GPIO_Port, Module_Foult_Pin);
    //--------------------------------------------------------------------------

   if (/*!g_Core.Status.bit.Fault && */SpeedRef)
   {     
    PWM_keys_enable();
      
    if (++InvCtrlTimer >= PWMPreScale)
    {   
      g_Core.rg1.Freq = (float)SpeedRef; //ToDo задание скорости - убрать
      rampgen_calc(&g_Core.rg1);         // пила основной гармоники 1.0 - 50 Гц
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
     // 
     }
                
}

// Действия выполняемые при стопе
void StopPowerControl(void)
{
	SpeedEnable                     = 2;    //плавно остановились
        TimerInterp                     = 0;                
        SpeedMax                        = 0;
        speedstart                      = SpeedRef;
        g_Core.Status.bit.Stop          = 1;
        g_Core.Status.bit.Opening 	= 0;
	g_Core.Status.bit.Closing 	= 0;
	g_Core.Status.bit.Test 		= 0;
        
      /*  if (g_Core.Status.bit.Fault)
          g_Core.MotorControl.WorkMode = wmStop;
        else if (g_Core.DynBreakEnable) 
          g_Core.MotorControl.WorkMode = wmDynBreak;*/
         
      //  g_Core.MotorControl.WorkMode = wmStop;
        
        
        if (g_Core.Status.bit.Fault){
            g_Core.MotorControl.WorkMode = wmStop ;
            TimeSpeedStop = g_Ram.UserParam.TimeSpeedStop;
        }
        else  
        {
          if (SpeedRef != 0){
              g_Core.MotorControl.WorkMode = wmDynBreak; 
              TimeSpeedStop = 1;
          }else g_Core.MotorControl.WorkMode = wmStop;
        }
        g_Core.VlvDrvCtrl.StartDelay = (Uns)START_DELAY_TIME;
}        

// Действия при пуске
void StartPowerControl(TValveCmd ControlWord)
{
        g_Core.Status.bit.Stop = 0;
	switch (ControlWord)
	{
		case vcwClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmStart;
                        g_Core.MotorControl.RequestPos = 0;
                        SpeedMax = (((float)g_Ram.UserParam.SpeedStart)/10)/50;
                        
			break;
		case vcwOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmStart;
                        g_Core.MotorControl.RequestPos = 1000;
                        SpeedMax = ((((float)g_Ram.UserParam.SpeedStart)/10)*-1.0)/50;                       
			break;
                        
	}
        
        if(g_Core.MotorControl.RequestDir > 0) g_Core.Status.bit.Opening = 1; 
        if(g_Core.MotorControl.RequestDir < 0) g_Core.Status.bit.Closing = 1; 
   
          SpeedEnable = 1;
          speedstart = 0;
          TimerInterp = 0;      
}

// Стэйт машина
void Core_ControlMode(TCore *p) // 50 Гц
{

    switch(p->MotorControl.WorkMode)
    {
        case wmStop:		StopMode(); break;
	case wmStart:		StartMode();break;	
	case wmMove:		MoveMode(); break;
        case wmDynBreak:        DynBreakMode(); break;
    }    
}

static void StopMode(void)
{
	// что то делаем в стопе
    /* if (g_Ram.UserParam.MuDuSetup != mdDac)
      {
          if (SpeedEnable == 2 && SpeedRef != 0)
          { 
            SpeedRef = AngleInterp(speedstart, SpeedMax, g_Ram.UserParam.TimeSpeedStop);
          }
          else if (SpeedEnable == 2 && SpeedRef == 0)
          {
            SpeedEnable = 0;
            SpeedMax = 0;
          }
      }
     else 
     {*/
      
  
           SpeedEnable = 0;
           SpeedMax = 0;
           SpeedRef = 0.0;
           PWM_keys_disable();
          // g_Core.VlvDrvCtrl.StartDelay = (Uns)START_DELAY_TIME;
     
  
}

static void StartMode(void)
{
      // что то делаем при старте
    g_Core.MotorControl.WorkMode = wmMove;
   /* if (g_Ram.UserParam.MuDuSetup != mdDac)
    {
          if (SpeedEnable == 1 && SpeedRef < fabs(SpeedMax))
          {
            SpeedRef = AngleInterp(speedstart, SpeedMax, g_Ram.UserParam.TimeSpeedStart);
          }
          else if (SpeedEnable == 1 && SpeedRef == fabs(SpeedMax))
          {
              g_Core.MotorControl.WorkMode = wmMove;
          }
    }
    else 
    {
         g_Core.MotorControl.WorkMode = wmMove;
    }*/
}

double tmpdata = 0;
double tmpdata2 = 0;
static void MoveMode(void)
{
  if (g_Ram.UserParam.RegEnable == 0)
  {
      if (g_Ram.Status.Status.bit.MuDu == 0 && g_Ram.UserParam.DuSource == mdsDac)
      {
        tmpdata   = p_Controller(g_Ram.UserParam.SetPosition, g_Ram.Status.PositionPr);
        tmpdata2 = g_Core.MotorControl.RequestDir * -1.0;
        SpeedRef = tmpdata2 * tmpdata;
      }
      else if (g_Ram.Status.Status.bit.MuDu == 1 || (g_Ram.Status.Status.bit.MuDu == 0 && g_Ram.UserParam.DuSource == mdsDigital))
      {
          tmpdata   = p_Controller(g_Core.MotorControl.RequestPos, g_Ram.Status.PositionPr);
          tmpdata2 = g_Core.MotorControl.RequestDir * -1.0;
          SpeedRef = tmpdata2 * tmpdata;
      }
  }
  else 
  {
    
        tmpdata   = p_Controller(g_Ram.UserParam.SetPosition, g_Ram.Status.PositionPr);
        tmpdata2 = g_Core.MotorControl.RequestDir * -1.0;
        SpeedRef = tmpdata2 * tmpdata;
    
  }


/*

  // что то делаем пока едем
   if (g_Ram.UserParam.DuSource == mdsDac)
    {
      tmpdata   = p_Controller(g_Ram.UserParam.SetPosition, g_Ram.Status.PositionPr);
      tmpdata2 = g_Core.MotorControl.RequestDir * -1.0;
      SpeedRef = tmpdata2 * tmpdata;
      //Core_ValveDriveMove(&g_Core.VlvDrvCtrl, g_Ram.UserParam.SetPosition);
    }
   else 
   {    
      tmpdata   = p_Controller(g_Core.MotorControl.RequestPos, g_Ram.Status.PositionPr);
      tmpdata2 = g_Core.MotorControl.RequestDir * -1.0;
      SpeedRef = tmpdata2 * tmpdata;
   }
  */
}

static void DynBreakMode(void)
{
    // что то делаем пока тормозим
  
  /*  Float Level; 
  
    if (g_Ram.UserParam.DcBrakeType == dbtOff) 
    {
        g_Core.MotorControl.WorkMode = wmStop;
        return;
    }
 
    
    Level = _IQsat( g_Core.DcBrake.Level, 0.985, 0.000);
    
    switch (g_Core.DcBrake.Type)
    {
    case dbtVolt:
              g_Core.ipark.Ds = Level;
              g_Core.ipark.Qs = 0;
      break;      
    case dbtCurr:
          // for vector control
      break;
    }
    
    if (++g_Core.DcBrake.Timer >= g_Core.DcBrake.SetTime)
    {
        g_Core.DcBrake.Timer = 0;
        g_Core.DcBrake.Flag = true;
    }
    
    
    if (g_Core.DcBrake.Flag)
    {
        g_Core.MotorControl.WorkMode = wmStop;
        g_Core.DcBrake.Flag = false;
    }
 */
          g_Core.VlvDrvCtrl.StartDelay = 0xFFFF;
          if (SpeedEnable == 2 && SpeedRef != 0)
          { 
            SpeedRef = AngleInterp(speedstart, SpeedMax, TimeSpeedStop);
          }
          else if (SpeedEnable == 2 && SpeedRef == 0)
          {
            g_Core.MotorControl.WorkMode = wmStop;
            g_Core.VlvDrvCtrl.StartDelay = (Uns)START_DELAY_TIME;
          }
  
}
//-------------------------------------------------------------------------------

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
        if (g_Ram.Comands.PrtReset)
	{
          if(!g_Core.Status.bit.Stop) g_Core.VlvDrvCtrl.Mpu.CancelFlag = true;
          else {                
                  Core_ProtectionsClear(&g_Core.Protections);
                  g_Core.VlvDrvCtrl.EvLog.Value = CMD_RES_FAULT;
                }
          g_Ram.Comands.PrtReset = 0;
        }	
}

double p_Controller(LgInt setPoint, LgInt processValue)
{
  LgInt error;
  double out, p_term;
  
  g_Core.pidData.MAX_OUT = (((float)g_Ram.UserParam.SpeedStart)/10)/50;
  g_Core.pidData.kp = (double)g_Ram.UserParam.Kp / 1000;
  
  if (setPoint > processValue)
  {
    error = setPoint - processValue;
  }
  else if (processValue > setPoint)
  {
    error = processValue - setPoint;
  }
  else if (processValue == setPoint) error = 0;
 

	p_term = g_Core.pidData.kp * error;
	out = p_term;
        
	if (out > g_Core.pidData.MAX_OUT) 
        {
		out = g_Core.pidData.MAX_OUT;
	} 
        else if (out < g_Core.pidData.MIN_OUT)
        {
		out = g_Core.pidData.MIN_OUT;
	}

  return out;
}

void core10HZupdate(void)
{
    if (StartTimer > 0) StartTimer--;
    else {
    
      HAL_HRTIM_SimplePWMStart(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1);
    }
        
    if (g_Ram.UserParam.DuSource == mdsDigital) g_Peref.DAC_on_off = 0x02;
    else if (g_Ram.UserParam.DuSource == mdsDac)
    {
      if (g_Ram.Status.CalibState == csCalib)
      g_Peref.DAC_on_off = 0;
      else g_Peref.DAC_on_off = 0x2;
    }
    uint16_t addr = 0;
    Uns data = 0;
   if (GoToLocalFLag)
   {
        GoToLocalFLag = 0;
        if (g_Ram.UserParam.MuDuSetup != mdMuOnly && IsMemParReady())
        {
                g_Ram.UserParam.MuDuSetup = mdMuOnly;
                data = g_Ram.UserParam.MuDuSetup;
                addr = GetAdr(UserParam.MuDuSetup);
               
                ReadWriteEeprom(&Eeprom1,F_WRITE,addr,&data,1);
        }
   
   }
}

void coreTU(TCore *p)
{
  static Byte clrReset = 0;
  
  if (g_Ram.UserParam.DuSource == mdsDigital){
    
   
    
    if (g_Ram.UserParam.InputType == it24)
    {
      g_Ram.HideParam.TuState        =  g_Peref.TU_data24 ^ g_Ram.UserParam.TuInvert.all;
      g_Ram.Status.StateTu.all       =  g_Peref.TU_data24;
    }
    else if (g_Ram.UserParam.InputType == it220)
    {
          g_Ram.HideParam.TuState    = g_Peref.TU_data220  ^ g_Ram.UserParam.TuInvert.all;
          g_Ram.Status.StateTu.all   = g_Peref.TU_data220;
    }
    
    switch (g_Ram.UserParam.MuDuSetup)
    {
                case mdOff:
		case mdMuOnly:
		case mdDuOnly:
				/*GrA->Faults.Proc.bit.MuDuDef = 0;
				MuDuDefTimer = 0;
				mudustatedefect = 0;
				mudustatefault = 0;*/
				break;
                case mdSelect:{
                              if (g_Ram.Status.Status.bit.Stop)
                              {
                                   if (g_Ram.Status.StateTu.bit.DU ^ g_Ram.UserParam.TuInvert.bit.DU)
                                   {
                                        p->VlvDrvCtrl.MuDuInput = 0;
                                   }
                                   else 
                                   {
                                        p->VlvDrvCtrl.MuDuInput = 1;
                                   }
                              }
    
                  }
                  break;
    }
    
    if (g_Ram.Status.StateTu.bit.ResetAlarm ^ g_Ram.UserParam.TuInvert.bit.ResetAlarm)
    {
        
        if (clrReset==0)g_Ram.Comands.PrtReset = 1;
        clrReset = 1;
    }
    else 
    {
      clrReset = 0;
    }
  }
  else 
  {
    g_Ram.HideParam.TuState = 0;
    g_Ram.Status.StateTu.all = 0;
  }
}

void coreTS(TCore *p)
{
   g_Ram.HideParam.HideStateTs.bit.Opened       = p->Status.bit.Opened  ^ g_Ram.UserParam.TsInvert.bit.Dout0;
   g_Ram.HideParam.HideStateTs.bit.Closed       = p->Status.bit.Closed  ^ g_Ram.UserParam.TsInvert.bit.Dout1;
   g_Ram.HideParam.HideStateTs.bit.Opening      = p->Status.bit.Opening ^ g_Ram.UserParam.TsInvert.bit.Dout2;
   g_Ram.HideParam.HideStateTs.bit.Closing      = p->Status.bit.Closing ^ g_Ram.UserParam.TsInvert.bit.Dout3;
   g_Ram.HideParam.HideStateTs.bit.Fault        = (p->Status.bit.Fault || g_Core.Protections.Dac_No_Conn_Tmp) ^ g_Ram.UserParam.TsInvert.bit.Dout4;
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
       
       g_Core.VlvDrvCtrl.Mpu.Enable != g_Core.Status.bit.Program;
  
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
                  g_Ram.HideParam.CmdButton  =   KEY_OPEN;           
                    // Mcu.Mpu.BtnKey = KEY_OPEN;
                   /*   if (SpeedRef == 0){
                        TimerInterp = 0;
                                    speedstart = 0;
                                    SpeedMax = (((float)g_Ram.UserParam.SpeedStart)/10)/50;
                                    SpeedEnable = 1;}*/
                            break;
                    case BTN_CLOSE:
                                  
                    // Mcu.Mpu.BtnKey = KEY_CLOSE;
                   g_Ram.HideParam.CmdButton =KEY_CLOSE;
                  /*                 if (SpeedRef == 0){
                                     TimerInterp = 0;
                                    speedstart = 0;
                                    SpeedMax = ((((float)g_Ram.UserParam.SpeedStart)/10)*-1.0)/50;
                                    SpeedEnable = 1;}*/
                            break;
                    case BTN_PROG1:
                      {
                          if (p->Status.bit.Stop)
                          {
                           menu.Key = KEY_ENTER;
                           readyForNewCmd = false;
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
                 /* if (SpeedRef != 0){
                    TimerInterp = 0;
                          SpeedEnable = 2;
                          SpeedMax = 0;
                          speedstart = SpeedRef;}
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)*/
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                          
                          
                      GoToLocalFLag = 1;
                      g_Ram.HideParam.CmdButton =KEY_STOP;
                          
                          
                break;
                
                case BTN_STOP2:
                        /*  if (SpeedRef != 0){
                            TimerInterp = 0;
                          SpeedEnable = 2;
                          SpeedMax = 0;
                          speedstart = SpeedRef;}
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)*/
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                            g_Ram.HideParam.CmdButton =KEY_STOP;
                            GoToLocalFLag = 1;
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
 
   if (EndValue > 0)
   {
	if (TimerInterp == 0) 
          OutputQ15 = StartValue;
	//else OutputQ15 = OutputQ15 - _IQ15div(((LgInt)(StartValue - EndValue) << 15), _IQ15mpy((LgInt)Time * 3277, _IQ15(200.0)));	// РџСЂРё РїРµСЂРµРЅРѕСЃРµ С„СѓРЅРєС†РёРё РёР· РґСЂСѓРіРѕРіРѕ РїСЂРѕРµРєС‚Р°, Р±СѓРґСЊС‚Рµ РІРЅРёРјР°С‚РµР»СЊРЅС‹РјРё!!
        else 
        {
          data1 = (EndValue - StartValue); 
          data2 = Time * 5;
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
          data2 = Time * 5;
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
          data2 = Time * 5;
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
          data2 = Time * 5;
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

/*
void Coast_stop_calc(TInvControl *v)
{
	v->CoastStop.Gain = v->CoastStop.Gain - v->CoastStop.DeltaPower;
	if (v->CoastStop.Gain < 0) v->CoastStop.Gain = 0;
	
	v->ipark.Ds = _IQmpy(v->CoastStop.RefD, v->CoastStop.Gain);
	v->ipark.Qs = _IQmpy(v->CoastStop.RefQ, v->CoastStop.Gain);
}

 void CoastStopSet(TInvControl *v)
{
	if (_IQabs(v->Status.RefOut) < _IQ(0.02))
	{
		v->Status.WorkMode = iwmCoastStop;
		v->CoastStop.Gain  = _IQ(1.0);
		v->CoastStop.RefD  = v->ipark.Ds;
		v->CoastStop.RefQ  = v->ipark.Qs;
	}
	else
		if (v->Status.AlarmFlag) v->Status.WorkMode = iwmLock;
		else v->Status.WorkMode = iwmReady;
}
*/


