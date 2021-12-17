
topdir=../..

abstopdir=$(realpath ../..)

include $(topdir)/os-detect.mk

outdir=$(topdir)/out/native
absoutdir=$(abstopdir)/out/native

CFLAGS=-std=c99 -g

runtime_pack_dir_file?=$(topdir)/out/GetRuntimePack/bin/Debug/$(TARGET_FRAMEWORK)/$(RUNTIME_IDENTIFIER)/runtime-pack-dir.txt

runtime_dir=$(shell cat $(runtime_pack_dir_file))/runtimes/$(RUNTIME_IDENTIFIER)/native

CSHARP_SAMPLE_PUBLISH_DIR:=$(topdir)/out/CsharpSample/bin/Debug/$(TARGET_FRAMEWORK)/$(RUNTIME_IDENTIFIER)/publish

MONO_INCLUDE_DIR:=$(topdir)/vendor/mono-headers/include/mono-2.0

SOURCES:= \
	main.c

CORELIB_FULL_PATH:=$(absoutdir)/System.Private.CoreLib.dll

TOUCH_COPY_LIBS:=$(outdir)/.touch-copylibs

GENERATED_DIR=$(outdir)/generated

GEN_TPA_H=$(GENERATED_DIR)/tpa.h

ifeq ($(SYSTEM),osx)
INSTALL_NAME_TOOL_CMD:=install_name_tool -add_rpath @executable_path/
PATCHELF_CMD:=-true
else ifeq ($(SYSTEM),linux)
INSTALL_NAME_TOOL_CMD:=-true
PATCHELF_CMD:=cd $(outdir) && patchelf --set-soname libcoreclr$(SOEXT) libcoreclr$(SOEXT)
endif

.PHONY: all

all: $(outdir)/main

$(outdir):
	mkdir -p $@

$(outdir)/main: $(SOURCES) $(TOUCH_COPY_LIBS) $(GEN_TPA_H) | $(outdir)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -DCORELIB_FULL_PATH=\"$(CORELIB_FULL_PATH)\" -L$(outdir) -lcoreclr -I$(GENERATED_DIR) -I$(MONO_INCLUDE_DIR)
	$(INSTALL_NAME_TOOL_CMD) $@

$(TOUCH_COPY_LIBS): $(runtime_pack_dir_file) | $(outdir)
	cp $(runtime_dir)/*$(SOEXT) $(outdir)
	cp $(runtime_dir)/System.Private.CoreLib.dll $(outdir)
	$(PATCHELF_CMD)
	touch $@

$(GENERATED_DIR):
	mkdir -p $@

$(GEN_TPA_H): $(outdir)/../.touch-CsharpSample | $(GENERATED_DIR)
	./gen-managed.sh $(CSHARP_SAMPLE_PUBLISH_DIR) > $@
