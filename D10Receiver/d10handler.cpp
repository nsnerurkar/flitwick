#include "d10handler.h"
#include "rs232.h"
#include <iostream>

d10handler::d10handler()
{
	// Do Nothing.
}


d10handler::d10handler(int port, long baud, const char* md) : com(port,baud,md)
{
	com.Open();
}

d10handler::~d10handler()
{
	com.Close();
}


bool d10handler::isValid()
{
	return com.isValid();
}

bool d10handler::init()
{
	return com.Open();
}



void d10handler::transmitNAK()
{
	com.SendByte(unsigned char(21));
}

void d10handler::transmitACK()
{
	com.SendByte(unsigned char(6));
}


void d10handler::Start()
{
	while (true)
	{
		unsigned char byte = 0;
		com.ReadByte(byte);
		switch (byte)
		{
		case 4:
			continue;
		case 5:
			transmitACK();
			break;
		case 2:
			handleIncomingMessage();
			break;
		}
		Sleep(500);
	}
}


void d10handler::handleIncomingMessage()
{
	unsigned char rcvByte = 0;
	unsigned char buf[8128];
	buf[0] = 2;
	int len = 1;
	while (rcvByte != ETX_CHAR)
	{
		if(com.ReadByte(rcvByte))	
			buf[len++] = rcvByte;
	}
	com.ReadByte(buf[len++]);
	com.ReadByte(buf[len++]);
	com.ReadByte(buf[len++]);
	com.ReadByte(buf[len++]);
	buf[len] = 0;
	std::string recvdStr((char*)buf);
	d10Message* pMsg;
	bool l1Valid;
	pMsg = lis.parseMessage(recvdStr, l1Valid);

	std::cout << "  Acquired ... " << recvdStr << std::endl;

	if (l1Valid)
		transmitACK();
	else
		transmitNAK();

	lis.processData(pMsg);

	delete pMsg;
}



		
