/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 19:10:00 by obounri           #+#    #+#             */
/*   Updated: 2022/08/28 19:24:33 by obounri          ###   ########.fr       */
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
    int            v_server_fd;

    client(/* args */);
    client(int fd, std::string str, int server_fd);
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