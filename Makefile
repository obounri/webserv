CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = srcs/parsers/parse_config.hpp
# CFLAGS = -no-pie -Wall -Wextra -Werror -std=c++98
CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = srcs/server/server.hpp \
		srcs/parsers/location.hpp\
		srcs/sockets/sockets.hpp 

NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp \
			srcs/parsers/parse_config.cpp \
			srcs/parsers/server_block.cpp \
			srcs/parsers/location_block.cpp \
			srcs/parsers/utils.cpp \
			srcs/parsers/Syntax_errors.cpp \
			srcs/parsers/location.cpp \
			srcs/server/server.cpp \
			srcs/sockets/sockets.cpp 

CLIENT = client.cpp
# OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

# %.o:%.cpp
# 	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(SRC_FILE) $(HEADER)
	@$(CC) $(CFLAGS) $(SRC_FILE) -o $(NAME)

clean :
	@rm -f $(NAME) $(NAME_CLIENT)

fclean : clean

re : fclean all