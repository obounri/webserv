#include <arpa/inet.h>
#include <iostream>
#include <vector>

using   namespace std;

struct v_server
{
    int port;
    std::string root;
    // std::string ;
};

struct config
{
    /* data */
    static const int domain = AF_INET;
    static const int type = SOCK_STREAM;
    static const int interface = INADDR_ANY;
    int backlog;
    int n_v_servers;
    // v_server    *vservers;
    vector<v_server> vservers;
};

config parse_config(/* char *config_path */) {
    config data;
    v_server tmp;

    data.backlog = 10;
    // for example 3 virtual servers;
    data.n_v_servers = 3;
    tmp.port = 1010;
    data.vservers.push_back(tmp);
    tmp.port = 2020;
    data.vservers.push_back(tmp);
    tmp.port = 3030;
    data.vservers.push_back(tmp);
    return data;
}
