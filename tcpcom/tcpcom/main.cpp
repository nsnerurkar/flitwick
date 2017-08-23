#include <iostream>
#include "tcphandler.h"
#include "d10lis.h"

int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    tcphandler* tcp = new tcphandler("10.10.10.60",8080);
    d10lis* d10FSM = new d10lis();    
    while(!tcp->isValid)
    {
        std::cout << "Init failed..." << std::endl;
        usleep(100000);
        std::cout << "Initializing" << std::endl;
    }
    std::cout << "Initialized..." << std::endl;
    while(true)
    {
        if(tcp->receive() && tcp->getLastRecvdChar() == EOT_CHAR )
        {
            std::cout << "Processing..." << std::endl;
            // message was received and had EOT or ETX
            char buf[2048];
            int recv_size = tcp->readRxBuf(buf);
            // Parse Message
            int messageType = d10FSM->parseMessage(buf,recv_size);
            if(messageType == INVALID_D10_MESSAGE) 
            {
                tcp->transmitNAK();
                d10FSM->forgetLastFrame();
            }
            else
            {
                tcp->transmitACK();
                d10FSM->processData();
            }
        }
        //usleep(100000);
    }
    return 0;
}
