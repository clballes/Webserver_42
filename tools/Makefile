#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Wed Jan 24 15:55:10 2024                                                     #

CC				:=	c++
CPPFLAGS		:=	-MMD -iquotes$(INC_DIR)
#CPPFLAGS		+=	-g -fsanitize='address,undefined'
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98

all: getaddrinfo client

getaddrinfo: getaddrinfo.o
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $< -o $(basename $@)

clean:
	rm -f getaddrinfo.o getaddrinfo.d

fclean: clean
	rm -f getaddrinfo

re: fclean all

