#include "parse_config.hpp"

// int	is_numbers(std::string &str)
// {
// 	size_t i = 0;
// 	while (i < str.size())
// 	{
// 		if (str[i] == ';' && i + 1 == (str.size() - 1))
// 			return 2;
// 		else if (!std::isdigit(str[i]))
// 			return 0;
// 		i++;
// 	}
// 	return 1;
// }

void	left_brace(t_parse &vars , size_t &pos)
{
	if (vars.server_state.server == 2)
		throw std::runtime_error(duplicate_key(vars.tokens[pos]));

	while (pos < vars.tokens.size())
	{
		if (vars.tokens[pos].empty())
		{
			pos++;
			continue;
		}
		if (vars.tokens[pos][0] != '{')
			throw std::runtime_error(missing_open_brace(vars.tokens[pos]));
		else if (vars.tokens[pos][0] == '{' && vars.tokens[pos][1])
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		else
		{
			vars.server_state.server = 2;
			vars.server_state.left_brace = 1;
			pos++;
			break;
		}
			pos++;
	}
}

void	set_port(t_parse &vars, std::string &str)
{
		vars.tmp_server.port = std::stoi(str, nullptr, 10);
		if (vars.tmp_server.port < 0 || vars.tmp_server.port > 65536)
			throw std::runtime_error(Syntax_error("listen port out of range"));
		vars.server_state.listen = 2;
}

int		semicolon_check(t_parse &vars, size_t &pos)
{
	size_t found = vars.tokens[pos].find(";");
    if (found != std::string::npos && found == vars.tokens[pos].length() - 1)
		return 1;
	else if (!vars.tokens[pos + 1].empty() && !vars.tokens[pos + 1].compare(";"))
        return 2;
    return 0;
}

void     check_set_port(t_parse &vars, size_t &pos)
{
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.listen == 2)
		throw std::runtime_error(Syntax_error(vars.line));
    else if (found == 1)
    {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!IsNumber(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			set_port(vars, t_vec[0]);
			vars.server_state.listen = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if (!IsNumber(vars.tokens[pos]))
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		set_port(vars, vars.tokens[pos]);
		vars.server_state.listen = 2;
		pos = pos + 2;
	}
}

void	listen(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.listen)
	{
		vars.server_state.listen = 1;
		pos++;
	}
	while (pos < vars.tokens.size())
	{
		// if (!vars.tokens[pos].compare("listen"))
		// 	throw std::runtime_error(duplicate_key(vars.tokens[pos]));
		// else
		check_set_port(vars, pos);
	}
		if (vars.server_state.listen == 1)
			throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_ip(t_parse &vars, size_t &pos)
{
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.host == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!Is_IP_Adress(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
			vars.tmp_server.host = t_vec[0];
			vars.server_state.host = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if (!Is_IP_Adress(vars.tokens[pos]))
			throw std::runtime_error(unexpected_token(vars.tokens[pos]));
		vars.tmp_server.host = vars.tokens[pos];
		vars.server_state.host = 2;
		pos = pos + 2;
	}
}

void	host(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.host)
	{
		vars.server_state.host = 1;
		pos++;
	}
	while (pos < vars.tokens.size())
		check_set_ip(vars, pos);
	if (vars.server_state.host == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_server_name(t_parse &vars, size_t &pos)
{
	size_t found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.server_name == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
				vars.tmp_server.server_name = t_vec[0];
				vars.server_state.server_name = 2;
				pos++;
		}
	}
	else if (found == 2)
	{
			vars.tmp_server.server_name = vars.tokens[pos];
			vars.server_state.server_name = 2;
			pos++;
	}
}

void	server_name(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.server_name)
	{
		vars.server_state.server_name = 1;
		pos++;
	}
	while (pos < vars.tokens.size())
		check_set_server_name(vars, pos);
	if (vars.server_state.host == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_client_max_body_size(t_parse &vars, size_t &pos)
{
	size_t found = semicolon_check(vars, pos);
	if (found == 0 || vars.server_state.client_max_body_size == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	if (found == 1)
	{
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			if (!IsNumber(t_vec[0]))
				throw std::runtime_error(unexpected_token(vars.tokens[pos]));
				// in progress ...
			// set_client_max_body_size();
			vars.server_state.client_max_body_size = 2;
			pos++;
		}
	}
}

void	client_max_body_size(t_parse &vars, size_t &pos)
{
	if (!vars.server_state.client_max_body_size)
	{
		vars.server_state.client_max_body_size = 1;
		pos++;
	}
	while (pos < vars.tokens.size())
		std::cout << "enter client function\n";
	if (vars.server_state.client_max_body_size == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	server_block(t_parse &vars, config &configs)
{
	size_t pos = 0;
	size_t cmp = vars.tokens[pos].compare("server");
	if (vars.server_state.server == 0 && cmp)
		throw std::runtime_error(unexpected_token(vars.tokens[pos]));
	else if (!vars.server_state.server) // if server_state 0
	{
		vars.server_state.server = 1;
		pos++;
		configs.n_v_servers++;
	}
	while (pos < vars.tokens.size())
	{
		if (!vars.tokens[pos].compare("{"))
			left_brace(vars, pos);
		else if (!vars.tokens[pos].compare("listen"))
			listen(vars, pos);
		else if (!vars.tokens[pos].compare("host"))
			host(vars, pos);
		else if (!vars.tokens[pos].compare("server_name"))
			server_name(vars, pos);
		else if (!vars.tokens[pos].compare("client_max_body"))
			client_max_body_size(vars, pos);
		else if (!vars.tokens[pos].compare("root"))
		{
			std::cout << "root found\n";
			pos++;
		}
		else if (!vars.tokens[pos].compare("location"))
		{
			std::cout << "location found\n";
			pos++;
		}
		else if (!vars.tokens[pos].compare("index"))
		{
			std::cout << "index found\n";
			pos++;
		}
		else if (!vars.tokens[pos].compare("}"))
		{
			std::cout << "right brace found\n";
			pos++;
		}
		else
			throw std::runtime_error(unexpected_token("unknown : " + vars.tokens[pos]));
	}
	return ;
}