## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

libutilitydocdir = ${prefix}/doc/libutility
libutilitydoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(libutilitydoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(libutilitydocdir)
