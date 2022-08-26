#include "server.hpp"

// Server::Server(config data)
// {
//     struct kevent evSet;

//     std::cout << "constructing server.." << std::endl;
//     if ((keq = kqueue()) == -1)
//         throw std::runtime_error("kqueue failed..");
//     for (int i = 0; i < data.n_v_servers; i++) {
//         listeners.push_back(new Socket(data.domain, data.type, data.interface, data.vservers[i].port, data.backlog));
//         EV_SET(&evSet, listeners[i]->get_socket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
//         kevent(keq, &evSet, 1, NULL, 0, NULL);
//     }
// }

// int     Server::is_listener(unsigned long int fd)
// {
//     for (unsigned int i = 0; i < listeners.size(); i++)
//         if (listeners[i]->get_socket() == fd)
//             return (1);
//     return (0);
// }

// Server::~Server()
// {
//     std::cout << "server destructor called.." << std::endl;
//     close(keq);
//     for (unsigned int i = 0; i < listeners.size(); i++)
//         delete listeners[i];
//     std::cout << std::endl << "server shut down, take care :).." << std::endl;
// }

// void    Server::accept_new_connection(unsigned long int fd) 
// {
//     struct kevent evSet;
//     int newfd;
//     struct sockaddr_in their_addr;
//     socklen_t   addr_size;
//     addr_size = sizeof their_addr;

//     std::cout << "accept_new_connection" << std::endl;
//     newfd = accept(fd, (sockaddr *)&their_addr, &addr_size);
//     clients.push_back(client(newfd, inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port)));
//     EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &clients[clients.size()-1]);
//     kevent(keq, &evSet, 1, NULL, 0, NULL);
//     std::cout << "got connection request from fd = " << newfd << ".." << std::endl;
//     std::cout << inet_ntoa(their_addr.sin_addr) << std::endl;
//     std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
// }

// void    Server::destroy_connection(int fd, int event)
// {
//     struct kevent evSet;

//     std::cout << "client " << fd << " disconnected.." << std::endl << std::endl;
//     EV_SET(&evSet, fd, event, EV_DELETE, 0, 0, NULL);
//     kevent(keq, &evSet, 1, NULL, 0, NULL);
//     close(fd);
// }

// void    Server::handle_request(client *c)
// {
//     size_t pos;
//     struct kevent evSet;
//     std::string tmp;

//     bzero(c->recBuffer, MAX_RECV_SIZE);
//     if ((c->rec = recv(c->fd, c->recBuffer, MAX_RECV_SIZE - 1, 0)) > 0) {
//         c->req.append(c->recBuffer);
//         std::cout << " << " << c->rec;
//     }
//     else {
//         std::cout << "reading failed.." << std::endl << std::endl;
//         destroy_connection(c->fd, EVFILT_READ);
//         return ;
//     }
//     if ((pos = c->req.find("\r\n\r\n")) != std::string::npos) {
//         if (c->header.empty()) {
//             c->header = c->req.substr(0, pos);
//             c->req.erase(0, pos + 4);
//             c->body_len = 0;
//             if ((pos = c->header.find("Content-Length: ")) != std::string::npos) {
// 				tmp = c->header;
// 				tmp.erase(0, pos + 16);
// 				c->body_len = stoi(tmp.substr(0, tmp.find("\r\n")));
// 			}
//         }
//     }
//     if (!c->header.empty() && (c->body_len) == c->req.length()) {
//         std::cout << "\ncompleted request from fd " << c->fd << std::endl << "HEADER:\n" << c->header << std::endl << "BODY\n" << c->req << std::endl;
//         c->req.clear();
//         c->header.clear();
//         EV_SET(&evSet, c->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
//         kevent(keq, &evSet, 1, NULL, 0, NULL);
//         EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, c);
//         kevent(keq, &evSet, 1, NULL, 0, NULL);
//     }
// }

// void    Server::send_request(client *c)
// {
//     int sent;
//     struct kevent evSet;

//     std::cout << ">> fd " << c->fd << " is ready for writing" << std::endl;
//     if ((sent = send(c->fd, DUMMY_HTTP_RESPONSE, sizeof DUMMY_HTTP_RESPONSE, 0)) != -1) {
//         std::cout << "message sent = " << sent << std::endl << std::endl;;
//     }
//     else {
//         std::cout << "sending failed.." << std::endl << std::endl;
//         destroy_connection(c->fd, EVFILT_WRITE);
//         return ;
//     }
//     EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
//     kevent(keq, &evSet, 1, NULL, 0, NULL);
//     EV_SET(&evSet, c->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, c);
//     kevent(keq, &evSet, 1, NULL, 0, NULL);
// }

// void Server::run() {
//     struct kevent evList[32];
//     int num_events;

//     std::cout << "server up and running.." << std::endl << std::endl;
//     for(;;) {
//         // std::cout << "waiting for events.." << std::endl;
//         if ((num_events = kevent(keq, NULL, 0, evList, 32, NULL)) == -1)
//             throw std::runtime_error("");
//         // std::cout << "catched " << num_events << " event(s).." << std::endl;
//         for (int i = 0; i < num_events; i++) {
//             if (is_listener(evList[i].ident))
//                 accept_new_connection(evList[i].ident);
//             else if (evList[i].flags & EV_EOF)
//                 destroy_connection(evList[i].ident, EVFILT_READ);
//             else if (evList[i].filter == EVFILT_READ)
//                 handle_request((client *)evList[i].udata);
//             else if (evList[i].filter == EVFILT_WRITE)
//                 send_request((client *)evList[i].udata);
//         }
//     }
// }

// SECTION - Constructors / Destructor

Server::Server() {}
Server::~Server() {}


// SECTION - Seters


void                                                            Server::set_port(int &port) { this->port = port; }

void                                                            Server::set_host(std::string &str) { this->host = str; }

void                                                            Server::set_index(std::string &index) { this->index = index; }

void                                                            Server::set_name(std::string &str) { this->name; }

void                                                            Server::set_limit(int &limit) { this->limit_body = limit; }

void                                                            Server::set_methods(std::string &str) { this->methods.push_back(str); }

void                                                            Server::set_root(std::string &root) { this->root = root; }

std::pair<std::map<int, std::string>::iterator, bool>           Server::set_error_pages(std::pair<int, std::string> &error_page)
{
    std::pair<std::map<int , std::string>::iterator, bool> exist = this->error_pages.insert(error_page);
    return exist;
}


// Section - Geters
int                                                             Server::get_port() const { return (this->port); }
std::string                                                     Server::get_host() const { return (this->host); }
std::string                                                     Server::get_index() const { return (this->index); }
std::string                                                     Server::get_name() const { return (this->name); }
std::string                                                     Server::get_root() const { return (this->root); }
int                                                             Server::get_limit_body() const { return (this->limit_body); }
std::vector<std::string>                                        Server::get_methods() const { return (this->methods); }
std::map<int, std::string>                                      Server::get_error_pages() const { return (this->error_pages); }

// void Server::set_cgi(std::string &name, std::string &path) { }