check_PROGRAMS = \
	       test_logger

BUILD_DEFS	=  -ggdb -O0
LINK_DEPS	= 

AM_CFLAGS 	=  $(BUILD_DEFS)
AM_CPPFLAGS 	= \
        -I$(top_srcdir)/src   \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

AM_LDFLAGS	=  $(LINK_DEPS)

test_logger_SOURCES = 		test_logger.cpp \
    				$(top_srcdir)/src/libhelper/Logger.hpp
test_logger_LDADD = $(LINK_DEPS)
test_logger_DEPENDENCIES = $(LINK_DEPS)

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f ${check_PROGRAMS}
