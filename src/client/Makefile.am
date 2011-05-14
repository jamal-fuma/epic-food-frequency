ACLOCAL_AMFLAGS= -I m4

bin_PROGRAMS = epic_ffq

confdir = $(sysconfdir)/$(PACKAGE)/conf

epic_ffq_CPPFLAGS=  \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/dataset \
        -I$(top_srcdir)/include/pattern \
        -I$(top_srcdir)/include/logging \
        -I$(top_srcdir)/include/config \
        -I$(top_srcdir)/include/license \
        -I$(top_srcdir)/include/cmdline \
        -I$(top_srcdir)/include/conversion \
        -I$(top_srcdir)/src/client \
	-DPACKAGE_VERSION="\"${PACKAGE_VERSION}\"" \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

epic_ffq_SOURCES = Client.cpp 		\
		   Application.cpp 	\
		   Application.hpp 	\
		   ReportWriter.hpp	\
		   ReportWriter.cpp

epic_ffq_LDADD   = \
		   $(top_builddir)/src/libdao/libEpicDAO.la 		\
		   $(top_builddir)/src/libhelper/libEpicHelper.la 

epic_ffq_LDFLAGS = 


