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
        
        std::string host; // server host
		int         port; // server port
		std::string name; // server_name
		std::string root; // server root
        std::string index;
		std::string default_page;   // unknown  (default_page) maybe it index?
		std::string limit_body_s; // didn't use it
        int limit_body; // client max body size
		std::list<std::string> methods; // allowed methods
		std::map<std::string ,location> locations; // location blocks
        std::map<std::string, std::string> error_pg; // server error pages
		std::map<std::string, std::string> cgi; // location cgi
		// std::string file_open; // unknown
		// std::map<std::string ,std::string> extra_headers;  also unknown
        // headers header_var; also unknown

        std::vector<Socket *>   listeners;
        std::vector<client>     clients;
        int                     keq;

        void    accept_new_connection(unsigned long int fd);
        void    destroy_connection(int fd, int event);
        void    handle_request(client *c);
        void    send_request(client *c);

        int     is_listener(unsigned long int fd);

    public:
        Server();
        // Server(config data);
        ~Server();

        void    run();

        // Seters

		void                                                                set_port(int    &port);
        void                                                                set_host(std::string& str);
        void                                                                set_index(std::string &index);
		void                                                                set_name(std::string &str);
		void                                                                set_limit(int &limit);
		void                                                                set_methods(std::string &str);
		void                                                                set_root(std::string &str);
        void                                                                set_cgi(std::string &name, std::string &path);
        std::pair<std::map<std::string, std::string>::iterator, bool>       set_error_page(std::pair<std::string, std::string> &error_page);

        // Geters

        int                                                                 get_port() const ;
        std::string                                                         get_host() const ;
        std::string                                                         get_index() const ;
        int                                                                 get_limit() const ;
        std::string                                                         get_name() const ;
        std::list<std::string>                                              get_methods() const ;
        std::string                                                         get_root() const ;
        std::map<std::string, std::string>                                  get_error_pages() const ;
        int                                                                 get_limit() const ;
        int                                                                 value_port() const ;
		int                                                                 get_pg_error() const ;


};

// int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
// EV_SET(&kev, ident, filter, flags, fflags, data, udata);

#endif