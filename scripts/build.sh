#!/bin/bash
export CXX="ccache g++"
export CC="ccache gcc"

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

# serialise extracted version of build
echo "${project_with_version}" > version.txt
echo "Building ${project_with_version}"

f=$( cat << 'EOF'
aclocal -I m4 && \
autoheader && \
libtoolize --force --no-warn && \
automake --foreign --add-missing && \
autoreconf -fi && \
autoconf
EOF
);

# cross-configure
mkdir -p build-native/root;
PREFIX="`pwd`/build-native/root";
cd build-native;
../configure --prefix=${PREFIX} --enable-maintainer-mode

make
# hack so the files depended on by tests are in place
make check
exit $?
