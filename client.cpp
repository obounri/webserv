#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>

int main(int ac, char **av) {

    if (ac == 4) {
        std::string mess = av[2];
        struct sockaddr_in hints;
        int conn_to = atoi(av[1]);
        int n = atoi(av[3]);

        hints.sin_family = AF_INET;
        hints.sin_port = htons(conn_to);     // short, network byte order
        hints.sin_addr.s_addr = inet_addr("192.168.1.69");

        int sent;
        while (n--)
        {
            int sockfd = socket(PF_INET, SOCK_STREAM, 0);
            int yes=1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
                std::cout << "Failed initialising socket.. quitting.." << std::endl;
                _exit(EXIT_FAILURE);
            }   
            if ((connect(sockfd, (struct sockaddr *)&hints, sizeof hints)) != -1) {
                std::cout << "connected" << std::endl;
                if ((sent = send(sockfd, av[2], mess.length(), 0)) != -1) {
                    std::cout << "Message sent = " << sent << std::endl;
                }
                else
                    std::cout << "Sending failed" << std::endl;
            }
            else
                std::cout << "connection failed" << std::endl;
            close(sockfd);
        }
        

    }
    return 0;
}