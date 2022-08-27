#include "parse_config.hpp"

// first if { not exist after server throw missing 
// if { not exist after location throw missing
// if location not inside server throw unexpected token
// if location inside location throw unexpected
// if any other directive after location throw location should the last directive
// 

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
			std::cout << "server is on\n";
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

// void	right_brace(t_parse &vars, size_t &pos)
// {

// }

void     check_set_port(t_parse &vars, size_t &pos)
{
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

// void	listen(t_parse &vars, size_t &pos)
// {
// 	if (!vars.server_state.listen)
// 	{
// 		vars.server_state.listen = 1;
// 		pos++;
// 	}
// 		check_set_port(vars, pos);
// 		// if (vars.server_state.listen == 1)
// 		// 	throw std::runtime_error(Syntax_error(vars.line));
// }

void	check_set_host(t_parse &vars, size_t &pos)
{
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

// void	host(t_parse &vars, size_t &pos)
// {
// 	if (!vars.server_state.host)
// 	{
// 		vars.server_state.host = 1;
// 		pos++;
// 	}
// 	while (pos < vars.tokens.size())
// 		check_set_ip(vars, pos);
// 	if (vars.server_state.host == 1)
// 		throw std::runtime_error(Syntax_error(vars.line));
// }

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

// void	server_name(t_parse &vars, size_t &pos)
// {
// 	if (!vars.server_state.server_name)
// 	{
// 		vars.server_state.server_name = 1;
// 		pos++;
// 	}
// 	// while (pos < vars.tokens.size())
// 		check_set_server_name(vars, pos);
// 	if (vars.server_state.server_name == 1)
// 		throw std::runtime_error(Syntax_error(vars.line));
// }

// void	set_client_max_body_size(t_parse &vars, std::string &str)
// {
// 	vars.tmp_server.client_max_body_size = std::stoi(str);
// 	if (vars.tmp_server.client_max_body_size <= 0)
// 		throw std::runtime_error("Invalid client max body size : " + str);
// }

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

// void	client_max_body_size(t_parse &vars, size_t &pos)
// {
// 	if (!vars.server_state.client_max_body_size)
// 	{
// 		vars.server_state.client_max_body_size = 1;
// 		pos++;
// 	}
// 	// while (pos < vars.tokens.size())
// 		check_set_client_max_body_size(vars, pos);
// 	if (vars.server_state.client_max_body_size == 1)
// 		throw std::runtime_error(Syntax_error(vars.line));
// }


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
			std::pair<std::string, std::string> error_page(vars.tokens[pos], t_vec[0]); 
			std::pair<std::map<std::string , std::string>::iterator, bool> exist = vars.tmp_serv.set_error_pages(error_page);  
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
		// int error_page_number = std::stoi(vars.tokens[pos]);
		std::pair<std::string, std::string> error_page(vars.tokens[pos], vars.tokens[pos + 1]); 
		std::pair<std::map<std::string , std::string>::iterator, bool> exist = vars.tmp_serv.set_error_pages(error_page);
		if (!exist.second)
			throw std::runtime_error(duplicate_key(vars.line));
		vars.server_state.error_page = 2;
		pos = pos + 3;
	}
}

void	check_set_server_index(t_parse &vars, size_t &pos)
{
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

// void	index(t_parse &vars, size_t &pos, std::string &index, int &ref_state)
// {
// 	if (!ref_state)
// 	{
// 		ref_state = 1;
// 		pos++;
// 	}
// 	// while (pos < vars.tokens.size())
// 		check_set_index(vars, pos, index, ref_state);
// }

void	check_set_location(t_parse &vars, size_t &pos)
{
	if (vars.tokens.size() < 2 || vars.tokens.size() > 4)
		throw std::runtime_error(Invalid_arguments(vars.line));
	else
	{
		pos++;
		vars.tmp_location.set_path(vars.tokens[pos]);
		// vars.tmp_location.location.second = vars.tokens[pos];
		pos++;
		vars.server_state.location = 1;
		if (pos < vars.tokens.size())
			left_brace(vars, pos);
	}
}

void	server_block(t_parse &vars, config &configs)
{
	size_t pos = 0;
	if (!vars.server_state.server && vars.tokens[pos].compare("server"))
		throw std::runtime_error(unexpected_token(vars.tokens[pos]));
	else if (!vars.server_state.server) // if server_state 0
	{
		vars.server_state.server = 1;
		pos++;
		configs.n_v_servers++;
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
			server_name(vars, pos);
		else if (!vars.tokens[pos].compare("client_max_body_size"))
			check_set_client_max_body_size(vars, pos);
		else if (!vars.tokens[pos].compare("root"))
			check_set_root(vars, pos);
		else if (!vars.tokens[pos].compare("index"))
			check_set_server_index(vars, pos);
		else if (!vars.tokens[pos].compare("error_page"))
			check_set_error_page(vars, pos);
		else
			throw std::runtime_error(unexpected_token("unknown : " + vars.tokens[pos]));
	}
		// else if (!vars.tokens[pos].compare("}"))
		// {
		// 	std::cout << "right brace found\n";
		// 	pos++;
		// }
	return ;
}