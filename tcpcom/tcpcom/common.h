#ifndef _COMMON_H_
#define _COMMON_H_


#ifdef _WIN32
	#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501  /* Windows XP. */
	#endif

	#pragma warning(disable:4996)

	#include <winsock2.h>
	#include <Ws2tcpip.h>

#else
	/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
	#include <unistd.h> /* Needed for close() */

	typedef int SOCKET;
#endif

#include <deque>
#include <vector>
#include <string>


#endif //_COMMON_H_
