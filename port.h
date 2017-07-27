#ifndef _PORT_H_
#define _PORT_H_

#include <queue>

class port
{
public:
    port();
    port(int cport, int baud, char mode[]);

	void initPort(int cport, int baud, char mode[]);
    ~port();

    void print(unsigned char buf);
    bool port_open();
    void port_close();

    int receive();
    int transmit();

	void clearRxBuf();
	void clearTxBuf();
	
	std::queue<unsigned char> rxbuf;
	std::queue<unsigned char> txbuf;

private:
    int com_port;
    int baud;
    char mode[4];
};






#endif // ! _PORT_H_

