
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
