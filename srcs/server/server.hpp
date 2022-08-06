#ifndef SERVER_HPP
# define SERVER_HPP

struct config
{
    // parsed shit
    int listen_port;
};


class server
{
private:
    /* data */
    config  _config;
    int     socket; // listener socket
public:
    server(/* args */);
    ~server();

    int    run() {
        // socket()
        // bind
        // listen
        // return socket
    }

    void    shutdown() {
        // close socket
    }
};

server::server(/* args */)
{
}

server::~server()
{
}


#endif