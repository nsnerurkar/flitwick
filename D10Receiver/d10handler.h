#ifndef _D10HANDLER_H_
#define _D10HANDLER_H_

#include "common.h"
#include "nqueue.h"
#include "d10lis.h"
#include "serialport.h"


class d10handler
{
public:
	d10handler();

	d10handler(int portNum, long baud, const char* mode);
	~d10handler();

	bool init();
	void Start();
	
	bool isValid();
private:
	void transmitNAK();
	void transmitACK();
	void handleIncomingMessage();
private:
	serialport com;
	d10lis lis;
};

#endif //_D10HANDLER_H_

