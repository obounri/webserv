/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:07:18 by amoujane          #+#    #+#             */
/*   Updated: 2022/08/28 17:22:09 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
#include <map>
#include <vector>
#include <sys/stat.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define MAX_BODY 1000000000

std::string     tostring(int n);


typedef struct file_log
{
    std::string file;
    int size;
    std::string location;
    std::string first_req;
    int auto_index;
    struct stat st;
    int valid;
    int erno_val;
    int limit_body;
    std::string req;
    std::string q_string;
    std::string s_file;
    
    void rest(){
        file.clear();
        location.clear();
        first_req.clear();
        size = 0;
        auto_index = 0;
        valid = 0;
        erno_val = 0;
        limit_body = MAX_BODY;
        req.clear();
        q_string.clear();
        s_file.clear();
    };
    void set_size(){
        valid = 0;
        if (stat(file.c_str(), &st) < 0)
        {
            valid = -1;
            size = 0;
            erno_val = errno;
            return ;
        }
        else if (S_ISREG(st.st_mode))
            valid = 1;
        size = st.st_size;
    };
    void set_method(std::string val)
    {
        req = val;
    }
    void info(){
        std::cout << "file: " << file << "\n";
        std::cout << "size: " << size << "\n";
        std::cout << "location: " << location  << "\n";
        std::cout << "first request: " << first_req << "\n";
        std::cout << "autoindex: " << auto_index << "\n";
        std::cout << "Req: " << req << "\n";
        if (valid == -1)
            std::cout << "working: " << "no\n";
        else if (valid == 1)
            std::cout << "working: " << "yes\n";
        else
            std::cout << "working: ?\n";
    }
} t_file_log;

typedef struct s_file_info
{
    int size;
    int works;
    char *tab;
    int fd;
    
    s_file_info(std::string file){
        size = 0;
        works = 0;
        tab = NULL;
        struct stat st; 
        fd = open(file.c_str(), O_RDONLY);
        if (fd < 0)
            return ;
        if (stat(file.c_str(), &st) < 0)
            return ;
        size = st.st_size;
        if (!(tab = (char*)malloc(sizeof(char) * size + 1)))
        {
            std::cerr << "Malloc Faild: "  << strerror(errno) << std::endl; // internal server 500
            works = -5;
            return ;
        }
        bzero(tab, size + 1);
        if (read(fd, tab, size) < 0)
        {
            std::cerr << BLACK << "Error Read: " << file << RESET << std::endl;
            works = -1;
            close(fd);
            return ;
        }
        works = 1;
    }
    ~s_file_info(){
        if (fd != -1)
            close(fd);
    }
}             t_file_info;

class headers
{
private:
    std::map<std::string, std::string> _data;
	std::string http_status;
	std::string def;

public:
    headers(/* args */);
    ~headers();
	void push(std::string key, std::string value);
	void print_header(int fd);
	void set_http(int val, std::string stat);
	void reset_header();
	std::string get_file();
	std::string get_key(std::string key);
	std::string header_msg();
	
};

typedef struct s_error_messages
{
    std::map<int, std::string> _data;
    s_error_messages(){
        _data[204] = "No Content"; // POST update
        _data[400] = "Bad Request";
        _data[401] = "Unauthorized";
        _data[403] = "Forbidden"; // WWW-Authenticate or necessary permissions
        _data[404] = "Not Found";
        _data[405] = "Not Allowed";
        _data[406] = "Not Acceptable";
        _data[413] = "Payload Too Large";
        _data[415] = "Unsupported Media Type";
        _data[500] = "Internal Server Error";
        _data[501] = "Not Implemented";
        _data[503] = "Service Unavailable";
        _data[505] = "HTTP Version Not Supported";
    };
    std::string print_error_val(int value){
        std::string ret;

        ret.clear();
        ret += "<!DOCTYPE HTML><html><head>\n<style>h1 {text-align: center;}</style>\n";
        ret += "<title>" + tostring(value) + "  " + _data[value] + "</title>\n";
        ret += "</head><body>";
        ret += "<h1>" + tostring(value) + "  " + _data[value] + "</h1></body><html>"; // \n
        return (ret);
    };
}   t_error_messages;

class location
{
	private:
		std::string path;							// Location Name
		std::string root;							// Root Directory
		std::vector<std::string> default_file;		// Default Index Files
		std::vector<std::string> allowed;			// Allowed Methods
		std::string body_s;							// Limit Body String
		int index;									// index ON or OFF
		int par_error;								// Parse Error Happen
		int limit_client_body;						// max body size
		std::string _access;						// access file path

	public:
		location(/* args */);
		~location();
		
		// set
		void set_path(std::string path);
		void set_root(std::string root);
		void set_default(std::string def);
		void set_index(std::string index);
		void set_methods(std::string methods);
		void set_limit_body_size(std::string var);
		void set_access(std::string val);

		// get
		std::string get_path();
		std::string get_root();
		std::vector<std::string> get_default();
		std::vector<std::string> get_methods();
		std::string get_body_s();
		std::string get_string_methods();
		int get_limit();
		int get_index();
		int valid();
		void print_location();
		int ft_method_check(std::string val);
		std::string get_access();
};

class cgi_env
{
private:
    std::map<std::string, std::string> _data;
    char **ret;
    int client;
public:
    cgi_env(/* args */);
    cgi_env(int val);
    ~cgi_env();
    void push(std::string key, std::string value);
    char **get_env();
    char *get_var(std::string key, std::string value);
    void print_cgi();
    int check_key(std::string tmp);
};

int             ft_strncmp(const char *s1, const char *s2, size_t n);
int             ft_strcmp(char const *s1, char const *s2);
int             ft_atoi(const char *str);
size_t          ft_strlen(const char *str);
char           *ft_strdup(const char *s1);
char           *ft_strsub(char const *s, unsigned int start, size_t len);
char           *ft_strjoin(char const *s1, char const *s2);
char           *ft_strchr(const char *s, int c);
char			*ft_inet_ntoa(struct in_addr adr);
void			ft_write_string(int fd, std::string str);
int				ft_strlen(char *str);
unsigned long   hex2dec(std::string hex);
std::string     base64_decode(std::string const& encoded_string);
// void            ft_bzero(void * s, size_t n);
bool            is_digits(const std::string &str);
bool			check_host(std::string host);
std::vector<std::string> ft_split(std::string str1, std::string delimiter);
void pending_write(int fd, std::string str);
std::string send_auto(std::string dir, headers var, std::string method, file_log f);