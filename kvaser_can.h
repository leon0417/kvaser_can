// Copyright (c) 2019 Fujian(Quanzhou)-HIT Research Institute of Engineering and Technology All rights reserved.
#ifndef KVASER_CAN_H
#define KVASER_CAN_H

#include "canlib.h"

class KvaserCan
{
private:
  CanHandle hnd_;
  //std::vector<T> &msg_;

public:
  KvaserCan() : hnd_(0) {}
  ~KvaserCan() {}
  void kvaserCanInit();
  void sdoParaService(unsigned short can_id, unsigned short can_index, unsigned char can_sub_index,int can_data, unsigned char can_cmd);
  void pdoDataSend(unsigned short cob_id, int can_data_low, int can_data_high, unsigned char can_len);
  
  int pdoDataReceive(long *id,
                     void *msg,
                     unsigned int *dlc,
                     unsigned int *flag,
                     unsigned long *time);
};

#endif