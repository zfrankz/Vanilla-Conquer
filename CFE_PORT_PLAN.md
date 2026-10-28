# CFE Patch Redux → Vanilla-Conquer port plan

Goal: a native macOS (arm64/universal2) build of Vanilla-Conquer that carries
the CFE Patch Redux gameplay features (TD 1.9 / RA 1.8).

Sources:
- CFE fork: https://github.com/zfrankz/CnC_Remastered_Collection (local remote `cfe`)
- EA base it diverged from: `d9e287e` (2020-06-01)
- Vanilla-Conquer upstream: `upstream/vanilla` @ `ce83b59e`

CFE delta vs EA base (whitespace ignored): 552 commits;
TD +26.5k/-1.9k lines in 449 files, RA +11.8k/-2.2k lines in 630 files.

## Phase 0 – Baseline
- [x] `brew install cmake ninja openal-soft` (SDL2 via sdl2-compat)
- [x] `CMAKE_PREFIX_PATH=/opt/homebrew/opt/openal-soft cmake --preset release && cmake --build --preset release`
- [x] Run vanillatd / vanillara with freeware retail data (RA: missions played; TD: GDI and Nod missions played)

Data layout (`~/Library/Application Support/Vanilla-Conquer/`):
- `vanillara/`: `REDALERT.MIX`; each disc's `MAIN.MIX` in `allied/` and `soviet/`. The game switches
  folder by mission side (`Change_Local_Dir` in `redalert/conquer.cpp`), which is how the Soviet videos
  are found; a `MAIN.MIX` in `vanillara/` itself would always win, so there must not be one.
  Soviet disc: `gamedata/soviet/RA_Soviet.iso`, converted from the MODE1/2352 bin in `RA_Soviet.zip`.
- `vanillatd/`: shared MIX files from the GDI CD root, plus `UPDATE`, `UPDATEC`, `TRANSIT`,
  `SPEECH`, `*ICNH` and `CCLOCAL.MIX` extracted from the CD's `INSTALL/SETUP.Z` (InstallShield 3).
  `vanillatd/gdi/` and `vanillatd/nod/` hold each CD's `GENERAL.MIX` and `MOVIES.MIX`.
  The CD's loose `INSTALL/CCLOCAL.MIX` is German – do not use it.
- [x] GitHub fork of Vanilla-Conquer (https://github.com/zfrankz/Vanilla-Conquer), push `cfe-port`

## Phase 1 – Feature inventory ✅ (see `CFE_FEATURE_INVENTORY.md`)
Map every README feature to its CFE commits and files, then bucket it:
- A: pure game logic – port directly
- B: already in VC (megamaps, RA-derived INI/CRC/straw/pipe infra in common/, shared bugfixes) – skip or reconcile
- C: GlyphX/DLLInterface-only (pixel-perfect zoom, legacy-render auto-disable, GlyphX LAN fixes, wall preview) – drop
- D: needs input/UI in the legacy client (attack-move, rally points, wall Ctrl, Alt-click) – reimplement

## Phase 2 – Config plumbing ✅
Port `CFEPatchConfig` (CFE `TIBERIANDAWN/EXTERNS.H:85`, used in 52 files), load via VC's INI
from `~/Library/Application Support/Vanilla-Conquer/<game>/`. All features default off.

Done in `tiberiandawn/cfeconfig.{h,cpp}` and `redalert/cfeconfig.{h,cpp}`:
- Struct copied from CFE: same fields, types, defaults and order (TD 98, RA 53; checked by script).
- Same INI keys and sections as CFE (TD 94, RA 50 entries), so CFE users' INIs work unchanged.
  Files: `CFEPATCHREDUX.INI` (TD), `CFEPATCHREDUX_RA.INI` (RA). A lowercase file was found in the
  runtime test; VC's `RawFileClass` looks up existing files ignoring case (`common/rawfile.cpp:177`),
  not tested on a case-sensitive volume.
- Same post-processing (clamps, lepton conversion when A* is off, computed weights); script-compared
  with CFE: RA identical, TD identical except `AllowSandbagging`'s if/else written as one comparison.
- Differences from CFE: no `DEFAULT.CFEPATCHREDUX*.INI` (which turns most features on), and the
  user's file is never rewritten. Uses a C++11 functor because VC builds with `-std=c++11`.
- Called where CFE calls it: TD after `Options.Load_Settings()`, RA right after `Bootstrap()`.
- Runtime check (temporary stderr print, removed): with no file both games report the defaults
  (`astar=0 wall=5 vet=0 qjump=1024`); with a test file they report `astar=1 wall=10 vet=2 qjump=4`
  (`WALL_BUILD_LENGTH=20` clamped to 10). Nothing reads the config yet, so gameplay is unchanged.

## Phase 2b – Cheats (TD and RA, not part of CFE) ✅
`tiberiandawn/cheats.{h,cpp}`, `redalert/cheats.{h,cpp}`. Single-player and skirmish only
(`GAME_NORMAL`/`GAME_SKIRMISH`, not in the map editor): the actions bypass the event queue
and would desync multiplayer. Off unless `[Cheats] Enabled=yes` in `conquer.ini` / `redalert.ini`
(the games re-read that file before saving options, so the section survives). Every activation
shows an in-game message, e.g. "Cheat: +5000 credits".

| Key (INI entry, default SDL scancode) | Cheat | Implementation |
|---|---|---|
| F1 (`KeyCredits`, 58) | +5000 credits | `PlayerPtr->Refund_Money(5000)` |
| F2 (`KeyRevealMap`, 59) | Map reveal, one-shot | reveal-map crate loop: `Map.Map_Cell` over `MAP_CELL_TOTAL` |
| F3 (`KeyRevealShroud`, 60) | Shroud off/on toggle | `Debug_Unshroud`; reset by the game at each mission start |
| F4 (`KeySpawnCommando`, 61) | Commando (TD) / Tanya (RA) at the cursor cell | `new InfantryClass` + `Unlimbo`, guard mission; message if the cell is blocked |
| F5 (`KeyFullCatalog`, 62) | Full catalog toggle | `PlayerPtr->DebugUnlockBuildables` (tech level 98, no prerequisites) + `IsRecalcNeeded`, as the Remastered debug request does |
| F6 (`KeyInstantBuild`, 63) | Instant construction toggle | player's factories only; completes on the next production step when the rest of the price is affordable (still paid in full). TD reuses the `Debug_Instant_Build` check site, RA gets the same check |

Not a key: `StartMission=N` makes "Start New Game" begin the chosen side's campaign at mission N,
with that mission's own starting credits and no carry-over money. TD: GDI 1–15, Nod 1–13.
RA (`redalert.ini`): Allies and Soviets 1–14, first existing map file (`SC?NNEA.INI` for all 28 in
the installed data, checked with a temporary print); falls back to mission 1 if none exists.
In-game: the user reported on 2026-10-28 that RA Start New Game with `StartMission=2` began at mission 2.
TD `StartMission` not yet confirmed in-game.

Keys are compared without modifiers (Shift+F1 does nothing) and with `WWKEY_VK_BIT` stripped.
Key codes: F9 is stored by the games as `KeyBookmark1=66`, so F1–F6 are 58–63.

Why F1–F6 (real grep, both games): outside debug and map-editor code they're used only for
multiplayer chat (TD `conquer.cpp:855`, RA `conquer.cpp:1083`) and TD multiplayer lobby dialogs
(`netdlg.cpp`); F9–F12 are bookmarks. On Mac keyboards they need Fn unless "Use F1, F2, etc.
keys as standard function keys" is on.

Verified: both games build; cheat code is linked (`nm`); runtime config check (temporary print,
removed): no section → `enabled=0 keys=58,59,60,61,62,63`; `Enabled=yes`, `KeyCredits=70` →
`enabled=1 keys=70,59,60,61,62,63`, in both games.
In-game: the user reported on 2026-10-27 that all six cheats work in both games.
Not verified: commando placement on occupied cells.

VC's `debug.cpp` handlers sit behind `CHEAT_KEYS`, which no VC CMake file defines; not enabled.

## Phase 3 – TD features (one commit per feature, built and play-tested on/off)
1. Small bugfixes, tiberium index fix – ported, play-test pending
   - Done (one commit each, switch off = original code): `TIB_INDEX_BUGFIX`, `GDI9_FIX`,
     `DEATHS_DOOR_FIX`, `DEAD_BUILDING_SCATTER_FIX`, `DISTANCE_CALC_FIX`, `NUKE_RADIUS_FIX`,
     `GUNBOAT_BUGFIX`, `BURNING_TREE_FIX`. Each builds; TD starts with all eight on (30 s smoke test).
   - Deferred: `SELL_CHANCE_FIX` (needs CFE's `EffectiveScenario`), `HUNT_FIX` + `SEARCH_RANGE_FIX`
     (CFE adds 108 lines to `TECHNO.CPP`, a 90-line search function at CFE HEAD; port together), hunks inside CFE-only code (A* cell weights,
     new tiberium spread function, meaner visceroids) come with those features.
   - Crash found in play-test (burning tree, `AnimClass::~AnimClass`), fixed in `c0407c42` by porting
     CFE `8f5389c`, which my first pass missed because it doesn't name the switch. From now on each
     feature's CFE commits are also found by message keywords and by `git log -L` on the touched functions.
   - Not ported on purpose: CFE's extra `Limbo()` in `TerrainClass::AI` (VC's destructor already
     calls it); CFE's unflagged tidy-ups in the same functions.
2. Harvester features (queue jump, optimization, self-repair, smarter harvesters/refineries)
3. Tiberium growth/resurgence, visceroids
4. A* pathing + tiberium aversion (FINDPATH.CPP, SEARCH.H)
5. Veterancy, smarter SAMs/mammoths/aircraft/commandos
6. Walls (TS-style building, crushing, ownership, AI wall shooting)
7. Bucket D: attack-move, rally points, Q-move
8. Balance patch, JD's adjustments

VC renamed files to lowercase and clang-formatted them, so CFE commits cannot be
cherry-picked; each feature's diff is applied by hand, then clang-formatted.

## Phase 4 – RA features (same process)

## Phase 5 – Multiplayer / 8 players
VC `MAX_PLAYERS 6`; needs legacy lobby/score UI work. Needs a CFE config hash shared
between peers to avoid desyncs. Until then CFE features are SP/skirmish only.

## Phase 6 – Packaging
universal2 `.app`, dylibbundler, ad-hoc codesign, GitHub Actions macOS job.
