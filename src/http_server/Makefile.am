AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = www 

BUILT_SOURCES = sym_404.h

CLEANFILES = sym_404.h resource.zip

AM_CPPFLAGS = @NEED_ALL_CFLAGS@ \
				 -I$(srcdir)/.. \
				 -I$(srcdir)/../misc

OBJCOPY:=@OBJCOPY@ 

RESTOOBJ=$(OBJCOPY) -I binary -B @obj_machine@ -O @obj_format@

SUFFIXES = .ico  .zip .html

resource.zip : $(srcdir)/www/*
	(cd $(srcdir)/www && zip -9 - `find .`) > $@
	
.zip.o:
	$(RESTOOBJ)	 $< $@
	
.html.o:
	$(RESTOOBJ)	 $< $@
	
sym_404.h:404.o
	echo '/*' > $@
	cat	$(top_srcdir)/COPYING >> $@
	echo '*/' >> $@
	( objdump  -t  $< | grep _start |(read a b c d e ; echo "extern char $$e[];" ; echo "#define _binary_404_html_start $$e") \
	 && objdump  -t  $< | grep _end |(read a b c d e ; echo "extern char $$e[];" ; echo "#define _binary_404_html_end $$e") \
	 && objdump  -t  $< | grep _size |(read a b c d e ; echo "extern char $$e[];" ;echo "#define _binary_404_html_size $$e" ) \
	 ) >> $@
		

noinst_LIBRARIES = libhttp_server.a

libhttp_server_a_SOURCES = http_server.c http_server.h
libhttp_server_a_SOURCES += htmlnode.c htmlnode.h
libhttp_server_a_SOURCES += html_paths.h
libhttp_server_a_SOURCES += html_path_static_file.c
libhttp_server_a_SOURCES += html_path_404.c
libhttp_server_a_SOURCES += html_path_stulist.c
libhttp_server_a_SOURCES += html_path_input_class_info.c
libhttp_server_a_SOURCES += html_path_dformjs.c
libhttp_server_a_SOURCES += html_path_input_student_info.c
libhttp_server_a_SOURCES += html_path_search.c


libhttp_server_a_DEPENDENCIES = sym_404.h resource.o
 
EXTRA_libhttp_server_a_SOURCES = 404.html

libhttp_server_a_LIBADD = resource.o 404.o 



