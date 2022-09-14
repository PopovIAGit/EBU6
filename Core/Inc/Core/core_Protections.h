

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- Подключение заголовочных файлов ------------------------------
#include "main.h"
#include "core_ProtectionsAlarm.h"
#include "core_ProtectionSHC.h"
#include "g_Structs.h"

//--------------------- Константы-------------------------------------------
#define PROTECT_SCALE (0.100 * PRD_50HZ)

// Номера битов g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7


// Номера битов g_Structs.h -> TNetReg
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


// Номера битов g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define ShCU_bit			5
#define ShCV_bit			6
#define ShCW_bit			7


// Номера битов g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_bit			        5
#define Tl_bit			        6
#define DriveTemper_bit			7
#define ModuleTemper_bit		8

//--------------------- Макросы --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- Структуры -------------------------------------------

typedef struct {
		// -------- Р”РёР°РіРЅРѕСЃС‚РёРєР° СЃРµС‚Рё ----------------
		TAlarmElem 			underVoltageR;		// РџРѕРЅРёР¶РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ R  - Р°РІР°СЂРёСЏ
		TAlarmElem 			underVoltageS;		// РџРѕРЅРёР¶РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ S  - Р°РІР°СЂРёСЏ
		TAlarmElem 			underVoltageT;		// РџРѕРЅРёР¶РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ T  - Р°РІР°СЂРёСЏ
		TAlarmElem 			overVoltageR;		// РџСЂРµРІС‹С€РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ R - Р°РІР°СЂРёСЏ
		TAlarmElem 			overVoltageS;		// РџСЂРµРІС‹С€РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ S - Р°РІР°СЂРёСЏ
		TAlarmElem 			overVoltageT;		// РџСЂРµРІС‹С€РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ T - Р°РІР°СЂРёСЏ
		TAlarmElem 			breakVoltR;		// РћР±СЂС‹РІ С„Р°Р·С‹ R - Р°РІР°СЂРёСЏ
		TAlarmElem 			breakVoltS;		// РћР±СЂС‹РІ С„Р°Р·С‹ S - Р°РІР°СЂРёСЏ
		TAlarmElem 			breakVoltT;		// РћР±СЂС‹РІ С„Р°Р·С‹ T - Р°РІР°СЂРёСЏ
                
                TAlarmElem                      VDCmax;
  	// -------- Р”РёР°РіРЅРѕСЃС‚РёРєР° РЅР°РіСЂСѓСЃРєРё ------------
		TAlarmElem			breakCurrU;			// РћР±СЂС‹РІ С„Р°Р·С‹ U	- Р°РІР°СЂРёСЏ
		TAlarmElem			breakCurrV;			// РћР±СЂС‹РІ С„Р°Р·С‹ V - Р°РІР°СЂРёСЏ
		TAlarmElem			breakCurrW;			// РћР±СЂС‹РІ С„Р°Р·С‹ W - Р°РІР°СЂРёСЏ
		TAlarmSHC			ShC_U;				// РљРѕСЂРѕС‚РєРѕРµ Р·Р°РјС‹РєР°РЅРёРµ С„Р°Р·С‹ U - Р°РІР°СЂРёСЏ
		TAlarmSHC			ShC_V;				// РљРѕСЂРѕС‚РєРѕРµ Р·Р°РјС‹РєР°РЅРёРµ С„Р°Р·С‹ V - Р°РІР°СЂРёСЏ
		TAlarmSHC			ShC_W;				// РљРѕСЂРѕС‚РєРѕРµ Р·Р°РјС‹РєР°РЅРёРµ С„Р°Р·С‹ W - Р°РІР°СЂРёСЏ      
                
                TAlarmElem                      IbreakMax;
    // -------- Р”РёР°РіРЅРѕСЃС‚РёРєР° СѓСЃС‚СЂРѕР№СЃС‚РІР° ----------

		TAlarmElem			overHeat;			// РџРµСЂРµРіСЂРµРІ Р‘РљР” - РЅРµРёСЃРїСЂР°РІРЅРѕСЃС‚СЊ
		TAlarmElem			underCold; 			// РџРµСЂРµРѕС…Р»Р°Р¶РґРµРЅРёРµ Р‘РљР” - РЅРµРёСЃРїСЂР°РІРЅРѕСЃС‚СЊ    
                
                TAlarmElem			driveTemper;               
                TAlarmElem			moduleTemper;
                      
                TFltUnion		        outFaults;			// Аварии
                Uns				ShcTmpState;		// Промежуточная переменная для определения КЗ
		Uns				FaultDelay;			// Пауза после инициализации для включения защит
                Uns                             Dac_No_Conn_Tmp;
}TCoreProtections;

void Core_ProtectionsInit(TCoreProtections *);			// Инициализация модуля защит
void Core_Protections18kHzUpdate(TCoreProtections *);	        // Реакция системы на срабатывание защит
void Core_Protections50HZUpdate(TCoreProtections *);	        // Реакция системы на срабатывание защит 50 хз
void Core_ProtectionsReset(TCoreProtections *);			// Сброс защит при повторном пуске
void Core_ProtectionsClear(TCoreProtections *);			// Сброс всех защит
void Core_ProtectionsEnable(TCoreProtections *);		// Включение/Выключение защит


#endif
