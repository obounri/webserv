#include "parse.hpp"

config parse_config(/* char *config_path */) {
    config data;
    v_server tmp;
    v_server tmp1;
    v_server tmp2;

    data.backlog = 10;
    // for example 3 virtual servers;
    data.n_v_servers = 3;
    tmp.set_port("1010");
    data.vservers.push_back(tmp);
    tmp1.set_port("2020");
    data.vservers.push_back(tmp1);
    tmp2.set_port("3030");
    data.vservers.push_back(tmp2);
    
    return data;
}
