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
	lib/museo/OpCierre.h \
	lib/museo/OpPersona.h \
	lib/museo/OpPuerta.h \
	lib/system/IPCName.h \
	lib/system/MessageQueue.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/SharedVariable.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h \
	lib/utils/yaml-converters.h

personaSources = \
	persona/ArgParser.cc \
	persona/ArgParser.h \
	persona/CtrlPersona.cc \
	persona/CtrlPersona.h \
	persona/persona.cc

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

administradorSources = \
	administrador/ArgParser.cc \
	administrador/ArgParser.h \
	administrador/administrador.cc

mkpersonasSources = \
	mkpersonas/ArgParser.cc \
	mkpersonas/ArgParser.h \
	mkpersonas/mkpersonas.cc

bin_PROGRAMS = \
	persona/persona \
	puerta/puerta \
	lanzador/lanzador \
	finalizador/finalizador \
	administrador/administrador \
	mkpersonas/mkpersonas

persona_persona_SOURCES = $(personaSources) $(libSources)
persona_persona_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/persona
persona_persona_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir)/persona $(YAMLCPP_CFLAGS)
persona_persona_LDADD = $(YAMLCPP_LIBS)

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

administrador_administrador_SOURCES = $(administradorSources) $(libSources)
administrador_administrador_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/administrador
administrador_administrador_CXXFLAGS = $(AM_CXXFLAGS) $(YAMLCPP_CFLAGS)
administrador_administrador_LDADD = $(YAMLCPP_LIBS)

mkpersonas_mkpersonas_SOURCES = $(mkpersonasSources) $(libSources)
mkpersonas_mkpersonas_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/mkpersonas
mkpersonas_mkpersonas_CXXFLAGS = $(AM_CXXFLAGS) $(YAMLCPP_CFLAGS)
mkpersonas_mkpersonas_LDADD = $(YAMLCPP_LIBS)
