#include <iostream>
#include "d10handler.h"
#include <thread>


int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    d10handler d10H(13,9600,"8N1");
       
    while(!d10H.isValid)
    {
        std::cout << "Init failed..." << std::endl;
        usleep(100000);
        std::cout << "Initializing" << std::endl;        
    }
    std::cout << "Initialized..." << std::endl;
    
    std::thread txThread(&d10handler::transmit,std::ref(d10H));
    std::thread rxThread(&d10handler::receive,std::ref(d10H));
    std::thread procThread(&d10handler::process,std::ref(d10H));

    char ch = 0;
    while(ch != 'q')
    {
        std::cin>>ch;
    }

    d10H.stop = true;

    rxThread.join();
    txThread.join();
    procThread.join();

    return 0;
}
