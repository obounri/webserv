#include <iostream>


std::string Syntax_error(std::string const &name) { return std::string("Syntax Error : '" + name + "'"); }

std::string unexpected_token(std::string const &name) { return std::string("Syntax Error : unexpected token '" + name + "'"); }

std::string missing_close_brace(std::string const &name) { return std::string("Syntax Error : Missing close brace '" + name + "'"); }

std::string missing_open_brace(std::string const &name) { return std::string("Syntax Error : Missing open brace '" + name + "'"); }

std::string missing_semicolon(std::string const &name) { return std::string("Syntax Error : Missing ';' '" + name + "'"); }

std::string too_many_arguments(std::string const &name) { return std::string("Syntax Error : too many arguments '" + name + "'"); }

std::string no_server(std::string const &name) { return std::string("Syntax Error : no server to fill with '" + name + "'"); }

std::string missing_arguments(std::string const &name) { return std::string("Syntax Error : Missing arguments to '" + name + "'"); }

std::string duplicate_key(std::string  const &name) { return std::string("Syntax Error : duplicate key '" + name + "'"); }

std::string duplicate_location(std::string  const &name) { return std::string("Syntax Error : duplicate location '" + name + "'"); }