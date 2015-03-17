#
# Copyright © 2014-2015 myOS Group.
#
# This is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# Contributor(s):
# Amr Aboelela <amraboelela@gmail.com>
#

source ${MYOS_PATH}/sdk/config.sh

APPLICATION_DIRECTORY=$(pwd | awk -F'/' '{print $NF}')
#NATIVE_APP_PATH=$(pwd)
#source ${MYOS_PATH}/sdk/libs-clean.sh
#cd ${NATIVE_APP_PATH}

echo "===== Cleaning ${APPLICATION_DIRECTORY} ================================="
make clean

if [ ${BASE_OS} = "android" ]; then
#echo "===== Native app clean ================================="
ndk-build clean
ant clean
fi


