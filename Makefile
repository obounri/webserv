CC = c++

CFLAGS = -no-pie -Wall -Wextra -Werror -std=c++98
# CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = srcs/server/server.hpp \
		srcs/sockets/sockets.hpp \
		srcs/parsers/parse.hpp \
		srcs/client/client.hpp \
		srcs/extra.hpp

NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp \
	srcs/server/server.cpp \
	srcs/parsers/parse_config.cpp \
	srcs/sockets/sockets.cpp \
	srcs/client/client.cpp \
	srcs/extra_functions.cpp

# CLIENT = client.cpp
# OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

# %.o:%.cpp
# 	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(SRC_FILE) $(CLIENT) $(HEADER)
	@$(CC) $(CFLAGS) $(SRC_FILE) -lkqueue -lpthread -o $(NAME)
# 	@$(CC) $(CFLAGS) $(SRC_FILE) -o $(NAME)
# @$(CC) $(CFLAGS) $(CLIENT) -o

clean :
	@rm -f $(NAME)

fclean : clean

re : fclean all