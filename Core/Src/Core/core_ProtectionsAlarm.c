/*======================================================================
��� �����:          core_ProtectionsAlarm.c
�����:
������ �����:		1.00
���� ���������:		25.04.2014
��������:
������ ���������� ��������� �����
======================================================================*/

#include "g_Core.h"

void Core_ProtectionsAlarmInit(TAlarmElem *p)
{

}

void Core_ProtectionsAlarmUpdate(TAlarmElem *p)
{
	register Uns BitMask, Tmp;

	BitMask = (1UL << p->Cfg.bit.Num); // ������� ���� Unsigned �������� ����� �� ����� ���� � �������� ������. �������� ����� ������.

	if(!p->Cfg.bit.Enable)	// ���� ������ ���������
	{
		if (p->Cfg.bit.CanBeReseted)	*p->Output &= ~BitMask;

			 p->Timer = 0;
			 p->Signal = *p->Input;
		return;
	}

	if(p->Cfg.bit.CanBeReseted == CAN_BE_MUFTA)
	{
		if ( abs(*p->Input - p->Signal) >= (Uns)*p->EnableLevel)
		{
			p->Timer = 0;
			p->Signal = *p->Input;
		}
		else
		{
			if (p->Timer < (*p->Timeout * p->Scale))
				p->Timer++;
			else
				*p->Output |= BitMask;
		}
	   return;
	}

	if (!(*p->Output & BitMask))	// ���� ������ ��� (*p->Output & BitMask ���� "0")
		{
			// ������� �� Level("�������� �������").
			// �� ����������, ����� �������� ���������� ���������� �������� �������, ������������ ��� �����������
			switch(p->Cfg.bit.Level)
			{
				// ���� Level = 0, �� p->Level - ������ ���������� ������ �������� �������
				case 0: Tmp = (*p->Input <= *p->EnableLevel); break;	// ��������, ���� �� ���� ������� ������ ������������ �������
				// ���� Level = 1, �� p->Level - ������� ���������� ������ �������
				case 1: Tmp = (*p->Input >= *p->EnableLevel); break;	// ��������, �������� �� ������� ������ ������������ ������
			}
			if (!Tmp)	// ���� ��� � �����, �.�. ������ �� �������� ��� ���� ���� ��������
			{
				if (p->Timer > 0) p->Timer--;	// ���� ������ �� ����� "0", ��������� ������
			}
			else 	// ����� - ������ �������� ��� ���� ���� ����������� �������
			if (++p->Timer >= (*p->Timeout * p->Scale))	// ����������� ������, � ���� ������ ������ ������� ������������ ������
			{
				*p->Output |= BitMask;	// ���������� ������
				p->Timer = 0;
			}
		}
	else // ���� ������ ��� ���� (*p->Output & BitMask ���� "1")
		if (p->Cfg.bit.CanBeReseted == CAN_BE_RESETED) // ���� "����� ������"=1, �� ���� ������ ����� "�����",
		{
			// ������� �� Level("�������� �������").
			// �� ����������, ����� �������� ���������� ���������� �������� �������, ������������ ��� �����������
			switch(p->Cfg.bit.Level)
			{
				// ���� Level = 0, �� p->Level - ������ ���������� ������ �������� �������
				//case 0: Tmp = (*p->Input >= (*p->DisableLevel + (Int)p->Cfg.bit.Hyst)); break;// ��������, �������� �� ������� ������ ����������� ������
				case 0: Tmp = (*p->Input >= (*p->EnableLevel + (Int)p->Cfg.bit.Hyst)); break;// ��������, �������� �� ������� ������ ����������� ������
				// ���� Level = 1, �� p->Level - ������� ���������� ������ �������
				//case 1: Tmp = (*p->Input <= (*p->DisableLevel - (Int)p->Cfg.bit.Hyst)); break;// ��������, ���� �� ���� ������� ������ ������������� �������
				case 1: Tmp = (*p->Input <= (*p->EnableLevel - (Int)p->Cfg.bit.Hyst)); break;// ��������, ���� �� ���� ������� ������ ������������� �������
			}
			if (Tmp)	// ���� ������� �������� �������������� �� ���������� ��������
			{
				 *p->Output &= ~BitMask;	// ������� ������
			}
		}
}
