#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>

#define DUMMY_HTTP_REQUEST "GET / HTTP/1.1\r\nHost: localhost:PORT\r\n\r\n"

int main(int ac, char **av) {

    if (ac == 3) {
        struct sockaddr_in hints;
        int conn_to = atoi(av[1]);
        int n = atoi(av[2]);

        hints.sin_family = AF_INET;
        hints.sin_port = htons(conn_to);     // short, network byte order
        hints.sin_addr.s_addr = inet_addr("127.0.0.1");

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
                if ((sent = send(sockfd, DUMMY_HTTP_REQUEST, sizeof DUMMY_HTTP_REQUEST, 0)) != -1) {
                    std::cout << "Message sent = " << sent << std::endl;
                }
                else
                    std::cout << "Sending failed" << std::endl;
                int rec;
                char *buffer = new char[1024];
                if ((rec = recv(sockfd, buffer, 1024, 0)) != -1) {
                    std::cout << "received message of len " << rec << " content:" << std::endl;
                    std::cout << buffer << std::endl << std::endl;
                }
                else
                    std::cout << "reading failed.." << std::endl << std::endl;
                delete [] buffer;
            }
            else
                std::cout << "connection failed" << std::endl;
            close(sockfd);
        }
        

    }
    return 0;
}