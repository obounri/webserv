#include "sockets.hpp"

Socket::Socket(int domain, int type, int interface, int port, int backlog)
{
    std::cout << "constructing socket.." << std::endl;
    if ((sockfd = socket(domain, type, 0)) == -1)
        throw std::runtime_error("failed initialising socket.. quitting..");
    int yes=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        throw std::runtime_error("failed initialising socket.. quitting..");
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = interface;
    if (bind(sockfd, (struct sockaddr *)&address, sizeof address) == -1)
        throw std::runtime_error("failed socket binding.. quitting..");
    if (listen(sockfd, backlog) == -1)
        throw std::runtime_error("failed initialising socket.. quitting..");
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("failed initialising socket.. quitting..");
    std::cout << "socket " << sockfd << " listening.." << std::endl;
}

Socket::~Socket()
{
    std::cout << "destroying socket.." << std::endl;
    close(sockfd);
}