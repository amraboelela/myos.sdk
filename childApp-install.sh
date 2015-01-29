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

TARGET=ChildApp
APPLICATION_DIRECTORY=$(pwd | awk -F'/' '{print $NF}')
APPLICATION_PATH=$(pwd)
source ${MYOS_PATH}/sdk/childApp-build.sh

cd ${APPLICATION_PATH}
echo "============================== Building ${APPLICATION_DIRECTORY} =============================="
make || exit
echo "============================== installing ${APPLICATION_DIRECTORY} =============================="
make copy
