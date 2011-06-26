# build for relatively old machines
HOST=${HOST:-i586-mingw32}

# use ccache to speed up compilation if in path
CC="ccache ${HOST}-gcc";
CXX="ccache ${HOST}-g++";

# strip ccache from the commands if they are not present
which cche 2>  /dev/null;
if [ $? -eq 0 ] ;
then
    export CC="${CC/ccache /}";
    export CXX="${CXX/ccache /}";
fi

# default prefix is under the c drive
PREFIX=${PREFIX:-"c:/epic"}

# windows executables end with a suffix
EXT=${EXT:-.exe}

# Windows builds need these set otherwise the cross-configure fails
export ac_cv_func_realloc_0_nonnull=yes
export ac_cv_func_malloc_0_nonnull=yes

# Windows builds link against win32
export LD_FLAGS="-lwinsock32 -lws2_32 -lnetapi32 -luserenv -lshlwapi -lmingw32"

# cross-compilation builds link statically
export CFLAGS="--static"

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

# serialise extracted version of build
echo "${project_with_version}" > version.txt
echo "Building ${project_with_version}"

# cross-configure
mkdir -p build/dest;
cd build;
../autogen.sh --prefix=${PREFIX} --host=${HOST} --enable-maintainer-mode

# cross-compile
make;

# install to staging
make DESTDIR=`pwd`/dest install;

# package
tar -cvjf epic-win32.tbz `pwd`/dest


# ${CXX} epic_ffq-client.o src/libhelper/.libs/libEpicHelper.a src/libcsv/.libs/libEpicCSV.a src/libsqlite/.libs/libSqlite.a src/libdao/.libs/libEpicDAO.a -static -o epic_ffq${EXT}
