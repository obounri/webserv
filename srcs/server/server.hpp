#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include "../parsers/parse.hpp"
#if 1
    #include <sys/event.h>
#else
    #include "/usr/include/kqueue/sys/event.h"
#endif
    

#define MAX_REQUEST_SIZE 1024
#define DUMMY_HTTP_RESPONSE "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"

typedef struct s_client
{
    int         fd;
    int         port;
    std::string ip;

    s_client(int _fd, std::string _ip, int _port):fd(_fd), port(_port), ip(_ip) {} ;
} client;

class Server
{
    private:
        /* data */
        std::vector<Socket *>   listeners;
        std::vector<client>     clients;
        int                     keq;

        void    accept_new_connection(unsigned long int fd);
        void    destroy_connection(int fd, int event);
        void    handle_request(client *c);
        void    send_request(client *c);

        int     is_listener(unsigned long int fd);
    public:
        Server(config data);
        ~Server();

        void            run();
};

// int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
// EV_SET(&kev, ident, filter, flags, fflags, data, udata);

#endif