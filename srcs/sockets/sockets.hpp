#ifndef SOCKETS_H
# define SOCKETS_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

class Socket
{
private:
    int sockfd;
    struct sockaddr_in address;

public:
    Socket(int domain, int type, int port, int bklog);
    unsigned int get_socket() { return sockfd; };
    ~Socket();
};

Socket::Socket(int domain, int type, int port, int backlog)
{
    std::cout << "constructing socket.." << std::endl;
    if ((sockfd = socket(domain, type, 0)) == -1) {
        std::cout << "failed initialising socket.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    int yes=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        std::cout << "failed initialising socket.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = 0;
    if (bind(sockfd, (struct sockaddr *)&address, sizeof address) == -1) {
        std::cout << "failed socket binding.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    if (listen(sockfd, backlog) == -1) {
        std::cout << "failed initialising socket.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    std::cout << "socket listening.." << std::endl;
}

Socket::~Socket()
{
    std::cout << "destroying socket.." << std::endl;
    close(sockfd);
}


#endif