#include "layer1.h"
#include <stdio.h>

void layer1::calcCheckSum(const std::string& buf, unsigned char* high, unsigned char* low)
{
	int checksum = 0;
	int i;
	char strChkSum[10];
	for (i = buf.size() - 1; i > 0; --i)
	{
		checksum += (unsigned char)buf[i];
	}

	checksum %= 256;

	sprintf(strChkSum, "%.2X", checksum);
	*high = strChkSum[0];
	*low = strChkSum[1];
}


bool layer1::verifyCheckSum(const std::string& msgStr)
{
	// The string is saw. Remove the characters not needed for checksum before passing.
	unsigned char aHighCS, aLowCS, eHighCS, eLowCS;
	aHighCS = msgStr[msgStr.size() - 4];
	aLowCS = msgStr[msgStr.size() - 3];

	calcCheckSum(msgStr.substr(1, msgStr.size() - msgStr.find(ETX_CHAR)), &eHighCS, &eLowCS);

	if (eHighCS != aHighCS || eLowCS != aLowCS)
		return false;
	else
		return true;
}

std::string layer1::wrapLayer2Msg(const std::string& layer2msg)
{
	std::string retStr(" ");
	unsigned char chkH, chkL;
	char buf[5];

	retStr[0] = STX_CHAR;
	retStr += std::to_string(msgNum);
	retStr += layer2msg + (char)ETX_CHAR;
	calcCheckSum(retStr, &chkH, &chkL);
	buf[0] = chkH;
	buf[1] = chkL;
	buf[2] = CR_CHAR;
	buf[3] = LF_CHAR;
	buf[4] = 0;

	retStr += buf;
	incrementMsgNum();
	return(retStr);
}

std::string layer1::extractLayer2Msg(const std::string& layer1msg)
{
	std::string retStr = layer1msg.substr(1, layer1msg.size() - layer1msg.find(ETX_CHAR));
	return(retStr);
}

bool layer1::verifyMsgIntegrity(const std::string& layer1msg)
{
	if ((unsigned char)layer1msg[0] != STX_CHAR || layer1msg.find((char)ETX_CHAR) == std::string::npos)
		return false;
	return true;	
}