CC = c++

CFLAGS = -no-pie -Wall -Wextra -Werror -std=c++98

HEADER = srcs/server/server.hpp \
		srcs/sockets/sockets.hpp

NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp
CLIENT = client.cpp
# OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

# %.o:%.cpp
# 	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(SRC_FILE) $(CLIENT) $(HEADER)
	@$(CC) $(CFLAGS) $(SRC_FILE) -lkqueue -lpthread -o $(NAME)
	@$(CC) $(CFLAGS) $(CLIENT) -o $(NAME_CLIENT)

clean :
	@rm -f $(NAME) $(NAME_CLIENT)

fclean : clean

re : fclean all