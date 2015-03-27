AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/config/ConfigParser.cc \
	lib/config/ConfigParser.h \
	lib/logging/Logger.cc \
	lib/logging/Logger.h \
	lib/logging/LoggerRegistry.cc \
	lib/logging/LoggerRegistry.h \
	lib/museo/constantes.h \
	lib/museo/Museo.h \
	lib/system/IPCName.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/SharedVariable.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h \
	lib/utils/yaml-converters.h

puertaSources = \
	puerta/ArgParser.cc \
	puerta/ArgParser.h \
	puerta/puerta.cc

lanzadorSources = \
	lanzador/ArgParser.cc \
	lanzador/ArgParser.h \
	lanzador/lanzador.cc

finalizadorSources = \
	finalizador/ArgParser.cc \
	finalizador/ArgParser.h \
	finalizador/finalizador.cc

bin_PROGRAMS = \
	puerta/puerta \
	lanzador/lanzador \
	finalizador/finalizador

puerta_puerta_SOURCES = $(puertaSources) $(libSources)
puerta_puerta_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/puerta
puerta_puerta_CXXFLAGS = $(AM_CXXFLAGS) $(YAMLCPP_CFLAGS)
puerta_puerta_LDADD = $(YAMLCPP_LIBS)

lanzador_lanzador_SOURCES = $(lanzadorSources) $(libSources)
lanzador_lanzador_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/lanzador
lanzador_lanzador_CXXFLAGS = $(AM_CXXFLAGS) $(YAMLCPP_CFLAGS)
lanzador_lanzador_LDADD = $(YAMLCPP_LIBS)

finalizador_finalizador_SOURCES = $(finalizadorSources) $(libSources)
finalizador_finalizador_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/finalizador
finalizador_finalizador_CXXFLAGS = $(AM_CXXFLAGS) $(YAMLCPP_CFLAGS)
finalizador_finalizador_LDADD = $(YAMLCPP_LIBS)
