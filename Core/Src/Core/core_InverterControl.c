
#include "core_InverterControl.h"

extern void CommandControl(TInvControl *);

__inline void InvUpdate(TInvControl *);
__inline void SpeedRegulation(TInvControl *);
__inline void CurrFeedback(TInvControl *);
__inline void StartTest(TInvControl *);
__inline void PWM_Form(TInvControl *v);
__inline void SpeedEstemation(TInvControl *v);
__inline void GenerateSignals(TInvControl *v);

void InvHiControl(TInvControl *v)
{

}

void InvLoControl(TInvControl *v)
{

}