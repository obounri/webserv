#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <exception>
#include <algorithm>
#include <list>
#include <vector>
#include <map>
#include "../extra.hpp"
#include "../client/client.hpp"
#include "../sockets/sockets.hpp"

class MainServer;

class v_server
{
	private:
		// Default
		std::string host;
		int	port; // from string to int
		std::string name;
		std::string root;
		std::string default_page;
		int limit_body; 
		std::string limit_body_s; //  see if used
		std::list<std::string> methods;
		std::map<std::string ,location> locations;
		std::map<std::string, std::string> cgi;
		std::map<std::string ,std::string> extra_headers;
		std::string file_open;
        headers header_var;

		// Server
		// int server_fd;
		struct sockaddr_in address;

		// Errors
		int par_error;
		int page_error_val;
		t_error_messages _err;
		std::map<int, std::string> error_pg; // error pages

		// Request
		int qu_len;
		std::string file_type;
		std::string pwd_dir;
		std::string req_method;
		std::string qu_string;
		t_file_log f_log;


		// Clients
		std::string client_ip;
		// std::vector<client*> clients;

		
	public:
		Socket	*listener;
		v_server(/* args */);
		~v_server();

        // Set Methods
		void set_host(std::string &host);
		void set_port(int &port);
		void set_name(std::string &name);
		void set_limit(int limit);
		void set_root(std::string &root);
		void set_methods(std::string method);
		std::pair<std::map<int, std::string>::iterator, bool> set_error_page(std::pair<int, std::string> &error_page);
		void set_index(std::string &index);
		std::pair<std::map<std::string , std::string>::iterator, bool> set_cgi(std::pair<std::string, std::string> &cgi);
		void set_location(std::pair<std::string, location> &location);
		void set_access(std::string val);
		void cgi_parse(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
		void set_qu_string(std::string str);
		void set_pg_val(int val);
		void set_method(std::string val);
		void set_pwd(std::string p);
		void set_qu_len(std::string val);
		void set_client_ip(std::string val);
		
        // Get Methods
		int get_port() ;
		std::string get_host() ;
		std::string get_index() ;
		std::string get_name() ;
		std::string get_string_methods() ;
		std::list<std::string> get_methods() ;
		// std::string get_cgi() ;
		// std::map<std::string, std::string> get_cgi() ;
		std::string get_file_type() ;
		std::string get_root() ;
		std::map<int, std::string> get_error_pg();
		std::string get_error_page(int val) ;
		std::string get_method() ;
		std::string get_req_file(std::string val, std::string met) ;
		std::string get_file(std::map<std::string, location>::iterator loc, std::string val) ;
		std::map<std::string ,location> get_location() ;
		std::string get_body_s() ;
		std::string ft_read_file(std::string val) ;
		int get_limit() ;
        int value_port() ;
		int get_pg_error() ;
		unsigned long int get_fd_server() ;
		struct sockaddr_in get_server_address() ;
		file_log get_f_log() ;
		headers get_header_var(void) ;

		void	clear();

		// Extra
		bool check();
        void print_server();
		char **setup_env();
		cgi_env ft_add_other_headers();
		std::string get_name_headers(std::map<std::string, std::string>::iterator it);

		
        //  Launch
        // void run_server();
		std::string run_error(int val);
        void set_header(char *buff);
        // void reset_header();
		bool ft_allowed();
		std::string ft_cgi_exist();
		void update_location();
		int ft_access();

		// requests methods
		std::string run_file();
		std::string run_method();
		std::string put_method();
		std::string delete_method();
		std::string options_method();
		std::string run_cgi();
		std::string post_method();

		void cgi_status_code(std::string response);
		void update_port(std::string val);
};

typedef struct s_config
{
    /* data */
    static const int domain = AF_INET;
    static const int type = SOCK_STREAM;
    static const int interface = INADDR_ANY;
    int backlog;
    int n_v_servers;
    std::vector<v_server> vservers;
} config;


// SECTION - Server State Structure

typedef	struct							s_state
{
	int									server;
	int									listen;
	int									host;
	int									allow_method;
	int									client_max_body_size;
	int									server_name;
	int									root;
	int									error_page;
	int									location;
	int									index;
}										serv_state;

// SECTION - Location State Structure

typedef	struct							l_state
{
	int									index;
	int									root;
	int									access;
	int									client_max_body_size;
	int									autoindex;
	int									allow_method;
	int									cgi;
}										loc_state;


// SECTION - Location Block Structure

// typedef struct							s_location
// {
// 	std::pair<bool, std::string>		location;
// 	std::string							index;
// 	bool								autoindex;
// 	std::vector<std::string>			allow_method;
// }										t_location;

// SECTION - Server Block Structure

// typedef struct							s_server
// {
// 	int									port;
// 	std::string							host;
// 	std::string							server_name;
// 	int									client_max_body_size;
// 	std::string							index;
// 	std::string							root;
// 	std::map<int, std::string>			error_pages;
// 	std::map<std::string, location>		location;
// }										v_server;

// SECTION - Config File Parsing Structure

typedef struct							s_parse
{
	std::ifstream						conf_file;
	std::string							line;
	std::vector<std::string>			tokens;
	serv_state							server_state;
	loc_state							location_state;
	location							tmp_location;
	// v_server							tmp_server;
	v_server							tmp_serv;

}										t_parse;

// SETION - Config File Parsing 

void    					parse_config(char *config_path, MainServer& server);
void						comment_check(std::string& line);
void						server_block(t_parse &vars, MainServer &server);
void						left_brace(t_parse &vars , size_t &pos);
void						right_brace(t_parse &vars, size_t &pos, MainServer &server);
void						listen(t_parse &vars, size_t &pos);
void						set_listen_port(t_parse &vars, size_t &pos, int & found);
void						host(t_parse &vars, size_t &pos);
void						check_set_ip(t_parse &vars, size_t &pos);
void						index(t_parse &vars, size_t &pos, std::string &index, int &ref_state);
void    					location_block(t_parse &vars, MainServer &server);
void						check_set_autoindex(t_parse &vars, size_t &pos);
void						check_set_location_index(t_parse &vars, size_t &pos);
void						clear_location(t_parse &vars);


// SUBSECTION - Parsing Utils

bool						IsNumber(std::string &str);
bool						Is_IP_Adress(std::string &str);
std::vector<std::string> 	split_line(std::string  const &line, std::string const &delimiters);
int							semicolon_check(t_parse &vars, size_t &pos);
std::string trim_tok( std::string &str, std::string left_trimer, std::string right_trimer);



// SECTION - Syntax Error

std::string					Syntax_error(std::string const &name);
std::string 				unexpected_token(std::string const &name);
std::string 				missing_close_brace(std::string const &name);
std::string 				missing_open_brace(std::string const &name);
std::string 				missing_semicolon(std::string const &name);
std::string 				too_many_arguments(std::string const &name);
std::string 				no_server(std::string const &name);
std::string 				Invalid_arguments(std::string const &name);
std::string 				duplicate_key(std::string const &name);
std::string 				duplicate_location(std::string const &name);

#include "../server/server.hpp"

#endif