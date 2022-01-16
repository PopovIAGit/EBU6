/*======================================================================
��� �����:          ApFilter3.h
�����:              ������.�.
������ �����:       01.00
���� ���������:		04/04/14
�������������:      ��������� � ����������� IQmath
��������:
�������������� ������ �������� �������
======================================================================*/

#ifndef APFILTER3_
#define APFILTER3_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ��������� ��� ������ � ��������
typedef struct APFILTER3 {
	Float Input;   // ������� ������ lgint
	Float Output;  // �������� ������
	Float Output1; // ������� ������
	Float PrevOut1;// �������� ������
	Float Output2; // ������� ������
	Float PrevOut2;// �������� ������ lgint
	Float dt;      // ���������� ������� �������������
	Float Tf;      // ���������� ������� �������
	Float K1;      // ����������� �������
	Float K2;      // ����������� �������
	Float PrevOut; // ���������� �������� ��������� �������
} APFILTER3;

void peref_ApFilter3Init(APFILTER3 *p, Uns Freq, Float Tf);
void peref_ApFilter3Calc(APFILTER3 *);
void peref_ApFilter3Reset(APFILTER3 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
