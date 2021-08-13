
.PHONY: all run clean

include os-detect.mk

# should match what's in the .csproj files
TARGET_FRAMEWORK?=net6.0
RUNTIME_IDENTIFIER?=$(SYSTEM)-x64

export TARGET_FRAMEWORK
export RUNTIME_IDENTIFIER


ifndef LOCAL_RUNTIME
RUNTIME_PACK_DIR_FILE:= out/GetRuntimePack/bin/Debug/$(TARGET_FRAMEWORK)/$(RUNTIME_IDENTIFIER)/runtime-pack-dir.txt
else
RUNTIME_PACK_DIR_FILE:= out/local-build/runtime-pack-dir.txt
endif

all: out/.touch-CsharpSample $(RUNTIME_PACK_DIR_FILE) out/native/main$(EXE)

CSHARP_SAMPLE_CSPROJ:= src/CsharpSample/CsharpSample.csproj
GET_RUNTIME_PACK_CSPROJ:= src/GetRuntimePack/GetRuntimePack.csproj


CSHARP_SAMPLE_SRC:= \
	$(CSHARP_SAMPLE_CSPROJ)	\
	src/CsharpSample/CSharpSample.cs	\
	src/CsharpSample/Directory.Build.props

GET_RUNTIME_PACK_SRC:= \
	$(GET_RUNTIME_PACK_CSPROJ)	\
	src/GetRuntimePack/Directory.Build.props

NATIVE_SRC:= \
	src/native/Makefile \
	src/native/gen-managed.sh \
	src/native/main.c

DOTNET_PUBLISH_ARGS=-r $(RUNTIME_IDENTIFIER) -f $(TARGET_FRAMEWORK) -p:TargetFramework=$(TARGET_FRAMEWORK)

out/.touch-CsharpSample: $(CSHARP_SAMPLE_SRC)
	dotnet publish $< $(DOTNET_PUBLISH_ARGS) --self-contained
	touch $@

ifndef LOCAL_RUNTIME
$(RUNTIME_PACK_DIR_FILE): $(GET_RUNTIME_PACK_SRC)
	dotnet publish $< $(DOTNET_PUBLISH_ARGS) --self-contained  /bl:out/GetRuntimePack.binlog
else
$(RUNTIME_PACK_DIR_FILE): $(LOCAL_RUNTIME)
	if [ "z$(LOCAL_RUNTIME)" = z -o ! -d "$(LOCAL_RUNTIME)/runtimes/$(RUNTIME_IDENTIFIER)/native" ]; then echo expected $(LOCAL_RUNTIME) to contain a runtimes/$(RUNTIME_IDENTIFIER)/native subdirectory; false ; fi
	mkdir -p $(dir $(RUNTIME_PACK_DIR_FILE))
	echo "$(LOCAL_RUNTIME)" > $(RUNTIME_PACK_DIR_FILE)
endif

out/native/main$(EXE): $(NATIVE_SRC) $(RUNTIME_PACK_DIR_FILE) out/.touch-CsharpSample
	if [ "z$(RUNTIME_PACK_DIR_FILE)" = z -o ! -f "$(RUNTIME_PACK_DIR_FILE)" ]; then echo RUNTIME_PACK_DIR_FILE=$(RUNTIME_PACK_DIR_FILE) does not exist ; false ; fi
	make -C src/native runtime_pack_dir_file=$(realpath $(RUNTIME_PACK_DIR_FILE))

ifeq ($(SYSTEM),linux)
SET_LDLIBRARY_PATH=LD_LIBRARY_PATH=out/native
endif

run: out/native/main$(EXE)
	$(SET_LDLIBRARY_PATH) out/native/main$(EXE)

clean:
	-rm -rf out
