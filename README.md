# MonoVM Embedding sample #

**WARNING** This code is **EXPERIMENTAL** and not supported.

A sample suite of projects to show how to embed .NET 5/.NET 6 MonoVM in a native desktop application.


## Limitations ##

* This sample assumes OSX x64 in many places.
* It needs the mono embedding hearders which are currently not included in the runtime packs ([dotnet/runtime#46461](https://github.com/dotnet/runtime/issues/46461)), 
  the sample includes them in the `vendor/mono-headers/` directory, but this is not a good idea.
* It uses a gross hack to download a `Microsoft.NETCore.App.Mono` runtime pack for the current platform and to find the native Mono library within it.
* It pulls .NET 5 runtime packs, not .NET 6 previews

### Help Wanted ###

- If there's a problem in the sample [open an Issue in this repo](https://github.com/lambdageek/monovm-embed-sample/issues/new/choose)
- If the sample doesn't work [open an Issue in dotnet/runtime](https://github.com/dotnet/runtime/issues/new/choose) and mention "Mono embedding API"
- If you can improve the sample [please open a PR](https://github.com/lambdageek/monovm-embed-sample/compare) 

## Prerequisites ##

* .NET 5 SDK
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


