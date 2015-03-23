AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -O0 -g -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/museo/Museo.h \
	lib/system/IPCName.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/SharedVariable.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h

puertaSources = \
	puerta/puerta.cc

bin_PROGRAMS = \
	puerta/puerta

puerta_puerta_SOURCES = $(puertaSources) $(libSources)
puerta_puerta_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/puerta
#puerta_puerta_LDADD =
