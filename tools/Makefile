#                                                                              #
# Makefile                                                                     #
# mpuig-ma <mpuig-ma@student.42barcelona.com>                                  #
# Wed Jan 24 15:55:10 2024                                                     #

CC				:=	c++
CPPFLAGS		:=	-MMD -iquotes$(INC_DIR)
#CPPFLAGS		+=	-g -fsanitize='address,undefined'
CXXFLAGS		:=	-Wall -Werror -Wextra -std=c++98

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

all: getaddrinfo

getaddrinfo: getaddrinfo.o
	$(CC) $(CPPFLAGS) $(CXXFLAGS) getaddrinfo.o -o $(basename $@)
