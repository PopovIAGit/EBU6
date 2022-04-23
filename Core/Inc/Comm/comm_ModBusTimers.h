/*======================================================================
��� �����:          csl_timer.h
�����:              
������ �����:       01.02
���� ���������:		16/02/10
��������:
������������ ���� ��� ������ � ���������
======================================================================*/

#ifndef CSL_TIMER_
#define CSL_TIMER_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TTimerList {Uns Counter, Timeout;} TTimerList;

#define StopTimer(Timer)	(Timer)->Counter = 0
#define StartTimer(Timer)	(Timer)->Counter = (Timer)->Timeout

__inline void SetTimeout(TTimerList *Timer, Uns Timeout)
{
	if (!Timeout) Timeout = 1;
	Timer->Timeout = Timeout;
}

__inline void InitTimer(TTimerList *Timer, Uns Timeout)
{
	StopTimer(Timer);
	SetTimeout(Timer, Timeout);
}

__inline Bool TimerPending(TTimerList *Timer)
{
	if (Timer->Counter > 0)
	{
		Timer->Counter--;
		if (!Timer->Counter) return FALSE;
	}
	return TRUE;
}

#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CSL_TIMER_ */

