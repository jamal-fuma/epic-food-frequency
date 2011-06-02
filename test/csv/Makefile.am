# surely there is a better way then repeating declarations
confdir = $(sysconfdir)/$(PACKAGE)/conf

check_PROGRAMS = \
	       test_csv_pair \
	       test_csv_import

AM_CPPFLAGS =  -ggdb -O0 \
        -I$(top_srcdir)/include    \
        -I$(top_srcdir)/src	   \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

LINK_DEPS	= \
		  $(top_builddir)/src/libhelper/libEpicHelper.la \
		  $(top_builddir)/src/libcsv/libEpicCSV.la 

AM_LDFLAGS	=  $(LINK_DEPS)

test_csv_import_LDADD 		= $(LINK_DEPS)
test_csv_import_DEPENDENCIES 	= $(LINK_DEPS)
test_csv_import_SOURCES = \
	      test_csv_import.cpp \
	      $(top_srcdir)/include/config/Global.hpp
	      $(top_srcdir)/src/libcsv/CSVReader.hpp

test_csv_pair_LDADD 		= $(LINK_DEPS)
test_csv_pair_DEPENDENCIES 	= $(LINK_DEPS)
test_csv_pair_SOURCES = \
	      test_csv_pair.cpp \
	      $(top_srcdir)/src/libcsv/MatchedValues.hpp

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
