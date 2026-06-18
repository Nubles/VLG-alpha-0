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

## CI Playtest Artifact Path

Stabilisation Pass 10 updates GitHub Actions so successful `Windows Build` runs upload a Debug playtest artifact:

```text
realmbound-wilds-debug-playtest
```

The artifact is the recommended workaround while local configure/build remains unreliable from this shell.

Expected artifact contents:

- `RealmboundWilds.exe`
- `Game/Data/**`

The artifact should not contain:

- local `Saves/`
- local build directories
- CMake/Ninja/Visual Studio intermediate folders

Manual RC verification should now be performed by downloading this artifact from a successful GitHub Actions run, extracting it on Windows, launching `RealmboundWilds.exe`, and running the full checklist above. Rendering, cursor capture, HUD text, prompt/guidance behavior, and save/load still require manual Windows execution.

## Stabilisation Pass 11 CI Artifact Run

Stabilisation Pass 11 downloaded and ran the CI-produced Debug playtest artifact.

### Artifact Tested

Workflow run:

```text
27777071129
```

Commit:

```text
65a8377aeb27bf3ab9bb2786c0fe08e33fb0efa1
```

Artifact:

```text
realmbound-wilds-debug-playtest
```

Extracted test folder:

```text
C:\tmp\VLG-artifact-test\extracted
```

### Artifact Contents Result

Pass. The extracted artifact contained:

- `RealmboundWilds.exe`
- `Game/Data/Buildables/buildables.txt`
- `Game/Data/Items/items.txt`
- `Game/Data/Recipes/recipes.txt`

No save file was copied into the artifact folder before launch.

### Artifact Launch Result

Pass. `RealmboundWilds.exe` launched from the extracted artifact folder and opened a live window. The window title reported:

- `Mistwood Hollow`
- `HP 100`
- `ST 100`
- `target None`
- `enemy Idle 50`
- `obj fracture unknown`
- `mouse off`

### Manual Playthrough Pass/Fail Table

This pass used Win32 key events and the app's title/debug output as the observable signal. It did not provide a human visual viewport for precise aiming at scene objects, so any check that depends on visual target acquisition remains not certified by this pass.

| Check | Result | Notes |
| --- | --- | --- |
| App opens | Pass | Artifact executable launched and stayed running. |
| Mistwood Hollow loads | Pass | Window title reported `Mistwood Hollow`. |
| HUD bars render | Not visually certified | Health/stamina state was visible in the debug title, but render output was not visually inspected. |
| Debug text renders | Not visually certified | Debug/title text updated; in-window glyph rendering was not visually inspected. |
| `PROMPT:` line appears | Not visually certified | Prompt rendering requires visual viewport inspection. |
| `NEXT:` line appears | Not visually certified | Guidance rendering requires visual viewport inspection. |
| `H` help appears and is accurate | Pass by title/debug output | Help text appeared and listed movement, mouse look, interaction, combat, build, craft, save/load, and exit controls. |
| Arrow-key look works | Partial | Arrow keys were accepted without crash; camera direction was not visually confirmed. |
| `M` mouse look enables | Pass by title/debug output | Title changed to `mouse on` and logged `Mouse look enabled`. |
| Moving mouse changes view direction | Not visually certified | Mouse movement was sent while captured; visual camera motion was not observable from this shell. |
| Pitch clamps and camera does not flip | Covered by tests, not visually certified | Existing pure tests cover clamp logic. |
| `M` mouse look disables | Pass by title/debug output | Title changed to `mouse off` and logged `Mouse look disabled`. |
| Cursor visible/restored after disabling | Partial | The app released mouse-look state; cursor visibility was not visually inspected. |
| Gather wood, stone, and fiber | Not certified | Requires visual aiming at gatherables. Debug grants were separately verified through title inventory updates. |
| Prompt changes when aiming at gatherables | Not certified | Requires visual/aiming playthrough. |
| Cracked Rock missing-tool prompt | Not certified | Requires visual/aiming playthrough. |
| Craft `primitive_tool` | Pass by title/debug output | Debug crafting produced `primitive_tool:1`. |
| Gather Cracked Rock | Not certified | Requires visual/aiming playthrough. |
| Craft `camp_bundle` | Pass by title/debug output | Debug crafting produced `camp_bundle:1`. |
| Craft `workbench_kit` | Partial | Attempt correctly failed with missing wood after prior craft costs. |
| Enter build mode with `B` | Pass by title/debug output | Title reported build mode and selected buildable. |
| Select/place camp or workbench | Pass by title/debug output | `Placed Camp Marker` was logged. |
| Build prompt appears in build mode | Not visually certified | Build mode title state was confirmed; HUD prompt rendering was not visually inspected. |
| Fight Realm Wisp | Not certified | Requires visual aiming/combat playthrough. |
| Direct prompts not masked by Wisp prompt | Covered by tests, not visually certified | Prompt-priority regression test exists. |
| Defeat Realm Wisp | Not certified | Requires combat playthrough. |
| Interact with Realm Fracture | Not certified | Requires visual aiming/playthrough. |
| Missing requirement feedback | Covered by tests/docs, not visually certified | Objective requirement tests cover message construction. |
| Stabilise Realm Fracture | Not certified | Requires visual aiming/playthrough. |
| Craft `realm_anchor` | Partial | Attempt correctly failed while recipe was locked. |
| Press `O` to save | Pass by title/debug output | Logged `Saved game to Saves/quick_save.rwsave`. |
| Move to new position | Partial | Movement key was sent; visual position was not inspected. |
| Rotate camera | Partial | Arrow key was sent; visual yaw change was not inspected. |
| Place or gather something additional | Partial | Camp marker placement was confirmed before save. |
| Press `P` to load | Pass by title/debug output | Logged `Loaded save data`. |
| Player position restored | Not visually certified | Save/load succeeded; position restoration was not visually inspected. |
| Camera yaw/pitch restored | Not visually certified | Save/load succeeded; yaw/pitch restoration was not visually inspected. |
| Inventory restored | Pass by title/debug output | Inventory after load matched saved title state. |
| Placed buildables restored | Partial | Build mode/title state remained; visual placed object restoration was not inspected. |
| Depleted gatherables restored | Not certified | Requires real gather/depletion playthrough. |
| Realm Fracture state restored | Not certified | Fracture was not stabilised in this pass. |
| Realm Wisp state restored | Not certified | Wisp was not defeated in this pass. |
| HUD prompt and `NEXT:` after load | Not visually certified | Requires visual HUD inspection. |
| Escape exits | Pass | Process exited after Escape. |
| Cursor not stuck after shutdown | Partial | Process exited cleanly after mouse look was disabled; cursor state was not visually inspected. |

### Bugs Found

No code bugs were confirmed during the artifact launch and shell-driven control checks.

The remaining blocker is verification coverage: this environment can launch the native artifact and send keys, but it cannot honestly complete precise visual aiming, HUD glyph inspection, combat, gatherable depletion, or Realm Fracture interaction as a human playthrough.

### Fixes Made

No code fixes were made in Stabilisation Pass 11.

### Cursor And Mouse Capture Result

Partial pass. `M` toggled mouse-look state on and off, and the executable exited cleanly after mouse look had been disabled. Human-visible cursor restoration after shutdown still needs direct desktop observation.

### HUD, Prompt, And Guidance Result

Partial pass. The title/debug state updated correctly for help, mouse state, inventory, build state, save, and load. The in-window HUD bars, `PROMPT:`, and `NEXT:` glyph rendering still need direct visual confirmation.

### Save/Load Result

Partial pass. Saving created:

```text
C:\tmp\VLG-artifact-test\extracted\Saves\quick_save.rwsave
```

Loading reported `Loaded save data`, and inventory state remained consistent after load. Full restoration of visual position, camera orientation, placed buildables, depleted gatherables, Realm Fracture state, and Realm Wisp state still needs a real manual playthrough.

### Final Pass 11 Verdict

The CI artifact is downloadable, extractable, launchable, and capable of basic debug-control/save-load operation. Stabilisation Pass 11 does not fully certify the release candidate as manually playthrough-verified because visual target acquisition, rendered HUD inspection, combat, gatherable depletion, and Realm Fracture completion were not completed from this shell-driven environment.
