/*======================================================================
Имя файла:         	vector.h
Автор:             	PopovIA
						  	
Версия файла:      	01.00
Дата изменения:		09/12/22
Применяемость:     	
Описание:
vector control
======================================================================*/

#ifndef VECTOR_
#define VECTOR_

#include "main.h"
#include "std.h"
#include "stdint.h"
#include "mc_type.h"
#include "mc_math.h"
#include "speed_torq_ctrl.h"
#include "pwm_curr_fdbk.h"



#include "pid_regulator.h" 
#include "sto_pll_speed_pos_fdbk.h"


#ifdef __cplusplus
extern "C" {
#endif

// DEFINE--------------------------------------------------------------
  /* Speed control loop */
#define SPEED_LOOP_FREQUENCY_HZ       ( uint16_t )1000 /*!<Execution rate of speed
                                                      regulation loop (Hz) */
#define MF_TASK_OCCURENCE_TICKS  (PRD_2KHZ/SPEED_LOOP_FREQUENCY_HZ)-1u
/***************** MOTOR ELECTRICAL PARAMETERS  ******************************/
#define POLE_PAIR_NUM          4 /* Number of motor pole pairs */
#define RS                     2.85 /* Stator resistance , ohm*/
#define LS                     0.015000 /* Stator inductance, H
                                                 For I-PMSM it is equal to Lq */

/* When using Id = 0, NOMINAL_CURRENT is utilized to saturate the output of the
   PID for speed regulation (i.e. reference torque).
   Transformation of real currents (A) into int16_t format must be done accordingly with
   formula:
   Phase current (int16_t 0-to-peak) = (Phase current (A 0-to-peak)* 32767 * Rshunt *
                                   *Amplifying network gain)/(MCU supply voltage/2)
*/

#define NOMINAL_CURRENT         20255
#define MOTOR_MAX_SPEED_RPM     4000 /*!< Maximum rated speed  */
#define MOTOR_VOLTAGE_CONSTANT  50.0 /*!< Volts RMS ph-ph /kRPM */
#define ID_DEMAG                -20255 /*!< Demagnetization current */  
  
  
  
/* MMI Table Motor 1 MAX_MODULATION_99_PER_CENT */
#define MAX_MODULE 32439
  
// PID
#define IQMAX                          20255 
  
#define SPEED_UNIT                    10
#define PID_SPEED_KP_DEFAULT          1000/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/
#define PID_SPEED_KI_DEFAULT          700/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/
#define PID_SPEED_KD_DEFAULT          0/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/
/* Speed PID parameter dividers */
#define SP_KPDIV                      16
#define SP_KIDIV                      256
#define SP_KDDIV                      16
#define SP_KPDIV_LOG                  LOG2((16))
#define SP_KIDIV_LOG                  LOG2((256))
#define SP_KDDIV_LOG                  LOG2((16))
  
  typedef enum
{
  MCI_BUFFER_EMPTY,                  /*!< If no buffered command has been
                                            called.*/
  MCI_COMMAND_NOT_ALREADY_EXECUTED,  /*!< If the buffered command condition
                                            hasn't already occurred.*/
  MCI_COMMAND_EXECUTED_SUCCESFULLY,  /*!< If the buffered command has been
                                            executed successfully.*/
  MCI_COMMAND_EXECUTED_UNSUCCESFULLY /*!< If the buffered command has been
                                            executed unsuccessfully.*/
} MCI_CommandState_t ;

typedef enum
{
  MCI_NOCOMMANDSYET,            /*!< No command has been set by the user.*/
  MCI_CMD_EXECSPEEDRAMP,        /*!< ExecSpeedRamp command coming from the user.*/
  MCI_CMD_EXECTORQUERAMP,       /*!< ExecTorqueRamp command coming from the user.*/
  MCI_CMD_SETCURRENTREFERENCES, /*!< SetCurrentReferences command coming from the
                                 user.*/
} MCI_UserCommands_t;

/**
  * @brief  State_t enum type definition, it lists all the possible state machine states
  */
typedef enum
{
  ICLWAIT = 12,         /*!< Persistent state, the system is waiting for ICL
                           deactivation. Is not possible to run the motor if
                           ICL is active. Until the ICL is active the state is
                           forced to ICLWAIT, when ICL become inactive the state
                           is moved to IDLE */
  IDLE = 0,             /*!< Persistent state, following state can be IDLE_START
                           if a start motor command has been given or
                           IDLE_ALIGNMENT if a start alignment command has been
                           given */
  ALIGNMENT = 2,        /*!< Persistent state in which the encoder are properly
                           aligned to set mechanical angle, following state can
                           only be ANY_STOP */
  CHARGE_BOOT_CAP = 16, /*!< Persistent state where the gate driver boot
                           capacitors will be charged. Next states will be
                           OFFSET_CALIB. It can also be ANY_STOP if a stop motor
                           command has been given. */
  OFFSET_CALIB = 17,    /*!< Persistent state where the offset of motor currents
                           measurements will be calibrated. Next state will be
                           CLEAR. It can also be ANY_STOP if a stop motor
                           command has been given. */
  START = 4,            /*!< Persistent state where the motor start-up is intended
                           to be executed. The following state is normally
                           SWITCH_OVER or RUN as soon as first validated speed is
                           detected. Another possible following state is
                           ANY_STOP if a stop motor command has been executed */
  SWITCH_OVER = 19,     /**< TBD */
  RUN = 6,              /*!< Persistent state with running motor. The following
                           state is normally ANY_STOP when a stop motor command
                           has been executed */
  STOP = 8,             /*!< Persistent state. Following state is normally
                           STOP_IDLE as soon as conditions for moving state
                           machine are detected */
  FAULT_NOW = 10,       /*!< Persistent state, the state machine can be moved from
                           any condition directly to this state by
                           STM_FaultProcessing method. This method also manage
                           the passage to the only allowed following state that
                           is FAULT_OVER */
  FAULT_OVER = 11,       /*!< Persistent state where the application is intended to
                          stay when the fault conditions disappeared. Following
                          state is normally STOP_IDLE, state machine is moved as
                          soon as the user has acknowledged the fault condition.
                      */
  WAIT_STOP_MOTOR = 20

} MCI_State_t;

typedef enum
{
  MCI_NO_COMMAND = 0,    /**< No Command --- Set when going to IDLE */
  MCI_START,            /**< Start controling the Motor */
  MCI_ACK_FAULTS,       /**< Acknowledge Motor Control subsystem faults */
  MCI_MEASURE_OFFSETS,  /**< Start the ADCs Offset measurements procedure */
  /* Shouldn't we remove this command ? */
  MCI_ALIGN_ENCODER,    /**< Start the Encoder alignment procedure */
  MCI_STOP              /**< Stop the Motor and the control */
} MCI_DirectCommands_t;
  
  
  /**
  * @brief  CircleLimitation component parameters definition
  */
typedef struct
{
  uint16_t MaxModule;               /**<  Circle limitation maximum allowed
                                         module */
  uint16_t MaxVd;                   /**<  Circle limitation maximum allowed
                                         module */
  uint16_t Circle_limit_table[87];  /**<  Circle limitation table */
  uint8_t  Start_index;             /**<  Circle limitation table indexing
                                         start */
} CircleLimitation_Handle_t;



typedef struct
{
  SpeednTorqCtrl_Handle_t * pSTC; /*!< Speed and torque controller object used by MCI.*/
  pFOCVars_t pFOCVars;    /*!< Pointer to FOC vars used by MCI.*/
  PWMC_Handle_t *pPWM;    /*!< Pointer to PWM handle structure.*/
  MCI_UserCommands_t lastCommand; /*!< Last command coming from the user.*/
  int16_t hFinalSpeed;        /*!< Final speed of last ExecSpeedRamp command.*/
  int16_t hFinalTorque;       /*!< Final torque of last ExecTorqueRamp
                                   command.*/
  qd_t Iqdref;     /*!< Current component of last
                                   SetCurrentReferences command.*/
  uint16_t hDurationms;       /*!< Duration in ms of last ExecSpeedRamp or
                                   ExecTorqueRamp command.*/
 MCI_DirectCommands_t DirectCommand;
 MCI_State_t State;
 uint16_t CurrentFaults;
 uint16_t PastFaults;
 MCI_CommandState_t CommandState; /*!< The status of the buffered command.*/
 STC_Modality_t LastModalitySetByUser; /*!< The last STC_Modality_t set by the
                                             user. */
} MCI_Handle_t;



typedef struct {
    Uns bMCBootCompleted; // флаг инициализации
    uint16_t hMFTaskCounterM1;
    
    CircleLimitation_Handle_t CircleLimitationM1;
    MCI_Handle_t pMCI;

    
    PID_Handle_t      PIDSpeedHandle_M1;  
    STO_PLL_Handle_t  STO_PLL_M1;
} TVector;

// init--------------------------------------------------------------------------------------
void Vector_Init(TVector *p);


// Executes------------------------------------------------------------------------------------
void Vector_18KHZ(TVector *p);
void Vector_2KHZ(TVector *p);
void Vector_200HZ(TVector *p);
void Vector_50HZ(TVector *p);

/* Executes the Medium Frequency Task functions for each drive instance */
void MC_Scheduler(void);

/* Executes safety checks (e.g. bus voltage and temperature) for all drive instances */
void TSK_SafetyTask(void);

extern TVector	                g_Vector;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
