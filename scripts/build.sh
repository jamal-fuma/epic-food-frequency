#!/bin/bash
export CXX="ccache g++"
export CC="ccache gcc"
PREFIX=`pwd`/root
( cd ${PREFIX}/../;  ../autogen.sh --prefix=${PREFIX}; )
make
make distcheck
exit $?
