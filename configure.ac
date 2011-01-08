dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(Themanager, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
AC_CONFIG_HEADERS(config.h)

AC_PROG_CC
AC_PROG_RANLIB

AC_CHECK_TARGET_TOOL(OBJCOPY,objcopy,[objcopy],[])

case "$target" in
i386-* | i686-* )
AC_SUBST(obj_format,elf32-i386)
AC_SUBST(obj_machine,i386)
;;
x86_64-* )
AC_SUBST(obj_format,elf64-x86-64) 
AC_SUBST(obj_machine,i386:x86-64)
;;
* )
AC_MSG_ERROR("unsupported system",1)
;;
esac



PKG_CHECK_MODULES(ZLIB,zlib)
PKG_CHECK_MODULES(LIBXML2,libxml-2.0)
PKG_CHECK_MODULES(GLIB2,glib-2.0 gobject-2.0)
PKG_CHECK_MODULES(LIBSOUP,libsoup-2.4)
PKG_CHECK_MODULES(SQLITE,sqlite3)

PKG_CHECK_MODULES(NEED_ALL,libsoup-2.4 glib-2.0 gobject-2.0 zlib libxml-2.0 sqlite3)
AC_CONFIG_FILES(Makefile
	src/Makefile
	src/http_server/Makefile
	src/misc/Makefile
	src/sqlclient/Makefile)
AC_OUTPUT

