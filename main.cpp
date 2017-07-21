
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

#include "d10lis.h"
#include<iostream>

int main()
{
    d10Message* pMsg;
    pMsg = new d10TerminationMessage();
    pMsg->parseMessage("L|23|K");

    delete(pMsg);

    pMsg = new d10MessageHeader();
    pMsg->parseMessage("H|\\^&|||LIS^01^5.0|||||||||20030530121314");

    delete(pMsg);

    pMsg = new d10RequestInfoMessage();
    pMsg->parseMessage("Q|1|^12345||^^^4");

    delete(pMsg);

    pMsg = new d10PatientRecordMessage();
    pMsg->parseMessage("P|1");

    std::cout << pMsg->getMessage();

    delete(pMsg);

    pMsg = new d10TestOrderRecordMessage();
    pMsg->parseMessage("O|1|123456|123456-02-032-20031209-07|^^^4|||||||||||||||||||||||F");

    std::cout << pMsg->getMessage();

    delete(pMsg);

    pMsg = new d10ResultRecordMessage();
    pMsg->parseMessage("R|9|^^^A0^AREA|88.8|||||||||20031209132023");

    std::cout << pMsg->getMessage();

    return(0);
}

