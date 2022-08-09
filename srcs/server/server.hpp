#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <iostream>
#include "/usr/include/kqueue/sys/event.h"

#define MAX_REQUEST_SIZE 1024
#define DUMMY_HTTP_RESPONSE "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"

class Server
{
    private:
        /* data */
        Socket * listener; // listener socket(s)
        int      keq;
        // void    poll_connections();
        void    accept_new_connection();
        void    destroy_connection(int fd);
        void    handle_request(int fd);
        // void    receiver();
        // void    sender();
    public:
        Server(int domain, int type, int port, int backlog);
        ~Server();

        void            run();
};

Server::Server(int domain, int type, int port, int backlog)
{
    struct kevent evSet;

    std::cout << "constructing server.." << std::endl;
    listener = new Socket(domain, type, port, backlog);
    keq = kqueue();
    EV_SET(&evSet, listener->get_socket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
}

Server::~Server()
{
    std::cout << "server destructor called.." << std::endl;
    delete listener;
}

void    Server::accept_new_connection() 
{
    struct kevent evSet;
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    addr_size = sizeof their_addr;

    newfd = accept(listener->get_socket(), (sockaddr *)&their_addr, &addr_size);
    EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    std::cout << "got connection request from fd = " << newfd << ".." << std::endl;
    std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
    std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
}

void    Server::destroy_connection(int fd)
{
    struct kevent evSet;

    std::cout << "client " << fd << " disconnected.." << std::endl;
    EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    close(fd);
}

void    Server::handle_request(int fd)
{
    int rec, sent;

    char *buffer = new char[MAX_REQUEST_SIZE];
    if ((rec = recv(fd, buffer, MAX_REQUEST_SIZE, 0)) != -1) {
        std::cout << "received message of len " << rec << " content:" << std::endl;
        std::cout << buffer << std::endl << std::endl;
    }
    else
        std::cout << "reading failed.." << std::endl << std::endl;
    delete buffer;
    if ((sent = send(fd, DUMMY_HTTP_RESPONSE, sizeof DUMMY_HTTP_RESPONSE, 0)) != -1) {
        std::cout << "message sent = " << sent << std::endl;
    }
    else
        std::cout << "sending failed.." << std::endl;
}

void Server::run() {
    struct kevent evList[32];

    std::cout << "server up and running.." << std::endl;
    for(;;) {
        int num_events = kevent(keq, NULL, 0, evList, 32, NULL);
        for (int i = 0; i < num_events; i++)
        {
            if (evList[i].ident == listener->get_socket())
                accept_new_connection();
            else if (evList[i].flags & EV_EOF)
                destroy_connection(evList[i].ident);
            else if (evList[i].filter == EVFILT_READ)
                handle_request(evList[i].ident);
        }
    }
}

#endif