# AI Chat BakkesMod Plugin

This repository assumes you are using Visual Studio on Windows. It leverages `vcpkg` in an effort to decouple external dependencies from source code.

## Getting Started

Run the command below on initial setup to install dependencies.

```
.\Setup.ps1
```

If you're using Visual Studio you can trigger builds with `Ctrl + B`.

If you are using Visual Studio 2019 you will need to update the 2 instances of `<PlatformToolset>` in `./ChatPlugin.vcxproj` to `v142` or you will get a build error.

If you're building from command line, you can use `msbuild` in the root of the repo.

```
msbuild ChatPlugin.sln
```

