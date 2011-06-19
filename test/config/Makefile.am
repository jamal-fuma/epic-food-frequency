check_PROGRAMS = \
	       test_config_finding \
	       test_config_loading

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	= $(top_builddir)/src/libhelper/libEpicHelper.la

AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	= \
       -I$(top_srcdir)/include   \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_config_loading_LDADD 		= $(LINK_DEPS)
test_config_loading_DEPENDENCIES 	= $(LINK_DEPS)
test_config_loading_SOURCES = \
	      test_config_loading.cpp \
	      $(top_srcdir)/include/config/Global.hpp

test_config_finding_LDADD 		= $(LINK_DEPS)
test_config_finding_DEPENDENCIES 	= $(LINK_DEPS)
test_config_finding_SOURCES = \
	      test_config_loading.cpp \
	      $(top_srcdir)/include/config/Global.hpp


reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
