#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <vector>


typedef struct s_server
{
    int port;
    std::string root;
    // std::string ;
}           v_server;

struct config
{
    /* data */
    // int domain = AF_INET;
    // int type = SOCK_STREAM;
    // int interface = INADDR_ANY;
    // int backlog;
    int n_v_servers;
    // v_server    *vservers;
    std::vector<v_server> vservers;
};

void    parse_config(char *config_path);

#endif