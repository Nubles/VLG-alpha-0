# GitHub Actions

The repository uses a single Windows workflow named:

```text
Windows Build
```

It runs on pushes to `main`, pull requests to `main`, and manual `workflow_dispatch` runs.

## What The Workflow Does

The workflow:

- checks out the repository
- configures CMake
- builds the Debug configuration
- runs `EngineCoreTests`
- runs `GameCoreTests`
- uploads the build log artifact on build failure
- uploads a Debug playtest artifact on success

## How To Inspect Failures

1. Open the failed workflow run in GitHub Actions.
2. Open the `Configure, build, and test` job.
3. Check the failed step first.
4. If the build step failed, read the emitted error annotations and the build output.
5. If present, download the `windows-build-output` artifact.

## Playtest Artifact

Successful workflow runs upload a playable Debug prototype artifact named:

```text
realmbound-wilds-debug-playtest
```

The artifact contains:

- `RealmboundWilds.exe`
- the copied runtime data folder under `Game/Data`

It intentionally does not upload the entire CMake build tree, local save files, or `Saves/`.

## How To Download And Run The Artifact

1. Open the successful `Windows Build` workflow run in GitHub Actions.
2. Scroll to the Artifacts section.
3. Download `realmbound-wilds-debug-playtest`.
4. Extract the downloaded `.zip` to a normal folder.
5. Run `RealmboundWilds.exe` from the extracted folder.

If Windows blocks the downloaded executable, open the file properties and use the unblock option if present, or approve the SmartScreen warning only if the artifact came from the expected repository workflow run.

This artifact is a Debug prototype for playtesting. It is not a signed release package, installer, or final distribution build.

## Common Failure Categories

- Compile error: a C++ source file does not compile on the Windows runner.
- Link error: a symbol or library is missing.
- Missing file in CMake: a new `.cpp` file was added but not listed in `CMakeLists.txt`.
- Test assertion failure: `EngineCoreTests` or `GameCoreTests` returned a failure.
- Missing data file copy: runtime code expects a file under `Game/Data`, but the file was not copied next to the executable.
- Missing playtest artifact contents: the artifact should contain `RealmboundWilds.exe` and `Game/Data`.

## What To Include When Reporting A Failure

Include:

- commit hash
- failed step name
- exact compiler, linker, or test error
- changed files
- whether the failure happens locally, only in CI, or both
- relevant build artifact name, if one was uploaded

## Local Equivalent Commands

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```
