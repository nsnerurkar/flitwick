#ifndef	_LAYER1_H_
#define _LAYER1_H_

#include <string>



#define STX_CHAR 2
#define ETX_CHAR 3
#define EOT_CHAR 4
#define ENQ_CHAR 5
#define ACK_CHAR 6
#define NAK_CHAR 21
#define CR_CHAR  13
#define LF_CHAR  10


class layer1
{
public:
	void calcCheckSum(const std::string& buf, unsigned char* high, unsigned char* low);
	bool verifyCheckSum(const std::string&);

	std::string wrapLayer2Msg(const std::string& layer2msg);
	std::string extractLayer2Msg(const std::string& layer1msg);

	bool verifyMsgIntegrity(const std::string& layer1msg);

	void resetMsgNum() { msgNum = 1; }
	void incrementMsgNum() { msgNum = (msgNum == 7) ? 0 : ++msgNum; }
private:
	unsigned char msgNum;
};






#endif // _LAYER1_H_