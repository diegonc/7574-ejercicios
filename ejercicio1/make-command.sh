#!/bin/sh
MAKE="$1"
shift

if [ ! -f Makefile -a -f config.status ]; then
	./config.status
elif [ ! -f Makefile ]; then
	./configure "$@"
fi
"$MAKE" -f Makefile
