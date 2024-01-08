#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Mon Jan  8 12:10:43 2024                                                     #

NAME			:=	webserv

SRC_DIR			:=	src
INC_DIR			:=	$(SRC_DIR)
BUILD_DIR		:=	build

CC				:=	c++
CPPFLAGS		:=	-MMD -iquotes$(INC_DIR)
#CPPFLAGS		+=	-g -fsanitize='address,undefined'
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98

SRC_FILES		:=	$(SRC_DIR)/$(NAME).cpp
OBJ_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean fclean re

#include $(DEP_FILES)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(OBJ_FILES) -o $(basename $@)

clean:
	rm -r $(BUILD_DIR)

fclean: clean
	rm $(NAME)

re: fclean
	$(MAKE)
