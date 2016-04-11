# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.61])
AC_INIT([epic-food-frequency],[0.0.2-l_RC3],[jamal.natour@fumasoftware.com])

AC_CONFIG_SRCDIR([src/client/Application.cpp])
AC_CONFIG_HEADERS([config.h])

#make sure $ACLOCAL_FLAGS are used during a rebuild.
AC_CONFIG_MACRO_DIR([m4])
AC_SUBST([ACLOCAL_AMFLAGS], ["-I $ac_macro_dir \${ACLOCAL_FLAGS}"])

# Checks for programs.
AC_MINGW32
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_MKDIR_P
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL

AC_CHECK_HEADERS([arpa/inet.h assert.h ctype.h errno.h errno.h ev.h fcntl.h])
AC_CHECK_HEADERS([inttypes.h libgen.h limits.h netdb.h netinet/in.h netinet/tcp.h])
AC_CHECK_HEADERS([pwd.h signal.h stdarg.h stdint stdio.h stdlib.h string.h sys/fcntl.h])
AC_CHECK_HEADERS([sys/fileio.h sys/ioctl.h sys/mman.h sys/select.h sys/socket.h sys/stat.h])
AC_CHECK_HEADERS([sys/time.h sys/types.h sys/uio.h sys/un.h])
AC_CHECK_HEADERS([time.h unistd.h unistd.h uuid/uuid.h windows.h winsock2.h ws2tcpip.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_CHECK_MEMBERS([struct stat.st_blksize])

AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

dnl  Check for specifics of Unix cred passing
AC_CHECK_TYPES([struct cmsgcred])
AC_CHECK_TYPES([struct fcred])
AC_CHECK_TYPES([struct sockcred])
# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_FUNC_STRTOD

AC_CHECK_FUNCS([atexit clock_gettime fdatasync getcwd gettimeofday localtime_r])

dnl  DNS resolving
AC_CHECK_FUNCS([getaddrinfo freeaddrinfo gai_strerror])

dnl  Check for specifics of Unix cred passing
AC_CHECK_FUNCS([getpeereid getpeerucred])

dnl Signal handling
AC_CHECK_FUNCS([atexit pselect])

dnl Memory mapping
AC_CHECK_FUNCS([ftruncate mmap munmap])

dnl  Networking
AC_CHECK_FUNCS([inet_ntoa select socket])

dnl  String functions
AC_CHECK_FUNCS([memchr memmove memset  strchr strerror strndup strstr strtol strtoul strtoull])

dnl  File and directory management
AC_CHECK_FUNCS([mkdir rmdir basename])
AC_C_INLINE

# Add the --enable-debug arg
AC_ARG_ENABLE([debug],AS_HELP_STRING([--enable-debug],[Turn on debugging]),[debug=true],[debug=false])

# Add the --enable-wine-tests arg
AC_ARG_ENABLE([wine-tests],[AS_HELP_STRING([--enable-wine-tests],[Turn on wine-tests])],[wine="wine"],[wine=""])

# TODO Set the CFLAGS in the makefiles if we are debugging
AM_CONDITIONAL([DEBUG], [test "x$debug" = "xtrue"])

# TODO Set the WIN32 conditional in the makefiles if the host is MinGW
AM_CONDITIONAL([WIN32],[test "x$wine" = "xwine"])

dnl this is a dirty hack but will do until we can get cross-compilation target autodetection working
AC_SUBST([WINE],[$wine])

#Make cross compile tests work
AC_SUBST([TESTS_ENVIRONMENT], [$WINE])

# turn off shared libs
AC_DISABLE_SHARED

# I like maintainer mode
AM_MAINTAINER_MODE

AC_CANONICAL_HOST
dnl  don't moan about missing gnu styles project files
AM_INIT_AUTOMAKE([foreign 1.10 no-define tar-pax])

# Use the latest short output macro if available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/libhelper/Makefile
                 src/libcsv/Makefile
                 src/libdao/Makefile
                 src/client/Makefile
                 src/libsqlite/Makefile
                 test/Makefile
                 test/fixtures/Makefile
                 test/fixtures/conf/Makefile
                 test/fixtures/sql/Makefile
                 test/fixtures/import/Makefile
                 test/split/Makefile
                 test/logger/Makefile
                 test/config/Makefile
                 test/csv/Makefile
                 test/database/Makefile
                 test/database/model/Makefile
                 sql/Makefile
                 import/Makefile
                 doc/Makefile
                 conf/Makefile
                 scripts/Makefile])
AC_OUTPUT

dnl  Compile banner
echo \
"-------------------------------------------------
${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Prefix: '${prefix}'.
Compiler: '${CC} ${CFLAGS} ${CPPFLAGS} ${LDFLAGS}'

Linker:             ${LDFLAGS}
CFlags:             ${CFLAGS}
CPPFlags:           ${CPPFLAGS}

Package features:
Async Execution:    ${async_exec}
Wine enabled:       ${WINE}
Win32 enabled:      ${WIN32}
Tests enabled:      ${tests}
Test_env:           ${TESTS_ENVIRONMENT}
Debug enabled:      ${debug}
Win32 flag:         ${win32}

Sqlite3 bundling:   ${bundled_sqlite}
Sqlite3 Path:       ${ac_sqlite3_path}
Sqlite3 Version:    ${ac_sqlite3_version}

LibUtility Version: ${LIBUTILITY_VERSION}

Sqlite3 Flag:       ${ac_sqlite3_amalgamation}
Sqlite3 CPPFlags:   ${ac_sqlite3_cppflags}
Sqlite3 LDFlags:    ${ac_sqlite3_ldflags}

Host/Build/Target:

build:           ${build}
build_alias:     ${build_alias}
build_cpu:       ${build_cpu}
build_os:        ${build_os}
build_vendor:    ${build_vendor}

host:            ${host}
host_alias:      ${host_alias}
host_cpu:        ${host_cpu}
host_os:         ${host_os}
host_vendor:     ${host_vendor}

target:          ${target}
target_alias:    ${target_alias}
target_cpu:      ${target_cpu}
target_os:       ${target_os}
target_vendor:   ${target_vendor}


Now type 'make @<:@<target>@:>@'
where the optional <target> is:
check              - run test suite
all                - build all binaries
install            - install everything
reallyclean        - remove autogenerated cruft
--------------------------------------------------"
