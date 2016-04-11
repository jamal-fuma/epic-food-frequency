#!/bin/sh
# Run this to generate all the initial makefiles, etc.
libtoolize --force --no-warn --copy && \
aclocal -I `pwd`/m4 &&  \
autoheader && \
automake --foreign --add-missing && \
autoconf
