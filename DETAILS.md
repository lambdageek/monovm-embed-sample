# Details of how the sample works

At a high level, the sample does 3 things:

1. Compiles and publishes a managed C# library [src/CsharpSample/](src/CsharpSample/).
2. Compiles and publishes a stub C# project that uses a custom target to write the location of the Mono runtime pack to a file. [src/GetRuntimePack/](src/GetRuntimePack/).
3. Compiles a native application that links with the Mono runtime from the runtime pack and that loads and runs some code from the sample C# library. [src/native/](src/native/).

## Compiling and publishing the managed C# library

There is nothing particularly special about the C# code itself.  (Although note that if you use platform specific APIs, it's probably a good idea to build it against the reference assemblies for that `RuntimeIdentifier`).

The `.csproj` sets the `PublishTrimmed` property to `true` and `TrimMode` to `copyused`: this means that the individual assemblies are not modified during publishing, but any that are completely unused are thrown away.  This is useful to ensure that the publish directory only contains the base class libraries that are actually needed by the sample.  Note that if you make heavy use of reflection, you may need to set `TrimMode` to `none` to ensure that the assemblies are not trimmed.

The actual publishing is just `dotnet publish -r RID --self-contained` where `RID` is the `RuntimeIdentifier` of the target platform and `self-contained` is a flag that tells dotnet to include the runtime in the published output so that we can pick up the class libraries.

## Discovering the location of the runtime pack

The `GetRuntimePack` project is a simple C# project that uses a custom target to write the location of the Mono runtime pack to a file.  The project specifies `UseMonoRuntime` to ensure that the Mono runtime is used.  The runtime pack is just a `.nupkg` with a name like `Microsoft.NETCore.App.Runtime.Mono.<RID>` (so a normal dotnet build will download it into the nuget packages folder), but you can't just add a `PackageReference` to it because runtime packs are treated specially by the .NET SDK.   But a custom target can look at the `ResolvedRuntimePack.PackageDirectory` metadata to find the location of the runtime pack.

The `GetRuntimePack` sample writes the runtime pack location to `out/GetRuntimePack/bin/Debug/net6.0/<RID>/runtime-pack-dir.txt`

## Building the native project

The native project copies the managed `.dll` libraries from the sample, and all the managed `.dll` and `.so` (or `.dylib`) native libraries from the Mono runtime pack into its output directory.  It also generates a `tpa.h` header that lists the full paths of the managed libraries that is used to initialize the runtime.

For various reasons, the Mono library in the runtime pack is named `libcoreclr`, although this may change in .NET 7+.

## Native project implementation

It's a fairly typical use of the Mono embedding API.  The one new thing in .NET 5 and .NET 6 embedding compared to Mono 6.x is that you have to call `monovm_initialize` with a key-value list of runtime configuration properties.  The most important one is the `TRUSTED_PLATFORM_ASSEMBLIES` property which is a path-separated list of the full paths of all the managed libraries that the application may want to load.

After initializing the runtime, the sample loads the `CsharpSample` assembly, finds a class in it, and calls a couple of methods.  See the Mono embedding API documentation for details on how this is supposed to work. It's the same as in previous Mono releases.

## Questions

Feel free to ask in the `#monovm` channel on [.NET Discord (aka.ms/dotnet-discord)](https://aka.ms/dotnet-discord) or open a GitHub Issue in this repo.
