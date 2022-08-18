#include "parse_config.hpp"

void	comment_check(std::string& line)
{
	size_t	found;
	found = line.find('#');
	if (found != std::string::npos)
		line = line.erase(found);
	return ;
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
				vars.tokens = split_line(vars.line, " \t");
				if (!vars.tokens.empty())
					server_block(vars , configs);
				for(size_t i= 0; i < vars.tokens.size(); i++)
					std::cout << "token : |" << vars.tokens[i] << "|\n";
				vars.tokens.clear();
			std::cout << "port : " << vars.tmp_server.port << "\n";
			std::cout << "host : " << vars.tmp_server.host << "\n";
			}
        }
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
