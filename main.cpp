#include <iostream>
#include <signal.h>

#include "srcs/server/server.hpp"

#define MYPORT 5000
#define V_SERVERS 3

Server * server;

void shutdown(int signal) {
    if (signal == SIGINT) {
        std::cout << " received SIGINT.." << std::endl;
        std::cout << "shutting server down.." << std::endl;
        delete server;
        _exit(0);
    }
}

int main() {

    // if (ac == 2) {
            // handle signals
            // parse config file
            // init server
            // wait for connection
        config data;

        // check if recv/send return is <= 0 close fd
        data = parse_config(/* char *config_path */);
        server = new Server(data);
        signal(SIGINT, &shutdown);
        server->run();
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}