#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <iostream>
#include "/usr/include/kqueue/sys/event.h"

#define DUMMY_HTTP_RESPONSE "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"

// struct config
// {
//     // parsed shit
//     int listen_port;
// };

class Server
{
    private:
        /* data */
        // config  _config;
        Socket * listener; // listener socket
        // void    poll_connections();
        // void    accept_new_connection();
        // void    handle_request();
        // void    receiver();
        // void    sender();
    public:
        Server(int domain, int type, int port, int backlog);
        ~Server();

        void            run();
};

Server::Server(int domain, int type, int port, int backlog)
{
    std::cout << "Constructing server.." << std::endl;
    listener = new Socket(domain, type, port, backlog);
}

Server::~Server()
{
    std::cout << "server destructor called" << std::endl;
    delete listener;
}

void Server::run() {
    // struct sockaddr_in their_addr;
    // socklen_t   addr_size;
    // std::string message(DUMMY_HTTP_RESPONSE);
    // fd_set write_pool;
    // addr_size = sizeof their_addr;

    int kq = kqueue();
    struct kevent evSet;
    EV_SET(&evSet, listener->get_socket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &evSet, 1, NULL, 0, NULL);
    struct kevent evList[32];

    std::cout << "server up and running.." << std::endl;
    int newfd;
    int rec;
    int sent;
    while (1) {
        int num_events = kevent(kq, NULL, 0, evList, 32, NULL);
        for (int i = 0; i < num_events; i++)
        {
            if (evList[i].ident == listener->get_socket()) {
                newfd = accept(listener->get_socket(), NULL, NULL);
                EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                std::cout << "got connection request from fd = " << newfd << ".." << std::endl;
                // std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
                // std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
            }
            else if (evList[i].flags & EV_EOF) {
                int fd = evList[i].ident;
                std::cout << "client " << fd << " disconnected.\n";
                EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                close(evList[i].ident);
            }
            else if (evList[i].filter == EVFILT_READ) {
                std::cout << ">> handling request here" << std::endl;
                char *buffer = new char[30000];
                if ((rec = recv(evList[i].ident, buffer, 30000, 0)) != -1) {
                    std::cout << "Received message of len " << rec << " content:" << std::endl;
                    std::cout << buffer << std::endl << std::endl;
                }
                else
                    std::cout << "Reading failed" << std::endl << std::endl;
                delete buffer;
                if ((sent = send(evList[i].ident, DUMMY_HTTP_RESPONSE, sizeof DUMMY_HTTP_RESPONSE, 0)) != -1) {
                    std::cout << "Message sent = " << sent << std::endl;
                }
                else
                    std::cout << "Sending failed" << std::endl;
            }
        }
    }
}

#endif