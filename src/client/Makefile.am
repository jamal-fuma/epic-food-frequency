bin_PROGRAMS = epic_ffq

confdir = $(sysconfdir)/$(PACKAGE)/conf

epic_ffq_CPPFLAGS=  \
        -I$(top_srcdir)/include    \
        -I$(top_srcdir)/src	   \
        -I$(top_srcdir)/src/client \
	-DPACKAGE_VERSION="\"${PACKAGE_VERSION}\"" \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

epic_ffq_SOURCES = \
        	   $(top_srcdir)/include/cmdline/Parser.hpp \
        	   $(top_srcdir)/include/license/GPL_License.hpp \
        	   $(top_srcdir)/src/libhelper/Logger.hpp \
		   Client.cpp 		\
		   Application.cpp 	\
		   Application.hpp 	\
		   ReportWriter.hpp	\
		   ReportWriter.cpp

epic_ffq_LDADD   = \
		   $(top_builddir)/src/libhelper/libEpicHelper.la	\
		   $(top_builddir)/src/libcsv/libEpicCSV.la		\
		   $(top_builddir)/src/libdao/libEpicDAO.la

epic_ffq_LDFLAGS = 


