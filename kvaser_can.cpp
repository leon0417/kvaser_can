// Copyright (c) 2019 Fujian(Quanzhou)-HIT Research Institute of Engineering and Technology All rights reserved.
#include "motor_test_demo/kvaser_can.h"

void KvaserCan::kvaserCanInit()
{
  canInitializeLibrary();
  hnd_ = canOpenChannel(3, canWANT_EXCLUSIVE);//第3路can,通道3
  canSetBusParams(hnd_, canBITRATE_500K, 4, 3, 1, 1, 0);
  canSetBusOutputControl(hnd_, canDRIVER_NORMAL);
  canBusOn(hnd_);
  canFlushTransmitQueue(hnd_);
  canFlushReceiveQueue(hnd_);
}

void KvaserCan::sdoParaService(unsigned short can_id, unsigned short can_index, unsigned char can_sub_index,int can_data, unsigned char can_cmd)
{
  unsigned char  can_sdo_msg[8] = {0};//对于sdo来说，后四个字节才是真正意义的数据
  
  can_sdo_msg[0] = can_cmd;//命令字，根据sdo是读取驱动器数据指令还是写入驱动器指令会有不同
  can_sdo_msg[1] = can_index & 0x00fff;//低８位
  can_sdo_msg[2] = can_index >> 8;//高八位
  can_sdo_msg[3] = can_sub_index;

  //实际数据
  can_sdo_msg[4] = can_data & 0x000000ff;
  can_sdo_msg[5] = (can_data & 0x0000ff00) >> 8;
  can_sdo_msg[6] = (can_data & 0x00ff0000) >> 16;
  can_sdo_msg[7] = (can_data & 0xff000000) >> 24;
  
  canSetBusParams(hnd_, canBITRATE_500K, 4, 3, 1, 1, 0);
  canSetBusOutputControl(hnd_, canDRIVER_NORMAL);
  canBusOn(hnd_);
  canWrite(hnd_, can_id, can_sdo_msg, 8, 0);//下发数据
  canWriteSync(hnd_, 500);
  canBusOff(hnd_);
  canClose(hnd_);
}


void KvaserCan::pdoDataSend(unsigned short cob_id, int can_data_low, int can_data_high, unsigned char can_len)
{
  unsigned char can_pdo_msg[8];
  int ret = -1;//判断发送完成
  int j = 0;
  for(int i = 0; i < 8; i++)
  {
    if(i < 4)can_pdo_msg[i] = (can_data_low >> (8 * i)) & 0x000000ff;
    else if(i >= 4)can_pdo_msg[i] = (can_data_low >> (8 * (i - 4))) & 0x000000ff;
  }

  ret = canWrite(hnd_, cob_id, can_pdo_msg, can_len, 0);//下发数据
  
  while(j < 0xfffa)
  {
    ret = canWrite(hnd_, cob_id, can_pdo_msg, can_len, 0);//下发数据

    if (canOK == ret) break;

    j++;
  } 
}

int KvaserCan::pdoDataReceive(long *id,
                              void *msg,
                              unsigned int *dlc,
                              unsigned int *flag,
                              unsigned long *time)
{
  return canRead(hnd_, id, msg, dlc, flag, time);
}