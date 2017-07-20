
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


int main()
{
    char mode[4] = { '8','N','1',0 };
    port rx(2, 9600, mode);

    port tx(3, 9600, mode);


    unsigned char buf[] = {2,'1','|','\\',3};

    tx.transmit(buf, 5);


    rx.listen();

    return(0);
}

