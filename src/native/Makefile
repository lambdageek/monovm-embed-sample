
topdir=../..

abstopdir=$(realpath ../..)

outdir=$(topdir)/out/native
absoutdir=$(abstopdir)/out/native

CFLAGS=-std=c99 -g

SOEXT?=.dylib

runtime_pack_dir_file?=$(topdir)/out/GetRuntimePack/bin/Debug/net5.0/osx-x64/runtime-pack-dir.txt

runtime_dir=$(shell cat $(runtime_pack_dir_file))/runtimes/osx-x64/native

CSHARP_SAMPLE_PUBLISH_DIR:=$(topdir)/out/CSharpSample/bin/Debug/net5.0/osx-x64/publish

SOURCES:= \
	main.c

CORELIB_FULL_PATH:=$(absoutdir)/System.Private.CoreLib.dll

TOUCH_COPY_LIBS:=$(outdir)/.touch-copylibs

GENERATED_DIR=$(outdir)/generated

GEN_TPA_H=$(GENERATED_DIR)/tpa.h

.PHONY: all

all: $(outdir)/main

$(outdir):
	mkdir -p $@

$(outdir)/main: $(SOURCES) $(TOUCH_COPY_LIBS) $(GEN_TPA_H) | $(outdir)
	clang $(CFLAGS) -o $@ $(SOURCES) -DCORELIB_FULL_PATH=\"$(CORELIB_FULL_PATH)\" -L$(runtime_dir) -lcoreclr -I$(GENERATED_DIR)
	install_name_tool -add_rpath @executable_path/ $@

$(outdir)/.touch-copylibs: $(runtime_pack_dir_file) | $(outdir)
	cp $(runtime_dir)/*$(SOEXT) $(outdir)
	cp $(runtime_dir)/System.Private.CoreLib.dll $(outdir)
	touch $@

$(GENERATED_DIR):
	mkdir -p $@

$(GEN_TPA_H): $(outdir)/../.touch-CsharpSample | $(GENERATED_DIR)
	./gen-managed.sh $(CSHARP_SAMPLE_PUBLISH_DIR) > $@
