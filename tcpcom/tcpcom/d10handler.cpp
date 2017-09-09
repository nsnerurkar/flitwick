#include "d10handler.h"
#include "quicksock.h"
#include "rs232.h"

d10handler::d10handler()
{
  isValid = true;
  rxBuf.clear();
  txBuf.clear();
  stop = false;
  lis = new d10lis();
}

/*
d10handler::d10handler(const char* address, int port)
{
  isValid = init(address,port);
  rxBuf.clear();
  txBuf.clear();
  stop = false;
  lis = new d10lis();
}
*/

d10handler::d10handler(int port,long baud,const char* md)
{
  portNum = port;
  this->baud = baud;
  mode[0] = md[0];
  mode[1] = md[1];
  mode[2] = md[2];
  mode[3] = md[3];
  isValid = init();
  rxBuf.clear();
  txBuf.clear();
  stop = false;
  lis = new d10lis();
}

d10handler::~d10handler()
{
  RS232_CloseComport(portNum);
}


bool d10handler::init()
{
  if (RS232_OpenComport(portNum, baud, mode))
  {
      return false;
  }
  return true;
}


/*
bool d10handler::init(const char* address, int port)
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

void d10handler::receive()
{  
  unsigned char temprxBuf[1024];
  while(!stop)
  {    
    int recvSize = 1023;
    if(recvSock(sock, temprxBuf, recvSize))
    {
      for(int i = 0; i < recvSize;i++)
      {
        if(temprxBuf[i] != 0)
          rxBuf.push(temprxBuf[i]);
      }
    }
  }
}

void d10handler::transmit()
{
  while(!stop)
  {
    if(!txBuf.isEmpty())
    {
      int len = txBuf.getQSize();
      unsigned char* tempbuf = new unsigned char[len];
      for(int i = 0; i < len;i++)
      {
	        tempbuf[i] = txBuf.pop();
      }            
      sendSock(sock, tempbuf,len);
    }
  }
}*/

void d10handler::receive()
{
  unsigned char buf[1024];
  
  while(!stop)
  {
    int recv_size = RS232_PollComport(portNum, buf, 1023);
    for(int i = 0; i < recv_size; ++i)
    {
      rxBuf.push(buf[i]);		
    }
  }
}



void d10handler::transmit()
{
  while(!stop)
  {
    if(!txBuf.isEmpty())
    {
      int len = txBuf.getQSize();
      unsigned char* tempbuf = new unsigned char[len];
      for(int i = 0; i < len;i++)
      {
	        tempbuf[i] = txBuf.pop();
      }
      RS232_SendBuf(portNum, tempbuf, len);
    }
  }
}




void d10handler::writeTxBuf(const char* buf, int size)
{
  for(int i = 0; i < size;++i)
  {
    txBuf.push(buf[i]);
  }
}

void d10handler::transmitNAK()
{
  unsigned char buf[2];
  buf[0] = 21;
  buf[1] = 4;
  writeTxBuf(buf,2);
}

void d10handler::transmitACK()
{
  unsigned char buf[2];
  buf[0] = 6;
  buf[1] = 4;
  writeTxBuf(buf,2);
}


void d10handler::process()
{
  while(!stop)
  {
    if(!rxBuf.isEmpty() && rxBuf.getQSize() >=2) 
    {
      
      unsigned char buf[1024];
      unsigned char rcvdByte = 0;
      int len = 0;
      while((rcvdByte != EOT_CHAR))
      {
        if(rxBuf.isEmpty())
          continue;        
        rcvdByte = rxBuf.pop();
        buf[len++] = rcvdByte;        
      }
      rxBuf.clear();
      buf[len] = 0;
      std::string recvdStr(buf);
      d10Message* pMsg;
      bool l1Valid;
      pMsg = lis->parseMessage(recvdStr,l1Valid);

      if(l1Valid)
        transmitACK();
      else
        transmitNAK();

      // Act on your Message here.
      lis->processData(pMsg);

      delete pMsg;
    }
    usleep(100000);
  }

}
