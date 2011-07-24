check_PROGRAMS = \
	test_config_finding \
	test_config_loading

TESTS=$(check_PROGRAMS)

LINK_DEPS	= $(top_builddir)/src/libhelper/libEpicHelper.la

AM_LDFLAGS	=  $(LINK_DEPS) -static-libgcc
AM_CPPFLAGS 	= \
       -I$(top_srcdir)/include   \
       -DDEFAULT_CONFIG_FILE="\"${abs_top_builddir}/test/fixtures/test.conf\""

test_config_loading_LDADD 		= $(LINK_DEPS)
test_config_loading_DEPENDENCIES 	= $(LINK_DEPS)
test_config_loading_SOURCES = \
	      test_config_loading.cpp \
	      $(top_srcdir)/include/config/Global.hpp

test_config_finding_LDADD 		= $(LINK_DEPS)
test_config_finding_DEPENDENCIES 	= $(LINK_DEPS)
test_config_finding_SOURCES = \
	      test_config_finding.cpp \
	      $(top_srcdir)/include/config/Global.hpp

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}

