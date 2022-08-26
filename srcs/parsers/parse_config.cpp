#include "parse_config.hpp"

static void	print_conf(t_parse &vars)
{
	std::cout << "\n=============== configs ==================\n";
	if (vars.tmp_server.port)
		std::cout << "listen : " << vars.tmp_server.port << "\n";
	if (!vars.tmp_server.host.empty())
		std::cout << "host : " << vars.tmp_server.host << "\n";
	if (!vars.tmp_server.server_name.empty())
		std::cout << "server_name : " << vars.tmp_server.server_name << "\n";
	if (!vars.tmp_server.client_max_body_size)
		std::cout << "client body : " << vars.tmp_server.client_max_body_size << "\n";
	if (!vars.tmp_server.root.empty())
		std::cout << "root : " << vars.tmp_server.root << "\n";
	if (!vars.tmp_server.index.empty())
		std::cout << "index : " << vars.tmp_server.index << "\n";
	if (!vars.tmp_server.error_pages.empty())
	{
		for (std::map<int , std::string>::iterator it = vars.tmp_server.error_pages.begin(); it != vars.tmp_server.error_pages.end(); it++)
			std::cout << "error " << (*it).first << "\t" << (*it).second << std::endl;
	}
	if (!vars.tmp_location.get_index())
		std::cout << "autoindex off\n";
	else
		std::cout << "autoindex on\n";
	if (!vars.tmp_location.get_default().empty())
	std::cout << "location index : " << vars.tmp_location.get_default()[0] << "\n";
	std::cout << "\n=========================================\n";
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
				{
					if (vars.server_state.location)
						location_block(vars);
					else
						server_block(vars, configs);
					vars.tokens.clear();
					print_conf(vars);
				}
			}
        }
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
