#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <iostream>


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

    std::cout << "server up and running.." << std::endl;
    fd_set read_pool, tmp_read;
    int biggest_fd = listener->get_socket();
    int newfd;
    
    int rec;
    int sent;
    FD_ZERO(&read_pool);
    FD_SET(listener->get_socket(), &read_pool);
    while (1) {
        tmp_read = read_pool;
        if (select(biggest_fd + 1, &tmp_read, NULL, NULL, NULL) == -1) {
            std::cout << "Runtime failure.. quitting.." << std::endl;
            _exit(EXIT_FAILURE);
        }
        for (int i = 0; i < (biggest_fd + 1); i++)
        {
            if (FD_ISSET(i, &tmp_read) != 0) {
                if (i == listener->get_socket()) {
                    newfd = accept(listener->get_socket(), NULL, NULL);
                    FD_SET(newfd, &read_pool);
                    if (newfd > biggest_fd)
                        biggest_fd = newfd;
                    std::cout << "got connection request from fd = " << newfd << ".." << std::endl;
                    // std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
                    // std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
                }
                else {
                    std::cout << ">> handling request here" << std::endl;
                    char *buffer = new char[30000];
                    if ((rec = recv(i, buffer, 30000, 0)) != -1) {
                        std::cout << "Received message of len " << rec << " content:" << std::endl;
                        std::cout << buffer << std::endl << std::endl;
                    }
                    else
                        std::cout << "Reading failed" << std::endl << std::endl;
                    delete buffer;
                    if ((sent = send(i, DUMMY_HTTP_RESPONSE, sizeof DUMMY_HTTP_RESPONSE, 0)) != -1) {
                        std::cout << "Message sent = " << sent << std::endl;
                    }
                    else
                        std::cout << "Sending failed" << std::endl;
                    close(i);
                    FD_CLR(i, &read_pool);
                }
            }
        }
    }
}

#endif