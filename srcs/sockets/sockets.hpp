#ifndef SOCKETS_H
# define SOCKETS_H

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class Socket
{
private:
    unsigned long int sockfd;
    struct sockaddr_in address;

public:
    Socket();
    Socket(int domain, int type, int interface, int port, int backlog);
    unsigned long int get_socket() { return sockfd; };
    ~Socket();
};

// Socket::Socket() {
    
// }


#endif