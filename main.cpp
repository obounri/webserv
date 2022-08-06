#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "srcs/sockets/sockets.hpp"

#define MYPORT 5000
#define DUMMY "HTTP/1.1 200 OK \
Date: Mon, 23 May 2005 22:38:34 GMT \
Content-Type: text/html; charset=UTF-8 \
Content-Length: 155 \
Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT \
Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux) \
ETag: \"3f80f-1b6-3e1cb03b\" \
Accept-Ranges: bytes \
Connection: close \
\
<!DOCTYPE html> \
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
    // shutdown server and _exit program, close sockets and maybe connections too

    if (signal == SIGINT) {
        std::cout << "Received SIGINT.." << std::endl;
        std::cout << "Shutting server down.." << std::endl;
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

        int newfd;
        struct sockaddr_in their_addr;
        socklen_t   addr_size;
        Socket listener(AF_INET, SOCK_STREAM, 8080, 10);

        addr_size = sizeof their_addr;
        // int size = sizeof DUMMY;
        int rec, sent;
        while (1) {
            char *buffer = new char[30000];
            newfd = accept(listener.get_socket(), (struct sockaddr *)&their_addr, &addr_size);
            std::cout << "got connection request from fd = " << newfd << std::endl;
            std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
            std::cout << ntohs(their_addr.sin_port) << std::endl;
            if ((rec = recv(newfd, buffer, 30000, 0)) != -1) {
                std::cout << "Received message of len " << rec << " content:" << std::endl;
                std::cout << buffer << std::endl;
            }
            else
                std::cout << "Reading failed" << std::endl;
            if ((sent = send(newfd, DUMMY, sizeof DUMMY, 0)) != -1) {
                std::cout << "Message sent = " << sent << std::endl;
            }
            else
                std::cout << "Sending failed" << std::endl;
            close(newfd);
            delete buffer;
        }
    // }
    // else
    //     std::cout << "Provide config file" << std::endl;
    
    return 0;
}