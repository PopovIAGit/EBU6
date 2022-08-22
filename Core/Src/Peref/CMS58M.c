//******************************************************************************
// Имя файла    :  'CMS58M.c'
// заголовок    :  Драйвер энкодера CMS58M
// Автор        :  Маньянов Р.Р.
// Контакты     :  
// Дата         :  12.10.2018
//******************************************************************************

#include "peref.h"


FDCAN_TxHeaderTypeDef  TxHeader;
FDCAN_RxHeaderTypeDef  RxHeader;
uint8_t CAN1RxMsg[]; 
uint8_t CAN1TxMsg[]; 

const sObject CyclicTimer = {0x6200, 0x00, UInt16, ReadWrite};
const sObject PresetValue = {0x6003, 0x00, UInt32, ReadWrite};


void txWriteNmt (eNmtService nmtService, sCms58m* cms58m);
void txWriteSdo (sObject object, uint8_t *data, sCms58m* cms58m);
void txReadSdo (sObject object, sCms58m* cms58m);

void Cms58mInir (sCms58m* cms58m)
{
  HAL_FDCAN_Start(&hfdcan2);
  cms58m->nodeId = 1;
  cms58m->value = 0;
  cms58m->lastLinkTime = 0;
  
    Cms58mConfig(cms58m);
    Cms58mStart(cms58m);
}

void Cms58mConfig (sCms58m* cms58m)
{
  uint32_t tt;
    txWriteNmt(StopNode, cms58m);            // останавливаю
 //  for (tt = 0; tt < 59560000; tt++);
    txWriteNmt(ResetCommunication, cms58m);  // вгоняю в режим настройки
// for (tt = 0; tt < 59560000; tt++);
    
    uint8_t data[4]={0x05,0x00,0x00,0x00};
    txWriteSdo(CyclicTimer,data, cms58m);    // в циклический таймер значение периода посылки PDO1
 // for (tt = 0; tt < 59560000; tt++);
}

void Cms58mStart (sCms58m* cms58m)
{
    // запуск узла в работу
    txWriteNmt(StartNode, cms58m);   
    return;
}

void Cms58mSetValue (uint32_t value, sCms58m* cms58m)
{
    txWriteNmt(StopNode, cms58m);   // останавливаю
  // Delay_ms(50);
    txWriteNmt(ResetCommunication, cms58m);   // вгоняю в режим настройки
  //  Delay_ms(50);
    
    uint8_t data[4];
    data[0]=(uint8_t)value;
    data[1]=(uint8_t)(value >> 8);
    data[2]=(uint8_t)(value >> 16);//
    data[3]=(uint8_t)(value >> 24);//={0x00,0x00,0x40,0x00};
    txWriteSdo(PresetValue, data, cms58m);
 //   Delay_ms(50);
    return;
}

void Cms58mRxHandler (sCms58m   *cms58m)
{
                         
 //  RxHeader.Identifier = 0x0;
  // RxHeader.IdType = FDCAN_STANDARD_ID;
    // RxHeader.RxFrameType = FDCAN_DATA_FRAME;
      // RxHeader.DataLength = FDCAN_DLC_BYTES_8;
       //  RxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
          // RxHeader.BitRateSwitch =  FDCAN_BRS_OFF;
            // RxHeader.FDFormat = FDCAN_CLASSIC_CAN;
              // RxHeader.RxTimestamp = 0x0;
                // RxHeader.FilterIndex = 0x0;
                  //RxHeader.IsFilterMatchingFrame = 0x0;//??????
      
        HAL_FDCAN_GetRxMessage(&hfdcan2,FDCAN_RX_FIFO0, &RxHeader, CAN1RxMsg);
            
    cms58m->value =/* 0x7FFFFF &*/ (CAN1RxMsg[2] << 16 |
                    CAN1RxMsg[1] << 8  |
                    CAN1RxMsg[0]);
    
    cms58m->lastLinkTime = 0;    
}

void txWriteNmt (eNmtService nmtService, sCms58m* cms58m)
{
   /* CAN1TxMsg.StdId = 0;       сруктура кадра       
    CAN1TxMsg.ExtId = 0;
    CAN1TxMsg.IDE = CAN_ID_STD;
    CAN1TxMsg.RTR = CAN_RTR_DATA;
    CAN1TxMsg.DLC = 2;
    CAN1TxMsg.Data[0] = (uint8_t)nmtService;
    CAN1TxMsg.Data[1] = cms58m->nodeId;
    CAN1TxMsg.Data[2] = 0;
    CAN1TxMsg.Data[3] = 0;
    CAN1TxMsg.Data[4] = 0; 
    CAN1TxMsg.Data[5] = 0; 
    CAN1TxMsg.Data[6] = 0; 
    CAN1TxMsg.Data[7] = 0; 
    
    CAN_Transmit(CAN1, &CAN1TxMsg);*/
      
     TxHeader.Identifier = 0x0;
       
     TxHeader.IdType = FDCAN_STANDARD_ID; // 11-битный ID

     TxHeader.TxFrameType = FDCAN_DATA_FRAME; // Передача обычного кадра данных

     TxHeader.DataLength = FDCAN_DLC_BYTES_8; // Длина данных 8 байт

     TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // Индикатор ошибок активен

     TxHeader.BitRateSwitch = FDCAN_BRS_OFF; // Без переключения частоты передачи

     TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // Формат классического кадра CAN

     TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;// Сохранение параметров события

     TxHeader.MessageMarker = 0x52;// Метка сообщения  
  
      CAN1TxMsg[0] = (uint8_t)nmtService;       // вот тут не понятно надо либо отправлять данные или формировать кадр can
      CAN1TxMsg[1] = cms58m->nodeId;
        
            if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan2, &TxHeader, CAN1TxMsg, FDCAN_TX_BUFFER0) != HAL_OK)

            {
    
              //  Error_Handler();

            }
            
            

              /* Send Tx buffer message */
           HAL_FDCAN_EnableTxBufferRequest(&hfdcan2, FDCAN_TX_BUFFER0);
          
    return;
}

void txWriteSdo (sObject object, uint8_t *data, sCms58m* cms58m)
{
    if (object.Access==ReadOnly)
    {
        txReadSdo(object, cms58m);
        return;
    }
    
    uint8_t noneDataSize;
    noneDataSize=4-object.Size;
   /* CAN1TxMsg.StdId=(uint32_t)(TxSdo+cms58m->nodeId);
    
    CAN1TxMsg.ExtId=0;
    CAN1TxMsg.RTR=CAN_RTR_DATA;
    CAN1TxMsg.IDE=CAN_ID_STD;
    CAN1TxMsg.DLC=8;
    
    CAN1TxMsg.Data[0]=(uint8_t)(WriteRequest) | (0x0C & (noneDataSize << 2)) | 0x03; // 0x73 | (noneDataSize << 2)
    CAN1TxMsg.Data[1]=(uint8_t)(object.Index);
    CAN1TxMsg.Data[2]=(uint8_t)(object.Index >> 8);
    CAN1TxMsg.Data[3]=object.SubIndex;
    for (uint8_t i=0;i<object.Size;i++)
        CAN1TxMsg.Data[i+4]=data[i];
    for (uint8_t i=object.Size;i<4;i++)
        CAN1TxMsg.Data[i+4]=0;
    
     CAN_Transmit(CAN1, &CAN1TxMsg);*/
       
     TxHeader.Identifier = 0x0;
       
     TxHeader.IdType = FDCAN_STANDARD_ID; // 11-битный ID

     TxHeader.TxFrameType = FDCAN_DATA_FRAME; // Передача обычного кадра данных

     TxHeader.DataLength = FDCAN_DLC_BYTES_8; // Длина данных 8 байт

     TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // Индикатор ошибок активен

     TxHeader.BitRateSwitch = FDCAN_BRS_OFF; // Без переключения частоты передачи

     TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // Формат классического кадра CAN

     TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;// Сохранение параметров события

     TxHeader.MessageMarker = 0x52;// Метка сообщения  
 
    
    CAN1TxMsg[0]=(uint8_t)(WriteRequest) | (0x0C & (noneDataSize << 2)) | 0x03; // 0x73 | (noneDataSize << 2)
    CAN1TxMsg[1]=(uint8_t)(object.Index);
    CAN1TxMsg[2]=(uint8_t)(object.Index >> 8);
    CAN1TxMsg[3]=object.SubIndex;
    for (uint8_t i=0;i<object.Size;i++)
        CAN1TxMsg[i+4]=data[i];
    for (uint8_t i=object.Size;i<4;i++)
        CAN1TxMsg[i+4]=0;
        
            if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, CAN1TxMsg) != HAL_OK)

            {

              //  Error_Handler();

            }     
     
    return;
}

void txReadSdo (sObject object, sCms58m* cms58m)
{
  /*  CAN1TxMsg.StdId=(uint32_t)(TxSdo + cms58m->nodeId);
    CAN1TxMsg.ExtId=0;
    CAN1TxMsg.RTR=CAN_RTR_DATA;
    CAN1TxMsg.IDE=CAN_ID_STD;
    CAN1TxMsg.DLC=8;
    
    CAN1TxMsg.Data[0]=(uint8_t)ReadRequest;
    CAN1TxMsg.Data[1]=(uint8_t)(object.Index);
    CAN1TxMsg.Data[2]=(uint8_t)(object.Index >> 8);
    CAN1TxMsg.Data[3]=object.SubIndex;
    for (uint8_t i=0;i<4;i++)
        CAN1TxMsg.Data[i+4]=0;
    
    CAN_Transmit(CAN1, &CAN1TxMsg);*/
      
     TxHeader.Identifier = 0x0;
       
     TxHeader.IdType = FDCAN_STANDARD_ID; // 11-битный ID

     TxHeader.TxFrameType = FDCAN_DATA_FRAME; // Передача обычного кадра данных

     TxHeader.DataLength = FDCAN_DLC_BYTES_8; // Длина данных 8 байт

     TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // Индикатор ошибок активен

     TxHeader.BitRateSwitch = FDCAN_BRS_OFF; // Без переключения частоты передачи

     TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // Формат классического кадра CAN

     TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;// Сохранение параметров события

     TxHeader.MessageMarker = 0x52;// Метка сообщения 
          
    CAN1TxMsg[0]=(uint8_t)ReadRequest;
    CAN1TxMsg[1]=(uint8_t)(object.Index);
    CAN1TxMsg[2]=(uint8_t)(object.Index >> 8);
    CAN1TxMsg[3]=object.SubIndex;
    for (uint8_t i=0;i<4;i++)
        CAN1TxMsg[i+4]=0;
            
             if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, CAN1TxMsg) != HAL_OK)

            {

                Error_Handler();

            }
                
    return;
}
