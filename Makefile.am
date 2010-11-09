## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = libutility \
          src \
	  test

pkgsysconfdir = $(sysconfdir)/$(PACKAGE)
pkgsysconf_DATA =

# shared application configs
confdir = ${pkgsysconfdir}/conf
conf_DATA = \
        conf/quantity.conf \
        $(top_builddir)/client.conf

$(top_builddir)/client.conf: $(top_srcdir)/conf/client.conf.in stamp-h1
	sed -e 's![@]prefix[@]!$(prefix)!g' \
	    -e 's![@]exec_prefix[@]!$(exec_prefix)!g' \
	    -e 's![@]includedir[@]!$(includedir)!g' \
	    -e 's![@]datadir[@]!$(datadir)!g' \
	    -e 's![@]confdir[@]!$(confdir)!g' \
	    -e 's![@]sqldir[@]!$(sqldir)!g' \
	    -e 's![@]importdir[@]!$(importdir)!g' \
	    -e 's![@]libdir[@]!$(libdir)!g' \
	    -e 's![@]PACKAGE_VERSION[@]!$(PACKAGE_VERSION)!g' \
	    -e 's![@]KRB5_LDFLAGS[@]!$(KRB5_LDFLAGS)!g' \
	    -e 's![@]KRB5_LIBS[@]!$(KRB5_LIBS)!g' \
	    $(top_srcdir)/conf/client.conf.in > $@

# application data imports
sqldir = ${pkgsysconfdir}/sql
sql_DATA = \
	sql/schema.sql

# application data imports
importdir = ${pkgsysconfdir}/import
import_DATA = \
	import/groups.csv \
	import/milk_types.csv \
	import/foods.csv \
	import/question_types.csv \
	import/cereal_brands_alt.csv \
	import/ingredients.csv \
	import/baking_fats.csv \
	import/cereal_types.csv \
	import/cereal_brands.csv \
	import/meals.csv \
	import/nutrients.csv \
	import/frequencies.csv \
	import/milk_types_alt.csv \
	import/weights.csv \
	import/frying_fats.csv \
	import/visible_fats.csv \
	import/cereal_types_alt.csv \
	import/cereals.csv

# docs and LICENSE
epic_ffqdocdir  = ${prefix}/doc/epic_ffq
epic_ffqdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

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
        -I$(top_srcdir)/include/conversion \
	-DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""

epic_ffq_SOURCES = $(top_srcdir)/src/client.cpp
epic_ffq_LDADD   = $(top_builddir)/src/libEpic.la
epic_ffq_LDFLAGS = -lpthread -ldl -lrt

EXTRA_DIST = $(epic_ffqdoc_DATA) $(conf_DATA) $(sql_DATA) $(import_DATA) $(top_srcdir)/conf/client.conf.in $(top_srcdir)/autogen.sh

DISTCLEANFILES = $(top_builddir)/client.conf

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(epic_ffqdocdir)
