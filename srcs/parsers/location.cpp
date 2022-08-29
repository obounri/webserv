
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

void 								location::set_limit_body_size(int &limit) { this->limit_client_body = limit; }

void 								location::set_access(std::string &_access) { this->_access = _access; }


void								location::clear()
{
	this->path.clear();
	this->root.clear();
	this->default_file.clear();
	this->allowed.clear();
	this->body_s.clear();
	this->_access.clear();
	this->index = 0;
	this->limit_client_body = 0;
}


void location::print_location()
{
	std::vector<std::string>::iterator it;
	std::cout << "Path : " + this->path << std::endl;
	std::cout << "root : " + this->root << std::endl;
	std::cout << "access : " + this->_access << std::endl;
	std::cout << "default: ";
	it = default_file.begin();
	while (it != default_file.end())
	{
		std::cout << *it << "\t";
		it++;
	}
	std::cout << std::endl;
	std::cout << "Allowed Methods: ";
	it = allowed.begin();
	while (it != allowed.end())
	{
		std::cout << "\t" << *it;
		it++;
	}
	std::cout << std::endl;
	std::cout << "Limit Body Size: " << limit_client_body << std::endl;
	if (index == 0)
		std::cout << "index: off" << std::endl;
	else
		std::cout << "index: on" << std::endl;
	std::cout << std::endl;
}