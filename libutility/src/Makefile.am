## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libUtility.la


libUtility_la_SOURCES = \
	sqlite3.c             \
	Utility_p2p_os.c             \
	Utility_p_vector.c             \
	Utility_state.c             \
	Utility_state_rule.c             \
	Utility_vector.c             \
	Utility_token.c             \
	Utility_sz_vector.c             \
	Utility_s_vector.c             \
	Utility_push_buffer.c             \
	Utility_find.c             \
	Utility_debug.c             \
	SQLite3_comp.c             \
	libUtility.c             \
	Utility_lib.c             \
	Utility_pack.c             \
	Utility_p2p_comms.c
	
libUtility_la_LDFLAGS = 

libUtility_la_LIBADD = 

noinst_HEADERS = \
        Utility_s_vector.h             \
        Utility_state.h             \
        Utility_push_buffer.h             \
        Utility_p_vector.h             \
        Utility_state_rule.h             \
        Utility_types.h             \
        Utility_token.h             \
        Utility_sz_vector.h             \
        Utility_string.h             \
        Utility_vector.h             \
        Utility_includes.h             \
        Utility_find.h             \
        Utility_errors.h             \
        libUtility.h             \
        Utility_internal.h             \
        Utility_pack.h             \
        Utility_p2p_comms.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libUtility-0.1.pc

EXTRA_DIST = \
	libUtility-0.1.pc.in
