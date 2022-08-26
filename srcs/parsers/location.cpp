
#include "location.hpp"

// SECTION - Constaructors / Destructor

location::location(/* args */):path(), root(), index(0), default_file(), allowed(), body_s(),
	limit_client_body(0), _access() {}

location::~location() {}


// SECTION - Geters

std::string 						location::get_path() const { return (this->path); }

std::string 						location::get_root() const { return (this->root); }

std::vector<std::string> 			location::get_default() const { return (this->default_file); }

int 								location::get_index() const  { return (this->index); }

int									location::get_limit() const { return (this->limit_client_body); }

std::vector<std::string>			location::get_methods() const { return (this->allowed); }

std::string 						location::get_access() const { return (this->_access); }


// SECTION - Seters

void								location::set_path(std::string &path) { this->path = path; }

void								location::set_root(std::string &root) { this->root = root; }

void 								location::set_default(std::string &def) { this->default_file.push_back(def); }

void								location::set_index(int index) { this->index = index; }

void 								location::set_method(std::string &method) { this->allowed.push_back(method); }

void 								location::set_limit_body_size(int &value) { this->limit_client_body = value; }

void 								location::set_access(std::string &_access) { this->_access = _access; }


// SECTION - Functions for Location Block

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

void    check_set_autoindex(t_parse &vars, size_t &pos)
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
            if (t_vec[0].compare("on"))
                vars.tmp_location.set_index(1);
            else if (t_vec[0].compare("off"))
                vars.tmp_location.set_index(0);
            else
                throw std::runtime_error(unexpected_token(vars.tokens[pos]));
            vars.location_state.autoindex = 2;
            pos++;
        }
    }
    else if (found == 2)
    {
        if ((vars.tokens.size()- 1) != 1)
        throw std::runtime_error(Invalid_arguments(vars.line));
        if (vars.tokens[pos].compare("on"))
            vars.tmp_location.set_index(1);
        else if (vars.tokens[pos].compare("off"))
            vars.tmp_location.set_index(0);
        else
            throw std::runtime_error(unexpected_token(vars.tokens[pos]));
        vars.location_state.autoindex = 2;
        pos = pos + 2;
    }
}