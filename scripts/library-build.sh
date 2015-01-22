#
# Copyright © 2014 myOS Group.
#
# This is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# Contributor(s):
# Amr Aboelela <amraboelela@gmail.com>
#

if [ ${TARGET} = "All" ]; then
    export TARGET=NativeApp
    source ${MYOS_PATH}/android/sdk/scripts/library-make.sh
    export TARGET=NativeActivity
    source ${MYOS_PATH}/android/sdk/scripts/library-make.sh
    TARGET=All
else
    export TARGET=${TARGET}
    source ${MYOS_PATH}/android/sdk/scripts/library-make.sh
fi
