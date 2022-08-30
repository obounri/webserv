#include "parse_config.hpp"

static void	print_conf(MainServer &server)
{
	std::vector<v_server>::iterator it = server.myvs.begin();
	while (it != server.myvs.end())
	{
		(*it).print_server();
		it++;
	}
}
	// std::cout << "\n=============== configs ==================\n";
	// if (vars.tmp_serv.get_port())
	// 	std::cout << "listen : " << vars.tmp_serv.get_port() << "\n";
	// if (!vars.tmp_serv.get_host().empty())
	// 	std::cout << "host : " << vars.tmp_serv.get_host() << "\n";
	// if (!vars.tmp_serv.get_name().empty())
	// 	std::cout << "server_name : " << vars.tmp_serv.get_name() << "\n";
	// if (!vars.tmp_serv.get_limit())
	// 	std::cout << "client body : " << vars.tmp_serv.get_limit() << "\n";
	// if (!vars.tmp_serv.get_root().empty())
	// 	std::cout << "root : " << vars.tmp_serv.get_root() << "\n";
	// if (!vars.tmp_serv.get_index().empty())
	// 	std::cout << "index : " << vars.tmp_serv.get_index() << "\n";
	// std::cout << "allow methods : ";
	// if (!vars.tmp_serv.get_methods().empty())
	// {
	// 	std::list<std::string> methd = vars.tmp_serv.get_methods();
	// 	std::list<std::string>::iterator lit = methd.begin();
	// 	std::list<std::string>::iterator end = methd.end();
	// 	while (lit != end)
	// 	{
	// 		std::cout << *lit << " ";
	// 		lit++;
	// 	}
	// }
	// std::cout << "\n";
	// if (!vars.tmp_serv.get_error_pg().empty())
	// {
	// 	std::map<int, std::string> error_pg = vars.tmp_serv.get_error_pg();
	// 	for (std::map<int , std::string>::iterator it = error_pg.begin(); it != error_pg.end(); it++)
	// 		std::cout << "error " << (*it).first << "\t" << (*it).second << std::endl;
	// }
	// std::cout << "\n============= LOCATION ===============\n";
	// std::cout << "path : " << vars.tmp_location.get_path() << "\n";
	// std::cout << "limit : " << vars.tmp_location.get_limit() << "\n";
	// std::cout << "autoindex : " << vars.tmp_location.get_index() << "\n";
	// std::cout << "location index : " << (!vars.tmp_location.get_default().empty() ? vars.tmp_location.get_default()[0] : "empty") << "\n";
	// std::cout << "access : " << vars.tmp_location.get_access() << "\n";
	// std::cout << "root : " << vars.tmp_location.get_root() << "\n";
	// std::cout << "allow methods : ";
	// for (size_t i = 0; i < vars.tmp_location.get_methods().size(); i++) { std::cout << vars.tmp_location.get_methods()[i] << " ";}
	// std::cout << "\n";
	// std::cout << "----------------------CGI------------------------\n";
	// std::map<std::string, std::string> cgi = vars.tmp_serv.get_cgi();
	// std::map<std::string, std::string>::iterator it = cgi.begin();
	// std::map<std::string, std::string>::iterator end = cgi.end();
	// while (it != end) { std::cout << "cgi : " << (*it).first << "\t" << (*it).second ;  it++;}
	// std::cout << "\n-------------------------------------------------\n";
	// std::cout << "\n=========================================\n";
// }

void parse_check(t_parse &vars)
{
	if (vars.server_state.server || vars.server_state.location)
		throw std::runtime_error(missing_close_brace("}"));
}

void parse_config(char *config_path, MainServer& server)
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
						location_block(vars, server);
					else
						server_block(vars, server);
					vars.tokens.clear();
				}
			}
        }
		parse_check(vars);
		print_conf(server);
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
