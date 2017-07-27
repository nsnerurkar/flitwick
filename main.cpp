
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

**************************************************/

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "d10handler.h"

int main(int argc, char* argv[]) 
{
	int com = 3;
	int baud = 9600;
	char mode[4] = { '8', 'N', '1', 0 };

	D10Handler dh(com, baud, mode);

	dh.start();
}

