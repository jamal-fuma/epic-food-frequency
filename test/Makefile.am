ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = \
	       test_csv \
	       test_dao \
	       test_db \
	       test_split \
	       test_header \
	       test_dao_import \
	       test_import	\
	       test_util_split

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	= \
		   $(top_builddir)/src/libdao/libEpicDAO.la 		\
		   $(top_builddir)/src/libhelper/libEpicHelper.la 


AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	=  -I$(top_srcdir)/libutility/src \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/src/libdao   	\
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
        -I$(top_srcdir)/dao \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_dao_SOURCES = test_dao.cpp
test_dao_LDADD = $(LINK_DEPS)
test_dao_DEPENDENCIES = $(LINK_DEPS)

test_db_SOURCES = test_db.cpp
test_db_LDADD = $(LINK_DEPS)
test_db_DEPENDENCIES = $(LINK_DEPS)

test_csv_SOURCES = test_csv.cpp
test_csv_LDADD = $(LINK_DEPS)
test_csv_DEPENDENCIES = $(LINK_DEPS)

test_split_SOURCES = test_split.cpp $(top_srcdir)/include/config/Util.hpp 
test_split_LDADD = $(LINK_DEPS)
test_split_DEPENDENCIES = $(LINK_DEPS)

test_util_split_SOURCES = 	test_util_split.cpp \
    				$(top_srcdir)/include/config/Resource.hpp \
    				$(top_srcdir)/include/config/Util.hpp 
test_util_split_LDADD = $(LINK_DEPS)
test_util_split_DEPENDENCIES = $(LINK_DEPS)


test_header_SOURCES = test_header.cpp
test_header_LDADD = $(LINK_DEPS)
test_header_DEPENDENCIES = $(LINK_DEPS)

test_import_SOURCES = test_import.cpp imp.cpp imp.hpp
test_import_LDADD = $(LINK_DEPS)
test_import_DEPENDENCIES = $(LINK_DEPS)

test_dao_import_SOURCES = test_dao_import.cpp imp.cpp imp.hpp
test_dao_import_LDADD = $(LINK_DEPS)
test_dao_import_DEPENDENCIES = $(LINK_DEPS)

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f  \
	       test_import \
	       test_dao_import \
	       test_header \
	       test_dao \
	       test_db \
	       test_csv
