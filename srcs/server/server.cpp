#include "server.hpp"

MainServer::MainServer(config data)
{
    struct kevent evSet;

    std::cout << "constructing server.." << std::endl;
    if ((keq = kqueue()) == -1)
        throw std::runtime_error("kqueue failed..");
    for (int i = 0; i < data.n_v_servers; i++) {
        listeners.push_back(new Socket(data.domain, data.type, data.interface, atoi(data.vservers[i].get_port().c_str()), data.backlog));
        EV_SET(&evSet, listeners[i]->get_socket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

int     MainServer::is_listener(unsigned long int fd)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
        if (listeners[i]->get_socket() == fd)
            return (1);
    return (0);
}

MainServer::~MainServer()
{
    std::cout << "server destructor called.." << std::endl;
    close(keq);
    for (unsigned int i = 0; i < listeners.size(); i++)
        delete listeners[i];
    std::cout << std::endl << "server shut down, take care :).." << std::endl;
}

void    MainServer::accept_new_connection(unsigned long int fd) 
{
    struct kevent evSet;
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    addr_size = sizeof their_addr;

    newfd = accept(fd, (sockaddr *)&their_addr, &addr_size);
    clients.push_back(client(newfd, inet_ntoa(their_addr.sin_addr)));
    EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &clients[clients.size()-1]);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    std::cout << "Got connection request from fd = " << newfd << ".." << inet_ntoa(their_addr.sin_addr) << std::endl;
    std::cout << ntohs(their_addr.sin_port) << std::endl << std::endl;
}

void    MainServer::destroy_connection(int fd, int event)
{
    struct kevent evSet;

    std::cout << "client " << fd << " disconnected.." << std::endl << std::endl;
    EV_SET(&evSet, fd, event, EV_DELETE, 0, 0, NULL);
    kevent(keq, &evSet, 1, NULL, 0, NULL);
    close(fd);
}

void    MainServer::recv_request(client *c)
{
    size_t pos;
    size_t rec;
    struct kevent evSet;
    std::string tmp;

    bzero(c->recBuffer, MAX_IO_SIZE);
    if ((rec = recv(c->fd, c->recBuffer, MAX_IO_SIZE - 1, 0)) > 0) {
        c->client_request.append(c->recBuffer);
        std::cout << " << " << rec;
    }
    else {
        std::cout << "reading failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_READ);
        return ;
    }
    if ((pos = c->client_request.find("\r\n\r\n")) != std::string::npos) {
        if (c->header.empty()) {
            c->header = c->client_request.substr(0, pos);
            c->client_request.erase(0, pos + 4);
            c->body_len = 0;
            if ((pos = c->header.find("Content-Length: ")) != std::string::npos) {
				tmp = c->header;
				tmp.erase(0, pos + 16);
				c->body_len = atoi(tmp.substr(0, tmp.find("\r\n")).c_str());
			}
        }
    }
    if (!c->header.empty() && (c->body_len) == c->client_request.length()) {
        std::cout << "\ncompleted request from fd " << c->fd << std::endl << "HEADER:\n" << c->header << std::endl << "BODY\n" << c->client_request << std::endl;
        c->client_request.clear();
        c->header.clear();
        c->client_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 3280\r\n\r\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Sed velit velit, convallis vel hendrerit ut, facilisis scelerisque lorem. Praesent quis lectus eu lorem tincidunt iaculis nec ac diam. Quisque scelerisque felis felis, nec varius ipsum placerat nec. Donec faucibus tellus a sollicitudin pretium. In ornare aliquet pretium. Donec vestibulum ut enim id mattis. Morbi ullamcorper auctor leo, a ultricies nunc maximus sit amet. Donec facilisis orci tortor, cursus tincidunt sem sodales tristique. Nam lacinia, mi ut aliquam varius, risus massa scelerisque tortor, id tristique ipsum dui placerat enim. Mauris vehicula commodo ex vel varius. Sed molestie ultricies sodales. Phasellus quam sem, sagittis non ornare et, posuere sit amet ligula. Vivamus dignissim mi sit amet sapien suscipit, sed sollicitudin metus faucibus. Nullam vitae ultricies ante. Quisque faucibus mi sit amet ligula suscipit tincidunt. Nunc sed augue pharetra, efficitur velit blandit, interdum est.\nSed suscipit viverra erat, sollicitudin fringilla orci tempus et. Vestibulum pellentesque nulla sed scelerisque venenatis. Cras augue velit, dapibus vel luctus sit amet, interdum ut erat. Praesent ac eros at tellus egestas luctus vitae vel nisi. Aliquam erat volutpat. Fusce felis neque, dignissim ac commodo non, euismod in est. Mauris maximus ac purus vel convallis. Sed efficitur vitae mauris ut tristique. Nam mi augue, elementum quis mi et, mattis lacinia leo. Etiam tempor hendrerit enim vel venenatis. Nullam molestie metus et sem ultricies elementum.\nPellentesque eget tortor vel neque rutrum sodales. Morbi vitae cursus nibh. Curabitur quis magna eu lorem venenatis tempus non non leo. Duis eget suscipit neque. Etiam finibus nunc massa, vel venenatis elit imperdiet fringilla. Pellentesque non quam vitae arcu pretium ullamcorper eu at metus. Donec euismod interdum sem, a volutpat nulla molestie quis. Interdum et malesuada fames ac ante ipsum primis in faucibus. Sed rhoncus turpis sit amet tellus aliquam, vel fringilla odio eleifend. Ut non ultrices mi. Maecenas diam augue, semper cursus ultricies at, hendrerit ac mi.\nSed elementum faucibus arcu, sed consequat metus luctus sed. Ut non metus risus. Nam eget blandit diam, at ultricies elit. Sed eget tortor vel erat varius semper eget sit amet neque. Vivamus dignissim maximus nulla, nec semper sapien luctus at. Curabitur ac lorem id odio fringilla sollicitudin. Nullam lobortis varius neque, eu feugiat purus faucibus quis. Donec scelerisque elit ut lacus ullamcorper, quis accumsan sapien elementum. Praesent interdum et ligula lacinia accumsan. Sed ligula enim, cursus ut erat ac, tristique ullamcorper erat.\nQuisque dictum sodales tortor in pulvinar. Sed malesuada sollicitudin felis. Vestibulum hendrerit mi metus, vel dictum diam fermentum vel. Morbi vestibulum sodales sapien, nec sagittis odio porttitor sit amet. Mauris at urna ultricies, tempus magna ac, consequat justo. Etiam suscipit leo nisi, eget bibendum dolor elementum a. Vestibulum id orci massa. Ut ligula leo, malesuada in imperdiet vehicula, facilisis laoreet erat. Aenean hendrerit quis arcu eget luctus. Nullam ornare urna consequat ligula ultricies pharetra. Nulla aliquet dictum tortor ac sodales. Quisque nec ipsum leo. Ut varius enim quis accumsan fermentum.\r\n\r\n";
        c->sent = 0;
        EV_SET(&evSet, c->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
        EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, c);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

void    MainServer::send_request(client *c) {
    int sent = 0, to_send = c->client_response.length() - c->sent;
    struct kevent evSet;

    if (to_send > MAX_IO_SIZE)
        to_send = MAX_IO_SIZE;
    if ((sent = send(c->fd, c->client_response.c_str() + c->sent, to_send, 0)) != -1) {
        std::cout << " >> " << sent;
        c->sent += sent;
    }
    else {
        std::cout << "sending failed.." << std::endl << std::endl;
        destroy_connection(c->fd, EVFILT_WRITE);
        return ;
    }
    std::cout << std::endl << c->sent << std::endl;
    if (c->sent == c->client_response.length()) {
        std::cout << "\nSent full request" << std::endl;
        EV_SET(&evSet, c->fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
        EV_SET(&evSet, c->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, c);
        kevent(keq, &evSet, 1, NULL, 0, NULL);
    }
}

void MainServer::run() {
    struct kevent evList[32];
    int num_events;

    std::cout << "server up and running.." << std::endl << std::endl;
    for(;;) {
        if ((num_events = kevent(keq, NULL, 0, evList, 32, NULL)) == -1)
            throw std::runtime_error("");
        for (int i = 0; i < num_events; i++) {
            if (is_listener(evList[i].ident))
                accept_new_connection(evList[i].ident);
            else if (evList[i].flags & EV_EOF)
                destroy_connection(evList[i].ident, EVFILT_READ);
            else if (evList[i].filter == EVFILT_READ)
                recv_request((client *)evList[i].udata);
            else if (evList[i].filter == EVFILT_WRITE)
                send_request((client *)evList[i].udata);
        }
    }
}

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/uio.h>
// #include <fstream>
// #include <dirent.h>
// #include <sys/wait.h>

v_server::v_server(/* args */):default_page(""),par_error(0),page_error_val(0){};
v_server::~v_server(){};

// void v_server::set_host(std::string str){this->host = str;};
void v_server::set_port(std::string str){this->port = str;}
// void v_server::set_name(std::string str){this->name = str;}
// void v_server::set_limit(std::string str){
// 	limit_body_s = str;
// 	this->limit_body = ft_atoi(str.c_str());
// }
// void v_server::set_methods(std::string str){
// 	std::vector<std::string> res = ft_split(str, ", ");
// 	std::vector<std::string>::iterator it = res.begin();
// 	while (it != res.end() && par_error != 1)
// 	{
// 		if (*it != "GET" && *it != "HEAD" && *it != "POST" && *it != "PUT" && *it != "DELETE" && *it != "CONNECT" && *it != "OPTIONS" && *it != "TRACE" && *it != "PATCH")
// 			par_error = 1;
// 		this->methods.push_back(*it);
// 		it++;
// 	}
// 	this->methods.sort();
// 	this->methods.unique();
// }
// void v_server::set_cgi(std::string name, std::string path){
// 	cgi.insert(std::make_pair(name, path));	
// }

// void v_server::cgi_parse(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end){
// 	std::vector<std::string> res;
// 	std::string name;
// 	std::string path;

// 	res = ft_split(*it, ":");
// 	if (res.size() != 2)
// 	{
// 		par_error = 1;
// 		return ;
// 	}
// 	name = res.back();
// 	it++;
// 	if ((*it) != "{")
// 	{
// 		par_error = 1;
// 		return ;
// 	}
// 	it++;
// 	while (it != end && *it != "}")
// 	{
// 		res = ft_split(*it, "=");
// 		if (res.front() == "cgi_uri" && path.size() == 0)
// 			path = res.back();
// 		else if (res.front() != "root" && res.front() != "allowed_methods")
// 		{
// 			par_error = 1;
// 			return ;
// 		}
// 		it++;
// 	}
// 	if (*it != "}" || cgi.find(name) != cgi.end())
// 	{
// 		par_error = 1;
// 		return ;
// 	}
// 	set_cgi(name, path);
// }

// headers v_server::get_header_var(void)
// {
// 	return (header_var);
// }

// std::string v_server::get_error_page(int val)
// {
// 	std::string ret;
// 	std::map<std::string, std::string>::iterator it = error_pg.find(tostring(val));
// 	ret.clear();
// 	if (it != error_pg.end())
// 		return (it->second);
// 	return (ret);
// }

// std::string v_server::get_body_s()
// {
// 	return (limit_body_s);
// }

// void v_server::set_location(std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end){
// 	std::vector<std::string> res;
// 	std::vector<std::string>::iterator vit;
// 	location var;
// 	int loc_size;

// 	res = ft_split(*it, ":");
// 	vit = res.begin();
// 	vit++;
// 	par_error = 0;
// 	if ((*vit).find("*.") != std::string::npos)
// 	{
// 		cgi_parse(it, end);
// 	}
// 	if ((*vit)[0] != '/')
// 	{
// 		par_error = 1;
// 		return ;
// 	}
// 	var.set_path(*vit);
// 	it++;
// 	if (*it != "{")
// 	{
// 		par_error = 1;
// 		return ;
// 	}
// 	it++;
// 	while (it != end && *it != "}" && par_error == 0)
// 	{
// 		res = ft_split(*it, "=");
// 		if (res.size() != 2)
// 			par_error = 1;
// 		if (res.front() == "default")
// 			var.set_default(res.back());
// 		else if (res.front() == "root")
// 			var.set_root(res.back());
// 		else if (res.front() == "autoindex")
// 			var.set_index(res.back());
// 		else if (res.front() == "allowed_methods")
// 			var.set_methods(res.back());
// 		else if (res.front() == "limit_client_body")
// 			var.set_limit_body_size(res.back());
// 		else if (res.front() == "access")
// 			var.set_access(res.back());
// 		else if (res.front() != "cgi_uri")
// 			par_error = 1;
// 		it++;
// 	}
// 	if (var.valid() != 1)
// 		par_error = 1;
// 	else
// 	{
// 		loc_size = locations.size();
// 		locations.insert(std::make_pair(var.get_path(), var));
// 		if (loc_size == (int)locations.size())
// 			par_error = 1;
// 	}
// };
// void v_server::set_error(std::vector<std::string> res){
// 	std::vector<std::string>::iterator it = res.begin();
// 	std::vector<std::string> locs;
// 	it++;
// 	locs = ft_split(*it, ",");
// 	it = locs.begin();
// 	while (it != locs.end())
// 	{
// 		error_pg[*it] = res.back();
// 		it++;
// 	}
// };

// std::string v_server::get_host(){return (this->host);};
std::string v_server::get_port(){return (this->port);};
// int v_server::get_limit(){return (this->limit_body);};
// std::list<std::string> v_server::get_methods(){return (this->methods);};
// std::string v_server::get_name(){return (this->name);};

// bool v_server::check(){
// 	if (par_error)
// 		return false;
// 	if (!is_digits(port) || !check_host(host))
// 		return false;
// 	return true;
// };
// void v_server::print_server(){
// 	std::map<std::string, location>::iterator it;
// 	std::list<std::string>::iterator lit;
// 	std::map<std::string, std::string>::iterator mit;
// 	std::cout << "host: " + host << std::endl;
// 	std::cout << "port: " + port << std::endl;
// 	std::cout << "name: " + name << std::endl;
// 	std::cout << "root: " + root << std::endl;
// 	std::cout << "location: ";
// 	it = locations.begin();
// 	while (it != locations.end())
// 	{
// 		it->second.print_location();
// 		it++;
// 	}
// 	std::cout << std::endl;
// 	std::cout << "limit body: " << limit_body << std::endl;
// 	std::cout << "methods: ";
// 	lit = methods.begin();
// 	while (lit != methods.end())
// 	{
// 		std::cout << *lit << "\t";
// 		lit++;
// 	}
// 	std::cout << std::endl;
// 	std::cout << "cgi: "<<std::endl;
// 	mit = cgi.begin();
// 	while (cgi.end() != mit)
// 	{
// 		std::cout << "file ext: " << mit->first << " ---->  cgi_path: " << mit->second << std::endl;
// 		mit++;
// 	}
// 	std::cout << "Error Pages: ";
// 	mit = error_pg.begin();
// 	while (mit != error_pg.end())
// 	{
// 		std::cout << mit->first + " ==> " + mit->second + "\n";
// 		mit++;
// 	}
// 	std::cout << std::endl;
// };

// void v_server::update_port(std::string val)
// {
// 	this->port = val;
// }

// void v_server::reset_header(){

// };

// std::string v_server::get_method(){
// 	return (req_method);
// };

// void v_server::set_pg_val(int val)
// {
// 	page_error_val = val;
// }

// void v_server::set_client_ip(std::string str)
// {
// 	client_ip = str;
// }

// void v_server::set_header(char *str){
// 	std::string cpy(str);
// 	std::vector<std::string> res;
// 	std::vector<std::string>::iterator it;
// 	std::vector<std::string> content;
// 	std::vector<std::string> met;
// 	std::string HTTP;
// 	char buff[100];
// 	time_t rawtime;
// 	struct tm * timeinfo;

// 	qu_len = 0;
// 	header_var.reset_header();
// 	time (&rawtime);
// 	timeinfo = localtime (&rawtime);
// 	strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
// 	header_var.push("Date", buff);
// 	res = ft_split(cpy, "\r\n");

// 	it = res.begin();
// 	file_open = f_log.file;
// 	if (f_log.valid == 1 && (f_log.req != "PUT" && f_log.req != "POST"))
// 	{
// 		header_var.push("Content-Length", tostring(f_log.size));
// 		timeinfo = localtime (&f_log.st.st_mtime);
// 		strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
// 	}
// 	header_var.push("Content-Location", f_log.first_req);
// 	if (f_log.valid == 1)
// 		header_var.push("Last-Modified", buff);
// 	header_var.push("Content-Type", get_file_type());
// 	header_var.push("Transfer-Encoding", "identity");
// 	header_var.push("Connection", "keep-alive");
// 	while (it != res.end())
// 	{
// 		content = ft_split(*it, ": ");
// 		if (content.front() == "Accept-Encoding")
// 			header_var.push("Accept-Encoding", content.back());
// 		if (content.front() == "Connection")
// 			header_var.push("Connection", content.back());
// 		if (content.front() == "Accept-Charsets")
// 			header_var.push("Accept-Charsets", content.back());
// 		if (content.front() == "Content-Type" || content.front() == "Content-type")
// 			header_var.push("Content-Type", &(*it)[(*it).find(":") + 2]);
// 		else if (content.front() == "Authorization")
// 			header_var.push("Authorization", content.back());
// 		else if (content.front() == "Content-Language")
// 			header_var.push("Content-Language", content.back());
// 		else if (content.front() == "Host")
// 			header_var.push("Host", &(*it)[(*it).find(":") + 2]);
// 		else if (content.front() == "Location")
// 			header_var.push("Location", content.back());
// 		else if (content.front() == "Referer")
// 			header_var.push("Referer", content.back());
// 		else if (content.front() == "Retry-After")
// 			header_var.push("Retry-After", content.back());
// 		else if (content.front() == "Server")
// 			header_var.push("Server", content.back());
// 		else if (content.front() == "User-Agent")
// 			header_var.push("User-Agent", content.back());
// 		else if (content.front() == "Authorization")
// 			header_var.push("WWW-Authenticate", &(*it)[(*it).find(":") + 2]);
// 		else if (content.front() == "Content-Length" && (f_log.valid != 1 || f_log.req == "POST" || f_log.req == "PUT"))
// 			header_var.push("Content-Length", content.back());
// 		else if (content.front() == "Content-length")
// 			header_var.push("Content-Length", content.back());
// 		else if (content.front() == "Transfer-Encoding")
// 			header_var.push("Transfer-Encoding", content.back());
// 		else if (content.size() == 2 && content.front() != "Status")
// 			extra_headers.insert(std::make_pair(content.front(), content.back()));
// 		else if (content.size() > 1 && content.front() != "Status")
// 			extra_headers.insert(std::make_pair(content.front(),  &(*it)[(*it).find(":") + 2]));
// 		it++;
// 	}
// 	if (req_method == "PUT" || req_method == "POST")
// 	{
// 		if (header_var.get_key("Content-Length").size())
// 			qu_len = ft_atoi(header_var.get_key("Content-Length").c_str());
// 		else
// 			qu_len = 0;
// 	}
// 	else if (req_method == "OPTIONS")
// 		header_var.push("Allow", get_string_methods());
// };

// int v_server::get_fd_server()
// {
// 	return (this->server_fd);
// };
// int v_server::get_pg_error(){
// 	return (this->page_error_val);
// };
// struct sockaddr_in v_server::get_server_address(){
// 	return (this->address);
// };

// int v_server::ft_access()
// {
// 	std::string ret;
// 	std::map<std::string, location>::iterator it = locations.find(f_log.location);
// 	std::string file;
// 	std::string keys;
// 	struct stat st;
// 	int fd = 0;

// 	if (it == locations.end() || it->second.get_access().size() == 0)
// 		return (1);
// 	file = it->second.get_access();
// 	ret = header_var.get_key("Authorization");
// 	if (ret.size() != 0)
// 	{
// 		if (stat(file.c_str(), &st) == 0 && st.st_size > 0)
// 		{
// 			fd = open(file.c_str(), O_RDONLY);
// 			if (fd > 0)
// 			{
// 				char buff[st.st_size + 1];
// 				ft_bzero(buff, st.st_size);
// 				read(fd, buff, st.st_size);
// 				close(fd);
// 				file.clear();
// 				file = buff;
// 				keys = base64_decode(ret);
// 				keys += "\n";
// 				if (file.find(keys) != std::string::npos)
// 				{
// 					header_var.push("Authorization", "");
// 					header_var.push("WWW-Authenticate", "");
// 					return (1);
// 				}
// 			}
// 		}
// 	}
// 	set_pg_val(401);
// 	return (0);
// }

// void v_server::run_server()
// {
// 	int var = 1;
// 	int bind_ret = 0;
// 	int val = 1;

// 	std::string full_req;

// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
// 	{ 
// 		std::cerr << RED << "Socket Faild: " << strerror(errno) << RESET << "\n";
// 		exit(-1);
// 	}
// 	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(var)))
// 	{ 
// 		std::cerr << RED << "setsockopt Faild: " << strerror(errno) << RESET << "\n";
// 		exit(-1);
// 	}
// 	address.sin_family = AF_INET;
// 	if (host == "localhost" || host.size() == 0)
// 		host = "0.0.0.0";
// 	address.sin_addr.s_addr = inet_addr(host.c_str());
// 	address.sin_port = htons(value_port());
// 	while (val) 
// 	{
// 		if ((bind_ret = bind(server_fd, (struct sockaddr *)&address, sizeof(address))) == -1)
// 		{	
// 			std::cerr << BLACK << "Bind Faild " << "(" << value_port() << "): " << strerror(errno) << RESET <<"\n";
// 			val++;
// 			if (val > 3)
// 				break;;
// 		}
// 		else
// 			break;
// 	}
// 	if (listen(server_fd, SOMAXCONN) < 0) 
// 	{ 
// 		std::cerr << RED << "Listen Faild: " << strerror(errno) << RESET << "\n";
// 		exit(-1);
// 	}
// 	if (val == 1)
// 		std::cerr << GREEN << "Server: " << host << ":" << port << " Running..."<< RESET << "\n";
// 	else
// 		std::cerr << RED << "Server: " << host << ":" << port <<  " Couldn't Run..."<< RESET << "\n";
// }

// bool v_server::ft_allowed()
// {
// 	if (f_log.location.size() != 0)
// 	{
// 		location loc = locations.find(f_log.location)->second;
// 		std::vector<std::string> m = loc.get_methods();
// 		std::vector<std::string>::iterator it = m.begin();
// 		std::vector<std::string>::iterator e_it = m.end();
// 		while (it != e_it)
// 		{
// 			if (req_method == *it)
// 				return true;
// 			it++;
// 		}
// 	}
// 	else
// 	{
// 		std::list<std::string>::iterator it;
// 		std::list<std::string>::iterator e_it;
// 		it = methods.begin();
// 		e_it = methods.end();
// 		while (it != e_it)
// 		{
// 			if (req_method == *it)
// 				return true;
// 			it++;
// 		}
// 	}
// 	return false;
// }

// std::string v_server::options_method(){
// 	std::string response;
	
// 	header_var.push("Content-Length", "");
// 	f_log.set_size();
// 	if (f_log.valid == -1)
// 		header_var.set_http(204, "No Content");
// 	response = header_var.header_msg() + "\r\n";
// 	header_var.reset_header();
// 	return (response);
// };

// std::string v_server::delete_method()
// {
// 	int i = 0;
// 	std::string response;
	
// 	header_var.push("Content-Length", "");
// 	header_var.push("Last-Modified", "");
// 	f_log.set_size();
// 	if (f_log.valid == -1)
// 		header_var.set_http(204, "No Content");
// 	else
// 	{
//   		if(remove(f_log.file.c_str()) != 0)
//     		header_var.set_http(202, "Accepted");
//     	else
// 		{
// 			header_var.set_http(200, "OK");
// 			header_var.push("Content-Length", "7");
// 			i = 1;
// 		}
// 	}
// 	response = header_var.header_msg() + "\r\n";
// 	if (i)
// 		response += "Success\r\n";
// 	header_var.reset_header();
// 	return (response);
// };

// std::string v_server::put_method()
// {
// 	int fd;
// 	std::string response;
	
// 	f_log.set_size();
// 	if ((f_log.size == 0 && f_log.valid == -1))
// 		header_var.set_http(201, "Created");
// 	fd = open(f_log.file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
// 	if (fd > 0)
// 		pending_write(fd, qu_string);
// 	close(fd);
// 	header_var.push("Content-Length", "7");
// 	header_var.push("Transfer-Encoding", "");
// 	response = header_var.header_msg() + "\r\n" + "Success";
// 	header_var.reset_header();
// 	return (response);
// }

// void v_server::set_root(std::string r)
// {
// 	this->root = r;
// }

// std::string v_server::get_root()
// {
// 	return this->root;
// }

// void v_server::set_qu_string(std::string str){
// 	qu_string = str;	
// }

// void v_server::set_pwd(std::string p){
// 	pwd_dir = p;
// };

// void v_server::set_qu_len(std::string val){
// 	qu_len = ft_atoi(val.c_str());
// };


// std::string v_server::ft_cgi_exist()
// {
// 	std::map<std::string, std::string>::reverse_iterator it = cgi.rbegin();
// 	std::vector<std::string> res;
// 	std::string line;
// 	std::string q_req;
// 	size_t num = 0;

// 	while (it != cgi.rend())
// 	{
// 		res = ft_split(it->first, "*");
// 		std::map<std::string, location>::iterator m = locations.find(it->first);
// 		if (res.size() == 2 && m->second.ft_method_check(f_log.req))
// 		{
// 			q_req = f_log.first_req.substr(0, f_log.first_req.find("?"));
// 			if (!ft_strncmp(res.front().c_str(), q_req.c_str(), res.front().size()))
// 			{
// 				if ((num = q_req.find(res.back())) == std::string::npos)
// 					return "";
// 				line = q_req.substr(num, (q_req.size() - num + 1));
// 				if (strcmp(line.c_str(), res.back().c_str()))
// 					return "";
// 				f_log.file = m->second.get_root() + "/" + &q_req[res.front().find_last_of("/") + 1];
// 				f_log.location = it->first;
// 				if (f_log.first_req.find("?") != std::string::npos)
// 					f_log.q_string = &f_log.first_req[f_log.first_req.find("?") + 1];
// 				f_log.s_file = q_req;
// 				return it->first;
// 			}
// 		}
// 		it++;
// 	}
// 	return "";
// }

// void v_server::update_location()
// {
// 	std::map<std::string ,location>::iterator it = locations.begin();
// 	while (it != locations.end())
// 	{
// 		if (it->second.get_root().size() == 0)
// 			it->second.set_root(root);
// 		if (it->second.get_methods().size() == 0)
// 			it->second.set_methods(get_string_methods());
// 		if (it->second.get_body_s().size() == 0)
// 			it->second.set_limit_body_size(tostring(MAX_BODY));
// 		it++;
// 	}
// }

// std::string v_server::get_name_headers(std::map<std::string, std::string>::iterator it)
// {
// 	std::string ret;

// 	ret = it->first;

// 	for (size_t i = 0; i < ret.size(); i++)
// 	{
// 		if (ret[i] >= 'a' && ret[i] <= 'z')
// 			ret[i] -= 32;
// 		if (ret[i] == '-')
// 			ret[i] = '_';
// 	}
// 	return (ret);
// }

// cgi_env v_server::ft_add_other_headers()
// {
// 	std::string tmp;
// 	cgi_env var(extra_headers.size());
// 	std::map<std::string, std::string>::iterator it = extra_headers.begin();
	
// 	while (it != extra_headers.end())
// 	{
// 		tmp = get_name_headers(it);
// 		if (var.check_key(tmp))
// 			var.push("HTTP_" + tmp + "=", it->second);
// 		it++;
// 	}
// 	return (var);
// }

// char ** v_server::setup_env()
// {
// 	cgi_env var(extra_headers.size());
// 	std::string ret;
// 	std::vector<std::string> tmp;

// 	var = ft_add_other_headers();
// 	ret = header_var.get_key("WWW-Authenticate");
// 	if (ret.size())
// 	{
// 		tmp = ft_split(ret, " ");
// 		if (tmp.size() == 2)
// 		{
// 			var.push("AUTH_TYPE=", tmp.front()); // first part of it : basic or digest
//     		var.push("REMOTE_USER=", tmp.back()); // second part of authanti.. the User-ID
// 		}
// 	}
//     var.push("CONTENT_TYPE=", header_var.get_key("Content-Type"));
// 	var.push("PATH_INFO=", f_log.s_file);
//     var.push("PATH_TRANSLATED=", f_log.file);
//     var.push("QUERY_STRING=", f_log.q_string); //POST req after The "?"
//     var.push("CONTENT_LENGTH=",  tostring(qu_len)); //
//     var.push("REMOTE_ADDR=", client_ip); //ip of user 
//     var.push("REMOTE_IDENT=", header_var.get_key("User-Agent"));
// 	var.push("REQUEST_METHOD=", f_log.req);
// 	var.push("REQUEST_URI=", f_log.first_req);
// 	var.push("SERVER_NAME=", name);
// 	var.push("SERVER_PORT=", port);
// 	var.push("SCRIPT_FILENAME=", f_log.file);
// 	var.push("SCRIPT_NAME=", f_log.file);
	
// 	// var.print_cgi();
// 	return (var.get_env());
// }

// std::string v_server::run_cgi()
// {
// std::map<std::string, std::string>::iterator it;
//     int id;
//     char store[65536] = {0};
//     int ret;
//     std::string response;
//     FILE *Fin = tmpfile();
//     FILE *Fout = tmpfile();
//     char *tab[3];
//     int fdin = fileno(Fin);
//     int fdout = fileno(Fout);
//     int out = dup(STDOUT_FILENO);
//     int in = dup(STDIN_FILENO);
// 	std::string sent_response;
// 	int status = 0;


//     it = cgi.find(f_log.location);
//     tab[0] = (char*)it->second.c_str();
//     tab[1] = (char*)file_open.c_str();
//     tab[2] = NULL;
//     ret = 1;
//     ft_write_string(fdin, qu_string);
//     lseek(fdin, 0, SEEK_SET);
//     id = fork();
//     if (id == -1)
//         std::cerr << RED << "Fork crashed." << RESET << std::endl;
//     else if (!id)
//     {
//         dup2(fdin, STDIN_FILENO);
//         dup2(fdout, STDOUT_FILENO);
// 		close(fdin);
// 		close(fdout);
//         execve(it->second.c_str(), tab, setup_env());
//         std::cerr << RED << "execve crashed: "  << strerror(errno) << RESET << std::endl;
// 		exit(-5);
//     }
//     else
//     {
//         waitpid(-1, &status, 0);
// 		if (status == 0)
// 		{
// 			lseek(fdout, 0, SEEK_SET);
// 			ret = 1;
// 			while (ret > 0)
// 			{
// 				ft_bzero(store, 65536);
// 				ret = read(fdout, store, 65535);
// 				response += store;
// 			}
// 			if (ret == -1)
// 				ft_write_string(2, "READ ERROR\n");
// 		}
//     }
//     dup2(in, STDIN_FILENO);
//     dup2(out, STDOUT_FILENO);
//     fclose(Fin);
//     fclose(Fout);
//     close(in);
//     close(out);
//     close(fdin);
//     close(fdout);
// 	if (status || id == -1)
// 		return (run_error(500));
// 	std::string tmp = &response[response.find("\r\n\r\n") + 4];
//     header_var.reset_header();
//     set_header((char*)response.c_str());
//     header_var.push("Accept-Charsets", "");
//     header_var.push("Accept-Encoding", "");
//     header_var.push("Content-Language", "");
//     header_var.push("Accept-Language", "");
//     header_var.push("Last-Modified", "");
// 	header_var.push("Date", "");
//     header_var.push("Content-Length", tostring(tmp.size())); // size body
// 	cgi_status_code(response);
// 	sent_response = header_var.header_msg() + "\r\n" + tmp;
// 	return (sent_response);
// }

// std::string v_server::post_method()
// {
// 	return (put_method());
// }

// void v_server::cgi_status_code(std::string response)
// {
// 	std::string ret;
// 	std::string num;
// 	std::string str;

// 	ret = response.substr(0, response.find("\r\n"));
// 	if (ret.find("Status") != std::string::npos)
// 	{
// 		ret = ret.substr(ret.find(": ") + 2, ret.size());
// 		num = ret.substr(0, ret.find(" "));
// 		str = ret.substr(ret.find(" ") + 1, ret.size());
// 		header_var.set_http(ft_atoi(num.c_str()), str);
// 	}
// 	else
// 		header_var.set_http(200, "OK");
// }

// std::string v_server::run_method()
// {
// 	std::string ret = ft_cgi_exist();
	
// 	if (ret.size())
// 		ret = run_cgi();
// 	else if (req_method == "GET" || req_method == "HEAD")
// 		ret = run_file();
// 	else if (req_method == "PUT")
// 		ret = put_method();
// 	else if (req_method == "DELETE")
// 		ret = delete_method();
// 	else if (req_method == "OPTIONS")
// 		ret = options_method();
// 	else if (req_method == "POST")
// 		ret = post_method();
// 	return (ret);
// }

// std::string v_server::get_file_type()
// {
// 	std::string ret;
	
// 	ret = &file_open[file_open.find_last_of(".")];
// 	file_type = ret;
// 	if (ret == ".gif")
// 		return("image/gif");
// 	if (ret == ".png")
// 		return ("image/png");
// 	if (ret == ".jpg")
// 		return ("image/jpeg");
// 	if (ret == ".html")
// 		return ("text/html");
// 	if (ret == ".ico")
// 		return ("image/x-icon");
// 	if (ret == ".js")
// 		return ("text/javascript");
// 	if (ret == ".css")
// 		return ("text/css");
// 	if (ret == ".json")
// 		return ("text/json");
// 	if (ret == ".xml")
// 		return ("text/xml");
// 	return ("text/plain");
// }

// std::string v_server::ft_read_file(std::string val)
// {
// 	std::string ret;
// 	struct stat st;
// 	int fd = 0;

// 	ret.clear();
// 	if (val.size() == 0 || stat(val.c_str(), &st) < 0)
// 		return (ret);
// 	if ((fd = open(val.c_str(), O_RDONLY)) < 0)
// 		return (ret);
// 	char buf[st.st_size + 1];
// 	if (read(fd, buf, st.st_size) < 0)
// 	{
// 		close (fd);
// 		return (ret);
// 	}
// 	buf[st.st_size] = '\0';
// 	ret = buf;
// 	close(fd);
// 	return (ret);
// }

// std::string v_server::run_error(int val)
// {
// 	time_t rawtime;
// 	struct tm * timeinfo;
// 	char buff[100];
// 	std::string ret;
// 	std::string response;

// 	header_var.reset_header();
// 	header_var.set_http(val, _err._data[val]);
// 	ret = ft_read_file(get_error_page(val));
// 	if (ret.size() == 0)
// 		ret = _err.print_error_val(val);
// 	time (&rawtime);
// 	timeinfo = localtime (&rawtime);
// 	strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
// 	header_var.push("Date", buff);
// 	header_var.push("Content-Length", tostring(ret.size()));
// 	header_var.push("Accept-Charsets", "");
// 	header_var.push("Accept-Encoding", "");
// 	header_var.push("Accept-Language", "");
// 	header_var.push("Content-Language", "");
// 	if (val == 401)
// 		header_var.push("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
// 	header_var.push("Content-Type", "text/html");
// 	header_var.push("Connection", "close");
// 	response = header_var.header_msg() + "\r\n";
// 	if (req_method != "HEAD")
// 		response += ret;
// 	header_var.set_http(200, "OK");
// 	return (response);
// }

// std::string v_server::run_file()
// {
// 	std::string cc;
// 	std::string response;

// 	if (page_error_val)
// 		return (run_error(page_error_val));
// 	if (f_log.valid != 1 && f_log.auto_index)
// 	{
// 		response = send_auto(f_log.file, header_var, req_method, f_log);
// 		if (response.size() == 0)
// 			return (run_error(404));
// 		return response;
// 	}
// 	t_file_info s_file(f_log.file);
// 	if (s_file.works <= 0)
// 	{
// 		if (s_file.tab != NULL)
// 			free(s_file.tab);
// 		if (s_file.works == 0)
// 		{
// 			if (errno == EACCES)
// 				return (run_error(403));
// 			else
// 				return (run_error(404));
// 		}
// 		else if (s_file.works == -5)
// 			return (run_error(500));
// 		return (run_error(404));
// 	}
// 	if (req_method == "HEAD")
// 		header_var.push("Content-Type", "");
// 	header_var.push("Content-Length", tostring(s_file.size));
// 	response = header_var.header_msg() + "\r\n";
// 	if (req_method == "GET")
// 		response += s_file.tab;
// 	free(s_file.tab);
// 	return (response);
// }

// std::string v_server::get_string_methods()
// {
// 	std::list<std::string>::iterator it = methods.begin();
// 	std::string ret;
// 	int i = 0;
// 	if (f_log.location.size() != 0 && locations.find(f_log.location) != locations.end())
// 		return (locations.find(f_log.location)->second.get_string_methods());
// 	while (it != methods.end())
// 	{
// 		if (i != 0)
// 			ret += ", ";
// 		ret += *it;
// 		it++;
// 		i = 1;
// 	}
// 	return (ret);
// }

// int v_server::value_port()
// {
// 	return (ft_atoi(port.c_str()));
// }


// std::string v_server::get_file(std::map<std::string, location>::iterator loc, std::string s_root)
// {
// 	std::string root = (s_root + "/");
// 	std::string str1;
// 	struct stat st;
// 	location a = loc->second;
// 	std::vector<std::string> var = a.get_default();
// 	std::vector<std::string>::iterator it = var.begin();

// 	f_log.location = loc->first;
// 	str1.clear();
// 	for ( ; it != var.end(); it++)
// 	{
// 		str1 = root;
// 		str1 += *it;
// 		if (stat(str1.c_str(), &st) != -1)
// 		{
// 			f_log.file = str1;
// 			f_log.set_size();
// 			f_log.first_req = loc->first;
// 			f_log.limit_body = a.get_limit();
// 			return (str1);
// 		}
// 	}
// 	f_log.file = root;
// 	f_log.auto_index = a.get_index();
// 	f_log.set_size();
// 	f_log.limit_body = a.get_limit();
// 	return (root);
// }

// file_log v_server::get_f_log(){
// 	return (this->f_log);
// };

// void v_server::set_method(std::string val){
// 	req_method = val;
// };

// std::string v_server::get_req_file(std::string val, std::string met)
// {
// 	std::map<std::string, location>::iterator loc = locations.find(val);
// 	std::map<std::string, location>::reverse_iterator r_it;
	
// 	f_log.rest();
// 	f_log.req = met;
// 	if (loc != locations.end())
// 		return (get_file(loc, loc->second.get_root()));
// 	r_it = locations.rbegin();
// 	for ( ; r_it != locations.rend(); r_it++)
// 	{
// 		if (!ft_strncmp(val.c_str(), (r_it)->first.c_str(), (r_it)->first.size()))
// 		{
// 			f_log.file = r_it->second.get_root() + "/" + val.substr((r_it)->first.size(), 1024);
// 			f_log.first_req = val;
// 			f_log.auto_index = r_it->second.get_index();
// 			f_log.location = r_it->first;
// 			f_log.set_size();
// 			if (S_ISDIR(f_log.st.st_mode))
// 				return (get_file(locations.find(r_it->first), f_log.file));
// 			f_log.limit_body = r_it->second.get_limit();
// 			return (f_log.file);
// 		}
// 	}
// 	f_log.file = root + "/" + val;
// 	f_log.set_size();
// 	f_log.limit_body = limit_body;
// 	return (f_log.file);
// }