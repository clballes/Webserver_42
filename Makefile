#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Mon Jan  8 12:10:43 2024                                                     #

NAME			:=	webserv

SRC_DIR			:=	src
INC_DIR         :=  include
BUILD_DIR		:=	build

CC				:=	c++
CPPFLAGS		:=	-MMD
CPPFLAGS		+=	-I$(INC_DIR)
CPPFLAGS		+=	-g -fsanitize='address,undefined'
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98
CXXFLAGS		+=	-DREUSE_SOCKET -DALLOW_FORBIDDEN

SRC_FILES		:=	$(SRC_DIR)/main.cpp \
					$(SRC_DIR)/Router.cpp \
					$(SRC_DIR)/Connection.cpp \
					$(SRC_DIR)/Server.cpp \
					$(SRC_DIR)/Location.cpp \
					$(SRC_DIR)/HTTP.cpp \
					$(SRC_DIR)/CGI.cpp \
					$(SRC_DIR)/string.cpp \
					$(SRC_DIR)/file.cpp \
					$(SRC_DIR)/autoindex.cpp

OBJ_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

.PHONY: all clean fclean re debug

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(OBJ_FILES) -o $(basename $@)

-include $(DEP_FILES)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)
