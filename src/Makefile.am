
SUBDIRS  = misc http_server

AM_CPPFLAGS =@LIBXML2_CFLAGS@ @LIBSOUP_CFLAGS@ @GLIB2_CFLAGS@ -I$(srcdir)/http_server

LIBS += @LIBXML2_LIBS@ @LIBSOUP_LIBS@ @GLIB2_LIBS@ @SQLITE_LIBS@

STATIC_LIBS =  $(builddir)/http_server/libhttp_server.a $(builddir)/misc/libmisc.a	


bin_PROGRAMS=Themanager
Themanager_SOURCES= main.c

Themanager_LDADD = ${STATIC_LIBS}

Themanager_DEPENDENCIES = ${STATIC_LIBS}