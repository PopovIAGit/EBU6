

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- Подключение заголовочных файлов ------------------------------
#include "main.h"
#include "g_Structs.h"

//--------------------- Константы-------------------------------------------
#define PROTECT_SCALE (0.100 * Prd50HZ)

// Номера битов g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7
#define SoftStarter_bit		8

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
#define OvR_max_bit			11
#define OvS_max_bit			12
#define OvT_max_bit			13


// Номера битов g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define I2t_bit				3
#define ShCU_bit			4
#define ShCV_bit			5
#define ShCW_bit			6
#define ISkew_bit			11

// Номера битов g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_BCP_bit			5
#define Tl_BCP_bit			6
#define Th_BCD_bit			7
#define Tl_BCD_bit			8

//--------------------- Макросы --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- Структуры -------------------------------------------

typedef struct {
		TFltUnion			        outFaults;			// Аварии
		TFltUnion			        outDefects;			// Неисправности
			// Флаг по которому сбрасывается КЗ
		Uns					FaultDelay;			// Пауза после инициализации для включения защит
}TCoreProtections;

void Core_ProtectionsInit(TCoreProtections *);			// Инициализация модуля защит
void Core_Protections18kHzUpdate(TCoreProtections *);	        // Реакция системы на срабатывание защит
void Core_Protections50HZUpdate(TCoreProtections *);	        // Реакция системы на срабатывание защит 50 хз
void Core_ProtectionsReset(TCoreProtections *);			// Сброс защит при повторном пуске
void Core_ProtectionsClear(TCoreProtections *);			// Сброс всех защит
void Core_ProtectionsEnable(TCoreProtections *);		// Включение/Выключение защит


#endif
