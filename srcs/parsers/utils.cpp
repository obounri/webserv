#include "parse_config.hpp"

bool	IsNumber(std::string &str)
{
	size_t i = 0;

	while (i < str.length())
	{
		if (str[0] == '-' || std::isdigit(str[i]))
			i++;
		else
			return false;
	}
	return true;
}

std::vector<std::string>	split_line(std::string const &line, std::string const &delimiters)
{
	std::vector<std::string> tokens;
	size_t begin = 0 , pos;

	while ((pos = line.find_first_of(delimiters, begin)) != std::string::npos)
	{
		if (pos > begin)
			tokens.push_back(line.substr(begin, pos - begin));
		begin = pos + 1;
	}
	if (begin < line.length())
		tokens.push_back(line.substr(begin, std::string::npos));
	return tokens;
}

bool	Is_IP_Adress(std::string &str)
{
	std::vector<std::string> splited_ip = split_line(str, ".");
	if (splited_ip.size() != 4)
		return false;
	size_t i = 0;
	while (i < splited_ip.size())
	{
		if (!IsNumber(splited_ip[i]) || std::stoi(splited_ip[i]) < 0 || std::stoi(splited_ip[i]) > 255)
			return false;
		i++;
	}
	return true;
}

int		semicolon_check(t_parse &vars, size_t &pos)
{
	size_t found;
	size_t i = pos;
	while (i < vars.tokens.size())
	{
		if ((found = vars.tokens[i].find(";")) == std::string::npos)
			i++;
		else
			break;
	}
	if (i == vars.tokens.size() - 1)
	{
		if (!found && vars.tokens[i].length() == 1)
			return 2;
		else if (found != std::string::npos && found == vars.tokens[i].length() - 1)
			return 1;
	}
    return 0;
}