#ifndef _D10HANDLER_H_
#define _D10HANDLER_H_

#include "common.h"
#include "nqueue.h"
#include "d10lis.h"



class d10handler
{
public:
  d10handler();
  //d10handler(const char*, int);
  d10handler(int portNum,long baud,const char* mode);
  ~d10handler();
  
  bool init();
  //bool init(const char*, int);
  
  void receive();
  void transmit();
  
  void transmitNAK();
  void transmitACK();

  void writeTxBuf(const char*, int);
  void process();

  bool isValid;  
  bool stop;
private:
  //SOCKET sock;
  
  int portNum;
  long baud;
  char mode[4];

  nqueue rxBuf;
  nqueue txBuf;
  d10lis* lis;
};

#endif //_D10HANDLER_H_

