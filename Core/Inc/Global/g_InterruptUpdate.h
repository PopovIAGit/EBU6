

#ifndef __InterruptUpdate_H
#define __InterruptUpdate_H

#include "main.h"


#define PrdElemInit(Name, Argument) \
	{ (TPrdFunc)(Name), (void *)(Argument), 0 }


#define TaskCount(List)	(SIZE(List) / SIZE(TPeriodicalFunction))

#define TASK_COUNT_MAX_2kHZ  10		
#define TASK_COUNT_MAX_200HZ 20		
#define TASK_COUNT_MAX_50HZ  80		
#define TASK_COUNT_MAX_10HZ  200	

typedef void (*TPrdFunc)(void *Data);	

typedef struct _TPrdElem 
{
  TPrdFunc Func;	
  void *Data;		
  Uint16  CpuTime;      
} TPeriodicalFunction;	

extern void InterruptUpdate (void);

// ¬ысокоприоритетные прерывани¤ на частоте 18 кvц

// ================================ 36 кvц ==================================

TPeriodicalFunction Task18kHz[] =         
{  
  PrdElemInit(peref_18KHzCalc,                &g_Peref),
  PrdElemInit(core18kHZupdate,                NULL)  
};

//—юда вставл¤ем функции дл¤ соответствующей группы

// ================================ 2 Рєв€љС† ==================================

TPeriodicalFunction Task2kHz[] =          //Ќе более 8-х задач
{
  PrdElemInit(FM25V10_Update,		      &Eeprom1),
  PrdElemInit(SerialCommTimings,	      &g_Comm.mbAsu),	//РЅР° 2 Рєв€љС†
  PrdElemInit(SerialCommTimings,	      &g_Comm.mbEncoder),	//РЅР° 2 Рєв€љС†
  PrdElemInit(peref_2KHzCalc,                 &g_Peref)
};
// ================================ 200 vц ==================================

TPeriodicalFunction Task200Hz[] =       	//не более  20-ти задач
{
     PrdElemInit(DISPL_Update,                   &g_Peref.Display),
     PrdElemInit(core200HZupdate,			NULL),	//	20
     PrdElemInit(Peref_CalibUpdate, 						&g_Peref.Position),
     PrdElemInit(peref_200HzCalc,                &g_Peref)
};

// ================================ 50 vц ==================================

TPeriodicalFunction Task50Hz[] =        //не более  80-ти задач
{
 
    PrdElemInit(Core_SetDeff,				&menu),
    PrdElemInit(coreTS,					&g_Core),
    PrdElemInit(coreTLocalControl,			&g_Core),
    PrdElemInit(coreTU,			                &g_Core),
    PrdElemInit(LogInputCalc,			&g_Peref.BtnClose),	//	3
    PrdElemInit(LogInputCalc,			&g_Peref.BtnStop1),	//	4
    PrdElemInit(LogInputCalc,			&g_Peref.BtnStop2),	//	5
    PrdElemInit(LogInputCalc,			&g_Peref.BtnOpen),	//	6
    PrdElemInit(LogInputCalc,			&g_Peref.BtnProg1),	//	5
    PrdElemInit(LogInputCalc,			&g_Peref.BtnProg2),	//	6
    PrdElemInit(Core_CalibControl, 				&g_Core),
    PrdElemInit(Core_CalibStop, 				&g_Core),
    PrdElemInit(core50HZupdate,			NULL),	//	20
    PrdElemInit(Comm_50HzCalc,					&g_Comm),
    PrdElemInit(Comm_CommandUpdate,				&g_Comm),
    PrdElemInit(Core_ControlMode, 				&g_Core),
    PrdElemInit(Core_ValveDriveUpdate, 			&g_Core.VlvDrvCtrl),
    
    PrdElemInit(peref_50HzCalc,                 &g_Peref)
};

// ================================ 10 vц ==================================
TPeriodicalFunction Task10Hz[] =        //не более  200-т задач
{  
    PrdElemInit(peref_10HzCalc,                 &g_Peref),
    PrdElemInit(g_Ram_Update,			&g_Ram),
    PrdElemInit(peref_74HC595D,			&g_Peref),
    PrdElemInit(Core_MenuUpdate,		&menu),			// 1
    PrdElemInit(Core_MENU_Display,		&menu),			// 2
    PrdElemInit(DisplayStart,			NULL),	//	20
    PrdElemInit(core10HZupdate,			NULL),	//	20
    PrdElemInit(Peref_Calibration,                 &g_Peref.Position),
    PrdElemInit(CalcClbCycle, 				&g_Peref.Position),
    PrdElemInit(Peref_SpeedCalc,			&g_Peref.Position),
    
    
    
    PrdElemInit(RtcControl,				NULL)	
  
};
//------------ конец файла-----------------------------------------------


#endif
