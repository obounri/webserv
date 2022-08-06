#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <iostream>

struct config
{
    // parsed shit
    int listen_port;
};


class Server
{
private:
    /* data */
    config  _config;
    Socket * listener; // listener socket
    // int     accept_connections();
    // void    receiver();
    // void    sender();
public:
    Server(int domain, int type, int port, int backlog);
    ~Server();

    void    run();
    void    shutdown() {
        // close socket
    }
};

Server::Server(int domain, int type, int port, int backlog)
{
    listener = new Socket(domain, type, port, backlog);
}

void Server::run() {
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;

    addr_size = sizeof their_addr;
    int rec;
    // int sent;
    while (1) {
        char *buffer = new char[30000];
        newfd = accept(listener->get_socket(), (struct sockaddr *)&their_addr, &addr_size);
        std::cout << "got connection request from fd = " << newfd << std::endl;
        std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
        std::cout << ntohs(their_addr.sin_port) << std::endl;
        if ((rec = recv(newfd, buffer, 30000, 0)) != -1) {
            std::cout << "Received message of len " << rec << " content:" << std::endl;
            std::cout << buffer << std::endl;
        }
        else
            std::cout << "Reading failed" << std::endl;
        // if ((sent = send(newfd, DUMMY, sizeof DUMMY, 0)) != -1) {
        //     std::cout << "Message sent = " << sent << std::endl;
        // }
        // else
        //     std::cout << "Sending failed" << std::endl;
        close(newfd);
        delete buffer;
    }
}

Server::~Server()
{
}


#endif