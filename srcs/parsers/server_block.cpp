#include "parse_config.hpp"

// first if { not exist after server throw missing 
// if { not exist after location throw missing
// if location not inside server throw unexpected token
// if location inside location throw unexpected
// if any other directive after location throw location should the last directive
// 

#define MAX_BODY 1000000000

void	clear_server(t_parse &vars)
{
	vars.server_state.server = 0;
	vars.server_state.server_name = 0;
	vars.server_state.listen = 0;
	vars.server_state.client_max_body_size = 0;
	vars.server_state.allow_method = 0;
	vars.server_state.error_page = 0;
	vars.server_state.index = 0;
	vars.server_state.host = 0;
	vars.server_state.root = 0;
	vars.tmp_serv.clear();
}

void    check_server(t_parse &vars)
{
    if (!vars.server_state.listen || !vars.server_state.host)
                throw std::runtime_error("Error : server block not valid\n");
    char pwd[1024];
    if (vars.tmp_serv.get_methods().empty())
    {
		std::cout << "enter \n";
        vars.tmp_serv.set_methods("GET");
        vars.tmp_serv.set_methods("HEAD");
    }
    if (!vars.tmp_serv.get_limit())
        vars.tmp_serv.set_limit(MAX_BODY);
    if (vars.tmp_serv.get_root().empty())
    {
        if (!getcwd(pwd, 1024))
            throw std::runtime_error("Error : Failed to get default root");
        std::string root = pwd;
        vars.tmp_serv.set_root(root);
    }
	vars.tmp_serv.update_location();
}

void	right_brace(t_parse &vars, size_t &pos, MainServer &server)
{
	if (vars.server_state.server == 2)
	{
		if (vars.server_state.location == 2)
		{
			std::pair<std::string, location> _location(vars.tmp_location.get_path(), vars.tmp_location);
			vars.tmp_serv.set_location(_location);
			clear_location(vars);
			pos++;
		}
		else
		{
			check_server(vars);
			server.myvs.push_back(vars.tmp_serv);
			clear_server(vars);
			pos++;
		}
	}
	else
		throw std::runtime_error(unexpected_token(vars.tokens[pos]));
}

void	left_brace(t_parse &vars , size_t &pos)
{
	if ((vars.server_state.server == 2 && !vars.server_state.location) || vars.server_state.location == 2 )
		throw std::runtime_error(duplicate_key(vars.tokens[pos]));

	if (vars.server_state.server == 1)
	{
		if (vars.tokens[pos][0] == '{' && vars.tokens[pos][1])
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		else if (!vars.tokens[pos].compare("{"))
		{
			vars.server_state.server = 2;
			pos++;
		}
		else
			throw std::runtime_error(missing_open_brace("server"));
	}
	else if (vars.server_state.location == 1)
	{
		if (vars.tokens[pos][0] == '{' && vars.tokens[pos][1])
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		else if (!vars.tokens[pos].compare("{"))
		{
			vars.server_state.location = 2;
			pos++;
		}
		else
			throw std::runtime_error(missing_open_brace("location"));
	}
}

void     check_set_port(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.listen)
	{
		vars.server_state.listen = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.listen == 2)
		throw std::runtime_error(Syntax_error(vars.line));
    else if (found == 1)
    {              
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!IsNumber(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			int port = std::stoi(t_vec[0], nullptr, 10);
			if (port < 0 || port > 65536)
				throw std::runtime_error(Syntax_error("listen port out of range"));
			vars.tmp_serv.set_port(port);
			vars.server_state.listen = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		if (!IsNumber(vars.tokens[pos]))
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		int port = std::stoi(vars.tokens[pos], nullptr, 10);
		if (port < 0 || port > 65536)
			throw std::runtime_error(Syntax_error("listen port out of range"));
		vars.tmp_serv.set_port(port);
		vars.server_state.listen = 2;
		pos = pos + 2;
	}
}

void	check_set_host(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.host)
	{
		vars.server_state.host = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.host == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!Is_IP_Adress(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			vars.tmp_serv.set_host(t_vec[0]);
			vars.server_state.host = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2 )
			throw std::runtime_error(Invalid_arguments(vars.line));
		if (!Is_IP_Adress(vars.tokens[pos]))
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		vars.tmp_serv.set_host(vars.tokens[pos]);
		vars.server_state.host = 2;
		pos = pos + 2;
	}
	if (vars.server_state.host == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_server_name(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.server_name)
	{
		vars.server_state.server_name = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.server_name == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
				vars.tmp_serv.set_name(t_vec[0]);
				vars.server_state.server_name = 2;
				pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
			vars.tmp_serv.set_name(vars.tokens[pos]);
			vars.server_state.server_name = 2;
			pos = pos + 2;
	}
	if (vars.server_state.server_name == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_client_max_body_size(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.client_max_body_size)
	{
		vars.server_state.client_max_body_size = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.client_max_body_size == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!IsNumber(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			int  limit_body  = std::stoi(t_vec[0]);
			if (limit_body <= 0)
				throw std::runtime_error("Invalid client max body size : " + vars.tokens[pos]);
			vars.tmp_serv.set_limit(limit_body);
			vars.server_state.client_max_body_size = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		if (!IsNumber(vars.tokens[pos]))
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		int limit_body = std::stoi(vars.tokens[pos]);
		if (limit_body <= 0)
			throw std::runtime_error("Invalid client max body size : " + vars.tokens[pos]);
		vars.tmp_serv.set_limit(limit_body);
		vars.server_state.client_max_body_size = 2;
		pos = pos + 2;
	}
	if (vars.server_state.client_max_body_size == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_root(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.root)
	{
		vars.server_state.root = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.root == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1 )
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			vars.tmp_serv.set_root(t_vec[0]);
			vars.server_state.root = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		vars.tmp_serv.set_root(vars.tokens[pos]);
		vars.server_state.root = 2;
		pos = pos + 2;
	}
	if (vars.server_state.root == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_error_page(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.error_page)
		vars.server_state.error_page = 1;
	pos++;
	size_t found = semicolon_check(vars, pos);
	if (found == 0)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens.back(), ";");
		if (!t_vec.empty())
		{
			if (!IsNumber(vars.tokens[pos]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			int error_page_number = std::stoi(vars.tokens[pos]);
			std::pair<int, std::string> error_page(error_page_number, t_vec[0]); 
			std::pair<std::map<int , std::string>::iterator, bool> exist = vars.tmp_serv.set_error_page(error_page);  
			if (!exist.second)
				throw std::runtime_error(duplicate_key(vars.line));
			vars.server_state.error_page = 2;
			pos = pos + 2;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 3)
			throw std::runtime_error(Invalid_arguments(vars.line));
		if (!IsNumber(vars.tokens[pos]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		int error_page_number = std::stoi(vars.tokens[pos]);
		std::pair<int, std::string> error_page(error_page_number, vars.tokens[pos + 1]); 
		std::pair<std::map<int , std::string>::iterator, bool> exist = vars.tmp_serv.set_error_page(error_page);
		if (!exist.second)
			throw std::runtime_error(duplicate_key(vars.line));
		vars.server_state.error_page = 2;
		pos = pos + 3;
	}
}

void	check_set_server_index(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.index)
	{
		vars.server_state.index = 1;
		pos++;
	}
	size_t found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.index == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			vars.tmp_serv.set_index(t_vec[0]);
			vars.server_state.index = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		vars.tmp_serv.set_index(vars.tokens[pos]);
		vars.server_state.index = 2;
		pos = pos + 2;
	}
}

void    check_set_server_allow_method(t_parse &vars, size_t &pos)
{
    if (!vars.server_state.allow_method)
    {
        vars.server_state.allow_method = 1;
        pos++;
    }
    size_t found = semicolon_check(vars, pos);
    if (found == 0 || vars.server_state.allow_method == 2)
        throw std::runtime_error(Syntax_error(vars.line));
    else if (found == 1)
    {
        if (vars.tokens[pos][0] != '[' || (vars.tokens[pos][vars.tokens[pos].length() - 2] != ']'))
		{
            throw std::runtime_error("Syntax Error : Invalid arguments " + vars.line);
		}
		if ((vars.tokens.size() - pos) != 1)
            throw std::runtime_error(Invalid_arguments(vars.line));
        std::string trimed = trim_tok(vars.tokens[pos], "[", "];");
        std::vector<std::string> t_vec = split_line(trimed, ",");
        if (!t_vec.empty())
        {
			for (size_t i = 0; i < t_vec.size(); i++)
			{
				if (t_vec[i] != "GET" && t_vec[i] != "PUT" && t_vec[i] != "POST" && t_vec[i] != "HEAD" && t_vec[i] != "TRACE" && t_vec[i] != "OPTIONS" && t_vec[i] != "DELETE")
					throw std::runtime_error("Syntax Error : unknown method '" + (t_vec[i] + "'"));
				vars.tmp_serv.set_method(t_vec[i]);
			}
			vars.server_state.allow_method = 2;
			pos++;
        }
    }
    else if (found == 2)
    {
		if (vars.tokens[pos][0] != '[' || (vars.tokens[pos][vars.tokens[pos].length() - 1] != ']'))
            throw std::runtime_error("Syntax Error : Invalid arguments '" + (vars.line + "'"));
		if ((vars.tokens.size() - pos) != 2)
            throw std::runtime_error(Invalid_arguments(vars.line));
        std::string trimed = trim_tok(vars.tokens[pos], "[", "]");
        std::vector<std::string> t_vec = split_line(trimed, ",");
        if (!t_vec.empty())
        {
			for (size_t i = 0; i < t_vec.size(); i++)
			{
				if (t_vec[i] != "GET" && t_vec[i] != "PUT" && t_vec[i] != "POST" && t_vec[i] != "HEAD" && t_vec[i] != "TRACE" && t_vec[i] != "OPTIONS" && t_vec[i] != "DELETE")
					throw std::runtime_error("Syntax Error : unknown method '" + (t_vec[i] + "'"));
				vars.tmp_serv.set_method(t_vec[i]);
			}
			vars.server_state.allow_method = 2;
			pos = pos + 2;;
        }
    }
	if (vars.server_state.allow_method == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_location(t_parse &vars, size_t &pos)
{
	if (vars.tokens.size() < 2 || vars.tokens.size() > 4)
		throw std::runtime_error(Invalid_arguments(vars.line));
	else
	{
		pos++;
		if (vars.tokens[pos][0] != '/')
			throw std::runtime_error("Syntax Error : Invalid location path '" + vars.tokens[pos] + "'");
		vars.tmp_location.set_path(vars.tokens[pos]);
		pos++;
		vars.server_state.location = 1;
		if (pos < vars.tokens.size())
			left_brace(vars, pos);
	}
}

void	server_block(t_parse &vars, MainServer &server)
{
	size_t pos = 0;
	if (!vars.server_state.server && vars.tokens[pos].compare("server"))
		throw std::runtime_error(unexpected_token(vars.tokens[pos]));
	else if (!vars.server_state.server)
	{
		vars.server_state.server = 1;
		pos++;
	}
	while (pos < vars.tokens.size())
	{
		if (!vars.tokens[pos].compare("location"))
			check_set_location(vars, pos);
		else if (vars.server_state.server == 1 || !vars.tokens[pos].compare("{"))
			left_brace(vars, pos);
		else if (!vars.tokens[pos].compare("listen"))
			check_set_port(vars, pos);
		else if (!vars.tokens[pos].compare("host"))
			check_set_host(vars, pos);
		else if (!vars.tokens[pos].compare("server_name"))
			check_set_server_name(vars, pos);
		else if (!vars.tokens[pos].compare("client_max_body_size"))
			check_set_client_max_body_size(vars, pos);
		else if (!vars.tokens[pos].compare("root"))
			check_set_root(vars, pos);
		else if (!vars.tokens[pos].compare("index"))
			check_set_server_index(vars, pos);
		else if (!vars.tokens[pos].compare("error_page"))
			check_set_error_page(vars, pos);
		else if (!vars.tokens[pos].compare("allow_method"))
			check_set_server_allow_method(vars, pos);
		else if (!vars.tokens[pos].compare("}"))
			right_brace(vars, pos, server);
		else
			throw std::runtime_error(unexpected_token("unknown : " + vars.tokens[pos]));
	}

	return ;
}