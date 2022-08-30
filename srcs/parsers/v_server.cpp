#include "parse_config.hpp"

// SECTION - Seters


void                                                                v_server::set_port(int &port) { this->port = port; }

void                                                                v_server::set_host(std::string &host) { this->host = host; }

void                                                                v_server::set_index(std::string &index) { this->default_page = index; }

void                                                                v_server::set_name(std::string &name) { this->name = name; }

void                                                                v_server::set_limit(int limit) { this->limit_body = limit; }

void                                                                v_server::set_methods(std::string method) { this->methods.push_back(method); }

void                                                                v_server::set_root(std::string &root) { this->root = root; }

void																v_server::set_location(std::pair<std::string, location> &_location) { this->locations.insert(_location);}


std::pair<std::map<int, std::string>::iterator, bool>       		v_server::set_error_page(std::pair<int, std::string> &error_page)
{
    std::pair<std::map<int , std::string>::iterator, bool> exist = this->error_pg.insert(error_page);
    return exist;
}

std::pair<std::map<std::string , std::string>::iterator, bool> v_server::set_cgi(std::pair<std::string, std::string> &cgi)
{
	std::pair<std::map<std::string , std::string>::iterator, bool> exist = this->cgi.insert(cgi);
	return exist;
}

// Section - Geters

int                                                                 v_server::get_port() { return (this->port); }

std::string                                                         v_server::get_host() { return (this->host); }

std::string                                                         v_server::get_index() { return (this->default_page); }

std::string                                                         v_server::get_name() { return (this->name); }

std::string                                                         v_server::get_root() { return (this->root); }

int                                                                 v_server::get_limit() { return (this->limit_body); }

std::list<std::string>                                              v_server::get_methods() { return (this->methods); }

// std::map<int, std::string> 											v_server::get_error_pg()  { return (this->error_pg); }

// std::map<std::string, std::string>									v_server::get_cgi() { return (this->cgi); }

std::string	v_server::get_error_page(int val)
{

	std::map<int, std::string>::iterator it = this->error_pg.find(val);

	if (it != error_pg.end())
		return (it->second);
	return (std::string());
}

void	v_server::clear()
{
	this->host.clear();
	this->port = 0;
	this->name.clear();
	this->root.clear();
	this->default_page.clear();
	this->limit_body = 0;
	this->limit_body_s.clear();
	this->methods.clear();
	this->locations.clear();
	this->cgi.clear();
}

void 		v_server::print_server()
{
	std::cout << "\n=============================== SERVER ==================================\n";
	std::map<std::string, location>::iterator it;
	std::list<std::string>::iterator lit;
	std::map<std::string, std::string>::iterator mit;
	std::map<int, std::string>::iterator error_it;
	std::cout << "host: " <<  host << std::endl;
	std::cout << "port: " <<  port << std::endl;
	std::cout << "name: " << name << std::endl;
	std::cout << "root: " << root << std::endl;
	std::cout << "\n============= LOCATION ===============\n";
	it = locations.begin();
	while (it != locations.end())
	{
		it->second.print_location();
		it++;
	}
	std::cout << "\n=========================================\n";
	std::cout << "limit body: " << limit_body << std::endl;
	std::cout << "methods: ";
	lit = methods.begin();
	while (lit != methods.end())
	{
		std::cout << *lit << "\t";
		lit++;
	}
	std::cout << std::endl;
	std::cout << "cgi: "<<std::endl;
	mit = cgi.begin();
	while (cgi.end() != mit)
	{
		std::cout << "file ext: " << mit->first << " ---->  cgi_path: " << mit->second << std::endl;
		mit++;
	}
	std::cout << "Error Pages: ";
	error_it = this->error_pg.begin();
	while (error_it != error_pg.end())
	{
		std::cout << error_it->first << " ==> " + error_it->second + "\n";
		error_it++;
	}
	std::cout << "\n============================== SERVER END ===========================\n";
	std::cout << std::endl;
};
