CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = srcs/parsers/parse_config.hpp

NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp \
			srcs/parsers/parse_config.cpp \
			srcs/parsers/Syntax_errors.cpp
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