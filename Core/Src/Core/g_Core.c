
#include "g_Core.h"
#include "stat_fm25v10.h"


TFM25V10 Eeprom1;

TCore	g_Core;

void Core_Init(TCore *p)
{	
  p->DisplayTimer = 1 * PRD_10HZ;
  p->Status.bit.Stop = 1;  
  FM25V10_Init(&Eeprom1);   
  Core_MenuInit(&menu);
}

void core18kHZupdate(void)
{

}

void core200HZupdate(void)
{

}

void core50HZupdate(void)
{

}

void core10HZupdate(void)
{

}

void coreTU(TCore *p)
{
  if (g_Ram.UserParam.InputType == it24)
  {
      g_Ram.Status.StateTu.all = g_Peref.TU_data24;
  }
  else if (g_Ram.UserParam.InputType == it220)
  {
        g_Ram.Status.StateTu.all = g_Peref.TU_data220;
  }
}

void coreTS(TCore *p)
{
   g_Ram.HideParam.HideStateTs.bit.Opened       = p->Status.bit.Opened  ^ g_Ram.UserParam.TsInvert.bit.Dout0;
   g_Ram.HideParam.HideStateTs.bit.Closed       = p->Status.bit.Closed  ^ g_Ram.UserParam.TsInvert.bit.Dout1;
   g_Ram.HideParam.HideStateTs.bit.Opening      = p->Status.bit.Opening ^ g_Ram.UserParam.TsInvert.bit.Dout2;
   g_Ram.HideParam.HideStateTs.bit.Closing      = p->Status.bit.Closing ^ g_Ram.UserParam.TsInvert.bit.Dout3;
   g_Ram.HideParam.HideStateTs.bit.Fault        = p->Status.bit.Fault   ^ g_Ram.UserParam.TsInvert.bit.Dout4;
   g_Ram.HideParam.HideStateTs.bit.MuftaOpen    = p->Status.bit.Mufta   ^ g_Ram.UserParam.TsInvert.bit.Dout5;
   g_Ram.HideParam.HideStateTs.bit.MuftaClose   = p->Status.bit.Mufta   ^ g_Ram.UserParam.TsInvert.bit.Dout6;
   g_Ram.HideParam.HideStateTs.bit.MUDU         = p->Status.bit.MuDu    ^ g_Ram.UserParam.TsInvert.bit.Dout7; 
}

void coreTLocalControl(TCore *p)
{
        static Uns prevHalls = 0;
	static Bool readyForNewCmd = true;	// flag ready to new cmd
        
       if (menu.State == 0) g_Core.Status.bit.Program = 0;
       else g_Core.Status.bit.Program = 1;
  
        if (g_Core.Status.bit.Program)
         {
           
           if (prevHalls != g_Peref.BtnStatus) readyForNewCmd = true;
           
            if (readyForNewCmd)
            {
                switch (g_Peref.BtnStatus)
                 {
                   case BTN_OPEN: 
                      menu.Key = KEY_LEFT;
                     break;         
                   case BTN_CLOSE: 
                       menu.Key = KEY_RIGHT;
                     break; 
                   case BTN_STOP1:    //
                        menu.Key = KEY_UP;
                     break;
                   case BTN_STOP2:    //
                        menu.Key = KEY_DOWN;
                     break;
                   case BTN_PROG1:    // Enter
                          menu.Key = KEY_ENTER;
                     break;
                   case BTN_PROG2:    // exit
                          menu.Key = KEY_ESCAPE;
                      break; 
                 }
            }
           readyForNewCmd = false;
           
         }
         else 
         {
             switch(g_Peref.BtnStatus & (~BTN_STOP))
              {
                    case BTN_OPEN: 
                                   // Mcu.Mpu.BtnKey = KEY_OPEN;
                            break;
                    case BTN_CLOSE:
                                   // Mcu.Mpu.BtnKey = KEY_CLOSE;
                            break;
                    case BTN_PROG1:
                      {
                          if (p->Status.bit.Stop)
                          {
                           menu.Key = KEY_ENTER;
                          }
                      }       
                       break; 
                      case BTN_PROG2:
                      {
                          if (p->Status.bit.Stop)
                          {                       
                          // menu.Key = KEY_ENTER;
                          }
                      }       
                       break; 
              }
              switch(g_Peref.BtnStatus & BTN_STOP)
              {
                case BTN_STOP1:
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                break;
                
                case BTN_STOP2:
                          if (p->Status.bit.MuDu || g_Ram.UserParam.MuDuSetup == mdOff)
                         // Mcu.Mpu.BtnKey = KEY_STOP;
                break;
              }

         }
  
    prevHalls = g_Peref.BtnStatus;    
        
    g_Ram.FactoryParam.HallBlock.bit.Open  = g_Peref.BtnOpen.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Close = g_Peref.BtnClose.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Stop1 = g_Peref.BtnStop1.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Stop2 = g_Peref.BtnStop2.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Prog1 = g_Peref.BtnProg1.Flag;
    g_Ram.FactoryParam.HallBlock.bit.Prog2 = g_Peref.BtnProg2.Flag;
  

}



