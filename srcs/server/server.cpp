#include "server.hpp"

Server::Server(config data)
{
    struct kevent evSet;

    std::cout << "constructing server.." << std::endl;
    keq = kqueue();
    for (int i = 0; i < data.n_v_servers; i++) {
        listeners.push_back(new Socket(data.domain, data.type, data.interface, data.vservers[i].port, data.backlog));
        EV_SET(&evSet, listeners[i]->get_socket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

int     Server::is_listener(unsigned long int fd)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
    {
        if (listeners[i]->get_socket() == fd)
            return (1);
    }
    return (0);
}

Server::~Server()
{
    std::cout << "server destructor called.." << std::endl;
    close(keq);
    for (unsigned int i = 0; i < listeners.size(); i++)
        delete listeners[i];
    std::cout << std::endl << "server shut down, take care :).." << std::endl;
}

void    Server::accept_new_connection(unsigned long int fd) 
{
    struct kevent evSet;
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    addr_size = sizeof their_addr;

    std::cout << "accept_new_connection" << std::endl;
    newfd = accept(fd, (sockaddr *)&their_addr, &addr_size);
    clients.push_back(client(newfd, inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port)));
    EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &clients[clients.size()-1]);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    std::cout << "got connection request from fd = " << newfd << ".." << std::endl;
    std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
    std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
}

void    Server::destroy_connection(int fd, int event)
{
    struct kevent evSet;

    std::cout << "client " << fd << " disconnected.." << std::endl << std::endl;
    EV_SET(&evSet, fd, event, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    close(fd);
}

void    Server::handle_request(client *c)
{
    int rec;
    struct kevent evSet;

    std::cout << "handle_request" << std::endl;
    char* buffer = new char[2];
    std::cout << "from fd = " << c->fd << " and ip = " << c->ip << " and port = " << c->port << std::endl;
    if ((rec = recv(c->fd, buffer, 2, 0)) > 0) {
        std::cout << "received message of len " << rec << " content:" << std::endl;
        std::cout << buffer << std::endl << std::endl;
        delete [] buffer;
    }
    else {
        std::cout << "reading failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_READ);
        delete [] buffer;
        return ;
    }
    EV_SET(&evSet, c->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, c);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
}

void    Server::send_request(client *c)
{
    int sent;
    struct kevent evSet;

    std::cout << "send_request" << std::endl;
    std::cout << ">> fd " << c->fd << " is ready for writing" << std::endl;
    if ((sent = send(c->fd, DUMMY_HTTP_RESPONSE, sizeof DUMMY_HTTP_RESPONSE, 0)) != -1) {
        std::cout << "message sent = " << sent << std::endl << std::endl;;
    }
    else {
        std::cout << "sending failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_WRITE);
        return ;
    }
    EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    EV_SET(&evSet, c->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, c);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
}

void Server::run() {
    struct kevent evList[32];

    std::cout << "server up and running.." << std::endl << std::endl;
    for(;;) {
        std::cout << "waiting for events.." << std::endl;
        int num_events = kevent(keq, NULL, 0, evList, 32, NULL);
        std::cout << "catched " << num_events << " event(s).." << std::endl;
        for (int i = 0; i < num_events; i++)
        {
            if (is_listener(evList[i].ident))
                accept_new_connection(evList[i].ident);
            else if (evList[i].flags & EV_EOF)
                destroy_connection(evList[i].ident, EVFILT_READ);
            else if (evList[i].filter == EVFILT_READ)
                handle_request((client *)evList[i].udata);
            else if (evList[i].filter == EVFILT_WRITE)
                send_request((client *)evList[i].udata);
        }
    }
}