CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = server.hpp

NAME = containers
SRC_FILE =  main.cpp
# OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

# %.o:%.cpp
# 	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(SRC_FILE) $(HEADER)
	@$(CC) $(CFLAGS) $(SRC_FILE) -o $(NAME)
	@./webserv
	@make clean

clean :
	@rm -f $(NAME) $(D_NAME)

fclean : clean

re : fclean all