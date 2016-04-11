# build for relatively old machines
TARGET_ARCH=${TARGET_ARCH:-i686-apple-darwin10}
TOOLS_DIR=${TOOLS_DIR:-/opt/${TARGET_ARCH}/toolchain/bin}

# use ccache to speed up compilation if in path
export CC="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-gcc";
export CXX="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-g++";

# cross builds for windows need these set otherwise the cross-configure fails
#export ac_cv_func_realloc_0_nonnull=yes
#export ac_cv_func_malloc_0_nonnull=yes

export LDFLAGS="-static -static-libgcc"

# cross-compilation builds link statically
export CFLAGS="-O2 -g -Wall"

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

# serialise extracted version of build
echo "${project_with_version}" > version.txt
echo "Building ${project_with_version}"

./autogen.sh

# cross-configure
mkdir -p build-osx/dest;
cd build-osx;

# default prefix is under the /Applications folder
PREFIX=${PREFIX:-"/Applications/${project}.app/Contents/MacOSX"}

# cross-compile and install to staging for packaging
../configure --prefix=${PREFIX} --host=${TARGET_ARCH}  --enable-maintainer-mode && \
        make && \
        make DESTDIR="`pwd`/dest/" install && \
        tar -cvjf ${project_with_version}_osx.tbz `pwd`/dest ;
