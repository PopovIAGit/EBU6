

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- ����������� ������������ ������ ------------------------------
#include "main.h"
#include "core_ProtectionsAlarm.h"
#include "core_ProtectionSHC.h"
#include "g_Structs.h"

//--------------------- ���������-------------------------------------------
#define PROTECT_SCALE (0.100 * PRD_50HZ)

// ������ ����� g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7


// ������ ����� g_Structs.h -> TNetReg
#define UvR_bit				0
#define UvS_bit				1
#define UvT_bit				2
#define OvR_bit				3
#define OvS_bit				4
#define OvT_bit				5
#define VSk_bit				7
#define BvR_bit				8
#define BvS_bit				9
#define BvT_bit				10


// ������ ����� g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define ShCU_bit			5
#define ShCV_bit			6
#define ShCW_bit			7


// ������ ����� g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_bit			        5
#define Tl_bit			        6
#define DriveTemper_bit			7
#define ModuleTemper_bit		8

//--------------------- ������� --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- ��������� -------------------------------------------

typedef struct {
		// -------- Диагностика сети ----------------
		TAlarmElem 			underVoltageR;		// Понижение напряжения R  - авария
		TAlarmElem 			underVoltageS;		// Понижение напряжения S  - авария
		TAlarmElem 			underVoltageT;		// Понижение напряжения T  - авария
		TAlarmElem 			overVoltageR;		// Превышение напряжения R - авария
		TAlarmElem 			overVoltageS;		// Превышение напряжения S - авария
		TAlarmElem 			overVoltageT;		// Превышение напряжения T - авария
		TAlarmElem 			breakVoltR;		// Обрыв фазы R - авария
		TAlarmElem 			breakVoltS;		// Обрыв фазы S - авария
		TAlarmElem 			breakVoltT;		// Обрыв фазы T - авария
                
                TAlarmElem                      VDCmax;
  	// -------- Диагностика нагруски ------------
		TAlarmElem			breakCurrU;			// Обрыв фазы U	- авария
		TAlarmElem			breakCurrV;			// Обрыв фазы V - авария
		TAlarmElem			breakCurrW;			// Обрыв фазы W - авария
		TAlarmSHC			ShC_U;				// Короткое замыкание фазы U - авария
		TAlarmSHC			ShC_V;				// Короткое замыкание фазы V - авария
		TAlarmSHC			ShC_W;				// Короткое замыкание фазы W - авария      
                
                TAlarmElem                      IbreakMax;
    // -------- Диагностика устройства ----------

		TAlarmElem			overHeat;			// Перегрев БКД - неисправность
		TAlarmElem			underCold; 			// Переохлаждение БКД - неисправность    
                
                TAlarmElem			driveTemper;               
                TAlarmElem			moduleTemper;
                      
                TFltUnion		        outFaults;			// ������
                Uns				ShcTmpState;		// ������������� ���������� ��� ����������� ��
		Uns				FaultDelay;			// ����� ����� ������������� ��� ��������� �����
                Uns                             Dac_No_Conn_Tmp;
}TCoreProtections;

void Core_ProtectionsInit(TCoreProtections *);			// ������������� ������ �����
void Core_Protections18kHzUpdate(TCoreProtections *);	        // ������� ������� �� ������������ �����
void Core_Protections50HZUpdate(TCoreProtections *);	        // ������� ������� �� ������������ ����� 50 ��
void Core_ProtectionsReset(TCoreProtections *);			// ����� ����� ��� ��������� �����
void Core_ProtectionsClear(TCoreProtections *);			// ����� ���� �����
void Core_ProtectionsEnable(TCoreProtections *);		// ���������/���������� �����


#endif
