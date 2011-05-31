
check_PROGRAMS = \
	       test_database_import

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	= \
		  $(top_builddir)/src/libhelper/libEpicHelper.la \
		  $(top_builddir)/src/libdao/libEpicDAO.la 

AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	= \
        -I$(top_srcdir)/include   \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_database_import_LDADD 		= $(LINK_DEPS)
test_database_import_DEPENDENCIES 	= $(LINK_DEPS)
test_database_import_SOURCES = \
	      test_database_import.cpp \
	      $(top_srcdir)/include/Global.hpp
	      $(top_srcdir)/src/libdao/Database.hpp

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
