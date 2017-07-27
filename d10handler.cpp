#include "d10handler.h"
#include <fstream>

void D10Handler::start()
{
	if (!com_port.port_open())
	{
		stopFlag = true;
		return;
	}
	stopFlag = false;
	while (!stopFlag)
	{
		// If there is anythng to transmit in the buffer, transmit it.
		// Loop over transmit vector
		if (!txStrings.empty())
		{			
			int i = 0, attempt = 0;
			while (i < txStrings.size() && attempt < 5)
			{
				com_port.clearRxBuf();
				com_port.clearTxBuf();
				transmit(txStrings[i]);
				int numRx = com_port.receive();
				if (numRx > 0 && com_port.rxbuf.front() == ACK_CHAR)
					++i;
				else
					++attempt;
			}

			if (attempt == 5)
			{
				// We could not transmit. In this case, we should suspend receipt. Sleep for 5 sec and try again.
#ifdef _WIN32
				Sleep(5000);
#else
				usleep(5000000);  /* sleep for 100 milliSeconds */
#endif
				continue;
			}
			else
			{
				//Transmit success. Clear com port buf for rx
				com_port.clearRxBuf();
				com_port.clearTxBuf();
				// Clear self tx Strings,
				txStrings.clear();
			}

		}
		else
		{
			// Receive mode by default.
			std::string tmp = getReceiveBuffer();
			if (!tmp.empty())
				rxStrings.push_back(tmp);
			com_port.clearTxBuf();
			com_port.txbuf.push(ACK_CHAR);
			com_port.transmit();			
			com_port.clearRxBuf();
			com_port.clearTxBuf();

			std::ofstream outfile;

			outfile.open("recv.txt", std::ios_base::app);
			outfile << tmp << std::endl; 

#ifdef _WIN32
			Sleep(500);
#else
			usleep(500000);  /* sleep for 100 milliSeconds */
#endif
		}
	}
	com_port.port_close();
}

void D10Handler::stop()
{
	stopFlag = true;
}

void D10Handler::transmit(const std::string& str)
{
	for (int i = 0; i < str.size(); ++i)
	{
		com_port.txbuf.push((unsigned char) str[i]);
	}

	com_port.transmit();
}

std::string D10Handler::getReceiveBuffer()
{
	int n = com_port.receive();
	if (n > 0)
	{
		//port has data.
		unsigned char* buf = new unsigned char[n+1];
		for (int i = 0; i < n; ++i)
		{
			buf[i] = com_port.rxbuf.front();
			com_port.rxbuf.pop();
		}
		buf[n] = 0;

		std::string retStr((char*)buf);
		return(retStr);
	}
	else
	{
		return("");
	}
}