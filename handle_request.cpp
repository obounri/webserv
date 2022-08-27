// #include "../includes/server.hpp"
// #include "../includes/header.hpp"
#include "srcs/client/client.hpp"
// add my methods to server and client classes

class server;

int ft_isuppercase(std::string val)
{
	for (std::string::iterator i = val.begin(); i != val.end(); i++)
	{
		if (*i > 'Z' || *i < 'A')
			return (0);
	}
	return (1);
}

bool check_status(std::string str, server &s)
{
	file_log f;
	std::vector<std::string> head = ft_split(str, "\n\r");
	if (head.size() == 0)
		return true;
	std::string method = head[0].substr(0, head[0].find_first_of(" "));
	std::string file = head[0].substr(head[0].find_first_of(" ") + 1, head[0].find_last_of(" ") - head[0].find_first_of(" ") - 1);
	std::string HTTP = head[0].substr(head[0].find_last_of(" ") + 1, head[0].length());
	s.get_req_file(file, method);
	f = s.get_f_log();
	s.set_method(method);
	if ("HTTP/1.1" != HTTP)
	{
		s.set_pg_val(505);
		return true;
	}
	if (!ft_isuppercase(method) || file[0] != '/')
	{
		s.set_pg_val(400);
		return true;
	}
	return false;
}

void before_polling_and_pass_to_handle_request() {
    std::multimap<std::string, server> extra_map;
	std::vector<server> g_ret; // this is from parser
	std::map<std::string, server> extra; // what is this
	std::vector<server>::iterator it = g_ret.begin();

	while (it != g_ret.end())
	{
		extra_map.insert(std::make_pair((*it).get_host() + ":" + (*it).get_port() + "#" + (*it).get_name(), *it));
		extra.insert(std::make_pair((*it).get_host() + ":" + (*it).get_port(), *it));
		it++;
	}
}

void    handle_request(server &s, client *c, std::multimap<std::string, server> extra)
{
    std::string server_response;
    std::multimap<std::string, server>::iterator m_ea;

    s.get_header_var().set_http(200, "OK");
    check_status(c->get_status_header(), s);
    s.set_header((char*)c->get_request().c_str());
    if (s.get_header_var().get_key("Host").size() == 0)
        s.set_pg_val(400);
    else
    {
        m_ea = extra.find(s.get_host() + ":" + s.get_port() + "#" + s.get_header_var().get_key("Host"));
        if (m_ea != extra.end())
        {
            s = m_ea->second;
            check_status(c->get_status_header(), s);
            s.set_header((char*)c->get_request().c_str());
        }
    }
    s.ft_cgi_exist();
    c->set_method(s.get_method());
    s.set_client_ip(c->get_ip());
    if (s.get_f_log().limit_body < (int)c->get_body().size())
        s.set_pg_val(413);
    s.set_qu_string(c->get_body());
    if (s.ft_allowed() && s.get_pg_error() == 0 && s.ft_access())
        server_response = s.run_method();
    else if (s.get_pg_error())
        server_response = s.run_error(s.get_pg_error());
    else
        server_response = s.run_error(405);
    s.set_pg_val(0);
    c->set_write_index(0);
    c->set_response(server_response);
}