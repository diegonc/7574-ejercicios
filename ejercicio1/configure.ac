#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([ejercicio1], [1.0.0], [dnietoc@gmail.com])
AC_CONFIG_SRCDIR([src/lib/system/System.cc])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_include([m4/boost.m4])

AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Check for pkg-config compatible modules.
PKG_CHECK_MODULES([YAMLCPP], [yaml-cpp >= 0.5.1])

# Checks for libraries.

# Check for Boost libraries
BOOST_REQUIRE
BOOST_SMART_PTR
BOOST_STRING_ALGO

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([strerror])

AC_CHECK_FUNC([argp_parse], ,
	AC_MSG_ERROR([argp library not found.]))

AC_CHECK_FUNC([semget], ,
	AC_MSG_ERROR([semget is required]))

AC_CHECK_FUNC([shmget], ,
	AC_MSG_ERROR([shmget is required]))


AC_CONFIG_FILES([Makefile
		 src/Makefile])

AC_OUTPUT
