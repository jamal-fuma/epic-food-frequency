#!/bin/bash - 
#===============================================================================
#
#          FILE:  build-darwin.sh
# 
#         USAGE:  ./build-darwin.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: YOUR NAME (), 
#       COMPANY: 
#       CREATED: 07/25/11 00:54:04 BST
#      REVISION:  ---
#===============================================================================

export CC="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-gcc";
export CXX="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-g++";

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`

mkdir -p build-darwin/dest;
cd build-darwin/;

../configure    --enable-maintainer-mode   \
                --host=i686-apple-darwin10 \
                --prefix="/Applications/epic-food-frequency.app" && \
        make && \
        make DESTDIR="`pwd`/dest/" install && \
        tar -cvjf ${project_with_version}_osx.tbz `pwd`/dest;
