## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@ @GTK_CFLAGS@ @GTKGLEXT_CFLAGS@ @LIBGLADE_CFLAGS@

bin_PROGRAMS = gnu_shader_composer

gnu_shader_composer_SOURCES = \
	main.c \
	support.c support.h \
	callbacks.c callbacks.h \
	trackball.c trackball.h

gnu_shader_composer_LDADD = @PACKAGE_LIBS@ @GTK_LIBS@ @GTKGLEXT_LIBS@ @LIBGLADE_LIBS@ $(INTLLIBS)

