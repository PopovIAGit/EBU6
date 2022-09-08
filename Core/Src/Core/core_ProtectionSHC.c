/*======================================================================
��� �����:          core_ProtectionI2T.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		28.04.2014
��������:
������ ������ �� ��������� ���������
======================================================================*/

#include "g_Core.h"

void Core_ProtecionSHC_Update(TAlarmSHC *p)
{
	register Uns BitMask;

	BitMask = (1UL << p->Cfg.bit.Num); // ������� ���� Unsigned �������� ����� �� ����� ���� � �������� ������. �������� ����� ������.

	if (!p->Cfg.bit.Enable)
		return;

	if (!(*p->Output & BitMask))
	{
		if ((*p->InputCurrent > *p->HighCurrentLevel) || (*p->InputCurrent < *p->LowCurrentLevel))
		{
			*p->Output |= BitMask;
		}
	}
}
