
UNAME_S=$(shell uname -s)

ifeq ($(SYSTEM),)
ifeq ($(UNAME_S),Linux)
SYSTEM:=linux
SOEXT:=.so
EXE:=
else ifeq ($(UNAME_S),Darwin)
SYSTEM:=osx
SOEXT:=.dylib
EXE:=
else
SYSTEM:=$(error could not detect the OS)
endif
endif

UNAME_M=$(shell uname -m)
ifeq ($(UNAME_M),arm64)
CPUARCH=arm64
else ifeq ($(UNAME_M),x86_64)
CPUARCH=x64
else
SYSTEM:=$(error could not detech the CPU architecture)
endif

