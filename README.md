# MonoVM Embedding sample #

**WARNING** This code is **EXPERIMENTAL** and not supported.

A sample suite of projects to show how to embed .NET 5/.NET 6 MonoVM in a native desktop application.

## Limitations ##

* This sample assumes OSX x64 in many places.
* It needs the mono embedding hearders which are currently not included in the runtime packs ([dotnet/runtime#46461](https://github.com/dotnet/runtime/issues/46461)), 
  the sample includes them in the `vendor/mono-headers/` directory, but this is not a good idea.
* It uses a gross hack to download a `Microsoft.NETCore.App.Mono` runtime pack for the current platform and to find the native Mono library within it.

## Prerequisites ##

* .NET 5 SDK (*TODO*: Use .NET 6 alpha builds)
* Clang (Xcode)
* GNU Make

## Building ##

Just run `make` from the toplevel folder

## Running ##

Run `make run`, or `out/native/main` directly.

Expected output:
```
runtime initialized
Hello From Managed
```


