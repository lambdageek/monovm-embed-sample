# MonoVM Embedding sample #

**WARNING** This code is **EXPERIMENTAL** and not supported.

A sample suite of projects to show how to embed .NET 5/.NET 6 MonoVM in a native desktop application.


## Limitations ##

* Only works on OSX or Debian 11, or Ubuntu 20.04 (other recent Linux distros may work too)
* It needs the mono embedding hearders which are currently not included in the runtime packs ([dotnet/runtime#46461](https://github.com/dotnet/runtime/issues/46461)), 
  the sample includes them in the `vendor/mono-headers/` directory, but this is not a good idea.
* Until https://github.com/dotnet/runtime/issues/49661 is fixed, Linux builds need `patchelf`

### Help Wanted ###

- If there's a problem in the sample [open an Issue in this repo](https://github.com/lambdageek/monovm-embed-sample/issues/new/choose)
- If the sample doesn't work [open an Issue in dotnet/runtime](https://github.com/dotnet/runtime/issues/new/choose) and mention "Mono embedding API"
- If you can improve the sample [please open a PR](https://github.com/lambdageek/monovm-embed-sample/compare) 

## Prerequisites ##

* .NET 6 Preview 7 SDK
* Clang (Xcode on osx, or manually install on Linux)
* GNU Make
* (Linux only) [`patchelf`](https://github.com/NixOS/patchelf) (Ubuntu has a .deb package)

## Building ##

Just run `make` from the toplevel folder

## Running ##

Run `make run`, or `out/native/main` directly. (You may need to set `LD_LIBRARY_PATH` on Linux)

Expected output:
```
runtime initialized
.NET 6.0.0-preview.7.21377.19
Hello From Managed
```


## Using a local build of the runtime ##

This is the process for using this sample with a local build of the runtime.

### Building the runtime ###

Build a checkout of dotnet/runtime using something like

```
./build.sh -s mono+libs -c Release
```

Verify that `artifacts/bin/microsoft.netcore.app.runtime.osx-x64/Release` (or
other RID and configuration) exists.  It should contain a
`runtimes/osx-x64/native` directory.  The makefile dependson it.

### Limitations ###

1. The `CsharpSample` is compiled using a normal (installed) `dotnet` SDK, so
it won't be able to directly use any new managed APIs from the dotnet/runtime
checkout.  If the new API is in `System.Private.CoreLib.dll`, you may be able
to call it using reflection.

2. We will take most libraries from the normal installed dotnet runtime, the
only thing we pick up from the local build is `libcoreclr.dylib` and
`System.Private.CoreLib.dll` this may lead to broken apps.

3. If you add a new `MONO_API` and want ot use it in the native sample, you
   will need to update the headers in `vendor/mono-headers` to include it.

### Building the sample ###

Pass the `LOCAL_RUNTIME` variable on the `make` command line:

```
make LOCAL_RUNTIME=~/work/runtime/artifacts/bin/microsoft.netcore.app.runtime.osx-x64/Release
make LOCAL_RUNTIME=~/work/runtime/artifacts/bin/microsoft.netcore.app.runtime.osx-x64/Release run
```

The output from running should be

```
runtime initialized
.NET 6.0.0-dev
Hello From Managed
```

If it doesn't say `.NET 6.0.0-dev` it's not using the local build.

When in doubt, `rm -rf out/` and try again.
