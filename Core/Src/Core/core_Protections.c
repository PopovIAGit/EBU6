

#include "g_Core.h"
#include "peref.h"

Uns U,V,W;
Uns State = 0;

void Core_ProtectionsInit(TCoreProtections *p)
{
    p->FaultDelay = (Uns) (PRD_50HZ * 3);
      
    
    	//---------Нет Движенния-----------------------------------------------

	p->NoMove.Cfg.all = PRT_CFG_SET(CAN_BE_MUFTA, INP_LESS_LEVEL, NoMove_bit, HYST_OFF);
	p->NoMove.Input = (Int *) &g_Ram.Status.Position;
	p->NoMove.Output =  &p->NoMoveFlag;
	p->NoMove.EnableLevel = (Int *) &g_Ram.FactoryParam.MuffZone;
	p->NoMove.DisableLevel = (Int *) &g_Ram.FactoryParam.MuffZone;
	p->NoMove.Timeout = &g_Ram.UserParam.NoMoveTime;
	p->NoMove.Scale = PROTECT_SCALE;
	p->NoMove.Signal = 0;
        //---------Защиты по напряжению---------------------------------------
	//---------просадка напряжения---------------------------------------

	p->underVoltageR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvR_bit, 20);
	p->underVoltageS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvS_bit, 20);
	p->underVoltageT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvT_bit, 20);

	p->underVoltageR.Input = (Int *) &g_Ram.Status.Ur;
	p->underVoltageS.Input = (Int *) &g_Ram.Status.Us;
	p->underVoltageT.Input = (Int *) &g_Ram.Status.Ut;

	p->underVoltageR.Output = (Uns *) &p->outFaults.Net.all;
	p->underVoltageS.Output = (Uns *) &p->outFaults.Net.all;
	p->underVoltageT.Output = (Uns *) &p->outFaults.Net.all;

	p->underVoltageR.EnableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;
	p->underVoltageS.EnableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;
	p->underVoltageT.EnableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;

	p->underVoltageR.DisableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;
	p->underVoltageS.DisableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;
	p->underVoltageT.DisableLevel = (Int *) &g_Ram.FactoryParam.UvLevel;

	p->underVoltageR.Timeout = &g_Ram.FactoryParam.UvTime;
	p->underVoltageS.Timeout = &g_Ram.FactoryParam.UvTime;
	p->underVoltageT.Timeout = &g_Ram.FactoryParam.UvTime;

	p->underVoltageR.Scale = PROTECT_SCALE;
	p->underVoltageS.Scale = PROTECT_SCALE;
	p->underVoltageT.Scale = PROTECT_SCALE;

	//---------Перенапряжение по входу------------------------------------------
	p->overVoltageR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvR_bit, 20);
	p->overVoltageS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvS_bit, 20);
	p->overVoltageT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvT_bit, 20);

	p->overVoltageR.Input = (Int *) &g_Ram.Status.Ur;
	p->overVoltageS.Input = (Int *) &g_Ram.Status.Us;
	p->overVoltageT.Input = (Int *) &g_Ram.Status.Ut;

	p->overVoltageR.Output = (Uns *) &p->outFaults.Net.all;
	p->overVoltageS.Output = (Uns *) &p->outFaults.Net.all;
	p->overVoltageT.Output = (Uns *) &p->outFaults.Net.all;

	p->overVoltageR.EnableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;
	p->overVoltageS.EnableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;
	p->overVoltageT.EnableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;

	p->overVoltageR.DisableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;
	p->overVoltageS.DisableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;
	p->overVoltageT.DisableLevel = (Int *) &g_Ram.FactoryParam.OvLevel;

	p->overVoltageR.Timeout = &g_Ram.FactoryParam.OvTime;
	p->overVoltageS.Timeout = &g_Ram.FactoryParam.OvTime;
	p->overVoltageT.Timeout = &g_Ram.FactoryParam.OvTime;

	p->overVoltageR.Scale = PROTECT_SCALE;
	p->overVoltageS.Scale = PROTECT_SCALE;
	p->overVoltageT.Scale = PROTECT_SCALE;

	//---------обрыв фаз питания-----------------------
	p->breakVoltR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvR_bit, 20);
	p->breakVoltS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvS_bit, 20);
	p->breakVoltT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvT_bit, 20);

	p->breakVoltR.Input = (Int *) &g_Ram.Status.Ur;
	p->breakVoltS.Input = (Int *) &g_Ram.Status.Us;
	p->breakVoltT.Input = (Int *) &g_Ram.Status.Ut;

	p->breakVoltR.Output = (Uns *) &p->outFaults.Net.all;
	p->breakVoltS.Output = (Uns *) &p->outFaults.Net.all;
	p->breakVoltT.Output = (Uns *) &p->outFaults.Net.all;

	p->breakVoltR.EnableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;
	p->breakVoltS.EnableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;
	p->breakVoltT.EnableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;

	p->breakVoltR.DisableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;
	p->breakVoltS.DisableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;
	p->breakVoltT.DisableLevel = (Int *) &g_Ram.FactoryParam.BvLevel;

	p->breakVoltR.Timeout = &g_Ram.FactoryParam.BvTime;
	p->breakVoltS.Timeout = &g_Ram.FactoryParam.BvTime;
	p->breakVoltT.Timeout = &g_Ram.FactoryParam.BvTime;

	p->breakVoltR.Scale = PROTECT_SCALE;
	p->breakVoltS.Scale = PROTECT_SCALE;
	p->breakVoltT.Scale = PROTECT_SCALE;
	//---------Защиты по току---------------------------------------------
	//---------Обрыв выходных фаз---------------------------------
	p->breakCurrU.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, PhlU_bit, HYST_OFF);
	p->breakCurrV.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, PhlV_bit, HYST_OFF);
	p->breakCurrW.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, PhlW_bit, HYST_OFF);

	p->breakCurrU.Input = (Int *) &g_Ram.HideParam.IuPr;
	p->breakCurrV.Input = (Int *) &g_Ram.HideParam.IvPr;
	p->breakCurrW.Input = (Int *) &g_Ram.HideParam.IwPr;

	p->breakCurrU.Output = (Uns *)&p->outFaults.Load.all;
	p->breakCurrV.Output = (Uns *)&p->outFaults.Load.all;
	p->breakCurrW.Output = (Uns *)&p->outFaults.Load.all;

	p->breakCurrU.EnableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;
	p->breakCurrV.EnableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;
	p->breakCurrW.EnableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;

	p->breakCurrU.DisableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;
	p->breakCurrV.DisableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;
	p->breakCurrW.DisableLevel = (Int *) &g_Ram.FactoryParam.PhlLevel;

	p->breakCurrU.Timeout = &g_Ram.FactoryParam.PhlTime;
	p->breakCurrV.Timeout = &g_Ram.FactoryParam.PhlTime;
	p->breakCurrW.Timeout = &g_Ram.FactoryParam.PhlTime;

	p->breakCurrU.Scale = PROTECT_SCALE;
	p->breakCurrV.Scale = PROTECT_SCALE;
	p->breakCurrW.Scale = PROTECT_SCALE;

	//------КЗ----------------------------------------------
	p->ShC_U.Cfg.bit.Num = ShCU_bit;
	p->ShC_V.Cfg.bit.Num = ShCV_bit;
	p->ShC_W.Cfg.bit.Num = ShCW_bit;

	p->ShC_U.Cfg.bit.Hyst = HYST_OFF;
	p->ShC_V.Cfg.bit.Hyst = HYST_OFF;
	p->ShC_W.Cfg.bit.Hyst = HYST_OFF;

	p->ShC_U.InputCurrent = (Uns *)&U;
	p->ShC_V.InputCurrent = (Uns *)&V;
	p->ShC_W.InputCurrent = (Uns *)&W;

	p->ShC_U.HighCurrentLevel = &g_Ram.FactoryParam.ShC_Up;
	p->ShC_V.HighCurrentLevel = &g_Ram.FactoryParam.ShC_Up;
	p->ShC_W.HighCurrentLevel = &g_Ram.FactoryParam.ShC_Up;

	p->ShC_U.LowCurrentLevel = &g_Ram.FactoryParam.ShC_Down;
	p->ShC_V.LowCurrentLevel = &g_Ram.FactoryParam.ShC_Down;
	p->ShC_W.LowCurrentLevel = &g_Ram.FactoryParam.ShC_Down;

	p->ShC_U.Output = &p->outFaults.Load.all;
	p->ShC_V.Output = &p->outFaults.Load.all;
	p->ShC_W.Output = &p->outFaults.Load.all;

	//------Защиты устройства------------------------------------------
	//------перегрв блока”----------------------------------------------
	p->overHeat.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, Th_bit, 1);
	p->overHeat.Input = &g_Ram.Status.Temper;
	p->overHeat.Output = &p->outFaults.Dev.all;
	p->overHeat.EnableLevel = &g_Ram.FactoryParam.overHeatLevel;	
	p->overHeat.DisableLevel = &g_Ram.FactoryParam.overHeatLevel;
	p->overHeat.Timeout = &g_Ram.FactoryParam.overHeatTime;
	p->overHeat.Scale = PROTECT_SCALE;

	//------переохлаждение блока----------------------------------------
	p->underCold.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, Tl_bit, 1);
	p->underCold.Input = &g_Ram.Status.Temper;
	p->underCold.Output = &p->outFaults.Dev.all;
	p->underCold.EnableLevel  = &g_Ram.FactoryParam.underColdLevel;
	p->underCold.DisableLevel = &g_Ram.FactoryParam.underColdLevel;
	p->underCold.Timeout = &g_Ram.FactoryParam.underColdTime;
	p->underCold.Scale = PROTECT_SCALE;     
        //----------перегрев двигателя-------------------------------------------
        p->driveTemper.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, DriveTemper_bit, 1);
	p->driveTemper.Input = &g_Ram.Status.DriveTemper;
	p->driveTemper.Output = &p->outFaults.Dev.all;
	p->driveTemper.EnableLevel  = &g_Ram.FactoryParam.driveTemperLevel;
	p->driveTemper.DisableLevel = &g_Ram.FactoryParam.driveTemperLevel;
	p->driveTemper.Timeout = &g_Ram.FactoryParam.driveTemperTime;
	p->driveTemper.Scale = PROTECT_SCALE; 
        //--------перегрев силового модуля------------------------
        p->moduleTemper.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, ModuleTemper_bit, 1);
	p->moduleTemper.Input = &g_Ram.Status.ModuleTempers;
	p->moduleTemper.Output = &p->outFaults.Dev.all;
	p->moduleTemper.EnableLevel  = &g_Ram.FactoryParam.moduleTemperLevel;
	p->moduleTemper.DisableLevel = &g_Ram.FactoryParam.moduleTemperLevel;
	p->moduleTemper.Timeout = &g_Ram.FactoryParam.moduleTemperTime;
	p->moduleTemper.Scale = PROTECT_SCALE; 
   
}

void Core_ProtectionsEnable(TCoreProtections *p)
{

	Bool Enable;

	if (p->FaultDelay > 0)
		return;
        State++;
        
	switch (State)
	{
            case 1:  //Muffta
		p->NoMove.Cfg.bit.Enable = ((g_Core.MotorControl.WorkMode & wmMove) != 0);
		break;
            case 2:  // Защиты по наряжению
		Enable = (g_Ram.FactoryParam.Uv != pmOff) && (!p->outFaults.Net.bit.BvR)&& (!p->outFaults.Net.bit.BvS)&& (!p->outFaults.Net.bit.BvT);						// Понижеие напряжения
		p->underVoltageR.Cfg.bit.Enable = Enable;
		p->underVoltageS.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		p->underVoltageT.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		
		Enable = (g_Ram.FactoryParam.Ov != pmOff);						// Повышение напряжения
		p->overVoltageR.Cfg.bit.Enable = Enable;
		p->overVoltageS.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		p->overVoltageT.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		
		Enable = (g_Ram.FactoryParam.Bv != pmOff);						// Обрыв фаз напряжения
		p->breakVoltR.Cfg.bit.Enable = Enable;
		p->breakVoltS.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		p->breakVoltT.Cfg.bit.Enable = Enable & !g_Ram.FactoryParam.MCU220380;
		break;
            case 3:  // Защиты по току
		Enable = (g_Ram.FactoryParam.Phl != pmOff) && (!g_Core.Status.bit.Stop);					// Обрыв выходных фаз (двиг.)
		p->breakCurrU.Cfg.bit.Enable = Enable;
		p->breakCurrV.Cfg.bit.Enable = Enable;
		p->breakCurrW.Cfg.bit.Enable = Enable;

		//Enable = (g_Ram.ramGroupC.ShC != pmOff) && (!g_Core.Status.bit.Stop);					// Короткое замыкание
		Enable = (g_Ram.FactoryParam.ShC != pmOff);
		p->ShC_U.Cfg.bit.Enable = Enable;
		p->ShC_V.Cfg.bit.Enable = Enable;
		p->ShC_W.Cfg.bit.Enable = Enable;

		break;
            case 4:  // Защиты устройства
		Enable = g_Ram.FactoryParam.overHeat != pmOff;
		p->overHeat.Cfg.bit.Enable = Enable;
                
		Enable = g_Ram.FactoryParam.underCold != pmOff;
		p->underCold.Cfg.bit.Enable = Enable;
                
                Enable = g_Ram.FactoryParam.driveTemper != pmOff;
		p->driveTemper.Cfg.bit.Enable = Enable;
                             
                Enable = g_Ram.FactoryParam.moduleTemper != pmOff;
		p->moduleTemper.Cfg.bit.Enable = Enable;
                
                State = 0;	// вернулись к истокам
		break;
	}
}

void Core_ProtectionsReset(TCoreProtections *p)
{

}

void Core_ProtectionsClear(TCoreProtections *p)
{
	p->outFaults.Dev.all = 0;					// СЃР±СЂРѕСЃРёР»Рё РІСЃРµ Р°РІР°СЂРёРё
	p->outFaults.Net.all = 0;
	p->outFaults.Load.all = 0;
	p->outFaults.Proc.all = 0;
        p->Dac_No_Conn_Tmp = 0;
        p->NoMoveFlag = 0;
}

void Core_Protections50HZUpdate(TCoreProtections *p)
{
	if (p->FaultDelay > 0)
	{
		p->FaultDelay--;
		return;
	}
        
        p->outFaults.Proc.bit.Overway = g_Core.MotorControl.OverWayFlag;
}

void Core_Protections18kHzUpdate(TCoreProtections *p)
{
	Uns MuffEnable, MuffAddr, TH_BCP_addr;

	if (p->FaultDelay > 0)
		return;
        
        U = g_Peref.adcData3[1];
        V = g_Peref.adcData3[2];
        W = g_Peref.adcData3[3];
        
        Core_ProtecionSHC_Update(&p->ShC_U);
	Core_ProtecionSHC_Update(&p->ShC_V);
	Core_ProtecionSHC_Update(&p->ShC_W);

        if (p->outFaults.Dev.all || p->outFaults.Load.all || p->outFaults.Net.all || p->outFaults.Proc.all)
	{
          g_Core.Status.bit.Fault = 1;
	
        }
	else
	  g_Core.Status.bit.Fault = 0;

	
	if (!g_Core.Status.bit.Stop)
	{
          
          	if (p->NoMoveFlag)
		{
			p->outFaults.Proc.bit.NoMove = p->NoMoveFlag;

		}

		if (g_Core.Status.bit.Fault)   	
		{
			Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
			g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
	}

}
