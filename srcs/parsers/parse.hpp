#ifndef PARSE_HPP
# define PARSE_HPP

#include <arpa/inet.h>
#include <iostream>
#include <vector>

typedef struct s_v_server
{
    int port;
    std::string root;
    // std::string ;
} v_server;

typedef struct s_config
{
    /* data */
    int domain = AF_INET;
    int type = SOCK_STREAM;
    int interface = INADDR_ANY;
    int backlog;
    int n_v_servers;
    // v_server    *vservers;
    std::vector<v_server> vservers;
} config;

config parse_config(/* char *config_path */);

#endif