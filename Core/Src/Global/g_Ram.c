#include "g_Ram.h"


TRam g_Ram;
Uns RefreshCub=0;
Uns size = 0;

void ReWriteParams(void);

void g_Ram_Init(TRam *p)
{
        p->Comands.SetDefaults = 0;
	p->FactoryParam.SetDefaults = 0;

	p->Status.Status.all = 0x01;
	p->Status.CalibState = p->HideParam.CalibState;
	p->Status.CycleCnt = p->HideParam.CycleCnt;
	//g_Core.PrevCycle = p->ramGroupH.CycleCnt;
        
        p->HideParam.StartIndic = 8;

	// Версия софта - Пример 1.001.
	p->Status.VersionPO = VERSION;				
	p->FactoryParam.SubVersionPO = SUBVERSION;
}

void g_Ram_Update(TRam *p)
{    
     p->Status.Temper = g_Peref.BlockTemper;
    //------ Core -> RAM ------------------------------------
     p->Status.Status 		= g_Core.Status;
    
    p->Status.Faults.Net.all    = (g_Core.Protections.outFaults.Net.all);
    p->Status.Faults.Load.all   = (g_Core.Protections.outFaults.Load.all);
    p->Status.Faults.Proc.all   = (g_Core.Protections.outFaults.Proc.all);
    p->Status.Faults.Dev.all    = (g_Core.Protections.outFaults.Dev.all);
    
      if (g_Ram.Status.Status.bit.Stop)
      {
        p->Status.Iu = 0;
        p->Status.Iv = 0;
        p->Status.Iw = 0;
        p->HideParam.IuPr = 0;
        p->HideParam.IvPr = 0;
        p->HideParam.IwPr = 0;
        p->HideParam.Imid = 0;
        p->HideParam.Imidpr = 0;  
      }
      else {
        p->Status.Iu = g_Peref.Ia.Output*10;
        p->Status.Iv = g_Peref.Ib.Output*10;
        p->Status.Iw = g_Peref.Ic.Output*10;
          
        p->HideParam.IuPr = (p->Status.Iu*1000) / p->FactoryParam.Inom;
	p->HideParam.IvPr = (p->Status.Iv*1000) / p->FactoryParam.Inom;
	p->HideParam.IwPr = (p->Status.Iw*1000) / p->FactoryParam.Inom;
          
         p->HideParam.Imid = (Uns)(g_Peref.Imfltr.Output*10);
         p->HideParam.Imidpr = (p->HideParam.Imid*1000) / p->FactoryParam.Inom;
         
      }
    g_Ram.Status.Position = g_Ram.HideParam.Position & 0xFFFF;
    g_Ram.FactoryParam.ADC_Data =(Uns) g_Peref.ADCToProcfltr.Output;
      
     //------------------------------------------------------
    Uns PassAddr;
    //-------- Для первого включения -----------------------

    if (g_Ram.HideParam.Password1 == 65535 && g_Ram.HideParam.Password2 == 65535)
	{
	    if (IsMemParReady())
	    {
			PassAddr = REG_PASSW1;
			g_Ram.HideParam.Password1 = 0;
			g_Ram.HideParam.Password2 = 0;
			WriteToEeprom(PassAddr, &g_Ram.HideParam.Password1, 2);
	    }
	}

     p->Status.Position 		= p->HideParam.Position;
     p->HideParam.FullStep 		= g_Peref.Position.FullStep;
     ReWriteParams();
}
//---------------------------------------------------

void ReWriteParams(void)
{
  	Drive_ReWrite_Update();
        if (RefreshCub==1){
		if (IsMemParReady())
		{
			RefreshCub = 0;
                          WriteToEeprom(GetAdr(HideParam.TqCurr.Data[0][0])-1, &g_Ram.HideParam.TransCurr, 6);
		}
	}

}

// Обновление значение по требованию
void RefreshParams(Uns addr)
{

	TPerefPosition *pPosition 		= &g_Peref.Position;

	if (addr >= REG_ADC_DOTS && addr <= REG_DAC_DOTS)	{

		      peref_ADCtoPRCObserverInit(&g_Peref);

	}else if (addr >= REG_DAC_DOTS && addr <= REG_RSVD196){
                      peref_ProctoDACObserverInit(&g_Peref);
        }else if (addr == REG_ADC_REVERS)	{
		      peref_ADCtoPRCObserverInit(&g_Peref);
	}else if (addr >= REG_DAC_REVERS ){
                      peref_ProctoDACObserverInit(&g_Peref);
        }else if (addr == REG_DRIVE_TYPE) {

		Core_Drive_Update();
		CubRefresh(&g_Core.TorqObs.Cub1, &g_Ram.HideParam.TqCurr);
		RefreshCub = 1;
        }else if (addr == REG_MAX_TRQE)
	{
		 g_Core.TorqObs.TorqueMax = g_Ram.FactoryParam.MaxTorque * 10; //??? убрать в обновление параметров
	}else if (addr == REG_I_NOM){
                g_Core.Mash1 = (1.0 / (float)g_Ram.FactoryParam.Inom);
        }
          

/*else if (addr == REG_OVERWAY_ZONE) { g_Core.VlvDrvCtrl.Valve.BreakDelta = (((LgUns)pPosition->GearRatio * (LgUns)g_Ram.ramGroupB.OverwayZone) << *pPosition->PosSensPow)/10; //CalcClbAbsRev(&Calib, g_Ram.ramGroupB.OverwayZone);

	} else if (addr == REG_DRIVE_TYPE) {

		Core_Drive_Update();
		CubRefresh(&g_Core.TorqObs.Cub1, &g_Ram.ramGroupH.TqCurr);
		CubRefresh(&g_Core.TorqObs.Cub2, &g_Ram.ramGroupH.TqAngUI);
		RefreshCub = 1;

	}	else if (addr >= REG_TORQUE_CURR && addr < REG_TORQUE_ANG_SF)
	{
		CubRefresh(&g_Core.TorqObs.Cub1, &g_Ram.ramGroupH.TqCurr);
		CubRefresh(&g_Core.TorqObs.Cub2, &g_Ram.ramGroupH.TqAngUI);
		RefreshCub = 1;
	}	else if (addr == REG_MAX_TRQE)
	{
		 g_Core.TorqObs.TorqueMax = g_Ram.ramGroupC.MaxTorque * 10; //??? убрать в обновление параметров
	}	else if (addr == REG_SIN_FILTER_TF){

			peref_ApFilter1Init(&g_Peref.URfltr, 		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);		// Инициализируем фильтры
			peref_ApFilter1Init(&g_Peref.USfltr,		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UTfltr, 		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IUfltr, 		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IVfltr, 		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IWfltr, 		(Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);


			peref_ApFilter1Init(&g_Peref.UfltrOpen, 	 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);		// Инициализируем фильтры
			peref_ApFilter1Init(&g_Peref.UfltrClose, 	 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrStopOpen,  (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrMu, 		 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrResetAlarm,(Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrStopClose, (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrDU, 		 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);

	} else if (addr == REG_RMS_FILTER_TF) {

			peref_ApFilter3Init(&g_Peref.UR3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);		// Инициализируем фильтры
			peref_ApFilter3Init(&g_Peref.US3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.UT3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IU3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IV3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IW3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);

			peref_ApFilter1Init(&g_Peref.Phifltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter1Init(&g_Peref.Umfltr,  (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.Imfltr,  (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);

			peref_ApFilter3Init(&g_Peref.U3fltrOpen, 	  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);		// Инициализируем фильтры
			peref_ApFilter3Init(&g_Peref.U3fltrClose, 	  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrStopOpen,  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrMu, 		  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrResetAlarm,(Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrStopClose, (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrDU, 		  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);

	} else if (addr == REG_TU_TYPE) {

		if (g_Ram.ramGroupB.InputType==it24)
		{
			g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy		= &g_Ram.ramGroupC.p_UOpen_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy		= &g_Ram.ramGroupC.p_UClose_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UStopOpen_Mpy	= &g_Ram.ramGroupC.p_UStopOpen_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy		= &g_Ram.ramGroupC.p_UMu_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UStopClose_Mpy	= &g_Ram.ramGroupC.p_UStopClose_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy= &g_Ram.ramGroupC.p_UResetAlarm_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy		= &g_Ram.ramGroupC.p_UDu_Mpy24;
		}
		else
		{
			g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy		= &g_Ram.ramGroupC.p_UOpen_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy		= &g_Ram.ramGroupC.p_UClose_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UStopOpen_Mpy	= &g_Ram.ramGroupC.p_UStopOpen_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy		= &g_Ram.ramGroupC.p_UMu_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UStopClose_Mpy	= &g_Ram.ramGroupC.p_UStopClose_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy= &g_Ram.ramGroupC.p_UResetAlarm_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy		= &g_Ram.ramGroupC.p_UDu_Mpy220;
		}
	}
		else if (addr == REG_INDICATOR_TYPE)	{
		DisplayReset(&g_Peref.Display, (Uns)g_Ram.ramGroupC.IndicatorType);
	}*/


}
