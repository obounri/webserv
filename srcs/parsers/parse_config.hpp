#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <exception>
#include <algorithm>
#include <vector>
#include <map>

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
typedef	struct		s_state
{
	int				server;
	int				left_brace;
	int				right_brace;
	int				listen;
	int				host;
	int				client_max_body_size;
	int				server_name;
	int				root;
	int				location;
	int				index;
}					serv_state;


typedef struct						s_server
{
	int								port;
	std::string						host;
	std::string						server_name;
	int								client_max_body_size;
	std::string						index;
	std::string						root;
	std::map<int, std::string>		error_pages;
}									v_server;

typedef struct		s_parse
{
	std::ifstream				conf_file;
	std::string					line;
	std::vector<std::string>	tokens;
	serv_state					server_state;
	v_server					tmp_server;
}					t_parse;
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


// SETION - Config File Parsing 

void    					parse_config(char *config_path);
void						comment_check(std::string& line);
void						server_block(t_parse &vars, config &configs);
void						left_brace(t_parse &vars , size_t &pos);
void						listen(t_parse &vars, size_t &pos);
void						set_listen_port(t_parse &vars, size_t &pos, int & found);
void						host(t_parse &vars, size_t &pos);
void						check_set_ip(t_parse &vars, size_t &pos);


// SUBSECTION - Parsing utils

bool						IsNumber(std::string &str);
bool						Is_IP_Adress(std::string &str);
std::vector<std::string> 	split_line(std::string  const &line, std::string const &delimiters);


// SECTION - Syntax Error
std::string					Syntax_error(std::string const &name);
std::string 				unexpected_token(std::string const &name);
std::string 				missing_close_brace(std::string const &name);
std::string 				missing_open_brace(std::string const &name);
std::string 				missing_semicolon(std::string const &name);
std::string 				too_many_arguments(std::string const &name);
std::string 				no_server(std::string const &name);
std::string 				missing_arguments(std::string const &name);
std::string 				duplicate_key(std::string const &name);
std::string 				duplicate_location(std::string const &name);

#endif