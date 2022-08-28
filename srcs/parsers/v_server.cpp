#include "parse_config.hpp"


v_server::v_server() {}

v_server::~v_server() {}


// SECTION - Seters


void                                                                v_server::set_port(int &port) { this->port = port; }

void                                                                v_server::set_host(std::string &host) { this->host = host; }

void                                                                v_server::set_index(std::string &index) { this->default_page = index; }

void                                                                v_server::set_name(std::string &name) { this->name = name; }

void                                                                v_server::set_limit(int &limit) { this->limit_body = limit; }

void                                                                v_server::set_methods(std::string &method) { this->methods.push_back(method); }

void                                                                v_server::set_root(std::string &root) { this->root = root; }

std::pair<std::map<std::string, std::string>::iterator, bool>       v_server::set_error_page(std::pair<std::string, std::string> &error_page)
{
    std::pair<std::map<std::string , std::string>::iterator, bool> exist = this->error_pg.insert(error_page);
    return exist;
}


// Section - Geters


int                                                                 v_server::get_port() const { return (this->port); }

std::string                                                         v_server::get_host() const { return (this->host); }

std::string                                                         v_server::get_index() const { return (this->default_page); }

std::string                                                         v_server::get_name() const { return (this->name); }

std::string                                                         v_server::get_root() const { return (this->root); }

int                                                                 v_server::get_limit() const { return (this->limit_body); }

std::list<std::string>                                              v_server::get_methods() const { return (this->methods); }

std::map<std::string, std::string>                                  v_server::get_error_pages() const { return (this->error_pg); }
