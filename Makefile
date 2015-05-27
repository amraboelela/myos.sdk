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

CFLAGS+=-fblocks
LDLIBS+=-lCoreFoundation

ifeq ($(BASE_OS),android)
    LDLIBS+=-llog
endif

ifeq ($(TARGET),NativeApp) && ($(BASE_OS),android)
    LIB_NAME=$(APPLICATION_NAME)
    LDLIBS+=-landroid
    OBJECTS+= \
        rd_app_glue.o \
        rd_main.o \

    include ${MYOS_PATH}/sdk/$(BASE_OS)/makefile-shared
else
    APP_NAME=$(APPLICATION_NAME)
    OBJECTS+= \
        main.o \

    include ${MYOS_PATH}/sdk/$(BASE_OS)/makefile-executable
endif