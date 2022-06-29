#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

int main(int ac, char **av) {

    if (ac == 2) {

        struct sockaddr_in hints;
        int conn_to = atoi(av[1]);
        int sockfd = socket(PF_INET, SOCK_STREAM, 0);

        hints.sin_family = AF_INET;
        hints.sin_port = htons(conn_to);     // short, network byte order
        hints.sin_addr.s_addr = inet_addr("169.254.5.174");

        if ((connect(sockfd, (struct sockaddr *)&hints, sizeof hints)) == 0) {
            std::cout << "connected, closing connection" << std::endl;
        }
        else
            std::cout << "connection failed" << std::endl;

        close(sockfd);
    }
    return 0;
}