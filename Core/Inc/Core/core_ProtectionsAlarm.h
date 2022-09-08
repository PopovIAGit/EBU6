/*======================================================================
��� �����:          core_ProtectionsAlarm.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		25.04.2014
��������:
������ ���������� ��������� �����
======================================================================*/

#ifndef CORE_PROTECTIONS_ALARM_
#define CORE_PROTECTIONS_ALARM_

//----------- ����������� ������������ ������ ------------------------------
#include "main.h"

//--------------------- ���������-------------------------------------------

#define ON				0
#define OFF				1

#define	CAN_NOT_BE_RESETED		0 // ������ �� ��������� �������������
#define	CAN_BE_RESETED			1 // ������ ��������� �������������
#define	CAN_BE_MUFTA			2 // ����� ������ �����

#define	INP_LESS_LEVEL			0 // ������ ��� ���������� ������� ��������� ������
#define	INP_GREATER_LEVEL		1 // ������ ��� ���������� ������� ��������� ������

#define	HYST_OFF				0 // ��� �����������
//--------------------- ������� --------------------------------------------

#define Alarm_On_Off(cfg,ALARM) 	g_Core.protections.alarmModes.##ALARM##.bit.off = cfg
#define Alarm_On(ALARM)			Alarm_On_Off(ON,ALARM)
#define Alarm_Off(ALARM)		Alarm_On_Off(OFF,ALARM)

#define IsAlarm_On_Off(cfg,ALARM) 	(g_Core.protections.alarmModes.##ALARM##.bit.off == cfg)
#define IsAlarm_On(ALARM) 		IsAlarm_On_Off(ON,ALARM)
#define IsAlarm_Off(ALARM) 		IsAlarm_On_Off(OFF,ALARM)

#define AlarmElem_On_Off(cfg,ELEM) 	g_Core.protections.##ELEM##.Cfg.bit.Enable = !cfg
#define AlarmElem_On(ALARM)		AlarmElem_On_Off(ON,ALARM)
#define AlarmElem_Off(ALARM)	AlarmElem_On_Off(OFF,ALARM)

#define IsAlarmElem_On_Off(cfg,ELEM) (g_Core.protections.##ELEM##.Cfg.bit.Enable == !cfg)
#define IsAlarmElem_On(ALARM)	IsAlarmElem_On_Off(ON,ALARM)
#define IsAlarmElem_Off(ALARM)	IsAlarmElem_On_Off(OFF,ALARM)

//-------------------- ��������� -------------------------------------------

// ��������� ��������� ������
typedef union {
	Uns all;
	struct
	{
		Uns Enable	:1;		// 0	���������� ������ (0-����, 1-���.)
		Uns CanBeReseted:2;	// 1	����� ������ (1-���������, 0-�� ���������)
		Uns Level	:1;		// 3	�������� ������� 0:Input <= Level, 1:Input >= Level
		Uns Num		:4;		// 4-7	����� ���� � �������� ������
		Uns Hyst	:8;		// 8-15 �������� �����������
	} bit;
} TAlarmCfg;
//--------------------------------------------------------
// ��������� ������� ������
typedef struct {
	TAlarmCfg Cfg;			// ������������ ������
	Int    *Input;			// ����
	Uns    *Output;			// �����
	Int    *EnableLevel;	// ������� ������������ ������
	Int    *DisableLevel;	// ������� ������ ������
	Uns    *Timeout;		// ����-��� ������������
	Uns     Scale;			// ����������� ��������������� �������
	Uns   	Timer;			// ������ ������������
	Uns		Signal;			// ���������� �������� �������
} TAlarmElem;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Core_ProtectionsAlarmInit(TAlarmElem *);
void Core_ProtectionsAlarmUpdate(TAlarmElem *);


#endif
