
SUBDIRS  = misc http_server sqlclient

AM_CPPFLAGS = @NEED_ALL_CFLAGS@  -I$(srcdir)/http_server

LIBS += @NEED_ALL_LIBS@ 

STATIC_LIBS =  $(builddir)/http_server/libhttp_server.a $(builddir)/misc/libmisc.a	\
						$(builddir)/sqlclient/libsqlclient.a
		


bin_PROGRAMS=Themanager
Themanager_SOURCES= main.c

Themanager_LDADD = ${STATIC_LIBS}

Themanager_DEPENDENCIES = ${STATIC_LIBS}
