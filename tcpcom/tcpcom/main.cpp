#include<stdio.h>
#include "quicksock.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char *argv[])
{
	SOCKET s;
	struct sockaddr_in server;
	char server_reply[MAX_BUF_LEN];
	int recv_size = 0;

	printf("\nInitialising Winsock...");
	if ( initSock() != 0)
	{
		printf("Failed");
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if (!createSock(s))
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return 1;
	}

	printf("Socket created.\n");

	//Connect to remote server
	if (!connectSock(s,8080,"127.0.0.1"))
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	
	while (1)
	{
		if (recvSock(s, server_reply, recv_size))
		{
			puts(server_reply);
			sendSock(s, server_reply, recv_size);
		}
	}

	return 0;
}