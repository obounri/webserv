#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include "../parsers/parse.hpp"
#include <exception>
#include <strings.h>
#include <stdlib.h>
#include "../client/client.hpp"

#if 0
    #include <sys/event.h>
#else
    #include "/usr/include/kqueue/sys/event.h"
#endif

// typedef struct s_client
// {
//     int         fd;

//     std::string client_request;
//     // int         rec;

//     // Respone res;
//     std::string client_response;

//     s_client(int _fd, std::string _ip, int _port):fd(_fd) {} ;
// } client;

class MainServer
{
    private:
        std::vector<Socket *>   listeners;
        std::vector<client>     clients;
        int                     keq;

        void    accept_new_connection(unsigned long int fd);
        void    destroy_connection(int fd, int event);
        void    recv_request(client *c);
        void    send_request(client *c);

        int     is_listener(unsigned long int fd);
    public:
        MainServer(config data);
        ~MainServer();

        void            run();
};

#endif