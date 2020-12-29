
topdir=../..

outdir=$(topdir)/out/native

SOEXT?=dylib

runtime_pack_dir_file?=%(topdir)/GetRuntimePack/bin/Debug/net5.0/osx-x64/runtime-pack-dir.txt

runtime_dir=$(shell cat $(runtime_pack_dir_file))/runtimes/osx-x64/native

SOURCES:= \
	main.c

TOUCH_COPY_LIBS:=$(outdir)/.touch-copylibs

.PHONY: all

all: $(outdir)/main

$(outdir):
	mkdir -p $@

$(outdir)/main: $(SOURCES) $(TOUCH_COPY_LIBS) | $(outdir)
	clang -o $@ $(SOURCES) -L$(runtime_dir) -lcoreclr
	install_name_tool -add_rpath @executable_path/ $@

$(outdir)/.touch-copylibs:
	cp $(runtime_dir)/*.$(SOEXT) $(outdir) 
