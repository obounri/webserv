#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <exception>
#include <algorithm>
#include <vector>

// class m_errors : public std::exception
// {
//     private :
//                 std::string _error_msg;
//     public :
//                 m_errors(std::string error_msg) : _error_msg(error_msg) {}
//                 virtual ~m_errors() {}
//                 virtual const char* what() const throw()
//                 {
//                     return _error_msg.c_str();
//                 }
// };
typedef struct		s_parse
{
	std::ifstream	conf_file;
	std::string		line;
	int				server_stat;
	int				location_stat;

}					t_parse;

typedef struct s_server
{
	int port;
	std::string root;
	std::string host;
}           v_server;

struct config
{
	/* data */
	// int domain = AF_INET;
	// int type = SOCK_STREAM;
	// int interface = INADDR_ANY;
	// int backlog;
	// v_server    *vservers;

	int n_v_servers;
	std::vector<v_server> vservers;
};

void    parse_config(char *config_path);

// Syntax Error
std::string unexpected_token(std::string &name);
std::string missing_close_brace(std::string &name);
std::string missing_open_brace(std::string &name);
std::string missing_semicolon(std::string &name);
std::string too_many_arguments(std::string &name);
std::string no_server(std::string &name);
std::string missing_arguments(std::string &name);
std::string duplicate_key(std::string &name);
std::string duplicate_location(std::string &name);

#endif