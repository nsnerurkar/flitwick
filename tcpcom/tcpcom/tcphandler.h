#ifndef _TCPHANDLER_H_
#define _TCPHANDLER_h_

#include "common.h"

class tcphandler
{
public:
  tcphandler();
  tcphandler(const char*, int);
  ~tcphandler();
  
  bool init(const char*, int);
  
  bool receive();
  bool transmit();
  
  void transmitNAK();
  void transmitACK();

  char getLastRecvdChar();
  int readRxBuf(char*);

  void writeTxBuf(const char*, int);

  bool isValid;  
private:
  SOCKET sock;
  std::deque<char> rxBuf;
  std::deque<char> txBuf;
};

#endif //_TCPHANDLER_H_

