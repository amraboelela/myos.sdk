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

echo ${BASE_OS}

source ${MYOS_PATH}/sdk/scripts/nativeApp-build.sh
PARAM=x$1

if [ ${PARAM} = "xuninstall" ] ; then
    echo "============================== uninstall ================================="
    ant uninstall
fi

echo "============================ Native app run ================================="

adb shell input keyevent 3
android update project -p . -s --target 1;
ant debug install
ant run
