

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- Ïîäêëþ÷åíèå çàãîëîâî÷íûõ ôàéëîâ ------------------------------
#include "main.h"
#include "core_ProtectionsAlarm.h"
#include "core_ProtectionSHC.h"
#include "g_Structs.h"

//--------------------- Êîíñòàíòû-------------------------------------------
#define PROTECT_SCALE (0.100 * PRD_50HZ)

// Íîìåðà áèòîâ g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7


// Íîìåðà áèòîâ g_Structs.h -> TNetReg
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


// Íîìåðà áèòîâ g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define ShCU_bit			5
#define ShCV_bit			6
#define ShCW_bit			7


// Íîìåðà áèòîâ g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_bit			        5
#define Tl_bit			        6
#define DriveTemper_bit			7
#define ModuleTemper_bit		8

//--------------------- Ìàêðîñû --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- Ñòðóêòóðû -------------------------------------------

typedef struct {
		
                TAlarmElem			NoMove;				// ÐÐµÑ‚ Ð´Ð²Ð¸Ð¶ÐµÐ½Ð¸Ñ - Ð°Ð²Ð°Ñ€Ð¸Ñ 
                 // -------- Ð”Ð¸Ð°Ð³Ð½Ð¾ÑÑ‚Ð¸ÐºÐ° ÑÐµÑ‚Ð¸ ----------------
		TAlarmElem 			underVoltageR;		// ÐŸÐ¾Ð½Ð¸Ð¶ÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ R  - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			underVoltageS;		// ÐŸÐ¾Ð½Ð¸Ð¶ÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ S  - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			underVoltageT;		// ÐŸÐ¾Ð½Ð¸Ð¶ÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ T  - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			overVoltageR;		// ÐŸÑ€ÐµÐ²Ñ‹ÑˆÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ R - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			overVoltageS;		// ÐŸÑ€ÐµÐ²Ñ‹ÑˆÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ S - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			overVoltageT;		// ÐŸÑ€ÐµÐ²Ñ‹ÑˆÐµÐ½Ð¸Ðµ Ð½Ð°Ð¿Ñ€ÑÐ¶ÐµÐ½Ð¸Ñ T - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			breakVoltR;		// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ R - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			breakVoltS;		// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ S - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem 			breakVoltT;		// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ T - Ð°Ð²Ð°Ñ€Ð¸Ñ
                
                TAlarmElem                      VDCmax;
  	// -------- Ð”Ð¸Ð°Ð³Ð½Ð¾ÑÑ‚Ð¸ÐºÐ° Ð½Ð°Ð³Ñ€ÑƒÑÐºÐ¸ ------------
		TAlarmElem			breakCurrU;			// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ U	- Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem			breakCurrV;			// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ V - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmElem			breakCurrW;			// ÐžÐ±Ñ€Ñ‹Ð² Ñ„Ð°Ð·Ñ‹ W - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmSHC			ShC_U;				// ÐšÐ¾Ñ€Ð¾Ñ‚ÐºÐ¾Ðµ Ð·Ð°Ð¼Ñ‹ÐºÐ°Ð½Ð¸Ðµ Ñ„Ð°Ð·Ñ‹ U - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmSHC			ShC_V;				// ÐšÐ¾Ñ€Ð¾Ñ‚ÐºÐ¾Ðµ Ð·Ð°Ð¼Ñ‹ÐºÐ°Ð½Ð¸Ðµ Ñ„Ð°Ð·Ñ‹ V - Ð°Ð²Ð°Ñ€Ð¸Ñ
		TAlarmSHC			ShC_W;				// ÐšÐ¾Ñ€Ð¾Ñ‚ÐºÐ¾Ðµ Ð·Ð°Ð¼Ñ‹ÐºÐ°Ð½Ð¸Ðµ Ñ„Ð°Ð·Ñ‹ W - Ð°Ð²Ð°Ñ€Ð¸Ñ      
                
                TAlarmElem                      IbreakMax;
    // -------- Ð”Ð¸Ð°Ð³Ð½Ð¾ÑÑ‚Ð¸ÐºÐ° ÑƒÑÑ‚Ñ€Ð¾Ð¹ÑÑ‚Ð²Ð° ----------

		TAlarmElem			overHeat;			// ÐŸÐµÑ€ÐµÐ³Ñ€ÐµÐ² Ð‘ÐšÐ” - Ð½ÐµÐ¸ÑÐ¿Ñ€Ð°Ð²Ð½Ð¾ÑÑ‚ÑŒ
		TAlarmElem			underCold; 			// ÐŸÐµÑ€ÐµÐ¾Ñ…Ð»Ð°Ð¶Ð´ÐµÐ½Ð¸Ðµ Ð‘ÐšÐ” - Ð½ÐµÐ¸ÑÐ¿Ñ€Ð°Ð²Ð½Ð¾ÑÑ‚ÑŒ    
                
                TAlarmElem			driveTemper;               
                TAlarmElem			moduleTemper;
                      
                TFltUnion		        outFaults;			// Àâàðèè
                Uns				ShcTmpState;		// Ïðîìåæóòî÷íàÿ ïåðåìåííàÿ äëÿ îïðåäåëåíèÿ ÊÇ
		Uns				FaultDelay;			// Ïàóçà ïîñëå èíèöèàëèçàöèè äëÿ âêëþ÷åíèÿ çàùèò
                Uns                             Dac_No_Conn_Tmp;
                Uns					NoMoveFlag;
                  Uns 				MuffFlag200Hz;		// Ð¤Ð»Ð°Ð³ Ð¼ÑƒÑ„Ñ‚Ñ‹ Ð¾Ð¿Ñ€ÐµÐ´ÐµÐ»ÐµÑÐµÐ¼Ñ‹Ð¹ Ð½Ð° 200 Ð“Ñ†
}TCoreProtections;

void Core_ProtectionsInit(TCoreProtections *);			// Èíèöèàëèçàöèÿ ìîäóëÿ çàùèò
void Core_Protections18kHzUpdate(TCoreProtections *);	        // Ðåàêöèÿ ñèñòåìû íà ñðàáàòûâàíèå çàùèò
void Core_Protections50HZUpdate(TCoreProtections *);	        // Ðåàêöèÿ ñèñòåìû íà ñðàáàòûâàíèå çàùèò 50 õç
void Core_ProtectionsReset(TCoreProtections *);			// Ñáðîñ çàùèò ïðè ïîâòîðíîì ïóñêå
void Core_ProtectionsClear(TCoreProtections *);			// Ñáðîñ âñåõ çàùèò
void Core_ProtectionsEnable(TCoreProtections *);		// Âêëþ÷åíèå/Âûêëþ÷åíèå çàùèò
void Protections_MuffFlag(void);

#endif
