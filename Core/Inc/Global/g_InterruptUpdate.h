

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

// ������������������ ��������� �� ������� 18 �v�

// ================================ 36 �v� ==================================

TPeriodicalFunction Task18kHz[] =         
{  
  PrdElemInit(peref_18KHzCalc,                &g_Peref),
    PrdElemInit(Core_Protections18kHzUpdate,			&g_Core.Protections),
    PrdElemInit(SerialCommTimings,	&g_Comm.Mb),
  PrdElemInit(core18kHZupdate,                NULL)  
};

//���� �������� ������� �� ��������������� ������

// ================================ 2 к√ц ==================================

TPeriodicalFunction Task2kHz[] =          //�� ����� 8-� �����
{
  PrdElemInit(FM25V10_Update,		      &Eeprom1),
  //PrdElemInit(SerialCommTimings,	      &g_Comm.mbAsu),	//на 2 к√ц
#if IS_VECTOR
  PrdElemInit(Vector_2KHZ,	                &g_Vector),	//на 2 к√ц
#endif  
  PrdElemInit(peref_2KHzCalc,                 &g_Peref)
};
// ================================ 200 v� ==================================

TPeriodicalFunction Task200Hz[] =       	//�� �����  20-�� �����
{
     PrdElemInit(DISPL_Update,                   &g_Peref.Display),
     PrdElemInit(core200HZupdate,			NULL),	//	20
     PrdElemInit(Peref_CalibUpdate, 						&g_Peref.Position),
     PrdElemInit(Protections_MuffFlag,						NULL),
     PrdElemInit(peref_200HzCalc,                &g_Peref)
};

// ================================ 50 v� ==================================

TPeriodicalFunction Task50Hz[] =        //�� �����  80-�� �����
{
 
    PrdElemInit(Core_SetDeff,				&menu),
    PrdElemInit(coreTS,					&g_Core),
    PrdElemInit(Core_DefineCtrlParams, 			&g_Core),
    PrdElemInit(coreTLocalControl,			&g_Core),
    PrdElemInit(coreTU,			                &g_Core),
    PrdElemInit(Core_ProtectionsEnable,		    &g_Core.Protections),
    PrdElemInit(Core_Protections50HZUpdate,		&g_Core.Protections),
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnClose),	//	3
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnStop1),	//	4
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnStop2),	//	5
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnOpen),	//	6
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnProg1),	//	5
    PrdElemInit(LogInputCalc,			        &g_Peref.BtnProg2),	//	6
    PrdElemInit(Core_CalibControl, 				&g_Core),
    PrdElemInit(Core_CalibStop, 				&g_Core),
    PrdElemInit(core50HZupdate,			        NULL),	//	20
    PrdElemInit(Comm_50HzCalc,					&g_Comm),
    PrdElemInit(Comm_CommandUpdate,				&g_Comm),
    PrdElemInit(Core_ControlMode, 				&g_Core),
    PrdElemInit(Core_ValveDriveUpdate, 			&g_Core.VlvDrvCtrl),
    PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.NoMove),
        PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrU),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrV),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrW),
    	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overHeat),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underCold),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.driveTemper),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.moduleTemper),
    PrdElemInit(Core_TorqueCalc, 				&g_Core.TorqObs),
    PrdElemInit(peref_50HzCalc,                 &g_Peref)
};

// ================================ 10 v� ==================================
TPeriodicalFunction Task10Hz[] =        //�� �����  200-� �����
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
//------------������ �����-----------------------------------------------


#endif
