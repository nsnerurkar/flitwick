
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

#include "port.h"

void calcCheckSum(unsigned char* buf, int size, unsigned char* high, unsigned char* low);
int makeTerminationMessage(unsigned char* buf, int recNum, char termCode = 'N');

int main()
{
    char mode[4] = { '8','N','1',0 };
	int size;
    port rx(10, 9600, mode);

    port tx(12, 9600, mode);


	unsigned char buf[10240];
	size = makeTerminationMessage(buf, 1, 'F');

    tx.transmit(buf, size);


    rx.listen();

    return(0);
}

