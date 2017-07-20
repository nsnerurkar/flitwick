#ifndef _PORT_H_
#define _PORT_H_


class port
{
public:
    port();
    port(int cport, int baud, char mode[]);

    ~port();

    void print(unsigned char buf);
    bool port_open();
    void port_close();
    void listen();

    void transmit(unsigned char* buf,int size);

private:
    int com_port;
    int baud;
    char mode[4];

    unsigned char buf[4096];
};






#endif // ! _PORT_H_

