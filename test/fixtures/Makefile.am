SUBDIRS = sql import conf .

fixturedir = $(builddir)
fixture_DATA = \
		$(fixturedir)/test.conf 	\
		$(fixturedir)/test.conf.in

CLEANFILES= $(fixture_DATA) 

EXTRA_DIST=\
	 test.conf.in

%.conf : %.conf.in
	sed -e 's![@]fixtures[@]!$(fixturedir)!g' \
	$< > $@

$(fixturedir)/test.conf.in: $(top_builddir)/stamp-h1
	cat $(srcdir)/test.conf.in > $@
