#include "tcpHandler.h"


tcphandler::tcphandler()
{
  isValid = true;
  rxBuf.clear();
  txBuf.clear();
}

tcphandler::tcphandler(const char* address, int port)
{
  isValid = init(address,port);
  rxBuf.clear();
  txBuf.clear();
}

tcphandler::~tcphandler()
{
}


bool tcphandler::init(const char* address, int port)
{	
  if ( initSock() != 0)
  {
    return false;
  }

  if (!createSock(sock))
  {
    return false;
  }
  
  if (!connectSock(sock,port,address))
  {
    return false;
  }
  return true;
}

void tcphandler::pushTxChar(char input)
{
  txBuf.push_back(input);
}

char tcphandler::popRxChar()
{
  char ret = rxBuf.front();
  rxBuf.pop_front();
  return ret;
}

int tcphandler::run()
{
  stop = false;
  // Main
  while(!stop)
  {
    // Transmit loop
    if(!txBuf.empty)
    {
      len = txBuf.length();
      char* tempbuf = new char[len];
      for(int i = 0; i < len;i++)
      {
	tempbuf[i] = txBuf.front();
	txBuf.pop_front();
      }            
      sendSock(s, tempbuf,len);
      continue;
    }

    // Receive
    
    char temprxBuf[2000];
    int recvSize = 2000;
    if(recvSock(s, temprxBuf, recvSize))
    {
      for(int i = 0; i < recvSize;i++)
	rxBuf.push_back(temprxBuf[i]);
    }        
    usleep(100000);
  }
  return 0;
}




