#ifndef _D10HANDLER_H_
#define _D10HANDLER_H_

#include "port.h"
#include "layer1.h"
#include "rs232.h"
#include <vector>
#include <string>

class D10Handler
{

	void start();
	void stop(); 

	void transmit(const std::string&);
	std::string getReceiveBuffer();

private:
	port com_port;
	layer1 message_layer;

	std::vector<std::string> txStrings;
	std::vector<std::string> rxStrings;

	bool stopFlag;
};


#endif //_D10HANDLER_H_