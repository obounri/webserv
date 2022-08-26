#ifndef SERVER_HPP
# define SERVER_HPP

#include "../sockets/sockets.hpp"
#include <list>
#include "../parsers/parse_config.hpp"

#if 1
    #include <sys/event.h>
#else
    #include "/usr/include/kqueue/sys/event.h"
#endif
    

#define MAX_RECV_SIZE 1024
#define DUMMY_HTTP_RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!\r\n\r\n"

typedef struct s_client
{
    int         fd;
    int         port;
    std::string ip;

    char        recBuffer[MAX_RECV_SIZE];
    std::string header;
    std::string req;
    int         rec;
    size_t      body_len;
    // std::string send;

    s_client(int _fd, std::string _ip, int _port):fd(_fd), port(_port), ip(_ip) {} ;
} client;

class Server
{
    private:
        /* data */
        
        std::string host;
		std::string port;
		std::string name;
		int limit_body;
		std::string limit_body_s;
		std::list<std::string> methods;
		std::map<std::string ,location> locations;
		std::string root;
        std::map<std::string, std::string> error_pg;
		std::map<std::string ,std::string> extra_headers;
		std::map<std::string, std::string> cgi;
		std::string default_page;
		std::string file_open;
        // headers header_var;

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

        // Seters

        void set_host(std::string str);
		void set_port(std::string str);
		void set_name(std::string str);
		void set_limit(std::string str);
		void set_methods(std::string str);
        void set_cgi(std::string name, std::string path);
};

// int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
// EV_SET(&kev, ident, filter, flags, fflags, data, udata);

#endif