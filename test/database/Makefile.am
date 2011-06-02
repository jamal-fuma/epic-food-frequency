
SUBDIRS= . model

check_PROGRAMS = \
	       test_database_report \
	       test_database_import

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	= \
		  $(top_builddir)/src/libhelper/libEpicHelper.la \
		  $(top_builddir)/src/libdao/libEpicDAO.la 

AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	= \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/src   \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_database_import_LDADD 		= $(LINK_DEPS)
test_database_import_DEPENDENCIES 	= $(LINK_DEPS)
test_database_import_SOURCES = \
	      test_database_import.cpp \
	      $(top_srcdir)/include/config/Global.hpp
	      $(top_srcdir)/src/libdao/Database.hpp

test_database_report_LDADD 		= $(LINK_DEPS)
test_database_report_DEPENDENCIES 	= $(LINK_DEPS)
test_database_report_SOURCES = \
	      test_database_report.cpp \
	      ${top_srcdir}/include/dao/Meal.hpp \
	      ${top_srcdir}/include/dao/Food.hpp \
	      ${top_srcdir}/include/dao/Nutrient.hpp \
	      ${top_srcdir}/include/dao/MealFood.hpp \
	      $(top_srcdir)/include/config/Global.hpp
	      $(top_srcdir)/src/libdao/Database.hpp


reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
