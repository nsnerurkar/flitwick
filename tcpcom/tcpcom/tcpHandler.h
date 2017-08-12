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


  int run();  
  
  void pushTxChar(char);
  char popRxChar();

private:
  SOCKET sock;
  bool isValid;
  bool stop;
  std::queue<char> rxBuf;
  std::queue<char> txBuf;
};

#endif //_TCPHANDLER_H_

