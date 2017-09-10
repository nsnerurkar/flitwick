#include "serialport.h"


/* CONSTRUCTOR
 * Initializes the com port with settings.
 */
serialport::serialport(int port, long baud, const char* mode)
{
	this->nPort = port;
	this->nBaud = baud;
	this->nByteSize = atoi(&mode[0]);

	switch (mode[1])
	{
	case 'N': eParity = NONE; break;
	case 'O': eParity = ODD; break;
	case 'E': eParity = EVEN; break;
	case 'M': eParity = MARK; break;
	case 'S': eParity = SPACE; break;
	default: eParity = NONE;
	}

	switch (mode[2])
	{
	case '1':	eStopBits = ONE; break;
	case '5': eStopBits = ONEFIVE; break;
	case '2': eStopBits = TWO; break;
	default: eStopBits = ONE;
	}
}

/* DEFAULT CONSTRUCTOR
 * This constructor just sends a blank uninitialized port object
 */
serialport::serialport()
{
	// Do nothing in th default constructor
}

/* DESTRUCTOR
 * Closes the port if open before destroying itself
 */
serialport::~serialport()
{
	if (isOpen)
		this->Close();
}

/* OPEN
 * Opens the com port
 * Returns true if success.
 */
bool serialport::Open()
{
	char mode[4];
	itoa(nByteSize,&mode[0],10);

	switch (eParity)
	{
	case NONE: mode[1] = 'N'; break;
	case ODD: mode[1] = 'O'; break;
	case EVEN: mode[1] = 'E'; break;
	case MARK: mode[1] = 'M'; break;
	case SPACE: mode[1] = 'S'; break;
	}

	switch (eStopBits)
	{
	case ONE:	mode[2] = '1'; break;
	case ONEFIVE: mode[2] = '5'; break;
	case TWO: mode[2] = '2'; break;
	}

	mode[3] = 0;

	if (RS232_OpenComport(nPort, nBaud, mode) > 0)
	{	
		isOpen = false;
		return false;
	}
	isOpen = true;
	return true;
}

/* CLOSE
 * Closes the com port.
 */
void serialport::Close()
{
	RS232_CloseComport(nPort);
	isOpen = false;
}

/* READBYTE
 * Takes the char buffer for reading a byte.
 * Returns true if success.
 */
bool serialport::ReadByte(unsigned char& byte)
{
	int rcvdSize = RS232_PollComport(nPort, &byte, 1);
	if (rcvdSize > 0)
		return true;
	return false;
}

/* READBUF
 * Reads from com port to a char buffer.
 * Returns the number of bytes read.
 */
int serialport::ReadBuf(unsigned char* buf, int size)
{
	int rcvdSize = RS232_PollComport(nPort, buf, size);
	return rcvdSize;
}

/* SENDBYTE
 * Write 1 byte to the com port.
 * Returns true if success.
 */
bool serialport::SendByte(unsigned char byte)
{
	int status = RS232_SendByte(nPort, byte);
	return (status == 0) ? true : false;
}

/* SENDBUF
 * Write the contents of a buffer to com port.
 * Returns true if success
 */
bool serialport::SendBuf(unsigned char* buf, int size)
{
	int status = RS232_SendBuf(nPort, buf, size);
	return (status < 0) ? false : true;
}