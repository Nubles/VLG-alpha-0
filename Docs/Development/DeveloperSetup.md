# Developer Setup

This guide explains how to build, run, and test Realmbound Wilds from a clean Windows checkout.

## Prerequisites

- Windows
- Visual Studio 2022 with C++ desktop development tools
- CMake 3.22 or newer
- Git

The project is a custom C++20 engine/game. It currently uses a Win32 platform backend with WGL and fixed-function OpenGL debug rendering.

## Configure

From the repository root:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

If this command fails because `cmake` is not recognized, install CMake or add the CMake executable folder to `PATH`. Visual Studio can also provide CMake through its developer tools, depending on your installation.

If local CMake is unavailable, GitHub Actions is the authoritative build/test gate for the repository.

## Build

```powershell
cmake --build build --config Debug
```

Expected Debug outputs include:

- `build/Debug/RealmboundWilds.exe`
- `build/Debug/EngineCoreTests.exe`
- `build/Debug/GameCoreTests.exe`

## Runtime Data Copy

After `RealmboundWilds` builds, CMake copies:

```text
Game/Data
```

to:

```text
build/Debug/Game/Data
```

The runtime data files currently include item, recipe, and buildable definitions.

## Run

```powershell
.\build\Debug\RealmboundWilds.exe
```

Run from the repository root if you want saves written under the repository `Saves` folder.

## Test

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

The main test executables are:

- `EngineCoreTests`
- `GameCoreTests`

## Save Location

Quick save writes to:

```text
Saves/quick_save.rwsave
```

The path is relative to the process working directory. When launching from the repository root, the save appears under the repo `Saves` folder.

## Common Local Issues

- `cmake` is not recognized: install CMake, add it to `PATH`, or use the Visual Studio developer environment.
- Configure succeeds but build fails: check the first compiler or linker error in the build output.
- App launches but data definitions seem missing: rebuild `RealmboundWilds` so CMake copies `Game/Data` beside the executable.
- Local environment differs from CI: trust GitHub Actions as the current repository gate, then compare the exact local command and generator.
