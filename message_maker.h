#ifndef _MESSAGE_MAKER_H_
#define _MESSAGE_MAKER_H_
#include "message_structures.h"
#include <stdio.h>

void calcCheckSum(unsigned char* buf, int size, unsigned char* high, unsigned char* low)
{
    int checksum = 0;
    int i;
	char strChkSum[10];
	for (i = size-1;i > 0; --i)
    {
        checksum += buf[i];
    }

	checksum %= 256;

	sprintf(strChkSum, "%.2X", checksum);
	*high = strChkSum[0];
	*low = strChkSum[1];
}



int makeTerminationMessage(unsigned char* buf, int recNum,char termCode = 'N')
{
    int size = 13;
    struct msgTerminator msg;
	msg.termCode = termCode;
    
    char convBuf[10];

    buf[0] = 2;
    sprintf((char*)&buf[1], "%d", recNum);
    buf[2] = msg.recType;
    buf[3] = '|';
    buf[4] = msg.seqNo;
    buf[5] = '|';
    buf[6] = msg.termCode;
    buf[7] = 13;
    buf[8] = 3;
    calcCheckSum(buf, size - 4, &buf[9], &buf[10]);
    buf[11] = 13;
    buf[12] = 10;

    return(size);
}




#endif //_MESSAGE_MAKER_H_