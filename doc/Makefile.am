epic_ffqdocdir  = ${prefix}/doc/epic_ffq


# docs and LICENSE
epic_ffqdoc_DATA = \
	${top_srcdir}/README\
	${top_srcdir}/COPYING\
	${top_srcdir}/AUTHORS\
	${top_srcdir}/INSTALL\
	${top_srcdir}/NEWS\
	${top_srcdir}/Acknowledgements

EXTRA_DIST=$(epic_ffqdoc_DATA)
 
# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(epic_ffqdocdir)


