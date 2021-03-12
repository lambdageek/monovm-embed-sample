
.PHONY: all run clean

# should match what's in the .csproj files, but we pass -p:TargetFramework to
# msbuild below directly, so we will override
TARGET_FRAMEWORK?=net6.0
RUNTIME_IDENTIFIER?=osx-x64

export TARGET_FRAMEWORK
export RUNTIME_IDENTIFIER

EXE:=

RUNTIME_PACK_DIR_FILE:= out/GetRuntimePack/bin/Debug/$(TARGET_FRAMEWORK)/$(RUNTIME_IDENTIFIER)/runtime-pack-dir.txt

all: out/.touch-CsharpSample $(RUNTIME_PACK_DIR_FILE) out/native/main$(EXE)

CSHARP_SAMPLE_CSPROJ:= src/CsharpSample/CsharpSample.csproj
GET_RUNTIME_PACK_CSPROJ:= src/GetRuntimePack/GetRuntimePack.csproj


CSHARP_SAMPLE_SRC:= \
	$(CSHARP_SAMPLE_CSPROJ)	\
	src/CsharpSample/CsharpSample.cs	\
	src/CsharpSample/Directory.Build.props

GET_RUNTIME_PACK_SRC:= \
	$(GET_RUNTIME_PACK_CSPROJ)	\
	src/GetRuntimePack/Directory.Build.props

NATIVE_SRC:= \
	src/native/Makefile \
	src/native/gen-managed.sh \
	src/native/main.c

out/.touch-CsharpSample: $(CSHARP_SAMPLE_SRC)
	dotnet publish $< -r $(RUNTIME_IDENTIFIER) -f $(TARGET_FRAMEWORK) --self-contained
	touch $@

$(RUNTIME_PACK_DIR_FILE): $(GET_RUNTIME_PACK_SRC)
	dotnet publish $< -r $(RUNTIME_IDENTIFIER) -f $(TARGET_FRAMEWORK) --self-contained

out/native/main$(EXE): $(NATIVE_SRC) $(RUNTIME_PACK_DIR_FILE) out/.touch-CsharpSample
	if [ "z$(RUNTIME_PACK_DIR_FILE)" = z -o ! -f "$(RUNTIME_PACK_DIR_FILE)" ]; then echo RUNTIME_PACK_DIR_FILE=$(RUNTIME_PACK_DIR_FILE) does not exist ; false ; fi
	make -C src/native runtime_pack_dir_file=$(realpath $(RUNTIME_PACK_DIR_FILE))

run: out/native/main$(EXE)
	out/native/main$(EXE)

clean:
	-rm -rf out
