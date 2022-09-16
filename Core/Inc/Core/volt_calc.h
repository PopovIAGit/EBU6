/*======================================================================
Имя файла:         	volt_calc.h
Автор:             	
Версия файла:      	01.01
Дата изменения:		12/02/10
Применяемость:     	Совместно с библиотекой IQmath
Описание:
The Voltage Reconstruction based on switching functions and DC-Bus voltage
======================================================================*/

#ifndef VOLT_CALC_
#define VOLT_CALC_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  #define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_2PI	    6.283185307179586476925
#define M_PI_2      1.57079632679489661923
#define M_PI_3		1.0471975511965977461542
#define M_PI_4      0.785398163397448309616
#define M_2PI_3		2.094395102393195492308
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_1DIV3		0.333333333333333333333
#define M_2DIV3		0.666666666666666666666
#define M_SQRT_2    1.41421356237309504880
#define M_SQRT_3		1.732050807568877293527
#define M_SQRT_6		2.449489742783178098197
#define M_SQRT_3_2	0.8660254037844386467637
#define M_SQRT_2_3	0.8164965809277260327324
#define M_1_SQRT2	0.707106781186547524401
#define M_1_SQRT3	0.577350269189625764509
  

#define   _IQmpy(A,B)         ((A) * (B))
#define   _IQdiv(A,B)         ((float)(A) / (float)(B))

typedef struct 	{
	Float DcBusVolt;		// Input: DC-bus voltage (pu)
	Float MfuncV1;  		// Input: Modulation voltage phase A (pu)
	Float MfuncV2;			// Input: Modulation voltage phase B (pu)	
	Float MfuncV3;			// Input: Modulation voltage phase C (pu) 
	Float OutOfPhase; 	// Parameter: Out of Phase adjustment (0 or 1) (Q0) - independently with global Q
	Float VphaseA;			// Output: Phase voltage phase A (pu)
	Float VphaseB;			// Output: Phase voltage phase B (pu) 
	Float VphaseC;			// Output: Phase voltage phase C (pu) 
	Float Valpha;			// Output: Stationary d-axis phase voltage (pu)
	Float Vbeta;  			// Output: Stationary q-axis phase voltage (pu)
} PHASEVOLTAGE;	                   

void phase_voltage_calc(PHASEVOLTAGE *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif

