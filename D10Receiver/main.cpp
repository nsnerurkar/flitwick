#include <iostream>
#include "d10handler.h"
#include <thread>


int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    d10handler d10H(10,9600,"8N1");
       
    while(!d10H.isValid())
    {
        std::cout << "Init failed..." << std::endl;
		Sleep(2000);
		d10H.init();
        std::cout << "Initializing" << std::endl;        
    }
    std::cout << "Initialized..." << std::endl;

	d10H.Start();

    return 0;
}
