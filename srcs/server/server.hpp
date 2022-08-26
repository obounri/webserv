#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include "../parsers/parse.hpp"
#include <exception>

#if 1
    #include <sys/event.h>
#else
    #include "/usr/include/kqueue/sys/event.h"
#endif
    

#define MAX_IO_SIZE 1024

typedef struct s_client
{
    int         fd;
    int         port;
    std::string ip;

    char        recBuffer[MAX_IO_SIZE];
    std::string header;
    std::string req;
    // int         rec;
    size_t      body_len;

    // Respone res;
    size_t         sent;
    std::string sendBuffer;

    s_client(int _fd, std::string _ip, int _port):fd(_fd), port(_port), ip(_ip) {} ;
} client;

class Server
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
        Server(config data);
        ~Server();

        void            run();
};

#endif