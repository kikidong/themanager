AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = @GLIB2_CFLAGS@ -I$(srcdir)/..

AM_CPPFLAGS += -DWITH_SQLITE3

noinst_LIBRARIES = libsqlclient.a

libsqlclient_a_SOURCES = gsqlconnect.c gsqlconnect.h
libsqlclient_a_SOURCES += gsqlresult.c gsqlresult.h
libsqlclient_a_SOURCES += sqlbackend.c
		 

libsqlclient_a_SOURCES += gsqlconnect_sqlite3.c	 gsqlconnect_sqlite3.h
libsqlclient_a_SOURCES += sqlite_static_template.h

