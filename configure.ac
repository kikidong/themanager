dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(Themanager, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
#AC_CONFIG_HEADERS(config.h)

AC_PROG_CC

PKG_CHECK_MODULES(GTK2,gtk+-2.0)
PKG_CHECK_MODULES(LIBXML2,libxml-2.0)

AC_CONFIG_FILES(Makefile
	src/Makefile)
AC_OUTPUT

