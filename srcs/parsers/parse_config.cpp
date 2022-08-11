#include "parse_config.hpp"

void	comment_check(std::string& line)
{
	size_t	found;
	found = line.find('#');
	if (found != std::string::npos)
		line = line.erase(found);
	return ;
}

void server_block(t_parse vars)
{
	size_t found;
	if (!vars.server_stat && !vars.line.empty() && (found = vars.line.find("server")) == std::string::npos)
		throw std::runtime_error(unexpected_token(vars.line));
	// else
	// {
	// 	size_t pos = vars.line.find_first_of("{");
	// 	// if (pos == std::string::npos)
	// 	// 	throw std::runtime_error(unexpected_token(vars.line.substr()));
	// }
	return ;
}

void parse_config(char *config_path)
{
	t_parse vars = {};
	vars.conf_file.open(config_path);
	if (vars.conf_file.is_open())
	{
		while (std::getline(vars.conf_file, vars.line))
        {
			if (!vars.line.empty())
			{
				comment_check(vars.line);
				vars.line.erase(std::remove_if(vars.line.begin(), vars.line.end(), isspace), vars.line.end());
				// server_block(vars);
				std::cout << vars.line << std::endl;
			}
        }
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
