
#ifndef INVCONTROL_
#define INVCONTROL_

#include "main.h"
#include "peref_ApFilter1.h"
#include "peref_ApFilter3.h"
/*#include "dmc\aci.h"
#include "dmc\clarke.h"
#include "dmc\ipark.h"
#include "dmc\park.h"
#include "dmc\pid_reg3.h"
#include "dmc\qgen.h"
#include "dmc\rampgen.h"
#include "dmc\S_curve.h"
#include "dmc\S_curve2.h"
#include "dmc\svgen_dq.h"
#include "dmc\svgen_dq_3ph.h"
#include "dmc\jump_dq.h"
#include "dmc\volt_calc.h"
#include "dmc\Autotuning.h"
#include "dmc\IdentRotorPosition.h"
#include "dmc\SLSCos.h"
#include "interp2D.h"*/

#ifdef __cplusplus
extern "C" {
#endif

// ��������� ������
typedef enum {
	iwmLock        = 0x0001,		// ����������
	iwmReady       = 0x0002,		// ����������
	iwmHeat        = 0x0004,		// ��������
	iwmRamp        = 0x0008,		// ������
	iwmMove        = 0x0010,		// ��������
	iwmSlowDn      = 0x0020,		// �������
	iwmCoastStop   = 0x0040,		// ������� �������
	iwmDcBrake     = 0x0080,		// ���������� ����. �����
	iwmStartTest   = 0x0100,		// ������������� ����
	iwmSlowDnRev   = 0x0200, 		// ������� � ��������
	iwmAutoTuning  = 0x0400,		// �������������
	iwmIdRotorPos  = 0x0800,		// ������������� ��������� ������
	iwmAutoTuneSM  = 0x1000			// ������������� ��� ����������� ���������
} TInvWorkMode;

// ������� ����������
typedef enum {
	icwNone        = 0x0000,		// ��� �������
	icwLock        = 0x0001,		// ���������������
	icwUnLock      = 0x0002,		// ����������������
	icwHeatOn      = 0x0004,		// �������� �������
	icwRunForward  = 0x0008,		// �����
	icwRunReverse  = 0x0010,		// �����
	icwJogForward  = 0x0020,		// �������� � JOG-������ �����
	icwJogReverse  = 0x0040,		// �������� � JOG-������ �����
	icwSlowDown    = 0x0080,		// ��������� �������
	icwCoastDown   = 0x0100,		// ������� �������
	icwDcBrake     = 0x0200,		// ���������� ����. �����
	icwAutoTuning  = 0x0400,		// �������������
	icwIdRotorPos  = 0x0800,		// ������������� ��������� ������
	icwAutoTuneSM  = 0x1000			// ������������� ��� ����������� ���������
} TInvCtrlWord;

// ������� ����������
typedef enum {
	icmScalar          = 0x0001,	// ��������� ����������
	icmScalarCurr      = 0x0002,	// ��������� ���������� + ���������������
	icmCurr            = 0x0004,	// ������� ����������
	icmVectFdb         = 0x0008,	// ��������� � ��
	icmVectSls         = 0x0010,	// ��������� ��� ��
	icmVectSMFdb	   = 0x0020,	// ��������� � �� ��� ��
	//icmSlsCurSatSmooth = 0x0020,	// ��������-��������� c ������������ ����  � ������ ����������� ����
	icmCurr3Relay      = 0x0040,	// ������� ���������� �� 3-�� ����� ��������
	icmScalarFdb       = 0x0080,	// ��������� ���������� � ��
	icmScalarCurrFdb   = 0x0100,	// ��������� ���������� � �� + ���������������
	icmSyncCurr        = 0x0200,	// ��������� ��� ����������� ���������
	icmSyncCurrFdb     = 0x0400,	// ����� ��� ����������� ��������� c ��
	icmScalCurrComb    = 0x0800,	// ��������� ���������� + ������� � �������� � ����������
	icmVectSlsSmooth   = 0x1000,	// ��������� ��� �� c ����������
	icmVectSlsCurSat   = 0x2000,	// ��������-��������� c ������������ ����
	icmCurr3           = 0x4000		// ������� ���������� �� 3-�� �����
} TInvCtrlMethod;

// ��������� ���������� �����������
typedef enum {
	ilsOff   = 0,						// ��������� ��������
	ilsOn    = 1,						// ��������� �������
	ilsQuick = 2,						// ��������� ������ �������
	ilsSlow  = 3						// ��������� �������� �������
} TInvLedsState;

// ����������� ��������
typedef enum {
  idNone    =  0,          		// ����
  idReverse = -1,            	// �����
  idForward =  1              	// ������
} TInvDirection;

// �������� ������� ������� �� ���������
typedef enum {
	rrsDef=0,							// ������� �� ���������
	rrsDir,								// ������ �������
	rrsGen								// ������� c ����������
} TRegRefSrc;

// ��� ���������� ���������� �����
typedef enum {
	dbtOff=0,							// ��������
	dbtVolt,								// �� ����������
	dbtCurr								// �� ����
} TDcBrakeType;

// ������ ��������� ������������� �������
typedef enum {
	mbsOff=0,							// ��������
	mbsFreq,								// �� �������
	mbsTime								// �� �������
} TMechBrakeSet;

// ��������� ��� ����������
typedef struct {
	TInvCtrlWord	Cmd;				// ������� ����������
	TInvCtrlMethod Method;			// ������ ����������
	Bool           RealDrive;		// ���� ������ � �������� ����������
	Bool				SpeedEstFlag;	// ���� ���������� �������������� ��������
	Bool				ForceFlag;		// ���� ����������
	Bool				SlowFlag;		// ���� �����������
	Bool				GenFltrFlag;	// ���� ���������� ��������� ������� ����������
	LgInt          Ref1;				// ������� 1 �� ������� ����������
	LgInt          PsiRefGen;		// ������� 2 �� ������� ����������
	LgInt          Curr1;			// �������� ����������� ���� � ���� 1
	LgInt          Curr2;			// �������� ����������� ���� � ���� 2
	LgInt				RotSpeed;		// �������� ������� ������� �������� ������ �� ��������
	Uns				PreMagTime;		// ����� ���������� ��������������
	Uns				SpdPreScale;	//	����������� ������� ������� �������� (���-�� INV_CTRL_FREQ)
	Uns				PsiPreScale;	//	����������� ������� ������� ��������������� (���-�� INV_CTRL_FREQ)
	LgInt				RR_corr;			// ��������� ������������� ������
	LgInt          PsiRefMot;		// ������� ������� ��������������� ��� ������������� ������
	LgInt          PsiRefForce;	// ������� ������� ��������������� ��� ����� ��������
  LgInt          LoadTorqGain;	// ������������� ��� ������� ������� �������� �� ���� ��
	Uns	 			PwmType;			// ��� ������������ ���
	Uns				PsiRType;		// ������ ����. ����.��������� ��� �� � ��
	LgInt				MaxCurrLevel;	// �����������-���������� ������� ��� ���������������
	LgInt				RefGain;			// ���������� ����������� �������
	LgInt				HallAngle;		// ���� ������� ��������� ������ �� �������� �����
	LgInt				DeltaUdc;		// ������������� ��������� Udc
	LgInt				UdcMash;			// ���������� ����������� ��� ������� ��������� ����������
	Uns				Udc;				// ������� Udc
	Uns				UdcComp;			// ����������� ��������� Udc
	Uns				GenType;			// ��� ���������� ��������
	TRegRefSrc     CurrRefSrc;		// ����� ��������� ������� �� ��������� ����
	TRegRefSrc     SpdRefSrc;		// ����� ��������� ������� �� ��������� ��������
	TRegRefSrc		PsiRefSrc;		// ����� ��������� ������� �� ��������� ���������������
	Uns				CurrConn;		// ����� ����������� �����
	Bool				CurrMode;		// ���� �������� ���������� ��� ������ ���������� ��� ��
	Bool				CurrModeAngle; // ��������������� ���� ��� ������������ ����
	Bool				CurrModeSt;		// ���� �������� ���������� ��� ������ ���������� ��� �� ��� ��������
	Bool				CurrModeAngleSt;// ��������������� ���� ��� ������������ ����  ��� ��������
	LgInt				CurrSwFreqLev;	// ������� ������������ �������� ���������� � ������ ���������� ��� ��
	LgInt				CurrLoadCoef;	// ����������� ������� ������� ��� ������� ����������
	LgUns				Curr_Ref_Stop;		// ��������-��������� ����������
	LgInt				Curr_Kp_Stop;
	LgInt				Curr_Ki_Stop;
	LgInt				Curr_Kc_Stop;
	LgInt				Bound;
	LgInt				Bound_Start;
	LgInt				Bound_Stop;
	LgUns				Curr_Ref_Init;		// ������ ����������� ���� ��� ����� (���� ��������� ������� ����)
	LgUns				Ti_Curr;				// ������������� �������� ���� ��� ����� (���� ������������� ���������� ����)
	LgUns				Curr_Ref_Smooth;	// ������� �������� ����
	LgUns				K_Curr_Stop;		// �����. ����. ������� ���� ��� ����.
	Uns				Move_flag;			// ���� ������ �������� �������
	Uns				Move_Init_flag; 	// ���� ���������� ���������� �������� ���� ��� �������
	Uns				Stop_flag;			// ���� ��������
	TMechBrakeSet	MechBrakeSet;
	LgInt			MechBrakeSetFreq;
	Uns				LiftObject;			// ����� ���������� ������
	LgInt			EstLoadReset;		// ���������� ��� ������ ��������
	Uns				LiftRampDn;			// ������ ���������� � �������� ������
	LgInt			LiftPreDefLoad;		// ��������������� �������� ����������� ��� �������� ������	
	LgInt			LiftDefLoad;		// �������� ����������� ��� �������� ������	
	Uns				LiftRampDnFlag;		// ���� ������� ��������
	LgInt			LiftRampLoadGain;	// ��������� U/f ��� ��������
	LgInt			LiftRampFreqGain;	// ��������� ������-f ��� �����������
	LgInt			LiftFreqAdd;		// ������� �� �������
	Uns				LiftModeTimer;		// ������ ������ � �������� ������
	LgInt			LiftVoltAdd0;		// ������� 1 �� U/f
	LgInt			LiftVoltAdd1;		// ������� 2 �� U/f
	LgInt			LiftVoltAdd2;		// ������� 3 �� U/f
	Uns				ChangeVoltFlag;		// ���� ����������� � �������� ���������� U/f
	Uns				OneFloorFlag;		// ���� ���������� ��������
} TInvCtrl;

// ��������� ��������� ����������
typedef struct {
	TInvWorkMode   WorkMode;		// ������� ����� ������
	Bool				InvEnable;		// ���� ������ ���������
	Bool				JogFlag;			// ���� ������ � JOG-������
	TInvDirection	Direction;		// ����������� ��������
	LgInt				FreqOut;			// �������� ������� � �.�.
	LgInt				VoltOut;			// �������� ���������� � �.�.
	LgInt				RefOut;			// ����� ��������� �������������
	LgInt				RampOut;			// ����� ���� ���������� ������� ����������
	TInvLedsState	LedForw;			// ���������� ����������� "������"
	TInvLedsState	LedRev;			// ���������� ����������� "�����"
	Uns				WishRotCmd;    // �������� ����������� �������� ��� ������� ����������
	Uns				CmdPrev;			// ���������� ������� �� ��������
	Uns				RefSign;			// ���� �������
	LgInt				RefSpeed;		// ������� ��� ������� �������� ������
	LgInt				EstSpeed;		// ��������������� �������� ������� �������� ������
	LgInt          LoadTorque;		// ������ �������� �� ���� ��
	Uns				PreMagTimer;	// ������ ��� ���������� ��������������
	Uns				SpdScTimer;		// ������ ��� ������� ������� ��������
	Uns				PsiScTimer;		// ������ ��� ������� ������� ���������������
	LgInt				DeltaRefErr;	// ������ �������
	LgInt				VoltAngle;		// ���� �������� ������� ����������	
	LgInt				DeltAngle;		// ������� ����� ��������� ���������� � ����
	LgInt				Ta;				// ������ ��������� ��� �� ���� � ���� �
	LgInt				Tb;				// ������ ��������� ��� �� ���� � ���� �
	LgInt				Tc;				// ������ ��������� ��� �� ���� � ���� �
	Uns				AlarmFlag;		// ���� ����������� ��������� ������������
	//INTERP2D		        *vhz;				// ������ V/F ��� ���������� ���������� ����������
} TInvStatus;

// ����������� ������������ ��������� �������������
typedef struct {
	LgInt Target;						// �������
	LgInt RelUpTime;					// ������������� ����� �������
	LgInt RelDnTime;					// ������������� ����� ��������
	LgInt AbsUpTime;					// ���������� ����� �������
	LgInt AbsDnTime;					// ���������� ����� ��������
	LgInt SlowDnTime1;				// ����� ���������� �� ����������� 1
	LgInt SlowDnTime2;				// ����� ���������� �� ����������� 2
} TInvRmpConfig;

// ��������� ��� ���������� ������������� ���������� ���������
typedef struct {
	TDcBrakeType Type;				// ��� ���������� ���������� �����
	Bool	Flag;							// ���� ��������� ������������� ����������
	Uns	SetTime;						// ����������������� ������������� ����������
	Uns	Timer;						// ������ ������������� ����������
	Uns   Mask;							// ����� ������� ��� ������� �������� ������������ ����������
	LgInt	Level;						// ������� ���������� ���������� �����
	LgInt	Freq;							// ������� ��� ��������� ���������� ���������� �����
} TInvDcBrake;

// ��������� ��� ���������� ������� ��������� ���������
typedef struct {
	Uns   Period;						// ������������� ��������� ���������
	Uns   OnTime;						// ������������ ��������� ���������
	Uns   Timer;						// ������ ���������
	Uns   PhaseSeqTime;				// ����� ������������ ���
	Uns   PhaseSeqTimer;				// ������ ������������ ���
	Uns   CurrPhase;					// ������� ����
	LgInt Reference;					// ������� ���������
} TInvDcHeat;

// ��������� ��� �������� ������ ���������� � ���������
typedef struct {
	LgInt RefD;							// ������� �� ��� D
	LgInt RefQ;							// ������� �� ��� Q
	LgInt DeltaPower;					// ��� ������ ����
	LgInt Gain;							// ����������� �������� ����
} TInvCoastStop;

// ��������� ��� ������ � ������������� ������
// ������ �� �� � ������ �������� ����� �� ����
typedef struct {
	Bool	Flag;						// ���� ��������� �����
	Uns		Stage;						// ������� ������ �����
	Uns		Timer;						// ������ �� ������� ������
	Uns		TimeCalib;					// ����� ��� ���������� ���������� ��� �����
	Uns		TimePhase;					// ����� �������� ������ ���� ����
	Uns		Enabled_TestSC;				// ���� ����� �� ��� �����
	Uns		TestingEnabled;				// ���� ���������� �� ���������� ������ ������ � ��
	Uns		LenOpenKey;					// ������������ �������� ����� ��� ����� �� ��
	Uns		IntervalOpenKey;			// ����� ����� ���������� ������ 
	Uns		CountOpenKey;				// ���������� ������ �� ��
	Uns		StepTestSC;					// ��� �������� ������ �� ��
	Uns		CountOpenKey_Counter;		// ������� ����������� ����������� ��������� (������)
	Uns		IntervalOpenKey_Counter;	// ������� ��������� �������� ������
	Uns		Type;
	LgUns 	Q_ref_integralProtect;
	LgUns 	K_U;
	LgUns 	Dt;
	Uns		PhazeProtect;
	Uns 		TestProtect_FAZA;
	Uns		CurrTest;
	Uns		Test_EnabledKey;
	Uns		Next_FAZA;
	Uns		TestFlagForward;
	Uns		TestRun;
	Uns		Old_FAZA;
	Uns		Test_EnabledKey2;
	Uns		StopFromScalar;
	LgInt		I_U;
	LgInt		I_V;
	LgInt		Result_I_W;
	Uns		Work_TestGroundSC;//���� ����������� ���� �� �� ����� � ������� ������
	Uns		ReadyTestSCGround;
	Uns		PredTestDmcMethod;
	Uns		PredTestDriveCommand;
	Uns		PredTestDviveMethod;
	Uns 		Key_Test;
	Uns		My_STOP;
	Uns 		EnabledTimerNextStep;
	Uns		next_stop;
	Uns 		PredStartTest;
	Uns 		Model;
	Uns		State;
	Uns		FlagOnBreak;
	Uns		BrResEnable;	// ���� ������������ ���������� �������������
	Uns		FactoryCommonModel; // ��� ����� 11 ��� 18 ���
	Uns 		End_TestSC;	// ���� �������� ���� ������������� ������ (��������� ����������)
	Uns 		EngineCMD;	// ���� ������� ����������� ��� ���������� InvertorControl
	Uns 		tmp;
} TInvStartTest;

// ��������� ��� ����������� �������
typedef struct {
	LgInt InputVolt;  				// ������� ����������
	LgInt OutputVolt; 				// �������� ����������
	LgInt Referense;				// ����������� ��������
	LgInt Gain; 	  				// ����������� ����������� �������
	LgInt GenGain; 	  				// ����������� ����������� ������� � ���.������
	LgInt RmsCurrent; 				// ����������� �������� ���������� ����
	LgInt Rs;						// ������������� �������
	LgInt OutMax;	  				// ������������ ����������� ������
	LgInt VoltAddIR;	  			// ������������� IR-�����������
	LgInt VoltAddLoad;	  			// ������������� �� ��������
	LgInt LoadTorque;	  			// ������ ��������
} TInvTorqComp;

// ��������� ��� ����������� ����������
typedef struct {
	LgInt InputRef;   				// ������� �������
	LgInt OutputRef;  				// �������� �������
	LgInt Gain; 	   				// ����������� ����������� ����������
	LgInt Load; 	   				// ������ ��������
	LgInt CmprLoad;   				// ��������� �������� ������� ��������
	LgInt OutMax;	   				// ������������ ����������� ������
	LgInt RefAdd;	   				// ��������� �������
} TInvSleepComp;

// ��������� ��� ����������� ���������� � ��������� ������
typedef struct {
	LgInt InputRef;   				// ������� �������
	LgInt OutputRef;  				// �������� �������
	LgInt Load;  				// �������� �� ���� ��
	LgInt UserGain; 	   			// ����������� ����������� ���������� (������������)
	LgInt Gain;		 	   			// ����������� ����������� ����������
	LgInt GenGain;		 	   			// ����������� ����������� ���������� � �����.������
	LgInt Tf;   					// ���������� ������� ����������� ����������
	LgInt OutMax;	   				// ������������ ����������� ������
	LgInt RefAdd;	   				// ��������� ������� �� �������
} TInvSleepCompScal;

typedef struct {
	Uns   Mode;
	LgInt KoefK;
	LgInt KoefB;
	LgInt PrevKHiSpeed;
	LgInt PrevKLowSpeed;
	LgInt PrevLvHiSpeed;
	LgInt PrevLvLowSpeed;
	LgInt DeltaRefLevel;
}
TKspeedAdapt;

typedef struct {
	LgInt RefLevel;
	LgInt FdbLevel;
	LgInt Gain1;
	LgInt Gain2;
	LgInt MaxOut;
	LgInt TmpMaxOut;
	LgInt Rezerv1;
	LgInt Rezerv2;
	LgInt Rezerv3;
}
TPosReg;

// ��������� ��� ���������� ���������� ����������
typedef struct {
	Uns   SmMode;
	Uns	  SmTestMode; 
	LgInt TestVolt;
	LgInt TestFreq;
	LgInt TestAngle;
	LgInt MotorAngle;
	LgInt TestZeroCurr; 
	LgInt MotorSpeed;
	Uns   ScaleAngle;
	LgInt ElectrAngle;
	Uns   LiftState;
	Uns	  BrakeDelayTimer;
	Uns	  BrakeDelayTimeOut;
	LgInt PrevElectrAngle;
	LgInt TestSpeed;
	LgInt LowSpeedLevel;
	LgInt HiSpeedLevel;
	TKspeedAdapt SpedReg;
	TPosReg PosReg;
	Uns StartPosMode;
	LgInt StartCorr;
	LgInt SpdTfBase;
	LgInt SpdTfStart;
	LgInt SpdTfActive;
} TInvSMCntrl;


// �������� �������� ����������
typedef struct _TInvControl {
	TInvCtrl			Control;	// ��������� ��� ����������
	TInvStatus			Status;		// ��������� ��������� ����������
	TInvRmpConfig		RefRmp;		// ��������� ��� ��������� �������������
	TInvRmpConfig		JogRmp;		// ��������� ��� ��������� ������������� � JOG-������
	TInvDcBrake			DcBrake;		// ��������� ��� ���������� ������������� ���������� ���������
	TInvDcHeat			DcHeat;		// ��������� ��� ���������� ������� ��������� ���������
	TInvCoastStop		CoastStop;	// ��������� ��� �������� ������ ���������� � ���������
	TInvStartTest     	Test;		// ��������� ��� ������ � ������������� ������
/*	TAutotuning			Autotuning;	// ���������� ��� ��
	TAutotuneSM			AutotuneSM;	// ���������� ��� ��
	TIRIP				IdentRotPos;// ������������� ��������� ������
	TSLSCos				SLSCos;		// ������������� ����������
	SVGENDQ				svgen;		// ��������� ���������������-��������������� ���
	SVGENDQ_3PH			svgen3ph;	// ��������� ��������������� ���
	PARK				park;		// ������ �������������� �����
	IPARK       		ipark;		// �������� �������������� �����
	RAMPGEN				rg1;		// ��������� ������� ������������ �����
	INTERP2D			vhzUp;		// ��������� ������ V/F ��� ���������� ���������� ����������
	INTERP2D			vhzDn;		// ������������ ������ V/F ��� ���������� ���������� ����������
	CLARKE            	clarke;		// ������ �������������� ������
	PIDREG3 			RegIa;		// ��������� ���� ���� A
	PIDREG3 			RegIb;		// ��������� ���� ���� B
	PIDREG3 			RegIc;		// ��������� ���� ���� �
	PIDREG3 			SpdReg;		// ��������� ��������
	PIDREG3 			PsiReg;		// ��������� ���������������
	PIDREG3 			PosReg;		// ��������� ���������
	PIDREG3				SpdRegNorm;	// ��������� �������� � ������� ������
	PIDREG3				SpdRegForce;// ��������� �������� ��� ����������
	PIDREG3 			SpdRegAccel;// ��������� �������� ��� ���������/����������
	PIDREG3 			MaxCurrReg;	// ��������� ��������������� � ��������� ����������
	PIDREG3 			SpdRegStart;// ��������� ��� �������������� ������*/
	APFILTER3	 		F1;			// ������ ������� ���������������	
	APFILTER3	 		F2;			// ������ ������ ���������� ��������
	APFILTER3	 		F3;			// ������ ������������ ��������� ���������� ��������
	APFILTER3	 		F4;			// ������ ������������� ������������ ���������� ��������
	APFILTER3	 		F5;			// ������ ������ ��������� �������������
	APFILTER3	 		F6;			// ������ ������ ����������� �������
	APFILTER3	 		F7;			// ������ ������ ����������� ����������
	APFILTER1			F8;			// ������ �� ������ ���������� ��������
 	APFILTER3	 		F9;			// ������ ������� ����������� ����������
 	APFILTER3 			FiltrVa;	// ������ Va
 	APFILTER3 			FiltrVb;	// ������ Vb
	APFILTER3 			FiltrLoad;	// ������ ��� ������� ��������
	APFILTER3	 		FCos;		// ������ ������ ��������� �������������
	APFILTER3	 		SpdTfFiltr;	// ������ ��������� ���������� ������� � �� �� ��������
	TInvTorqComp		TorqComp;	// ����������� ������� ��� ��������� ����������
	TInvSleepComp		SleepComp;	// ����������� ����������
	TInvSleepCompScal	SleepCompScal;// ����������� ���������� ��� ���������� ������
	TInvSMCntrl			SMControl;	// ���������� ��
	/*S_CURVE				SCurve;		// ������������� S-�������� ��������������
  	S_CURVE2			SCurve2;	// ������������� S-�������� ��������������
  	INTERP2D			PsiR;		// ������������ ����������� ���������������
	CURMOD            	cm;			// ������� ������ ��� ��������� ����������
	JUMP_DQ			  	Jump_d;		// 
	JUMP_DQ			  	Jump_q;		// 
	PHASEVOLTAGE		volt;		// ������ ����������� �������� ���������� �� ������ ���������
	ACIFE             	fe;			// ������ �������������� ������ ������
	ACISE             	se;			// ������ �������������� ��������
	QGEN              	gen;		// ��������� ��������*/

	void (*OnPowerKeysCtrl)(Bool State);
	void (*OnUnLockDevice)(void);
	Bool (*OnLockStart)(void);
	void (*OnTestCalib)(Bool State);
} TInvControl;

void InvHiControl(TInvControl *);
void InvLoControl(TInvControl *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif


