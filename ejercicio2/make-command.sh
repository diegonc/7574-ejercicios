#!/bin/sh
MAKE="$1"
shift

if [ ! -f Makefile -a -f config.status ]; then
	./config.status
elif [ ! -f Makefile ]; then
	test -f configure || ./bootstrap.sh
	./configure \
		CFLAGS="-O0 -g3 -gdwarf-2" \
		CXXFLAGS="-O0 -g3 -gdwarf-2" \
		"$@"
fi
"$MAKE" -f Makefile
