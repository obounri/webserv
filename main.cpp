#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define MYPORT 5000

void    shutdown(int signal) {
    // shutdown server and exit program, close sockets and maybe connections too

    if (signal == SIGINT) {
        std::cout << "Received SIGINT.." << std::endl;
        std::cout << "Shutting server down.." << std::endl;
        exit(0);
    }
}

int main() {

    // if (ac == 2) {
            // handle signals  
            // parse config file
            // init server
            // wait for connection

        signal(SIGINT, &shutdown);

        int sockfd, newfd;
        struct sockaddr_in my_addr;
        struct sockaddr_storage their_addr;
        socklen_t   addr_size;

        sockfd = socket(PF_INET, SOCK_STREAM, 0);

        int yes=1;
        // lose the pesky "Address already in use" error message
        if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
            perror("setsockopt");
            exit(1);
        }

        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(MYPORT);     // short, network byte order
        my_addr.sin_addr.s_addr = INADDR_ANY;
        // memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

        bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);
        listen(sockfd, 10);

        addr_size = sizeof their_addr;
        while (1) {
            newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
            std::cout << "got connection request from fd = " << newfd << std::endl;
        } 
        close(sockfd);
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}