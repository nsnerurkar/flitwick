#include "message_structures.h"


void calcCheckSum(unsigned char* buf, int size, unsigned char* high, unsigned char* low)
{
    int checksum = 0;
    int i;
    for (i = 0;i <= size; ++i)
    {
        checksum += buf[i];
    }


}



int makeHostTerminationMessage(unsigned char* buf, int recNum)
{
    int size = 13;
    struct msgTerminator msg;
    msg.termCode = 'N';
    
    char convBuf[10];

    buf[0] = 2;
    sprintf(buf[1], "%d", recNum);
    buf[2] = msg.recType;
    buf[3] = '|';
    buf[4] = msg.seqNo;
    buf[5] = '|';
    buf[6] = msg.termCode;
    buf[7] = 13;
    buf[8] = 3;
    calcCheckSum(buf, size - 4, buf[9], buf[10]);
    buf[11] = 13;
    buf[12] = 10;

    return(size);
}


