#include <iostream>
#include <signal.h>

#include "srcs/server/server.hpp"

#define MYPORT 5000

Server * servers;

void shutdown(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received SIGINT.." << std::endl;
        std::cout << "Shutting server down.." << std::endl;
        servers->~Server();
        _exit(0);
    }
}

int main() {

    // if (ac == 2) {
            // handle signals  
            // parse config file
            // init server
            // wait for connection

        signal(SIGINT, &shutdown);
        // Server server(AF_INET, SOCK_STREAM, MYPORT, 10);
        servers = new Server(AF_INET, SOCK_STREAM, MYPORT, 10);
        servers->run();
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}