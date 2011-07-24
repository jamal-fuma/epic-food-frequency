check_PROGRAMS = \
	       test_split

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	=

AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	= \
       -I$(top_srcdir)/include   \
       -DDEFAULT_CONFIG_FILE="\"${abs_top_builddir}/test/fixtures/test.conf\""

AM_LDFLAGS	=  $(LINK_DEPS) -static-libgcc
test_split_SOURCES = 		test_split.cpp \
   				$(top_srcdir)/include/config/Util.hpp \
   				$(top_srcdir)/include/config/Resource.hpp \
   				$(top_srcdir)/include/import/Import.hpp
test_split_LDADD = $(LINK_DEPS)
test_split_DEPENDENCIES = $(LINK_DEPS)

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
