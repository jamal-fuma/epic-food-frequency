sqldir=$(builddir)

# application database schema
sql_DATA = \
	$(sqldir)/schema.sql

CLEANFILES= $(sql_DATA)

EXTRA_DIST=\
	 $(sql_DATA)


$(sqldir)/schema.sql: $(top_builddir)/stamp-h1
	cat $(abs_top_srcdir)/sql/schema.sql > $@
