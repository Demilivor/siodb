# Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
# Use of this source code is governed by a license that can be found
# in the LICENSE file.

# Makefile for the common library "io"

SRC_DIR:=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
include ../../../../../mk/Prolog.mk

TARGET_LIB:=siodb_common_io

CXX_SRC:= \
	FdIo.cpp  \
	MemoryMappedFile.cpp

C_SRC:= \
	FileIO.c

CXX_HDR:= \
	MemoryMappedFile.h \
	FdIo.h  \
	IoBase.h  \
	IOError.h  \
	StreamFormatGuard.h

C_HDR:= \
	FileIO.h \

include $(MK)/Main.mk
