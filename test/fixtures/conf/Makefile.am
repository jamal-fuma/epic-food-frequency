confdir=$(builddir)

conf_DATA = \
		$(confdir)/defaults.conf 	\
		$(confdir)/quantity.conf

CLEANFILES= $(conf_DATA)

EXTRA_DIST=\
	 $(conf_DATA)

$(confdir)/defaults.conf: $(top_builddir)/stamp-h1
	cat $(abs_top_srcdir)/conf/defaults.conf > $@

$(confdir)/quantity.conf: $(top_builddir)/stamp-h1
	cat $(abs_top_srcdir)/conf/quantity.conf > $@
