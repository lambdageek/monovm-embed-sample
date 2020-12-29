# MonoVM Embedding sample #

**WARNING** This code is **EXPERIMENTAL** and not supported.


A sample suite of projects to show how to embed .NET 6 MonoVM in a native desktop application.

## Limitations ##

* This sample assumes OSX x64 in many places.
* It needs the mono embedding hearders which are currently not included in the runtime packs.
* It uses a gross hack to find out where the native Mono library is located.
* It doesn't actually run the managed sample yet.

## Prerequisites ##

* .NET 5 SDK (*TODO*: Use .NET 6 alpha builds)
* Clang (Xcode)
* GNU Make

## Building ##

Just run `make` from the toplevel folder

## Running ##

Run `make run`, or `out/native/main` directly.



