#!/bin/bash

# build for relatively old machines
TARGET_ARCH=${TARGET_ARCH:-i586-mingw32}

# use ccache to speed up compilation if in path
export CC="ccache ${TARGET_ARCH}-gcc";
export CXX="ccache ${TARGET_ARCH}-g++";

# Windows builds link against win32
#export LDFLAGS="-lwinsock32 -lws2_32 -lnetapi32 -luserenv -lshlwapi -lmingw32 -static -static-libgcc"
export LDFLAGS="-static -static-libgcc"

# cross-compilation builds link statically
export CFLAGS="-O2 -g -Wall -mwindows"

# windows executables end with a suffix
export EXT=${EXT:-.exe}

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

# default prefix is under the c drive
export PREFIX=${PREFIX:-"c:/${project_with_version}"}

# serialise extracted version of build
echo "${project_with_version}" > version.txt
echo "Building ${project_with_version}"

# cross-configure
./autogen.sh;

mkdir -p build-w32/dest;
cd build-w32;

# Windows builds need these set otherwise the cross-configure fails
export ac_cv_func_realloc_0_nonnull=yes
export ac_cv_func_malloc_0_nonnull=yes

# cross-compile and install to staging for packaging
../configure --prefix=${PREFIX} --host=${TARGET_ARCH}  --enable-maintainer-mode --enable-wine-tests && \
        make && \
        make check && \
        make DESTDIR="`pwd`/dest/" install && \
        tar -cvjf ${project_with_version}_win32.tbz `pwd`/dest ;
