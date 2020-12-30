
.PHONY: all run clean

EXE:=

RUNTIME_PACK_DIR_FILE:= out/GetRuntimePack/bin/Debug/net5.0/osx-x64/runtime-pack-dir.txt

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
	dotnet publish $< -r osx-x64 --self-contained 
	touch $@

$(RUNTIME_PACK_DIR_FILE): $(GET_RUNTIME_PACK_SRC)
	dotnet publish $< -r osx-x64 --self-contained

out/native/main$(EXE): $(NATIVE_SRC) $(RUNTIME_PACK_DIR_FILE) out/.touch-CsharpSample
	make -C src/native runtime_pack_dir_file=$(realpath $(RUNTIME_PACK_DIR_FILE))

run: out/native/main$(EXE)
	out/native/main$(EXE)

clean:
	-rm -rf out
