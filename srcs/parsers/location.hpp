#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

// SECTION - Location Class

// struct s_parse;
typedef struct s_parse t_parse;

class location
{
	private:
		std::string path;							// Location Name
		std::string root;							// Root Directory
		int index;									// index ON or OFF
		std::vector<std::string> default_file;		// Default Index Files
		std::vector<std::string> allowed;			// Allowed Methods
		std::string body_s;							// Limit Body String
		// int par_error;							// Parse Error Happen
		int limit_client_body;						// max body size
		std::string _access;						// access file path

	public:
		location(/* args */);
		~location();

		// Seters
		void set_path(std::string &path);
		void set_root(std::string &root);
		void set_default(std::string &def);
		void set_index(int index);
		void set_method(std::string &methods);
		void set_limit_body_size(int &limit);
		void set_access(std::string &val);

		// Geters

		std::string get_path() const ;
		std::vector<std::string> get_default() const ;
		std::string get_root() const ;
		std::vector<std::string> get_methods() const ;
		int get_limit() const ;
		int get_index() const ;
		std::string get_access() const ;

		// std::string get_body_s();
		// std::string get_string_methods();
		// int ft_method_check(std::string val);
		// int valid();
		void print_location();
		void	clear();
};

#endif