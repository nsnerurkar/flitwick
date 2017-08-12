#include "tcphandler.h"


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

bool tcphandler::recieve()
{  
  char temprxBuf[2000];
  int recvSize = 2000;
  if(recvSock(s, temprxBuf, recvSize))
  {
    for(int i = 0; i < recvSize;i++)
      rxBuf.push_back(temprxBuf[i]);
    return true;
  }
  return false;
}

bool tcphandler::transmit()
{
    if(!txBuf.empty)
    {
      len = txBuf.length();
      char* tempbuf = new char[len];
      for(int i = 0; i < len;i++)
      {
	tempbuf[i] = txBuf.front();
	txBuf.pop_front();
      }            
      return sendSock(s, tempbuf,len);
    }
    return true;
}


char tcphandler::getLastRecvdChar()
{
  return rxBuf.back();
}


int tcphandler::readRxBuf(char* buf)
{
  bytes = rxBuf.length();
  if(buf)
    delete(buf);
  buf = new char[bytes];
  for(int i = 0; i < bytes;++i)
  {
    buf[i] = rxBuf.front();
    rxBuf.pop_front();
  }
  return bytes;
}

void tcphandler::writeTxBuf(const char* buf, int size)
{
  for(int i = 0; i < size;++i)
  {
    txBuf.push_back(buf[i]);
  }
}

void tcphandler::transmitNAK()
{
  char buf[2];
  buf[0] = 21;
  buf[1] = 4;
  writeTxBuf(buf,2);
  transmit();
}

void tcphandler::transmitACK()
{
  char buf[2];
  buf[0] = 6;
  buf[1] = 4;
  writeTxBuf(buf,2);
  transmit();
}
