

#ifndef __InterruptUpdate_H
#define __InterruptUpdate_H

#include "main.h"


#define PrdElemInit(Name, Argument) \
	{ (TPrdFunc)(Name), (void *)(Argument), 0 }


#define TaskCount(List)	(SIZE(List) / SIZE(TPeriodicalFunction))

#define TASK_COUNT_MAX_2kHZ  5		
#define TASK_COUNT_MAX_200HZ 10		
#define TASK_COUNT_MAX_50HZ  40		
#define TASK_COUNT_MAX_10HZ  100	

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
  PrdElemInit(peref_18KHzCalc,                &g_Peref)
};

//���� �������� ������� �� ��������������� ������

// ================================ 2 к√ц ==================================

TPeriodicalFunction Task2kHz[] =          //�� ����� 8-� �����
{
  PrdElemInit(FM25V10_Update,		      &Eeprom1),

  PrdElemInit(peref_2KHzCalc,                 &g_Peref)
};
// ================================ 200 v� ==================================

TPeriodicalFunction Task200Hz[] =       	//�� �����  20-�� �����
{
     PrdElemInit(DISPL_Update,                   &g_Peref.Display),
     PrdElemInit(peref_200HzCalc,                &g_Peref)
};

// ================================ 50 v� ==================================

TPeriodicalFunction Task50Hz[] =        //�� �����  80-�� �����
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
    PrdElemInit(peref_50HzCalc,                 &g_Peref)
};

// ================================ 10 v� ==================================

TPeriodicalFunction Task10Hz[] =        //�� �����  200-� �����
{  
  
    
    PrdElemInit(peref_10HzCalc,			&g_Peref),	//	20
    PrdElemInit(g_Ram_Update,			&g_Ram),
    PrdElemInit(Core_MenuUpdate,		&menu),			// 1
    PrdElemInit(Core_MENU_Display,		&menu),			// 2
    PrdElemInit(DisplayStart,			NULL),	//	20
    PrdElemInit(RtcControl,				NULL),	
    PrdElemInit(peref_10HzCalc,                 &g_Peref)
};
//------------������ �����-----------------------------------------------


#endif
