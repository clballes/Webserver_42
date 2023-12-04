NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
SRC = srcs/
INC = inc/
# Sources
SRCS = $(SRC)main.cpp $(SRC)Config.cpp
LIB = $(INC)Config.hpp
# create obj and resources
OBJ = $(SRCS:%.cpp=%.o)
DEPS = $(SRCS:%.cpp=%.d)

# compiling makefile
$(NAME): $(OBJ) $(LIB)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
		$(CC) -I. -c $(CFLAGS) -MMD $< -o $@

clean:
		$(RM) $(NAME) $(OBJ) $(DEPS)

fclean: clean
re: fclean all