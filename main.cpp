
#include <iostream>

void    shutdown(int signal) {
    // shutdown server and exit program
}

int main(int ac, char**av) {

    if (ac == 2) {
        // handle signals
        // parse config file
        // init server
        // wait for connection
    }
    else
        std::cout << "Provide config file" << std::endl;
    
    return 0;
}