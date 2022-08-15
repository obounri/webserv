#include "sockets.hpp"

Socket::Socket(int domain, int type, int interface, int port, int backlog)
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
    address.sin_addr.s_addr = interface;
    if (bind(sockfd, (struct sockaddr *)&address, sizeof address) == -1) {
        std::cout << "failed socket binding.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    if (listen(sockfd, backlog) == -1) {
        std::cout << "failed initialising socket.. quitting.." << std::endl;
        _exit(EXIT_FAILURE);
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    std::cout << "socket " << sockfd << " listening.." << std::endl;
}

Socket::~Socket()
{
    std::cout << "destroying socket.." << std::endl;
    close(sockfd);
}