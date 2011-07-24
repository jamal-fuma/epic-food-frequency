# build for relatively old machines
HOST=${HOST:-i686-apple-darwin10}
TOOLS_DIR=${TOOLS_DIR:-/opt/${HOST}/toolchain/bin}

# use ccache to speed up compilation if in path
export CC="ccache ${TOOLS_DIR}/${HOST}-gcc";
export CXX="ccache ${TOOLS_DIR}/${HOST}-g++";

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

# cross-configure
mkdir -p build-osx/dest;
cd build-osx;

# default prefix is under the /Applications folder
PREFIX=${PREFIX:-"/Applications/${project_with_version}.app/Contents/MacOSX"}
( cd ../; autoreconf; );
../configure --prefix=${PREFIX} --host=${HOST}  --enable-maintainer-mode

echo $CC
echo $CXX

exit 
# cross-compile
make;
make check;
# hand compile one

# install to staging
make DESTDIR="`pwd`/dest/" install;

# package
tar -cvjf ${project_with_version}_osx.tbz `pwd`/dest

( 
export CC="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-gcc";
export CXX="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-g++";
 ../configure --enable-maintainer-mode  --host=i686-apple-darwin10 --prefix="/Applications/epic-food-frequency.app";
 make 
)

