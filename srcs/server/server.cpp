#include "server.hpp"

MainServer::MainServer(config data)
{
    struct kevent evSet;

    std::cout << "constructing server.." << std::endl;
    if ((keq = kqueue()) == -1)
        throw std::runtime_error("kqueue failed..");
    for (int i = 0; i < data.n_v_servers; i++) {
        listeners.push_back(new Socket(data.domain, data.type, data.interface, data.vservers[i].port, data.backlog));
        EV_SET(&evSet, listeners[i]->get_socket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

int     MainServer::is_listener(unsigned long int fd)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
        if (listeners[i]->get_socket() == fd)
            return (1);
    return (0);
}

MainServer::~MainServer()
{
    std::cout << "server destructor called.." << std::endl;
    close(keq);
    for (unsigned int i = 0; i < listeners.size(); i++)
        delete listeners[i];
    std::cout << std::endl << "server shut down, take care :).." << std::endl;
}

void    MainServer::accept_new_connection(unsigned long int fd) 
{
    struct kevent evSet;
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    addr_size = sizeof their_addr;

    newfd = accept(fd, (sockaddr *)&their_addr, &addr_size);
    clients.push_back(client(newfd, inet_ntoa(their_addr.sin_addr)));
    EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &clients[clients.size()-1]);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    std::cout << "Got connection request from fd = " << newfd << ".." << inet_ntoa(their_addr.sin_addr) << std::endl;
    std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
}

void    MainServer::destroy_connection(int fd, int event)
{
    struct kevent evSet;

    std::cout << "client " << fd << " disconnected.." << std::endl << std::endl;
    EV_SET(&evSet, fd, event, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    close(fd);
}

void    MainServer::recv_request(client *c)
{
    size_t pos;
    size_t rec;
    struct kevent evSet;
    std::string tmp;

    bzero(c->recBuffer, MAX_IO_SIZE);
    if ((rec = recv(c->fd, c->recBuffer, MAX_IO_SIZE - 1, 0)) > 0) {
        c->client_request.append(c->recBuffer);
        std::cout << " << " << rec;
    }
    else {
        std::cout << "reading failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_READ);
        return ;
    }
    if ((pos = c->client_request.find("\r\n\r\n")) != std::string::npos) {
        if (c->header.empty()) {
            c->header = c->client_request.substr(0, pos);
            c->client_request.erase(0, pos + 4);
            c->body_len = 0;
            if ((pos = c->header.find("Content-Length: ")) != std::string::npos) {
				tmp = c->header;
				tmp.erase(0, pos + 16);
				c->body_len = atoi(tmp.substr(0, tmp.find("\r\n")).c_str());
			}
        }
    }
    if (!c->header.empty() && (c->body_len) == c->client_request.length()) {
        std::cout << "\ncompleted request from fd " << c->fd << std::endl << "HEADER:\n" << c->header << std::endl << "BODY\n" << c->client_request << std::endl;
        c->client_request.clear();
        c->header.clear();
        c->client_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 3280\r\n\r\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Sed velit velit, convallis vel hendrerit ut, facilisis scelerisque lorem. Praesent quis lectus eu lorem tincidunt iaculis nec ac diam. Quisque scelerisque felis felis, nec varius ipsum placerat nec. Donec faucibus tellus a sollicitudin pretium. In ornare aliquet pretium. Donec vestibulum ut enim id mattis. Morbi ullamcorper auctor leo, a ultricies nunc maximus sit amet. Donec facilisis orci tortor, cursus tincidunt sem sodales tristique. Nam lacinia, mi ut aliquam varius, risus massa scelerisque tortor, id tristique ipsum dui placerat enim. Mauris vehicula commodo ex vel varius. Sed molestie ultricies sodales. Phasellus quam sem, sagittis non ornare et, posuere sit amet ligula. Vivamus dignissim mi sit amet sapien suscipit, sed sollicitudin metus faucibus. Nullam vitae ultricies ante. Quisque faucibus mi sit amet ligula suscipit tincidunt. Nunc sed augue pharetra, efficitur velit blandit, interdum est.\nSed suscipit viverra erat, sollicitudin fringilla orci tempus et. Vestibulum pellentesque nulla sed scelerisque venenatis. Cras augue velit, dapibus vel luctus sit amet, interdum ut erat. Praesent ac eros at tellus egestas luctus vitae vel nisi. Aliquam erat volutpat. Fusce felis neque, dignissim ac commodo non, euismod in est. Mauris maximus ac purus vel convallis. Sed efficitur vitae mauris ut tristique. Nam mi augue, elementum quis mi et, mattis lacinia leo. Etiam tempor hendrerit enim vel venenatis. Nullam molestie metus et sem ultricies elementum.\nPellentesque eget tortor vel neque rutrum sodales. Morbi vitae cursus nibh. Curabitur quis magna eu lorem venenatis tempus non non leo. Duis eget suscipit neque. Etiam finibus nunc massa, vel venenatis elit imperdiet fringilla. Pellentesque non quam vitae arcu pretium ullamcorper eu at metus. Donec euismod interdum sem, a volutpat nulla molestie quis. Interdum et malesuada fames ac ante ipsum primis in faucibus. Sed rhoncus turpis sit amet tellus aliquam, vel fringilla odio eleifend. Ut non ultrices mi. Maecenas diam augue, semper cursus ultricies at, hendrerit ac mi.\nSed elementum faucibus arcu, sed consequat metus luctus sed. Ut non metus risus. Nam eget blandit diam, at ultricies elit. Sed eget tortor vel erat varius semper eget sit amet neque. Vivamus dignissim maximus nulla, nec semper sapien luctus at. Curabitur ac lorem id odio fringilla sollicitudin. Nullam lobortis varius neque, eu feugiat purus faucibus quis. Donec scelerisque elit ut lacus ullamcorper, quis accumsan sapien elementum. Praesent interdum et ligula lacinia accumsan. Sed ligula enim, cursus ut erat ac, tristique ullamcorper erat.\nQuisque dictum sodales tortor in pulvinar. Sed malesuada sollicitudin felis. Vestibulum hendrerit mi metus, vel dictum diam fermentum vel. Morbi vestibulum sodales sapien, nec sagittis odio porttitor sit amet. Mauris at urna ultricies, tempus magna ac, consequat justo. Etiam suscipit leo nisi, eget bibendum dolor elementum a. Vestibulum id orci massa. Ut ligula leo, malesuada in imperdiet vehicula, facilisis laoreet erat. Aenean hendrerit quis arcu eget luctus. Nullam ornare urna consequat ligula ultricies pharetra. Nulla aliquet dictum tortor ac sodales. Quisque nec ipsum leo. Ut varius enim quis accumsan fermentum.\r\n\r\n";
        c->sent = 0;
        EV_SET(&evSet, c->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
        EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, c);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

void    MainServer::send_request(client *c) {
    int sent = 0, to_send = c->client_response.length() - c->sent;
    struct kevent evSet;

    if (to_send > MAX_IO_SIZE)
        to_send = MAX_IO_SIZE;
    if ((sent = send(c->fd, c->client_response.c_str() + c->sent, to_send, 0)) != -1) {
        std::cout << " >> " << sent;
        c->sent += sent;
    }
    else {
        std::cout << "sending failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_WRITE);
        return ;
    }
    std::cout << std::endl << c->sent << std::endl;
    if (c->sent == c->client_response.length()) {
        std::cout << "\nSent full request" << std::endl;
        EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
        EV_SET(&evSet, c->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, c);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

void MainServer::run() {
    struct kevent evList[32];
    int num_events;

    std::cout << "server up and running.." << std::endl << std::endl;
    for(;;) {
        if ((num_events = kevent(keq, NULL, 0, evList, 32, NULL)) == -1)
            throw std::runtime_error("");
        for (int i = 0; i < num_events; i++) {
            if (is_listener(evList[i].ident))
                accept_new_connection(evList[i].ident);
            else if (evList[i].flags & EV_EOF)
                destroy_connection(evList[i].ident, EVFILT_READ);
            else if (evList[i].filter == EVFILT_READ)
                recv_request((client *)evList[i].udata);
            else if (evList[i].filter == EVFILT_WRITE)
                send_request((client *)evList[i].udata);
        }
    }
}