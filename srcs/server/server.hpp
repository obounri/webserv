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
		std::string index;   // unknown  (default_page) maybe it index?
		int limit_body; // client max body size
		std::vector<std::string> methods; // allowed methods
		// std::string limit_body_s;
		std::map<std::string ,location> locations; // location blocks
		std::string root; // server root
        std::map<int, std::string> error_pages; // server error pages
		std::map<std::string, std::string> cgi; // location cgi
		std::string file_open; // unknown
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

        void            run();

        // Seters

		void                                                        set_port(int &str);
        void                                                        set_host(std::string& str);
        void                                                        set_index(std::string &index);
		void                                                        set_name(std::string& str);
		void                                                        set_limit(int &limit);
		void                                                        set_methods(std::string& str);
		void                                                        set_root(std::string &str);
        void                                                        set_cgi(std::string& name, std::string path);
        std::pair<std::map<int, std::string>::iterator, bool>       set_error_pages(std::pair<int, std::string> &error_page);

        // Geters

        int                                                         get_port() const ;
        std::string                                                 get_host() const ;
        std::string                                                 get_index() const ;
        int                                                         get_limit_body() const ;
        std::string                                                 get_name() const ;
        std::vector<std::string>                                    get_methods() const ;
        std::string                                                 get_root() const ;
        std::map<int, std::string>                                  get_error_pages() const ;


};

// int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
// EV_SET(&kev, ident, filter, flags, fflags, data, udata);

#endif