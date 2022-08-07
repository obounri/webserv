#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <iostream>


#define DUMMY "HTTP/1.1 200 OK \
Date: Mon, 23 May 2005 22:38:34 GMT \
Content-Type: text/html; charset=UTF-8 \
Content-Length: 51 \
Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT \
ETag: \"3f80f-1b6-3e1cb03b\" \
Accept-Ranges: bytes \
\
Hello World! My payload includes a trailing CRLF.\
\r"

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

    void            run();
    static void     shutdown(int signal);
};

Server::Server(int domain, int type, int port, int backlog)
{
    listener = new Socket(domain, type, port, backlog);
}

// void Server::run() {
//     int newfd;
//     struct sockaddr_in their_addr;
//     socklen_t   addr_size;
//     fd_set read_pool;
//     fd_set write_pool;

//     addr_size = sizeof their_addr;
//     signal(SIGINT, Server::shutdown);
//     int rec;
//     int sent;
//     while (1) {
//         char *buffer = new char[30000];
//         newfd = accept(listener->get_socket(), (struct sockaddr *)&their_addr, &addr_size);
//         std::cout << "got connection request from fd = " << newfd << std::endl;
//         std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
//         std::cout << ntohs(their_addr.sin_port) << std::endl;
//         if ((rec = recv(newfd, buffer, 30000, 0)) != -1) {
//             std::cout << "Received message of len " << rec << " content:" << std::endl;
//             std::cout << buffer << std::endl;
//         }
//         else
//             std::cout << "Reading failed" << std::endl;
//         if ((sent = send(newfd, DUMMY, sizeof DUMMY, 0)) != -1) {
//             std::cout << "Message sent = " << sent << std::endl;
//         }
//         else
//             std::cout << "Sending failed" << std::endl;
//         close(newfd);
//         delete buffer;
//     }
// }

void Server::shutdown(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received SIGINT.." << std::endl;
        std::cout << "Shutting server down.." << std::endl;
        _exit(0);
    }
}

Server::~Server()
{
    delete listener;
}

void Server::run() {
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    fd_set read_pool, tmp_read;
    // fd_set write_pool;
    int biggest_fd = listener->get_socket();

    addr_size = sizeof their_addr;
    signal(SIGINT, Server::shutdown);
    int rec;
    int sent;
    FD_ZERO(&read_pool);
    FD_SET(listener->get_socket(), &read_pool);
    while (1) {
        tmp_read = read_pool;
        if ((sent = select(biggest_fd + 1, &tmp_read, NULL, NULL, NULL)) == -1) {
            std::cout << "Runtime failure.. quitting.." << std::endl;
            _exit(EXIT_FAILURE);
        }
        else
            std::cout << "select return " << sent << std::endl;
        for (int i = 0; i < (biggest_fd + 1); i++)
        {
            if (FD_ISSET(i, &tmp_read) != 0) {
                std::cout << i << " is set in read_pool" << std::endl; 
                if (i == listener->get_socket()) {
                    newfd = accept(listener->get_socket(), (struct sockaddr *)&their_addr, &addr_size);
                    FD_SET(newfd, &read_pool);
                    if (newfd > biggest_fd)
                        biggest_fd = newfd;
                    std::cout << "got connection request from fd = " << newfd << std::endl;
                    std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
                    std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
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
                    close(i);
                    FD_CLR(i, &read_pool);
                    delete buffer;
                }
            }
        }
        
        // char *buffer = new char[30000];
        // newfd = accept(listener->get_socket(), (struct sockaddr *)&their_addr, &addr_size);
        // std::cout << "got connection request from fd = " << newfd << std::endl;
        // std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
        // std::cout << ntohs(their_addr.sin_port) << std::endl;
        // if ((rec = recv(newfd, buffer, 30000, 0)) != -1) {
        //     std::cout << "Received message of len " << rec << " content:" << std::endl;
        //     std::cout << buffer << std::endl;
        // }
        // else
        //     std::cout << "Reading failed" << std::endl;
        // if ((sent = send(newfd, DUMMY, sizeof DUMMY, 0)) != -1) {
        //     std::cout << "Message sent = " << sent << std::endl;
        // }
        // else
        //     std::cout << "Sending failed" << std::endl;
        // close(newfd);
        // delete buffer;
    }
}

#endif