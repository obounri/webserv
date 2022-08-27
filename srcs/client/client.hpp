/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoujane <amoujane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 13:45:59 by amoujane          #+#    #+#             */
/*   Updated: 2021/05/03 16:33:21 by amoujane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "../extra.hpp"
#include <sys/stat.h>
#include <cerrno>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define CYAN    "\033[36m"      /* Cyan */
#define MAX_BODY 1000000000

// GET   1
// HEAD  2
// PUT   3
// POST  4
// OPTIONS 5
// DELETE 6

#define MAX_IO_SIZE 1024

class client
{
private:
    int run;
    // int write_index;
    int req_done;
    time_t last_req;
    std::string ip;
    int req_tab[7];
    std::string client_body;
public:
    int fd;
    std::string client_request;
    std::string client_response;
    char        recBuffer[MAX_IO_SIZE];
    std::string header;
    size_t      body_len;
    size_t         sent;
    client(/* args */);
    client(int fd, std::string str);
    ~client();
    void update_time();
    bool still_alive();
    int get_fd();
    void set_fd(int fd);
    void set_ip(std::string str);
    std::string get_ip();
    void ft_read();
    int ft_write();
    int request_done();
    int client_closed();
    void ft_unchunck_body();
    std::string get_request();
    std::string get_body();
    std::string get_response();
    void set_response(std::string str);
    std::string get_status_header();
    void set_write_index(int val);
    void rest();
    void set_client_closed(int val);
    void set_method(std::string m);
    void client_finished();
};

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