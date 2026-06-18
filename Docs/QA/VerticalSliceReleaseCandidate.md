# Vertical Slice Release Candidate QA

## Commit Tested

Base commit inspected for this pass:

```text
4d8d3c320a686dd6e68e23a5f87211ba8ee6bfca
```

Final release-candidate pass commit is the commit that adds this document and the small prompt-priority stability fix.

## Build And Test Commands

Build:

```powershell
cmake --build build --config Debug
```

Tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

Local CMake was not available on this machine, so GitHub Actions remains the authoritative build/test gate for this pass.

## Full Manual Test Checklist

| Check | Result | Notes |
| --- | --- | --- |
| Launch app | Not locally run | Local CMake/build executable unavailable on this machine. |
| HUD/text/prompt/next-step lines render | Reasoned + CI covered compile | Rendering is manual-only; no GPU test added. |
| Toggle mouse look with `M` | Reasoned + existing tests | Input state and player look tests cover pure logic. |
| Arrow-key fallback | Reasoned + existing tests | Player look tests cover arrow fallback. |
| Gather wood, stone, fiber | Existing QA path | No code path changed. |
| Craft `primitive_tool` | Existing QA path | No code path changed. |
| Gather Cracked Rock | Existing QA path | Missing-tool prompt inspected. |
| Craft `camp_bundle` and `workbench_kit` | Existing QA path | No code path changed. |
| Place `camp_marker` or `workbench_stub` | Existing QA path | No code path changed. |
| Defeat Realm Wisp | Existing QA path | No code path changed. |
| Realm Fracture missing requirements | Existing tests/docs | Requirement message path unchanged. |
| Stabilise Realm Fracture | Existing tests/docs | Progression path unchanged. |
| Craft `realm_anchor` | Existing tests/docs | Recipe/progression path unchanged. |
| Save with `O` | Existing save/load tests | Save serializer unchanged. |
| Load with `P` | Existing save/load tests | Save serializer/apply path unchanged. |
| Verify yaw/pitch | Existing save/load tests | Existing persistence coverage remains. |
| Verify inventory | Existing save/load tests | Existing persistence coverage remains. |
| Verify placed buildables | Existing save/load tests | Existing persistence coverage remains. |
| Verify depleted gatherables | Existing save/load tests | Existing persistence coverage remains. |
| Verify fracture and Realm Wisp state | Existing save/load tests | Existing persistence coverage remains. |
| Verify HUD prompt/next-step after load | Reasoned | Derived from runtime state after load; no stored prompt state. |
| Escape clean shutdown | Existing platform path | No shutdown path changed. |

## Pass/Fail Table

| Area | Result | Notes |
| --- | --- | --- |
| CI configure/build/tests | Pending at commit time | Must be verified by GitHub Actions after push. |
| Mouse capture safety | Pass by inspection | Destructor releases cursor clipping and restores visibility; `M` toggles capture off. |
| Immediate mouse jump on enable | Pass by inspection | First captured frame recenters without adding delta. |
| Pitch clamp | Pass by test | Existing player look test covers clamp. |
| HUD/guidance readability | Fix made | Prompt priority adjusted so aimed targets are not masked by nearby enemy prompt. |
| Save/load regression | Pass by inspection/tests | Save data does not store prompt/guidance; these are derived after state restore. |
| Data fallback | Pass by existing tests | Item, recipe, and buildable fallback tests already exist. |

## Issues Found

One small guidance issue was found during inspection:

- A nearby Realm Wisp prompt could take priority over an aimed gatherable or interactable prompt.

## Fixes Made

- `ContextualPrompt` now prioritises direct target prompts over nearby enemy prompts.
- Added a regression assertion so a gatherable target still shows the gather prompt even when an enemy is relevant.

## Known Limitations

- This RC pass could not perform an interactive local playthrough because CMake is not available on the local PATH.
- Rendering and mouse cursor behaviour still require manual verification on a Windows machine with a local build.
- HUD text uses the primitive rectangle-glyph renderer and remains intentionally compact.
- Guidance is specific to the current Mistwood Hollow vertical slice.
- There is still no full UI, real font rendering, settings menu, input rebinding, controller support, terrain, procedural generation, audio/VFX, or multiplayer.

## Release-Candidate Readiness Result

Ready for release-candidate review once GitHub Actions is green.

## Stabilisation Pass 9 Local Verification Attempt

Stabilisation Pass 9 attempted to perform the missing local Windows release-candidate playthrough.

### Local Tooling Status

`cmake` is not available on the normal PATH:

```text
where cmake
INFO: Could not find files for the given pattern(s).

cmake --version
The term 'cmake' is not recognized as the name of a cmdlet, function, script file, or operable program.
```

The standard standalone CMake install path is also not present:

```text
C:\Program Files\CMake\bin\cmake.exe
not recognized as a command
```

Visual Studio's bundled CMake was found:

```text
C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
cmake version 4.3.1-msvc1
```

The requested Visual Studio 2022 generator failed because the local machine does not have the VS 2022 `v143` build tools available:

```text
error MSB8020: The build tools for Visual Studio 2022 (Platform Toolset = 'v143') cannot be found.
```

The installed Visual Studio is Visual Studio 2026. Its developer environment exposes `cl.exe` and Ninja:

```text
Visual Studio 2026 Developer Command Prompt v18.7.1
cl.exe: Version 19.51.36248 for x64
ninja.exe found under Visual Studio's CMake/Ninja tools
```

However, attempts to configure a local Ninja build from the Codex shell repeatedly hung during CMake configure. Those stuck `cmake`/`ninja` processes were stopped manually. No local executable was produced.

### Local Build/Test Result

Local configure/build/test did not complete in this environment. The local app was not launched, and the interactive playthrough was not performed.

GitHub Actions remains the authoritative build/test result for this repository until a normal local developer shell can run one of these successfully:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

or, on this machine's installed Visual Studio 2026 toolchain:

```powershell
cmake -S . -B build-vs18 -G "Visual Studio 18 2026" -A x64
cmake --build build-vs18 --config Debug
ctest --test-dir build-vs18 -C Debug --output-on-failure
```

### Local Interactive Playthrough Result

Not performed. The blocker is local toolchain/configure reliability from this shell, not a known project compile error.

### Final Pass 9 Verdict

Stabilisation Pass 9 does not certify the local interactive playthrough. It records the local tooling blocker exactly. The release candidate still requires a manual Windows playthrough from a working Visual Studio/CMake developer environment before it can be called locally verified.
