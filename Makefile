CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = server.hpp

NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp
CLIENT = client.cpp
# OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

# %.o:%.cpp
# 	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(SRC_FILE) $(CLIENT) $(HEADER)
	@$(CC) $(CFLAGS) $(SRC_FILE) -o $(NAME)
	@$(CC) $(CFLAGS) $(CLIENT) -o $(NAME_CLIENT)

clean :
	@rm -f $(NAME) $(NAME_CLIENT)

fclean : clean

re : fclean all