# Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
# Use of this source code is governed by a license that can be found
# in the LICENSE file.

# Variant Test Makefile

SRC_DIR:=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
include ../../../mk/Prolog.mk

TARGET_EXE:=registry_test

CXX_SRC:= \
	RegistryTest_Main.cpp  \
	RegistryTest_Serialization.cpp

# CXX_HDR:=

CXXFLAGS+=-I../../lib

TARGET_OWN_LIBS:=iomgr

TARGET_COMMON_LIBS:=iomgr_shared unit_test io options crypto proto utils data sys stl_ext crt_ext

TARGET_LIBS:=-lboost_filesystem -lboost_log -lboost_thread -lboost_program_options \
		-lboost_system -lcrypto -lxxhash

include $(MK)/Main.mk
