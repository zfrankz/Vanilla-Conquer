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
- [x] Run vanillatd / vanillara with freeware retail data (RA: missions played; TD: English main menu)

Data layout (`~/Library/Application Support/Vanilla-Conquer/`):
- `vanillara/`: `MAIN.MIX`, `REDALERT.MIX` from the Allied CD
- `vanillatd/`: shared MIX files from the GDI CD root, plus `UPDATE`, `UPDATEC`, `TRANSIT`,
  `SPEECH`, `*ICNH` and `CCLOCAL.MIX` extracted from the CD's `INSTALL/SETUP.Z` (InstallShield 3).
  `vanillatd/gdi/` and `vanillatd/nod/` hold each CD's `GENERAL.MIX` and `MOVIES.MIX`.
  The CD's loose `INSTALL/CCLOCAL.MIX` is German – do not use it.
- [x] GitHub fork of Vanilla-Conquer (https://github.com/zfrankz/Vanilla-Conquer), push `cfe-port`

## Phase 1 – Feature inventory
Map every README feature to its CFE commits and files, then bucket it:
- A: pure game logic – port directly
- B: already in VC (megamaps, RA-derived INI/CRC/straw/pipe infra in common/, shared bugfixes) – skip or reconcile
- C: GlyphX/DLLInterface-only (pixel-perfect zoom, legacy-render auto-disable, GlyphX LAN fixes, wall preview) – drop
- D: needs input/UI in the legacy client (attack-move, rally points, wall Ctrl, Alt-click) – reimplement

## Phase 2 – Config plumbing
Port `CFEPatchConfig` (CFE `TIBERIANDAWN/EXTERNS.H:85`, used in 52 files), load via VC's INI
from `~/Library/Application Support/Vanilla-Conquer/<game>/`. All features default off.

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
