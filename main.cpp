#include <iostream>
#include <signal.h>
#include "srcs/server/server.hpp"

void shutdown(int signal) {
    if (signal == SIGINT) {
        std::cout << " received SIGINT.." << std::endl;
    }
}

int main() {

    try {
        // if (ac == 2) {
                // handle signals
                // parse config file
                // init server
                // wait for connection

            MainServer server; // request now has no header / header vars are set somewhere while reading 3nd jimi look at that

            std::string pp[3] = { "1010", "2020", "3030"};

            server.myvs.push_back(v_server());
            server.myvs[0].set_port(pp[0]);
            // server.myvs[0].

            server.init_server();

            signal(SIGINT, &shutdown);
            server.run();
        // }
        // else
        //     throw std::runtime_error("Provide config file..");
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    
    return 0;
}