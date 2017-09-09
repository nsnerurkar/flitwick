#include "nqueue.h"


int nqueue::getQSize()
{
    std::lock_guard<std::mutex> locker(_mu);
    return fifo.size();
}

void nqueue::push(unsigned char toPush)
{
    std::lock_guard<std::mutex> locker(_mu);
    fifo.push_back(toPush);    
}

unsigned char nqueue::pop()
{
    std::lock_guard<std::mutex> locker(_mu);
    unsigned char front = fifo.front();
    fifo.pop_front();
    return front;
}

bool nqueue::isEmpty()
{
    std::lock_guard<std::mutex> locker(_mu);
    return (fifo.size() == 0);    
}

void nqueue::clear()
{
    std::lock_guard<std::mutex> locker(_mu);
    fifo.clear();    
}