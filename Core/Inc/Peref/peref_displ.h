/*======================================================================
��� �����:          displ.h
�����:              ������ �.�.
������ �����:       01.03
���� ���������:		03/03/10
�������������:      ��������� � ����������� CSL
��������:
������� ������������� ����������
======================================================================*/

#ifndef DISPL_
#define DISPL_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
// ��������� ��� ������ � �����������
typedef struct DISPL {
	Byte   SpiId;        // ������������� SPI-����� (0-SPIA, 1-SPIB, ...)
	Uns    SpiBaud;      // ������� ��������������� (����������� ��� ������������ ����)
	Bool   Enable;       // ���������/���������� ����������
	Bool   Status;			// ������� ��������� ������
	Bool   Restart;      // ����� ����������
	Uns    ResTimer; 		// ������ ��� ������
	Uns    ResTout;  		// ����-��� ������
	Uns    PauseTimer;	// ������ ��� ������������ �����
	Byte   State;        // ������� ��������� ���������� ������
	Byte   CursorPos;    // ������� ��� �������
	Byte   CursorAddr;	// ����� ��� ���������� ��������
	Char   HiStr[17];    // ����� ������� ������� ����������
	Char   LoStr[17];    // ����� ������ ������� ����������
	Char  *Data;         // ��������� �� ��������� ���� ������
	IOpin  EN;           // ����� ��� ������������ ������
	IOpin  RS;           // ����� ������ �������/������
	IOpin  RW;           // ����� ������ ������/������
	IOport Port;         // ���� ������
	void (*CsFunc)(Byte);// ������� ������ ����������
} DISPL;

__inline void DISPL_ClrHiStr(DISPL *p) {memset(p->HiStr, ' ', 16); p->HiStr[16] = '\0';}
__inline void DISPL_ClrLoStr(DISPL *p) {memset(p->LoStr, ' ', 16); p->LoStr[16] = '\0';}

__inline void DISPL_PutHiStr(DISPL *p, Ptr Adr) {PFUNC_blkRead(Adr, (Ptr)p->HiStr, 16);}
__inline void DISPL_PutLoStr(DISPL *p, Ptr Adr) {PFUNC_blkRead(Adr, (Ptr)p->LoStr, 16);}



void DISPL_Update(DISPL *);
void DISPL_AddSymb(DISPL *, Byte Addr, Ptr Data, Byte Count);
*/
#ifdef __cplusplus
}
#endif // extern "C"

#endif





