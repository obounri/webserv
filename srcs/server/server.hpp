#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include "../parsers/parse.hpp"
#include <exception>
#include <strings.h>
#include <stdlib.h>
#include "../client/client.hpp"
#include <sys/types.h>
// #include <sys/uio.h>
// #include <dirent.h>
// #include <sys/wait.h>
// #include <fstream>

#if 1
    #include <sys/event.h>
#else
    #include "/usr/include/kqueue/sys/event.h"
#endif

class MainServer
{
    private:
        // std::vector<Socket *>   listeners; // rm
        std::vector<client>     clients;
        int                     keq;
        // std::vector<v_server>   g_ret; is now myvs for testing

        void    accept_new_connection(unsigned long int fd);
        void    destroy_connection(int fd, int event);
        void    recv_request(client *c);
        void    handle_request(v_server &s, client *c, std::multimap<std::string, v_server> extra);
        void    send_request(client *c);

        int         is_listener(unsigned long int fd);
        v_server&   get_client_server(unsigned long int fd);
    public:
        std::vector<v_server>   myvs; // init to test
        MainServer();
        void init_server();
        ~MainServer();

        void            run();
};

#endif