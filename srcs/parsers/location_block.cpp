#include "parse_config.hpp"

void	clear_location(t_parse &vars)
{
	vars.server_state.location = 0;
	vars.location_state.access = 0;
	vars.location_state.allow_method = 0;
	vars.location_state.autoindex = 0;
	vars.location_state.cgi = 0;
	vars.location_state.client_max_body_size = 0;
	vars.location_state.index = 0;
	vars.location_state.root = 0;
	vars.tmp_location.clear();
}

void	check_set_location_client_max_body_size(t_parse &vars, size_t &pos)
{
	if (!vars.location_state.client_max_body_size)
	{
		vars.location_state.client_max_body_size = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.location_state.client_max_body_size == 2)
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
			vars.tmp_location.set_limit_body_size(limit_body);
			vars.location_state.client_max_body_size = 2;
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
		vars.tmp_location.set_limit_body_size(limit_body);
		vars.server_state.client_max_body_size = 2;
		pos = pos + 2;
	}
	if (vars.server_state.client_max_body_size == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}


void	check_set_location_index(t_parse &vars, size_t &pos)
{
    if (!vars.location_state.index)
	{
		vars.location_state.index = 1;
		pos++;
	}
	size_t found = semicolon_check(vars, pos);
	if (found == 0 || vars.location_state.index == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			vars.tmp_location.set_default(t_vec[0]);
			vars.location_state.index = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		vars.tmp_location.set_default(vars.tokens[pos]);
		vars.location_state.index = 2;
		pos = pos + 2;
	}
}

void    check_set_location_autoindex(t_parse &vars, size_t &pos)
{
    if (!vars.location_state.autoindex)
    {
        vars.location_state.autoindex = 1;
        pos++;
    }
    size_t found = semicolon_check(vars, pos);
    if (found == 0 || vars.location_state.autoindex == 2)
        throw std::runtime_error(Syntax_error(vars.line));
    else if (found == 1)
    {
        if ((vars.tokens.size()- 1) != 1)
            throw std::runtime_error(Invalid_arguments(vars.line));
        std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
        if (!t_vec.empty())
        {
            if (!t_vec[0].compare("on"))
                vars.tmp_location.set_index(1);
            else if (!t_vec[0].compare("off"))
                vars.tmp_location.set_index(0);
            else
                throw std::runtime_error(unexpected_token(vars.tokens[pos]));
            vars.location_state.autoindex = 2;
            pos++;
        }
    }
    else if (found == 2)
    {
        if ((vars.tokens.size()- 1) != 2)
        throw std::runtime_error(Invalid_arguments(vars.line));
        if (!vars.tokens[pos].compare("on"))
            vars.tmp_location.set_index(1);
        else if (!vars.tokens[pos].compare("off"))
            vars.tmp_location.set_index(0);
        else
            throw std::runtime_error(unexpected_token(vars.tokens[pos]));
        vars.location_state.autoindex = 2;
        pos = pos + 2;
    }
}

void	check_set_location_access(t_parse &vars, size_t &pos)
{
	if (!vars.location_state.access)
	{
		vars.location_state.access = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.location_state.access == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1 )
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			vars.tmp_location.set_access(t_vec[0]);
			vars.location_state.access = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		vars.tmp_location.set_access(vars.tokens[pos]);
		vars.location_state.access = 2;
		pos = pos + 2;
	}
	if (vars.location_state.access == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}


void	check_set_location_root(t_parse &vars, size_t &pos)
{
	if (!vars.location_state.root)
	{
		vars.location_state.root = 1;
		pos++;
	}
	int found = semicolon_check(vars, pos);
	if (found == 0 || vars.location_state.root == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1 )
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::vector<std::string> t_vec = split_line(vars.tokens[pos], ";");
		if (!t_vec.empty())
		{
			vars.tmp_location.set_root(t_vec[0]);
			vars.location_state.root = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
		vars.tmp_location.set_root(vars.tokens[pos]);
		vars.location_state.root = 2;
		pos = pos + 2;
	}
	if (vars.location_state.root == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void    check_set_location_allow_method(t_parse &vars, size_t &pos)
{
    if (!vars.location_state.allow_method)
    {
        vars.location_state.allow_method = 1;
        pos++;
    }
    size_t found = semicolon_check(vars, pos);
    if (found == 0 || vars.location_state.allow_method == 2)
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
				vars.tmp_location.set_method(t_vec[i]);
			}
			vars.location_state.allow_method = 2;
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
				vars.tmp_location.set_method(t_vec[i]);
			}
			vars.location_state.allow_method = 2;
			pos = pos + 2;;
        }
    }
	if (vars.location_state.allow_method == 1)
		throw std::runtime_error(Syntax_error(vars.line));
}

void	check_set_location_cgi(t_parse &vars, size_t &pos)
{
	if (!vars.location_state.cgi)
	{
		vars.location_state.cgi = 1;
		pos++;
	}
	size_t found = semicolon_check(vars, pos);
	if (found == 0 || vars.location_state.cgi == 2)
		throw std::runtime_error(Syntax_error(vars.line));
	else if (found == 1)
	{
		if ((vars.tokens.size() - pos) != 1)
			throw std::runtime_error(Invalid_arguments(vars.line));
		std::string trimed = trim_tok(vars.tokens[pos], "", ";");
		if (!trimed.empty())
		{
			std::pair<std::string, std::string> cgi(vars.tmp_location.get_path(), trimed); 
			std::pair<std::map<std::string , std::string>::iterator, bool> exist = vars.tmp_serv.set_cgi(cgi);  
			if (!exist.second)
				throw std::runtime_error(duplicate_key(vars.line));
			vars.location_state.cgi = 2;
			pos++;
		}
	}
	else if (found == 2)
	{
		if ((vars.tokens.size() - pos) != 2)
			throw std::runtime_error(Invalid_arguments(vars.line));
			std::pair<std::string, std::string> cgi(vars.tmp_location.get_path(), vars.tokens[pos]); 
			std::pair<std::map<std::string , std::string>::iterator, bool> exist = vars.tmp_serv.set_cgi(cgi);  
			if (!exist.second)
				throw std::runtime_error(duplicate_key(vars.line));
			vars.location_state.cgi = 2;
			pos = pos + 2;
	}
}

void    location_block(t_parse &vars, MainServer &server)
{
    size_t pos = 0;
    while (pos < vars.tokens.size())
    {
        if (vars.server_state.location == 1 || !vars.tokens[pos].compare("{"))
            left_brace(vars, pos);
        else if (!vars.tokens[pos].compare("autoindex"))
            check_set_location_autoindex(vars, pos);
        else if (!vars.tokens[pos].compare("index"))
            check_set_location_index(vars, pos);
        else if (!vars.tokens[pos].compare("client_max_body_size"))
            check_set_location_client_max_body_size(vars, pos);
        else if (!vars.tokens[pos].compare("access"))
            check_set_location_access(vars, pos);
        else if (!vars.tokens[pos].compare("root"))
        check_set_location_root(vars, pos);
        else if (!vars.tokens[pos].compare("allow_method"))
			check_set_location_allow_method(vars, pos);
        else if (!vars.tokens[pos].compare("CGI_param"))
			check_set_location_cgi(vars, pos);
		else if (!vars.tokens[pos].compare("}"))
			right_brace(vars, pos, server);
        else
            throw std::runtime_error(unexpected_token(vars.tokens[pos]));
    }
}