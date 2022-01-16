/*======================================================================
��� �����:          peref_SinObserver.c
�����:				����� �.�.
������ �����:		1.0
���� ���������:		04.04.14
��������: ��������� ��������� ���� � ����������
����  - �������������� �������� ���� � ����������
����� - RMS �������� I � U, ����������, ���� ���
======================================================================*/
#include "peref.h"


void Peref_SinObserverInit(TSinPhaseObserver *p, Uns Freq)
{
	p->CurAngle = 0;
	p->StepAngle = 18000 / Freq; // 50 �� / 18000 �� * 360 ���� = 1 ����
}

void Peref_SinObserverInitFloat(TSinPhaseObserverFloat *p, Uns Freq)
{
	p->CurAngle = 0;
	p->StepAngle = 18000 / Freq; // 50 �� / 18000 �� * 360 ���� = 1 ����
}

void Peref_SinObserverUpdateFloat(TSinPhaseObserverFloat *p)		// ���, ����, ���������� ������� Float
{
	register Uns Sign;

	if (p->Input > 0)
		Sign = 0;
	else
		Sign = 1;

	p->CurAngle = p->CurAngle + p->StepAngle;

	if (Sign != p->Sign)
	{
		if (p->CurAngle < MIN_ANGLE)  p->Polarity = 0;
		else if (!Sign) p->Polarity = 1;
		else p->Polarity = -1;
		p->CurAngle = 0;
		p->Sign = Sign;
	}
	else if (p->CurAngle > MAX_ANGLE)
	{
		p->Polarity = 0;
		p->CurAngle = MAX_ANGLE;
	}

	p->Sum += p->Input * p->Input;//pow(fabs(p->Input), 2);
	if(++p->Counter >= BASE_ANGLE*2)
	{
		p->Output = (sqrt(p->Sum / (BASE_ANGLE*2)));
		p->Sum = 0;
		p->Counter = 0;
	}
}

/*void Peref_SinObserverUpdate(TSinPhaseObserver *p)		// ���, ����, ���������� ������� Int
{
	register Uns Sign;

	Sign = (Uns) p->Input >> 15;

	p->CurAngle = p->CurAngle + p->StepAngle;

	if (Sign != p->Sign)
	{
		if (p->CurAngle < MIN_ANGLE)  p->Polarity = 0;
		else if (!Sign) p->Polarity = 1;
		else p->Polarity = -1;
		p->CurAngle = 0;
		p->Sign = Sign;
	}
	else if (p->CurAngle > MAX_ANGLE)
	{
		p->Polarity = 0;
		p->CurAngle = MAX_ANGLE;
	}

	p->Sum = p->Sum + abs(p->Input);
	if(++p->Counter >= BASE_ANGLE)
	{
		p->Output = (p->Sum * RMS_SCALE) >> 15;
		p->Sum = 0;
		p->Counter = 0;
	}
}*/

void Peref_PhaseOrderUpdate(TPhaseOrder *p)		// ����������� ���
{
	register Int Direction;

	if (p->UR->Polarity <= 0) return;
	if (p->UR->CurAngle != PHO_ANGLE) return;

	Direction = 0;

	if ((p->US->Polarity > 0) && (p->UT->Polarity < 0)) Direction = -1;
	if ((p->US->Polarity < 0) && (p->UT->Polarity > 0)) Direction =  1;

	if (Direction != p->TmpDir)
	{
		p->TmpDir = Direction;
		p->Timer = 0;
	}
	else if (++p->Timer >= p->Timeout)
	{
		p->Direction = Direction;
		p->Timer = 0;
	}
}
