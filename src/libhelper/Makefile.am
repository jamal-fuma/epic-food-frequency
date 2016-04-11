## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\"  \
        -I$(top_srcdir)/include   	\
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/conversion

noinst_LTLIBRARIES = libEpicHelper.la
libEpicHelper_la_SOURCES = \
			   Config.cpp \
			   ConfigReader.cpp \
			   Logger.cpp \
			   Logger.hpp \
			   $(top_srcdir)/include/config/Global.hpp  \
			   $(top_srcdir)/include/config/Reader.hpp  \
			   $(top_srcdir)/include/Singleton.hpp

libEpicHelper_la_LDFLAGS =
 # -lpthread -lrt -lltdl

libEpicHelper_la_LIBADD =
#$(top_builddir)/libutility/src/libUtility.la
