AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@

noinst_LTLIBRARIES = libcmulti.la

libcmulti_la_SOURCES = \
	libcmulti_definitions.h \
	libcmulti_extern.h \
	libcmulti_libcerror.h \
	libcmulti_libcstring.h \
	libcmulti_thread_lock.c libcmulti_thread_lock.h \
	libcmulti_types.h \
	libcmulti_unused.h

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcmulti ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcmulti_la_SOURCES)

