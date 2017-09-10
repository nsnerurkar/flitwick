#include "quicksock.h"
#include <stdio.h>




/* INITSOCK
 * Cross-platform function used to initialize sockets
 * returns int value of WSAStartup. 0 if success, !0 if fail
 */
int initSock()
{
#ifdef _WIN32
	// Windows winsock needs WSA Startup
	WSADATA wsa;
	return(WSAStartup(MAKEWORD(2, 2), &wsa));
#else
	// Linux always starts up
	return 0; 
#endif
}

/* CLOSESOCK
 * Cross-platform function used to close and shutdown sockets
 * returns int value which is status of socketclose
 */
int closeSock(SOCKET sock)
{
	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0) { status = closesocket(sock); }
	WSACleanup();
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
#endif
	return status;
}

/* CREATESOCK
 * Function to create a socket in both linux and windows.
 * Args - sock - SOCKET variable by reference
 * Returns boolean for success.
 */
bool createSock(SOCKET& sock)
{
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

/* CONNECTSOCK
 * Function to connect the socket to an address.
 * Args - Socket -  pass by reference
 * port - port of the server to connect
 * addr - address of server to connect
 * returns a boolean indicating success
 */
bool connectSock(const SOCKET& sock, int port, char* addr)
{
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		return false;
	return true;
}

/* BINDSOCK
* Function to bind socket for listening to connections
* Args - Socket -  pass by reference
* port - port to listen on
* returns a boolean indicating success
*/
bool bindSock(const SOCKET& sock, int port)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		return false;
	return true;
}

/* RECVSOCK
 * Function to receive data from the socket connection
 * Args - sock - Socket to recieve on
 * Args - buf - buffer to store the received message in
 * Args - recvSize - Size of the received data
 * Args - flags - Flags to pass to receive
 * Returns boolean for success.
 */
bool recvSock(const SOCKET& sock, void* buf, int& recvSize, int flags)
{
	memset(buf, 0, recvSize);
	
	if ((recvSize = recv(sock, buf, recvSize, flags)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

/* SENDSOCK
* Function to send data to the socket connection
* Args - sock - Socket to send on
* Args - buf - buffer to send
* Args - bufSize - Size of buffer
* Args - flags - Flags to pass to send
* Returns boolean for success.
*/
bool sendSock(const SOCKET& sock, const void* buf, int bufSize, int flags)
{
	int bytesSent;
	while(bufSize>0)
	{
		bytesSent = send(sock, buf, bufSize, flags);
		if (bytesSent < 0) return false;
		buf += bytesSent;
		bufSize -= bytesSent;
	}
	return true;
}