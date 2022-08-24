#include "parse_config.hpp"

void    check_set_autoindex(t_parse &vars, size_t &pos)
{
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
                vars.tmp_location.autoindex = true;
            else if (t_vec[0].compare("off"))
                vars.tmp_location.autoindex = false;
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
                vars.tmp_location.autoindex = true;
        else if (vars.tokens[pos].compare("off"))
            vars.tmp_location.autoindex = false;
        else
            throw std::runtime_error(unexpected_token(vars.tokens[pos]));
        vars.location_state.autoindex = 2;
        pos = pos + 2;
    }
}

void    autoindex(t_parse &vars, size_t &pos)
{
    if (!vars.location_state.autoindex)
    {
        vars.location_state.autoindex = 1;
        pos++;
    }
    check_set_autoindex(vars, pos);
}

void    location_block(t_parse &vars)
{
    size_t pos = 0;
    while (pos < vars.tokens.size())
    {
        if (vars.server_state.location == 1 || !vars.tokens[pos].compare("{"))
            left_brace(vars, pos);
        else if (!vars.tokens[pos].compare("autoindex"))
            autoindex(vars, pos);
        else if (!vars.tokens[pos].compare("index"))
            index(vars, pos, vars.tmp_location.index, vars.location_state.index);
        else if (!vars.tokens[pos].compare("allow_method"))
        {
            std::cout << "allow_method found\n"; pos++;
        }
        else if (!vars.tokens[pos].compare("return"))
        {
            std::cout << "return found\n"; pos++;
        }
        else if (!vars.tokens[pos].compare("root"))
        {
            std::cout << "root found\n"; pos++;
        }
        else
            throw std::runtime_error(unexpected_token(vars.tokens[pos]));
    }
}