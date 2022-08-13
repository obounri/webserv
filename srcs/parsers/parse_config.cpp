#include "parse_config.hpp"

void	comment_check(std::string& line)
{
	size_t	found;
	found = line.find('#');
	if (found != std::string::npos)
		line = line.erase(found);
	return ;
}

void	left_brace_check(t_parse &vars , size_t *pos)
{
	size_t cmp = vars.tokens[*pos].compare("{");
	if (vars.server_state.server == 1 && cmp)
		throw std::runtime_error(missing_open_brace(vars.tokens[*pos]));
	else if (vars.server_state.server == 2 && !cmp)
		throw std::runtime_error(duplicate_key(vars.tokens[*pos]));
	else if (!cmp)
	{
		vars.server_state.server = 2;
		(*pos)++;
	}
}

void	listen(t_parse &vars, size_t *pos)
{
	size_t cmp = vars.tokens[*pos].compare("listen");
	if (vars.server_state.server != 2 && !cmp)
		throw std::runtime_error(unexpected_token(vars.tokens[*pos]));
	if (!cmp && vars.tokens[*pos + 1].empty())
		throw std::runtime_error(missing_arguments(vars.tokens[*pos]));
	else if (!cmp)
	{
		std::cout << "listen and their arg found after server \n";
		(*pos)++;
	}
}

void	server_block(t_parse *vars, config *configs)
{
	size_t pos = 0;
	size_t cmp = vars->tokens[pos].compare("server");
	if (vars->server_state.server == 0 && cmp)
		throw std::runtime_error(unexpected_token(vars->tokens[pos]));
	else if (!vars->server_state.server) // if server_state 0
	{
		vars->server_state.server = 1;
		pos++;
		configs->n_v_servers++;
	}
	while (!vars->tokens[pos].empty())
	{
		left_brace_check(*vars, &pos);
		listen(*vars, &pos);
		pos++;
	}
	// if (vars->server_state)
	// {
	// 	size_t pos = vars->line.find_first_of("{");
	// 	if (pos == std::string::npos)
	// 		throw std::runtime_error(unexpected_token(vars->line));
	// }
	return ;
}

void	split_line(t_parse *vars, std::string const delimiters)
{
	size_t begin = 0 , pos;

	while ((pos = vars->line.find_first_of(delimiters, begin)) != std::string::npos)
	{
		if (pos > begin)
			vars->tokens.push_back(vars->line.substr(begin, pos - begin));
		begin = pos + 1;
	}
	if (begin < vars->line.length())
		vars->tokens.push_back(vars->line.substr(begin, std::string::npos));
}

void parse_config(char *config_path)
{
	t_parse vars = {};
	config	configs = {};
	vars.conf_file.open(config_path);
	if (vars.conf_file.is_open())
	{
		while (std::getline(vars.conf_file, vars.line))
        {
			if (!vars.line.empty())
			{
				comment_check(vars.line);
				split_line(&vars, " \t");
				if (!vars.tokens.empty())
					server_block(&vars , &configs);
				for(size_t i= 0; i < vars.tokens.size(); i++)
					std::cout << "token : |" << vars.tokens[i] << "|\n";
				vars.tokens.clear();
			}
        }
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
