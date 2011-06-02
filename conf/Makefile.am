
pkgsysconfdir 	= $(sysconfdir)/$(PACKAGE)
confdir   	= ${pkgsysconfdir}/conf
sqldir 	  	= ${pkgsysconfdir}/sql
importdir 	= ${pkgsysconfdir}/import

# shared application configs
conf_DATA = \
        quantity.conf \
        $(top_builddir)/client.conf

$(top_builddir)/client.conf: client.conf.in $(top_srcdir)/stamp-h1
	sed -e 's![@]confdir[@]!$(confdir)!g' \
	    -e 's![@]sqldir[@]!$(sqldir)!g' \
	    -e 's![@]importdir[@]!$(importdir)!g' \
	    -e 's![@]PACKAGE_VERSION[@]!$(PACKAGE_VERSION)!g' \
	    $< > $@

EXTRA_DIST= client.conf.in quantity.conf

# not explicitly removing this causes distclean to fail and so breaks distcheck
DISTCLEANFILES= $(top_builddir)/client.conf

