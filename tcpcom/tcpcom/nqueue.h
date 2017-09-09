#ifndef _NQUEUE_H_
#define _NQUEUE_H_

#include <deque>
#include <mutex>

class nqueue
{
public:
    int getQSize();
    unsigned char pop();
    void push(unsigned char);
    bool isEmpty();

    void clear();

private:
    std::deque<unsigned char> fifo;
    std::mutex _mu;
};

#endif //_NQUEUE_H_