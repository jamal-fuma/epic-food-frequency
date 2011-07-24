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

mkdir -p build-darwin/dest;

cd build-darwin/; ( export CC="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-gcc"; export CXX="ccache /opt/i686-apple-darwin10/toolchain/bin/i686-apple-darwin10-g++";  ../configure --enable-maintainer-mode  --host=i686-apple-darwin10 --prefix="/Applications/epic-food-frequency.app"; make  );
