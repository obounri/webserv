#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>

int main(int ac, char **av) {

    if (ac == 2) {

        struct sockaddr_in hints;
        int conn_to = atoi(av[1]);
        int sockfd = socket(PF_INET, SOCK_STREAM, 0);

        hints.sin_family = AF_INET;
        hints.sin_port = htons(conn_to);     // short, network byte order
        hints.sin_addr.s_addr = inet_addr("10.11.6.1");

        char *buffer = new char[10];
        int sent;
        if ((connect(sockfd, (struct sockaddr *)&hints, sizeof hints)) != -1) {
            std::cout << "connected" << std::endl;
            if ((sent = send(sockfd,"SYN", 3, 0)) != -1) {
                std::cout << "SYN, sent = " << sent << std::endl;
            }
            else
                std::cout << "SYN failed" << std::endl;
            
            int rec;
            if ((rec = recv(sockfd, buffer, 3, 0)) != -1) {
                std::cout << "ACK received of len " << rec << " content:" << std::endl;
                std::cout << buffer << std::endl;
            }
            else
                std::cout << "ACK failed" << std::endl;
            // rec = recv(sockfd, buffer, 7, MSG_WAITALL);
            // std::cout << "SYN-ACK rec" << std::endl;
            // sent = send(sockfd,"ACK", 3, MSG_DONTROUTE);
            // std::cout << "ACK sent" << std::endl;
        }
        else
            std::cout << "connection failed" << std::endl;

        close(sockfd);
    }
    return 0;
}