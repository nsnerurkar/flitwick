#pragma once
#ifndef _QUICKSOCK_H_
#define _QUICKSOCK_H_
#include "common.h"
 

#define MAX_BUF_LEN 2048



#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------
FUNCTION PROTOTYPES
----------------------------------------------*/
int initSock();							// Socket Initialization
int closeSock(SOCKET sock);				// Socket Close
bool createSock(SOCKET& sock);			// Create a Socket
bool connectSock(const SOCKET& sock, int port, char* addr);
bool bindSock(const SOCKET& sock, int port);
bool recvSock(const SOCKET& sock, char* server_reply, int& recvSize, int flags = 0);
bool sendSock(const SOCKET& sock, const char* buf, int bufSize, int flags = 0);

#ifdef __cplusplus
}
#endif


#endif //_QUICKSOCK_H_