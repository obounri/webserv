#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define MYPORT 5000
#define DUMMY "<!DOCTYPE html> \
<html> \
<head> \
<title>Welcome to nginx!</title> \
<style> \
html { color-scheme: light dark; } \
body { width: 35em; margin: 0 auto; \
font-family: Tahoma, Verdana, Arial, sans-serif; } \
</style> \
</head> \
<body> \
<h1>Welcome to nginx!</h1> \
<p>If you see this page, the nginx web server is successfully installed and \
working. Further configuration is required.</p> \
<p>For online documentation and support please refer to \
<a href=\"http://nginx.org/\">nginx.org</a>.<br/> \
Commercial support is available at \
<a href=\"http://nginx.com/\">nginx.com</a>.</p> \
<p><em>Thank you for using nginx.</em></p> \
</body> \
</html>"

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
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            perror("setsockopt");
            exit(1);
        }

        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(MYPORT);     // short, network byte order
        my_addr.sin_addr.s_addr = 0;
        // memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

        bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);
        listen(sockfd, 10);

        addr_size = sizeof their_addr;
        // int size = sizeof DUMMY;
        char *buffer = new char[2048];
        int rec, sent;
        while (1) {
            newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
            std::cout << "got connection request from fd = " << newfd << std::endl;
            rec = recv(sockfd, buffer, sizeof buffer, MSG_WAITALL);
            std::cout << rec << "\n" << buffer << std::endl;
            std::cout << "SYN rec" << std::endl;
            sent = send(newfd,"SYN-ACK", 7, MSG_DONTROUTE);
            std::cout << "SYN-ACK sent" << std::endl;
            rec = recv(sockfd, buffer, 3, MSG_WAITALL);
            std::cout << "ACK rec\nHandshake done" << std::endl;
            // close(newfd);
        }
        close(sockfd);
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}