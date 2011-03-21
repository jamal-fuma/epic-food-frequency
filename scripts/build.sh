export ac_cv_func_realloc_0_nonnull=yes
export ac_cv_func_malloc_0_nonnull=yes

HOST=${HOST:-i586-mingw32}

CC=${HOST}-g++

PREFIX=${PREFIX:-"unset"}
EXT=${EXT:-.exe}

export LD_FLAGS="-lwinsock32 -lws2_32 -lnetapi32 -luserenv -lshlwapi -lmingw32"
export CFLAGS="--static"

./configure --prefix=${PREFIX} --host=${HOST}
make
${CC} epic_ffq-client.o src/.libs/libEpic.a -static -o epic_ffq${EXT}
