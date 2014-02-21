if HAVE_LOCAL_LIBCDATETIME
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@

noinst_LTLIBRARIES = libcdatetime.la

libcdatetime_la_SOURCES = \
	libcdatetime_definitions.h \
	libcdatetime_elements.c libcdatetime_elements.h \
	libcdatetime_error.c libcdatetime_error.h \
	libcdatetime_extern.h \
	libcdatetime_libcerror.h \
	libcdatetime_libcstring.h \
	libcdatetime_support.c libcdatetime_support.h \
	libcdatetime_timestamp.c libcdatetime_timestamp.h \
	libcdatetime_types.h \
	libcdatetime_unused.h

libcdatetime_la_LIBADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcdatetime ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcdatetime_la_SOURCES)

