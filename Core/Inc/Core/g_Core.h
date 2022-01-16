/*======================================================================

Inputs 
Outputs
========================================================================*/

#ifndef CORE_
#define CORE_


#include "main.h"
#include "core_Menu.h"
#include "csl_memory.h"
#include "core_VlvDrvCtrl.h"
#include "core_TorqueObserver.h"
#include "stat_fm25v10.h"

#define CANCEL_TOUT				(2.000 * PRD_10HZ)

// обработка датчиков холла
#define BTN_OPEN			1
#define BTN_CLOSE			2
#define BTN_STOP1			4
#define BTN_STOP2			8
#define BTN_PROG1               	16
#define BTN_PROG2                       32
#define BTN_STOP                        (BTN_STOP1 | BTN_STOP2)

typedef struct {
	
    TStatusReg 			Status;			// Статус работы   
    
    TCoreVlvDrvCtrl		VlvDrvCtrl;		// Управление задвижкой
    // ---
    TTorqObs			TorqObs;		// Расчет момента
      
    Uns				NoErrFlag;
    Uns                         DisplayTimer;
    Uns                         DisplayRestartTimer;
    Uns 			DefFlag;
    Uns     			CancelTimer;       // время до отмены

} TCore;

void Core_Init(TCore *);
void core18kHZupdate(void);
void core200HZupdate(void);
void core50HZupdate(void);
void core10HZupdate(void);

void coreTS(TCore *);
void coreTLocalControl(TCore *);
void coreTU(TCore *);

extern TCore g_Core;
#endif // CORE_



