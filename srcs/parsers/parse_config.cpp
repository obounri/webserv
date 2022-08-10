#include <vector>

#include "parse_config.hpp"

void parse_config(char *config_path)
{
	std::ifstream   conf_file;
	std::string     line;
	conf_file.open(config_path);
	if (conf_file.is_open())
	{
		while (std::getline(conf_file, line))
        {
			if (!line.empty())
				std::cout << line << std::endl;
        }
	}
	else
		std::cerr << "File not found\n";
	conf_file.close();
	return ;
}