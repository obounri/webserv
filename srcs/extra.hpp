/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:07:18 by amoujane          #+#    #+#             */
/*   Updated: 2022/08/27 12:13:45 by obounri          ###   ########.fr       */
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
std::string     tostring(int n);
unsigned long   hex2dec(std::string hex);
std::string     base64_decode(std::string const& encoded_string);
void            ft_bzero(void * s, size_t n);
bool            is_digits(const std::string &str);
bool			check_host(std::string host);
std::vector<std::string> ft_split(std::string str1, std::string delimiter);

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