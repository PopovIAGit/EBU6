/*======================================================================
��� �����:          peref_SensObserver.h
�����:
������ �����:
���� ���������:
��������:
������ ��������� ��������
�����: ������� � ��� � Uns
������: ������� � IQ24
======================================================================*/

#ifndef PEREF_SENS_OBSERVER_
#define PEREF_SENS_OBSERVER_

//----------- ����������� ������������ ������ -----------------------------

//#include "config.h"
//--------------------- ���������-------------------------------------------
//--------------------- ������� --------------------------------------------
//#define ADC_CONV(Value, Gain, Offset) 	(((LgInt)(Value - Offset) * Gain) >> 13)  #define ADC_CONV(Value, Gain, Offset) 		(0.00001 * (int16)((Value) - (Offset)) * (Gain))
#define ADC_CONV(Value, Gain, Offset) 		(0.00001 * (int16)((Value) - (Offset)) * (Gain))
#define ADC_TU_CONV(Value, Gain, Offset) 	(0.00001 * (LgInt)((Value) - (Offset)) * (Gain))
//#define ADC_CONV(Value, Gain, Offset) 	((LgInt)(Value - Offset) * Gain)
//-------------------- ��������� ------------------------------------------
// ��������� ��������
typedef struct {
	Uns				*p_UR_Mpy;		// �������������� ��� ���������� R
	Uns 			*p_US_Mpy;		// �������������� ��� ���������� S
	Uns 			*p_UT_Mpy;		// �������������� ��� ���������� T
	Uns				*p_UR_Offset;	// ����� ��� ���������� R
	Uns 			*p_US_Offset;	// ����� ��� ���������� S
	Uns 			*p_UT_Offset;	// ����� ��� ���������� T
	Uns				*p_IU_Mpy;		// �������������� ��� ���� U
	Uns 			*p_IV_Mpy;		// �������������� ��� ���� V
	Uns 			*p_IW_Mpy;		// �������������� ��� ���� W
	Uns				*p_IU_Offset;	// ����� ��� ���� U
	Uns 			*p_IV_Offset;	// ����� ��� ���� V
	Uns 			*p_IW_Offset;	// ����� ��� ���� W
        Uns				*p_VDC_AU_Mpy;
        Uns				*p_VDC_AU_Offset;
        Uns				*p_I_brake_A_Mpy;
        Uns				*p_I_brake_A_Offset;
        
} TSensorsParams;

// ����� ���������� ��������
typedef enum{
	mManual     = 0,				// ����� ������ ���������
	mAutoOffset = 1,				// ����� ������������ offset
	mTest		= 2					// ����� ����
} TOffsetMode;

// �������� ��������� ��������� �������� ���� � ����������
typedef struct {
	TSensorsParams		        parSensors;		// ��������: ��������� ��������
	TOffsetMode		        pOffsetMode;	        // ��������: ������ ���������� ������ (Offset)
	float32				URinp;			// ����: ���������� R
	float32 			USinp;			// ����: ���������� S
	float32 			UTinp;			// ����: ���������� T
	float32				IUinp;			// ����: ��� U
	float32 			IVinp;			// ����: ��� V
	float32	 			IWinp;			// ����: ��� W
	float32				URout;			// �����: ���������� R
	float32 			USout;			// �����: ���������� S
	float32 			UTout;			// �����: ���������� T
	float32				IUout;			// �����: ��� U
	float32 			IVout;			// �����: ��� V
	float32 			IWout;			// �����: ��� W
        
        float32				VDC_AUinp;			// ����: 
        float32				VDC_AUout;                      // �����: 
	float32 			I_brake_Ainp;			// ����: 
        float32				I_brake_Aout;                   // �����: 
} TSensObserver;


//------------------- ���������� ���������� -------------------------------------
//------------------- ��������� ������� -----------------------------------------
void Peref_SensObserverInit(TSensObserver *p);
void SetOffsets(TSensObserver *p);
void Peref_SensObserverUpdate(TSensObserver *p);



#endif
