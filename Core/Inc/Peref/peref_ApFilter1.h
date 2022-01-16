/*======================================================================
��� �����:          ApFilter1.h
�����:              ������ �.�.
������ �����:       01.00
���� ���������:		02/02/10
�������������:      ��������� � ����������� IQmath
��������:
�������������� ������ ������� �������
======================================================================*/

#ifndef APFILTER1_
#define APFILTER1_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ��������� ��� ������ � ��������
typedef struct APFILTER1 {
	Float Input;   // ������� ������
	Float Output;  // �������� ������
	Float PrevOut; // �������� ������
	Float dt;      // ���������� ������� �������������
	Float Tf;      // ���������� ������� �������
	Float K1;      // ����������� �������
	Float K2;      // ����������� �������
} APFILTER1;

void peref_ApFilter1Init(APFILTER1 *p, Uns Freq, Float Tf);
void peref_ApFilter1Calc(APFILTER1 *);
void peref_ApFilter1Reset(APFILTER1 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
