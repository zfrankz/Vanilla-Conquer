# CFE Patch Redux – feature inventory (Phase 1)

Source: CFE fork `cfe/master` @ `d80c1bf` (TD 1.9 / RA 1.8).
Target: Vanilla-Conquer `upstream/vanilla` @ `ce83b59e`.

## How big the port really is

The raw diff against EA's first release (`d9e287e`) overstates the work. It includes:
- EA's official patches (post-launch, Aug 6, Aug 12, Sep 16, Sep 24). VC already has all
  five on its `vanilla` branch.
- A license-header wording change (`e8be463`) that touched nearly every file.

Measured against EA's Sep 24 hotfix (`7d496e8`), whitespace and `//` comment lines ignored:

| | Files | Lines added / removed |
|---|---|---|
| TD, everything | 173 | +24,189 / −799 |
| TD, minus the RA support code CFE copied into TD (all already in VC `common/`) | 135 | +13,719 / −3,157 |
| RA | 118 | +9,035 / −576 |
| `DLLInterface.*` + `SIDEBARGlyphx.*` (Remastered client glue, both games) | 4 | +581 / −41 |

The TD removals jump from −799 to −3,157 in the second row because CFE renamed `INI.CPP` → `INIOLD.CPP`.

EA's Nov 18 2024 patch (`dba4f55`, 7 lines in 6 files) is in neither VC nor CFE.

## Buckets

- **A – port directly**: game logic, gated by a config flag.
- **A\*** – logic, but it also touches Remastered glue; port the logic and drop the glue part.
- **B – already in VC**: skip or reconcile.
- **C – Remastered-only**: drop.
- **D – needs work in VC's legacy input/UI**.

The `TD` and `RA` columns give the number of `ActiveCFEPatchConfig.<Field>` references, counted
outside `CFEUTIL.CPP` and `EXTERNS.H`.

### Infrastructure

| Item | Bucket | Notes |
|---|---|---|
| `CFEPatchConfig` struct + loader (`EXTERNS.H`, `CFEUTIL.CPP`) | A | 98 fields TD, 53 RA. Loader reads/writes `CFEPATCHREDUX[_RA].INI` under `My Documents`; rewrite on VC `INIClass` + `Paths.User_Path()`. |
| RA support code copied into TD by CFE (`06f73fb`): b64pipe/b64straw/base64/buff/crc/fixed/ini/int/listnode/mp/pipe/pk/random/readline/rndstraw/search/sha/straw/xpipe/xstraw | B | All 20 present in VC `common/`. |
| `RAINI.CPP` / `INIOLD.CPP` (TD) | B / review | TD's old INI code, renamed. VC's TD already uses `common/ini`. `INIOLD.CPP` carries `ForceLoanerCopters` and `DefaultCustomMissionScenarioNumber` hooks; port those into VC's scenario INI code. |
| `CFEPATCHQUERY.cpp` (TD 471 / RA 341 lines) | A | Helper predicates (`CFE_Patch_Is_Wall`, `CFE_Patch_Can_Have_Rally_Point`, `CFE_Patch_Should_Extend_Walls`, …). |
| `CFEDEBUG.H` | A | Debug printf to the Remastered log; map it to VC's `DBG_LOG`. |
| `RANGEWRAPPER.H`, Vector `std::begin/end` (`a514953`) | A | Small C++ helpers. |
| `mod.mix` / `mod_ra.mix` (243 / 247 bytes) | A (asset) | Legacy-format MIX loaded in `INIT.CPP`. 2 entries each: `ZVTRN.SHP` (id `d9f7b655`, 122 bytes – veterancy pips for the legacy renderer) and an unidentified 85-byte file (id `54c2d545`). Must ship with the game data. |
| Mod Data `*.tga`, `*.DDS`, `*.XML`, `*.WAV` | C | Remastered client assets. |
| Savegame layout changes (padding, new members) | A | CFE breaks savegame compatibility by design; VC CFE saves will be incompatible with stock VC saves. |
| New `MISSION_ATTACKMOVE`, `MISSION_QATTACKMOVE`, `ACTION_ATTACKMOVE`, event changes | A | Changes the network protocol: CFE-VC and stock-VC builds can't play MP together. |

### Shared features (TD and RA)

| Feature | Flag(s) | TD | RA | Bucket | Notes |
|---|---|---|---|---|---|
| A* pathing | `EnableASPathing` | 13 | 6 | A | TD `FINDPATH.CPP` +1,174 lines. |
| Suspend building repairs | `EnableOOMRepair` | 1 | 1 | A | |
| Rally points | `EnableRallyPoints` | 1 | 2 | A | Set/clear through `EVENT.CPP`. Draws a dotted line in Remastered and `CC_Draw_Line` in legacy (already in CFE). |
| Harvester queue jumping | `EnableHarvyQueueJump`, `HarvyQueueJumpCutoff` | 4+2 | 5+2 | A | |
| Harvester load balancing | `EnableHarvyLoadBalancing` | 8 | 7 | A | |
| Harvester optimization | `EnableHarvyOptimize`, `HarvyOptimize*` | 12+4 | 11+4 | A | Some tuning ints are loaded but never read. |
| Attack-move | `EnableAttackMove` (+ RA `EnableChronoTankAttackMove`) | 6 | 10+1 | D (small) | Shift is read via `DLL_Export_Get_Input_Key_State(KN_LSHIFT)` in `What_Action()`. VC already has `#ifdef REMASTER_BUILD` → `Keyboard->Down(Options.KeySelect*)`; reuse that pattern. UI-side only, so MP-safe. |
| Q-move | TD `EnableQMove`; RA `EnableQMoveLoops`, `EnableAirQMove`, `EnableAirQAMLRecall` | 1 | 5+1+1 | D | TD toggle lives only in `DLLInterface.cpp`, so it needs a legacy hotkey. VC RA already has `HouseClass::IsQueuedMovementToggle`. |
| Commando / Tanya guard | `EnableCommandoGuard` / `EnableTanyaGuard` | 1 | 1 | A | |
| Safe sabotage | `SafeSabotage` | 2 | 2 | A | |
| Smarter aircraft | `EnableSmarterAircraft` | 6 | 8 | A | |
| Smarter repair bays | `EnableSmarterRepairBays` | 4 | 5 | A | |
| Configurable build distance | TD `BuildingGap`; RA `BuildingGapOffset`, `EnableLongGapFix` | 5 | 6+2 | A* | DLLInterface placement preview is glue; legacy uses `DisplayClass::Passes_Proximity_Check`. |
| Tiberium / ore growth scale | `TibGrowthScale` / `OreGrowthScale` | 4 | 5 | A | |
| TS/RA2-style wall building | `WallBuildLength`, `AllowSandbagging`, `WallsAlwaysHaveFullCost` | 11+3+4 | 8+3 | D (small) | Ctrl disables it. `CFE_Patch_Should_Extend_Walls()` reads Ctrl inside building-placement logic: **not lockstep-safe in VC MP** — the Ctrl state must travel in the PLACE event. |
| Inverse wall placement preview | `InverseBuildingPreviewMode` | 3 | 3 | C | Remastered-only by design (normal markers work in legacy). |
| Better tiberium / ore growth | `BetterTibGrowth` / `BetterOreGrowth` | 1 | 2 | A | |
| Tiberium / ore index bugfix | `TibIndexBugfix` / `OreIndexBugfix` | 11 | 5 | A | |
| Veterancy | `EnableVeterancy` | 7 | 8 | A | Legacy pip drawing already in CFE (`ZVTRN.SHP` from `mod.mix`). |
| Burning tree fix | `EnableBurningTreeFix` | 6 | 4 | A | |
| Death's door | `EnableDeathsDoorFix` | 1 | 1 | A | |
| New skirmish AI groundwork | `EnableNewSkirmishAI` | 8 | 7 | A | Not in README feature list. |
| Version announce | `EnableVersionAnnounce` | 1 | 1 | A | Uses the in-game message system. |

### TD-only features

| Feature | Flag(s) | Refs | Bucket | Notes |
|---|---|---|---|---|
| Infantry tiberium aversion | `InfantryTiberiumAversion` | 2 | A | Requires A*. |
| Harvester self-repair | `EnableHarvySelfRepair` | 2 | A | |
| Disable commando airstrikes | `DisableCommandoAirstrikes` | 1 | A | |
| Smarter SAMs | `EnableSmarterSAMs`, `SmarterSAMAmmo`, `SmarterSAMBalanceDelay` | 8+2+1 | A | |
| Smarter mammoths | `EnableSmarterMammoths` | 1 | A | |
| Smarter harvesters / refineries | `EnableSmarterHarvesters`, `EnableSmarterRefineries` | 2+1 | A | |
| Smarter CPU commandos | `SmarterCPUCommandos` | 10 | A | |
| Gunboat bugfix | `EnableGunboatBugfix` | 3 | A | |
| Tiberium resurgence | `TibResurgence`, `TibResurgeDelay` | 1+1 | A | |
| Visceroids (TS spawn, tree spawn, meaner) | `TSVisceroidSpawn`, `TreeVisceroidSpawn*`, `MeanerVisceroids` | 1+3+18 | A | Check the spawn animation asset in Phase 3. |
| Nuke tank | `EnableNukeTank` | 2 | Defer | Code ports, but the art only exists as Remastered textures. Needs legacy SHP art or reused graphics. |
| MP preplaced unit orders / disable neutral attacks | `MPPreplacedUnitOrderMode`, `DisableMPNeutralAttacks` | 4+1 | A | |
| GDI9 fix | `EnableGDI9Fix` | 1 | A | |
| Building death announcements | `BuildingDeathAnnounceMode` | 4 | A / check | Check whether the EVA lines exist in the legacy `SPEECH.MIX`. |
| Sell chance fix | `EnableSellChanceFix` | 1 | A | |
| Loaner copters | `ForceLoanerCopters` | 3 | A | Hook lives in `INIOLD.CPP` / `REINF.CPP`. |
| AI wall shooting / wall crushing / advanced wall ownership | `AIvsWalls`, `EnableWallCrush`, `EnableAdvancedWallOwnership` | 7+2+9 | A | |
| Hunt fix / search radius fix / distance calc fix | `EnableHuntFix`, `SearchRangeFixMode`, `EnableDistanceCalcFix` | 1+3+3 | A | |
| Nuke radius fix | `EnableNukeRadiusFix` | 1 | A | |
| Forced MCV engineer / split buildables / forbidden SP capturables | `MCVAlwaysSpawnEngineer`, `SplitDualCapturables`, `BuildForbiddenSPCapturables` | 1+7+2 | A / check | Split buildables adds new buildable types; check that legacy sidebar cameos exist. |
| Dead building scatter fix | `DeadBuildingScatterFix` | 1 | A | |
| Shroud remover | `ShroudRemover` | 4 | A* | Drop the DLLInterface line. |
| Quieter airstrips | `QuieterAirstrips` | 3 | A* | One reference is in `SIDEBARGlyphx.CPP`; map it to VC's legacy `sidebar.cpp`. |
| TD balance patch | `Override*`, `ApplyMPBalanceToSP`, `MPUnitsInSP` | ~25 | A | Mostly `SCENARIO.CPP`. |
| JD's extra adjustments | `JDExtraAdjustments` | 7 | A | |
| MP civilian sight | `MPCivSight` | 1 | A | |
| Custom-mission INI tags (scenario number, dinos, loaners) | `DefaultCustomMissionScenarioNumber` + tags | 1 | A | |
| Legacy render auto-disable | `LegacyRenderDisableMode` | 2 | C | Remastered client workaround. |

### RA-only features

| Feature | Flag(s) | Refs | Bucket |
|---|---|---|---|
| Instant capture | `EnableInstantCapture` | 4 | A |
| Ore/silo capacity | `OreCapacityOffset` | 1 | A |
| Gem overload fix / ore→gem promotion | `GemOverloadFix`, `OreToGemPromotionSlowFactor` | 3+2 | A |
| Inaccuracy bugfix | `EnableInaccuracyBugfix` | 2 | A |
| OpenRA supers | `EnableWideSupers`, `EnableQuickSupers` | 2+14 | A |
| Smarter chrono | `EnableSmarterChrono` | 8 | A |
| Chrono disaster overhaul | `EnableTimeQuakeFix`, `EnableSharedChronoDisasterOdds`, `EnableCumulativeTimelineDamage`, `SharedChronoDisasterOdds` | 3+2+5+2 | A |
| MIG ammo fudge | `EnableMIGAmmoFudge` | 1 | A |
| Building capture announcements | `EnableBuildingCaptureAnnouncements` | 1 | A / check (Remastered added `GENCAPT1` audio) |
| Smarter sonar | `EnableSmarterSonar` | 1 | A |
| Turkey bonus mode | `TurkeyFixMode` | 2 | A |

### Features without a flag

| Item | Bucket | Notes |
|---|---|---|
| Megamaps (TD) | B | VC compiles VanillaTD with `MEGAMAPS`. CFE's own port (`3e274c8`…`d515fff`) came from VC; reconcile the few CFE follow-ups (tib growth scaling on megamaps `d515fff`, `Goto_Tiberium` search radius `d2edf47`, base-node remap `f46a7ab`). |
| 8 players (TD) | Phase 5 | VC `MAX_PLAYERS 6`; CFE `8`. Needs legacy lobby/score UI and remap tables. CFE already made legacy remap tables for the new team color (`1d22980`). |
| Larger object limits, 64 KB map INI buffer | A | `44b0f3b`, `da3a781`, `ac86714`. |
| ~35 commits "lifted from vanilla-conquer" | B | Already in VC. Verify case by case; skip when present. |
| Other vanilla bugfixes (engineers, chinooks, power, layers, two-units-one-cell, ReservingVehicle, sound/PlayerPtr fixes …) | A / review | The largest bucket of unflagged changes. Port after the flagged features. Some touch remaster-only PlayerPtr issues. |
| Per-house unit visibility fixes (`79d7e0e` and follow-ups) | review | Written against Remastered's per-house visibility; check how much applies to the legacy renderer. |
| Pixel-perfect zoom | C | Remastered client. |
| Silver Funpark | A / C | Dino color: CFE did both remaster and legacy paths (`e1e51c4`). |
| Red USSR flag for Soviet barracks | C | Remastered texture. |
| TD unit renames (`a4c5909`) | check | Depends on whether names come from code or Remastered XML. |
| Tile patches (N64 Nod2 / GDI1) | C | Remastered textures/XML. |
| GlyphX save/load, padding, rally-dot-count crash workarounds | C | |

## Consequences for the plan

1. Bucket D is smaller than expected. Attack-move, rally points, walls and veterancy already have
   legacy-renderer or key-state paths in CFE. The real UI work left is the TD Q-move hotkey and
   making the wall Ctrl state lockstep-safe.
2. The config loader is the first deliverable: every feature depends on `ActiveCFEPatchConfig`.
3. Port order stays as in `CFE_PORT_PLAN.md`, with two additions:
   - Ship `mod.mix` / `mod_ra.mix` next to the game data (veterancy pips).
   - Port the unflagged vanilla bugfixes as their own batch after the flagged features.
4. Items to check while porting: nuke tank art, building death/capture EVA audio,
   split-buildables cameos, unit renames, per-house visibility fixes.
