#include <iostream>
#include <signal.h>

#include "srcs/server/server.hpp"
#include "srcs/parsers/parse_config.cpp"

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
    
        data = parse_config(/* char *config_path */);
        server = new Server(data.domain, data.type, data.interface, MYPORT, 10);
        signal(SIGINT, &shutdown);
        server->run();
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}