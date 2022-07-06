

#include "g_Core.h"
#include "peref.h"


void Core_ProtectionsInit(TCoreProtections *p)
{
    p->FaultDelay = (Uns) (PRD_50HZ * 2);
}

void Core_ProtectionsEnable(TCoreProtections *p)
{
	static Byte State = 0;
	Bool Enable;

	if (p->FaultDelay > 0)
		return;

	switch (++State)
	{
	case 1:  //Muffta
		
		break;
	case 2:  // Защиты по наряжению
		
		break;
	case 3:  // Защиты по току
		
		break;
	case 4:  // Защиты устройства
		
		break;
	}
}

void Core_ProtectionsReset(TCoreProtections *p)
{

}

void Core_ProtectionsClear(TCoreProtections *p)
{
	

}

void Core_Protections50HZUpdate(TCoreProtections *p)
{
	if (p->FaultDelay > 0)
	{
		p->FaultDelay--;
		return;
	}
}

void Core_Protections18kHzUpdate(TCoreProtections *p)
{
	Uns MuffEnable, MuffAddr, TH_BCP_addr;

	if (p->FaultDelay > 0)
		return;

	
	if (!g_Core.Status.bit.Stop)
	{

		if (g_Core.Status.bit.Fault )   	
		{
			Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
			g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
	}

}




