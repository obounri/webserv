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
            config data;

            // wait for message to finish before sending response
            data = parse_config(/* char *config_path */);
            Server server(data);

            signal(SIGINT, &shutdown);
            server.run();
        // }
        // else
        //     throw std::runtime_error("Provide config file..");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    return 0;
}