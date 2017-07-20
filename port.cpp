#include "port.h"
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

port::port()
{
    com_port = 0; // COM1 or devtty0
    baud = 9600;
    mode[0] = '8';
    mode[1] = 'N';
    mode[2] = '1';
    mode[3] = 0;
}

port::port(int cport, int bd, char md[])
{
    com_port = cport;
    baud = bd;
    mode[0] = md[0];
    mode[1] = md[1];
    mode[2] = md[2];
    mode[3] = md[3];
}

port::~port()
{

}


void port::listen()
{
    if (!port_open())
        return;
    while (1)
    {
        int n = RS232_PollComport(com_port, buf, 4095);

        if (n > 0)
        {
            buf[n] = 0;   /* always put a "null" at the end of a string! */

            for (int i = 0; i < n; i++)
            {
                print(buf[i]);
            }           
        }
        
#ifdef _WIN32
        Sleep(1000);
#else
        usleep(1000000);  /* sleep for 100 milliSeconds */
#endif
    }

    port_close();
}

bool port::port_open()
{
    if (RS232_OpenComport(com_port, baud, mode))
    {
        printf("Can not open comport\n");        
        return false;
    }
    return true;
}

void port::port_close()
{
    RS232_CloseComport(com_port);
}


void port::print(unsigned char buf)
{
    switch (buf)
    {
        case 0:
            printf("<NULL>");
            break;
        case 1:
            printf("<SOH>");
            break;
        case 2:
            printf("<STX>");
            break;
        case 3:
            printf("<ETX>");
            break;
        case 4:
            printf("<EOT>");
            break;
        case 5:
            printf("<ENQ>");
            break;
        case 6:
            printf("<ACK>");
            break;
        case 10:
            printf("<LF>");
            break;
        case 13:
            printf("<CR>");
            break;
        case 21:
            printf("<NAK>");
            break;
        case 23:
            printf("<ETB>");
            break;
        default:
            printf("%c", buf);
    }
}

void port::transmit(unsigned char* buf,int size)
{
    if (!port_open())
        return;
    if(!RS232_SendBuf(com_port,buf,size))
        printf("Cannot Transmit\n");
    port_close();
}