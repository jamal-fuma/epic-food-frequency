#!/bin/bash - 
#===============================================================================
#
#          FILE:  release.sh
# 
#         USAGE:  ./release.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: YOUR NAME (), 
#       COMPANY: 
#       CREATED: 07/25/11 01:23:12 BST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

# extract version of build
project_with_version=`grep AC_INIT configure.ac | cut -f 1,2 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
project=`grep AC_INIT configure.ac | cut -f 1 -d',' | tr -d '[[]]' | sed -e '/^[^(]\+./{ s///; s/,/-/; }'`
find . -wholename "./.git" -prune -o  -type f -a -print | xargs tar -cvzf ${project_with_version}.tgz;
mkdir $project_with_version;
tar -xf ${project_with_version}.tgz -C $project_with_version;
tar -cvzf ${project_with_version}.tgz $project_with_version/
rm -rvf $project_with_version
