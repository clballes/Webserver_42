#!/bin/bash

# set -x

EXECUTABLE="/Users/mpuig-ma/Documents/webserv/webserv"

run_test ()
{
	if &>/dev/null timeout --preserve-status --signal INT 0.42 $EXECUTABLE "$1"
	then
		echo -e "OK\t$1"
		return 0
	fi
	
	echo -e "KO\t$1"
	return 1 # EXIT_FAILURE
}

# main

if ! test -x "$EXECUTABLE"
then
	make -C "/Users/mpuig-ma/Documents/webserv"
fi

for file in "/Users/mpuig-ma/Documents/webserv/conf/"*
do
	run_test "$file"
done
