#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include "common.h"
#include "rs232.h"


enum enParity 
{
	NONE,
	ODD,
	EVEN,
	MARK,
	SPACE
};

enum enStopBits
{
	ONE,
	ONEFIVE,
	TWO
};


class serialport
{
public:
	serialport();
	serialport(int port, long baud, const char* mode);
	~serialport();

	void	setComPortNum(int num)	{ nPort = num; }
	int		getComPortNum()			{ return nPort; }

	void	setBaudRate(long num)	{ nBaud = num; }
	long	getBaudRate()			{ return nBaud; }

	void	setByteSize(int num)	{ nByteSize = num; }
	int		getByteSize()			{ return nByteSize; }

	void	setParity(enParity p)	{ eParity = p; }
	enParity getParity()			{ return eParity; }

	void	setStopBits(enStopBits b){ eStopBits = b; }
	enStopBits getStopBits()		{ return eStopBits; }

	bool Open();
	void Close();

	bool ReadByte(unsigned char& byte);
	int ReadBuf(unsigned char* buf, int size);

	bool SendByte(unsigned char byte);
	bool SendBuf(unsigned char* buf, int size);

	bool isValid() { return isOpen; }
private:
	int			nPort;
	long		nBaud;
	int			nByteSize;
	enParity	eParity;
	enStopBits	eStopBits;

	bool		isOpen;
};





#endif //_SERIALPORT_H_