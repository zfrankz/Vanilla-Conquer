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
- `vanillara/`: `MAIN.MIX`, `REDALERT.MIX` from the Allied CD
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

## Phase 2b – Cheats (TD and RA, not part of CFE)
Single-player and skirmish only, refused in multiplayer: these actions bypass the event
queue and would desync. Off by default; enabled with `[Cheats] Enabled=yes` in the game INI.
Each cheat gets a function key in the legacy client; every activation shows a short
in-game message (e.g. "Cheat: +5000 credits").

Default keys, rebindable in the INI like VC's other `Key*` options:

| Key | Cheat |
|---|---|
| F1 | +5000 credits |
| F2 | Map reveal |
| F3 | Fog/shroud reveal (toggle) |
| F4 | Spawn commando at cursor |
| F5 | Full construction catalog (toggle) |
| F6 | Instant construction (toggle) |

Why F1–F6: TD only uses them in multiplayer (messages, `conquer.cpp:855`) and drops them
in SP/skirmish (`conquer.cpp:920`); F4 is otherwise only a debug key. In RA, grep finds F2/F6
only in the optional map editor (`MAP_EDITORRA`). Not yet tested in the running games.
On Mac keyboards these need Fn unless "Use F1, F2, etc. keys as standard function keys" is on.

| Cheat | Existing VC code | Work |
|---|---|---|
| +5000 credits | `HouseClass::Refund_Money` (TD `house.h:509`, RA `house.h:773`) | call on `PlayerPtr` |
| Map reveal (one-shot: all cells become explored) | reveal-map crate: loop `Map.Map_Cell(cell, house)` over `MAP_CELL_TOTAL` (TD `cell.cpp:2195`, RA `cell.cpp:2493`) | same loop for `PlayerPtr`, then `Map.Flag_To_Redraw(true)` |
| Fog/shroud reveal (toggle: shroud not drawn, everything visible) | `Debug_Unshroud` (TD `conquer.cpp:463`, RA `debug.cpp:323`) | bind toggle, force redraw |
| Spawn commando at cursor | debug keys already unlimbo objects at `Map.Pixel_To_Coord(mouse)` | TD `INFANTRY_RAMBO`, RA `INFANTRY_TANYA` |
| Full construction catalog | `Debug_Cheat` → tech level 98 (TD `house.cpp:727`; RA `house.cpp:909` also zeroes prerequisites, behind `CHEAT_KEYS`) | TD: zero prerequisites too, as TD's `DebugUnlockBuildables` already does; RA: drop the `CHEAT_KEYS` guard for our flag; refresh sidebar |
| Instant construction | TD `Debug_Instant_Build` (`factory.cpp:276`) | RA: none, add the same check to RA `FactoryClass` |

VC's `debug.cpp` key handlers sit behind `CHEAT_KEYS`, which no VC CMake file defines. Don't
enable it wholesale (it also adds explosion, win/lose and editor keys); reuse the flags only.

## Phase 3 – TD features (one commit per feature, built and play-tested on/off)
1. Small bugfixes, tiberium index fix
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
