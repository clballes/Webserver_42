#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Mon Jan  8 12:10:43 2024                                                     #

NAME			:=	webserv

SRC_DIR			:=	src
INC_DIR			:=	$(SRC_DIR)
BUILD_DIR		:=	build

CC				:=	c++
CPPFLAGS		:=	-MMD -I$(INC_DIR)
CPPFLAGS		+=	-I$(SRC_DIR)/Server -I$(SRC_DIR)/ServerConf
#CPPFLAGS		+=	-g -fsanitize='address,undefined'
#CPPFLAGS		+=	-D SILENCE_LOGS
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98
SILENCE_LOGS	?=	false

SRC_FILES		:=	$(SRC_DIR)/main.cpp \
					$(SRC_DIR)/init.cpp \
					$(SRC_DIR)/getoptions.cpp \
					$(SRC_DIR)/webserv.cpp \
					$(SRC_DIR)/events.cpp \
					$(SRC_DIR)/loop.cpp \
					$(SRC_DIR)/Server/Server.cpp \
					$(SRC_DIR)/ServerConf/constructor.cpp \
					$(SRC_DIR)/ServerConf/destructor.cpp \
					$(SRC_DIR)/ServerConf/add.cpp \
					$(SRC_DIR)/ServerConf/file2mem.cpp \
					$(SRC_DIR)/ServerConf/parse.cpp
OBJ_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean fclean re debug

#include $(DEP_FILES)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(OBJ_FILES) -o $(basename $@)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)
