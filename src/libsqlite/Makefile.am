## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
      -I$(top_srcdir)/include   	\
      -I$(top_srcdir)/src

noinst_LTLIBRARIES = libSqlite.la

libSqlite_la_SOURCES = \
		   sqlite3.h		\
		   sqlite3.c

libSqlite_la_CPPFLAGS = \
	-DSQLITE_OMIT_LOAD_EXTENSION=1	\
	-DSQLITE_THREADSAFE=0
