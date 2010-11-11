ACLOCAL_AMFLAGS = -I m4

noinst_PROGRAMS = \
	       test_db \
	       test_header

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	=  $(top_builddir)/src/libEpic.la -lpthread -ldl -lrt
AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	=  -I$(top_srcdir)/libutility/src \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/client \
        -I$(top_srcdir)/include/dataset \
        -I$(top_srcdir)/include/pattern \
        -I$(top_srcdir)/include/logging \
        -I$(top_srcdir)/include/config \
        -I$(top_srcdir)/include/license \
        -I$(top_srcdir)/include/cmdline \
        -I$(top_srcdir)/include/conversion \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_db_SOURCES = test_db.cpp 
test_db_LDADD = $(LINK_DEPS)
test_db_DEPENDENCIES = $(LINK_DEPS)

test_header_SOURCES = test_db.cpp 
test_header_LDADD = $(LINK_DEPS)
test_header_DEPENDENCIES = $(LINK_DEPS)


reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f  \
	       test_header \
	       test_db
