## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = libutility \
          src
	

epic_ffqdocdir = ${prefix}/doc/epic_ffq
epic_ffqdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS    \
        \
        conf/quantity.conf \
        conf/client.conf \
        \
	$(top_srcdir)/import/groups.csv \
	$(top_srcdir)/import/milk_types.csv \
	$(top_srcdir)/import/foods.csv \
	$(top_srcdir)/import/question_types.csv \
	$(top_srcdir)/import/cereal_brands_alt.csv \
	$(top_srcdir)/import/ingredients.csv \
	$(top_srcdir)/import/baking_fats.csv \
	$(top_srcdir)/import/cereal_types.csv \
	$(top_srcdir)/import/cereal_brands.csv \
	$(top_srcdir)/import/meals.csv \
	$(top_srcdir)/import/nutrients.csv \
	$(top_srcdir)/import/frequencies.csv \
	$(top_srcdir)/import/milk_types_alt.csv \
	$(top_srcdir)/import/weights.csv \
	$(top_srcdir)/import/frying_fats.csv \
	$(top_srcdir)/import/visible_fats.csv \
	$(top_srcdir)/import/cereal_types_alt.csv \
	$(top_srcdir)/import/cereals.csv

bin_PROGRAMS = epic_ffq
epic_ffq_CPPFLAGS=  -I$(top_srcdir)/libutility/src \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/client \
        -I$(top_srcdir)/include/dataset \
        -I$(top_srcdir)/include/pattern \
        -I$(top_srcdir)/include/logging \
        -I$(top_srcdir)/include/config \
        -I$(top_srcdir)/include/license \
        -I$(top_srcdir)/include/cmdline \
        -I$(top_srcdir)/include/conversion

epic_ffq_SOURCES = $(top_srcdir)/src/client.cpp
epic_ffq_LDADD   = $(top_builddir)/src/libEpic.la
epic_ffq_LDFLAGS = -lpthread -ldl -lrt
EXTRA_DIST = $(epic_ffqdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(epic_ffqdocdir)
