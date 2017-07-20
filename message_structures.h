#ifndef _MESSAGE_STRUCTURES_H_
#define _MESSAGE_STRUCTURES_H_


struct msgTerminator
{
    unsigned char recType = 'L';
    unsigned char seqNo = '1';
    unsigned char termCode; // N - normal, F - last request, I - no info, E - error, Q - error in last request
};




#endif // _MESSAGE_STRUCTURES_H_

