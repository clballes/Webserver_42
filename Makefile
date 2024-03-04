#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Mon Jan  8 12:10:43 2024                                                     #

NAME			:=	webserv

SRC_DIR			:=	src
INC_DIR			:=	$(SRC_DIR)
BUILD_DIR		:=	build

LIBFT			:=	$(SRC_DIR)/libft/libft.a

CC				:=	c++
CPPFLAGS		:=	-MMD
CPPFLAGS		+=	-I$(INC_DIR) -I$(SRC_DIR)/ServerConf
CPPFLAGS		+=	-I$(dir $(LIBFT))/include
CPPFLAGS		+=	-g -fsanitize='address,undefined'
#CPPFLAGS		+=	-D SILENCE_LOGS
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98
CXXFLAGS		+=	-Wno-unused-command-line-argument
LDFLAGS			:=	-L$(dir $(LIBFT)) -lft
SILENCE_LOGS	?=	false

SRC_FILES		:=	$(SRC_DIR)/main.cpp \
					$(SRC_DIR)/init.cpp \
					$(SRC_DIR)/getoptions.cpp \
					$(SRC_DIR)/webserv.cpp \
					$(SRC_DIR)/loop.cpp \
					$(SRC_DIR)/ServerConf/constructor.cpp \
					$(SRC_DIR)/ServerConf/destructor.cpp \
					$(SRC_DIR)/ServerConf/add.cpp \
					$(SRC_DIR)/ServerConf/file2mem.cpp \
					$(SRC_DIR)/ServerConf/normalize.cpp \
					$(SRC_DIR)/ServerConf/split_elements.cpp \
					$(SRC_DIR)/ServerConf/setters.cpp \
					$(SRC_DIR)/ServerConf/parse.cpp \
					$(SRC_DIR)/Server/constructor.cpp \
					$(SRC_DIR)/Server/destructor.cpp \
					$(SRC_DIR)/Server/events.cpp \
					$(SRC_DIR)/Server/start.cpp \
					$(SRC_DIR)/Server/clear.cpp \
					$(SRC_DIR)/Client/constructor.cpp \
					$(SRC_DIR)/Client/destructor.cpp \
					$(SRC_DIR)/Client/events.cpp \
					$(SRC_DIR)/HTTP/constructor.cpp \
					$(SRC_DIR)/HTTP/destructor.cpp \
					$(SRC_DIR)/HTTP/events.cpp
OBJ_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES		=	$(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

.PHONY: all clean fclean re debug

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $(basename $@)

-include $(DEP_FILES)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)
