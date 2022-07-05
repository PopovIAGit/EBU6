/*======================================================================
��� �����:          peref_Calibs.h
�����:              ����� �.�.
������ �����:       01.00
���� ���������:		04/04/14
��������:
������ � ����������� ������� ���������
======================================================================*/

#ifndef CALIBS_
#define CALIBS_

#ifdef __cplusplus
extern "C" {
#endif

//----------- ����������� ������������ ������ -----------------------------


//--------------------- ���������-------------------------------------------
// ������� ����������
#define	CLB_CLOSE   0x01  // ������ �������/��������� � ���� �������
#define	CLB_OPEN    0x02  // ������ �������/��������� � ���� �������
#define	CLB_FLAG    0x03  // ���������� ���������

#define REV_MAX		0x7FFFFF
#define MUFF_CLB_TIME	(3.000 * PRD_50HZ)	// ����� ��������� ����� ��� ����������
#define SP_POS_LIMIT 400

//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

// ��������� ���������� ������������
typedef struct {
	Uns TaskClose;          // �������/����� �������
	Uns TaskOpen;           // �������/����� �������
	Uns RevOpen;            // ������� ���������� �������� �� ��������
	Uns RevClose;           // ������� ���������� �������� �� ��������
	Uns RevAuto;            // �������������� ����������
} ClbCommand;

// ��������� ��������� ����������
typedef struct {
	Uns    Status;          // ������ ����������
	Uns    Reserved;        // ������
	Uns    ClosePos[2];        // ��������� �������
	Uns    OpenPos[2];         // ��������� �������
} ClbIndication;

// ��������� ��� ������ � �������� ���������
typedef struct TPerefPosition {
	Bool   ResetFlag;       	// ���� ������
	Bool   CancelFlag;      	// ���� ������ �������
	Uns    GearRatio;       	// ������������ ����� ��������� * 100
	Uns    Zone;            	// ���� ����������
	LgUns  RevMax;          	// ������������ ���������� �������� - 1
	LgInt  LinePos;        		// ������� �������� ���������
	LgUns  FullStep;        	// ������ ���
	LgInt  BasePos;         	// ������� ���������
	Uns    CycleData;       	// �������� ��� ������� ������
	Uns    MuffTime;        	// ������� ���������� �����
	Uns    MuffTimer;       	// ������ ����������(�������� ������������) �����
	LgUns  speedMPS;       		// �������� �������� (�����/�) MarkersPerSecond
	LgUns  speedRPM;        	// �������� ��������� (��/�) RevolutionsPerMinut
	Uns    Reserved;        	// ������
	LgInt  GearInv;         	// ����������� �������� ������������� ����� ���������
	LgInt  FstepInv;        	// ����������� �������� ������� ����
	Uns   *PosSensPow;    		// ��������� �� ��� ������� ���������
	Uns   *PositionAcc;     	// ��������� �� �������� �������� ���������
	Uns   *RodType;         	// ��������� �� ��� �����
	LgUns *AbsPosition;     	// ��������� �� ������� ���������
	Int   *PositionPr;      	// ��������� �� ������� ��������� � %
	Uns   *FullWay;         	// ��������� �� ������ ��� ��������� �����
	Int   *CurWay;          	// ��������� �� ������� ��� ��������� �����
	Uns   *CycleCnt;        	// ��������� �� ���������� ����������� ������
	Uns    *TaskClose;    	// ��������� �� ��������� ���������� ������������
        Uns    *TaskOpen;
	ClbIndication *Indication; 	// ��������� �� ��������� ���������
} TPerefPosition;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------
void Peref_CalibInit(TPerefPosition *);
void Peref_Calibration(TPerefPosition *);
void Peref_CalibUpdate(TPerefPosition *);
void Peref_CalibControl(TPerefPosition *);
void  CalcClbCycle(TPerefPosition *);

static Byte CheckStatus(TPerefPosition *, Uns);

void Peref_SpeedCalc(TPerefPosition *);


Uint32 CalcClbGearInv(TPerefPosition *);
Uint32 CalcClbAbsRev(TPerefPosition *, Uint16 GearRev);
int16  CalcClbGearRev(TPerefPosition *, int32 AbsRev);
Uint32 CalcClbPercentToAbs(TPerefPosition *, Uint16 Percent);


#ifdef __cplusplus
}
#endif // extern "C"

#endif
