export ac_cv_func_realloc_0_nonnull=yes
export ac_cv_func_malloc_0_nonnull=yes

export LD_FLAGS="-lwinsock32 -lws2_32 -lnetapi32 -luserenv -lshlwapi"
export CFLAGS="--static"
../epic-food-frequency-0.0.2-a/configure --prefix=/home/me/workspace/clone/windows --host=i586-mingw32 --enable-maintainer-mode
