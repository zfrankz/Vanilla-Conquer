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

Two original-TD war factory bugs that F6 (instant build) exposes, fixed always-on (user's choice,
2026-10-29; not CFE, no switch):
- A unit finished while the factory is still unloading the previous one got an ignored
  `MISSION_UNLOAD` and sat inside forever, and production stopped with "Ready" showing.
  `BuildingClass::Exit_Object` now returns 1 (try later) while the factory is in `MISSION_UNLOAD`,
  as Red Alert does; `HouseClass::Place_Object` only counts 2 as exited. The AI retries after 3 s.
- Clicking "Ready" while the factory was busy cancelled the unit ("no factory"). The click now
  asks for the exit again when a factory exists but is busy; it still cancels if none is left.
Verified with a temporary harness (save 4, jeep added to the sidebar, instant build): before,
production stopped after 3 jeeps for 851+ frames (same with A* on and off); after, 25 and 23
jeeps in 1440 frames, longest gap 89/116 frames. Clicking "Ready" on a busy factory: before
101 clicks → 101 cancels; after 49 clicks → 0 cancels. Not verified: the AI path, and the
"no factory left" cancel at runtime.

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
2. Harvester features (queue jump, optimization, self-repair, smarter harvesters/refineries) – ported, play-test pending
   - Done (one commit each, switch off = original code): `HARVY_SELF_REPAIR`, `SMARTER_REFINERIES`,
     `SMARTER_HARVESTERS`. Each builds; TD starts with them compiled in (40 s smoke test).
   - Save format unchanged: `sizeof(UnitClass)` stays 400 (temporary print), and all 9 existing TD saves
     pass the version check in `Get_Savefile_Info`. Smarter harvesters' two flags use the padding byte
     after `Flagged`; the last stuck cell is kept in an unsaved per-slot table.
   - Differences from CFE: no `HasOpened` in VC (refinery check excludes construction/deconstruction
     instead); the wake-up flag is cleared for every unit on a new mission (CFE clears it only for
     gunboats); `Frame` instead of CFE's per-object `FrameMB()`.
   - `HARVY_QUEUE_JUMP`, `HARVY_LOAD_BALANCING`, `HARVY_OPTIMIZE`: ported together in one commit (they share
     `Mission_Harvest`, `Find_Best_Refinery` and the refinery's radio handling). Adds `RADIO_CANCEL`.
     Builds, no warnings on changed lines, 9/9 saves pass the version check, 40 s smoke test.
     13 key conditions string-compared with CFE HEAD (terms, not parenthesisation).
   - A-Star distance variants: ported in step 4c. CFE's A-Star `FootClass::Find_Docking_Bay` came with step 4a.
   - Step 2 play-tested by the user on 2026-10-29: nothing odd seen.
3. Tiberium growth/resurgence, visceroids – ported (except meaner visceroids), play-tested by the user on 2026-10-29 (nothing odd seen)
   - Done (one commit each, switch off = original code): `BETTER_TIB_GROWTH` + `TIB_GROWTH_SCALE`,
     `TIB_RESURGENCE` + `TIB_RESURGE_DELAY`, `TS_VISCEROID_SPAWN`, `TREE_VISCEROID_SPAWN_MODE` +
     `TREE_VISCEROID_SPAWN_OVERRIDE`. Each builds with no warnings on changed lines; 9/9 saves pass the
     version check; 40 s smoke test. Resurgence and tree spawning only run with `BETTER_TIB_GROWTH` on, as in CFE.
   - Differences from CFE: rolls above 15 bits use two draws from the game's own generator instead of
     `rand()` (uniformity checked with 300000 draws); new per-cell/per-infantry/map state lives in unsaved
     tables or statics instead of saved members (resurgence build-up restarts after a load); the visceroid
     grow-in animation is Remastered-renderer only, so spawning just plays the squish sound.
   - Moved: `MEANER_VISCEROIDS` needs CFE's per-unit firepower/armor/speed biases (come with veterancy,
     step 5) and its per-scenario type-stat adjustments (balance, step 8); port it after step 5.
     CFE's killed-by-visceroid and chem-soldier visceroid spawns come with it / with the balance options.
   - General bugfix bucket: CFE `1cab6b4` and `d2edf47` (`Goto_Tiberium` efficiency and megamap radius, no switch).
4. A* pathing + tiberium aversion – 4a, 4b and 4c ported, play-test pending
   (SEARCH.H turned out to be unrelated: it came with CFE's INIClass backport.)
   - 4a `ASTAR_PATHING` + `INFANTRY_TIBERIUM_AVERSION` (commit `f5e038eb`): A* search, CFE's traffic-flow cell
     cost (`CFE_Passable_Cell`, includes the tiberium aversion), periodic repathing, A* `Find_Docking_Bay`.
     Builds with no warnings on changed lines; the three FINDPATH functions and four helpers compared
     with CFE HEAD ignoring whitespace (only the listed differences remain).
     Instrumented runs (temporary code, removed): all 9 TD saves load and run 35 s each with A* on and
     the player's units ordered to hunt; no crash; no path step into a MOVE_NO cell; A* found 85–100 % of
     paths on saves 1–8. Save 9's failures are Delphi, boxed into a 24-cell area, hunting an unreachable
     target (the original search can't reach it either). Hovercraft ordered to hunt drive onto land with
     A* on and off alike (TD's terrain table gives hover speed on land); only the test order makes them do it.
   - Differences from CFE: `emplace` instead of `try_emplace` (C++11); blocker list as pointer+count;
     `RepathDelay`/`PriorCell` in unsaved side tables; the repath `Random_Pick` and the A* docking-bay
     choice only run with the switch on (CFE runs both always); no per-unit `SpeedBias` (not ported);
     `ReservingVehicle` lookup added in 4b. Kept as in CFE: `Find_Path` retries pass `threat`, not
     `asThreat`, and `Find_Passable_Position_Near` swaps x/y for the side columns (`XY_Cell(y, left)`).
   - 4b `CELL_RESERVATION_FIX` (new switch, not a CFE setting: CFE always runs these): CFE's "two units in
     one spot" fixes, from CFE HEAD for the hunks of `c1cc791`, `6240eb9`, `0409a4f`, `5a38301`, `3b95607`
     (and `b7e4d45`). Cells record which unit or aircraft set the vehicle flag (`Reserving_Vehicle()`);
     `Occupy_Down/Up` and `Mark_Track` leave another unit's flag alone; on a track jump the old track's flags
     are lifted before `TrackNumber` changes; at the per-cell point a unit re-checks its second cell and stops
     if something now blocks it; `UnitClass`/`InfantryClass::Can_Enter_Cell` get CFE's rewrite of the
     reserved-cell checks and the diagonal-crossing check (in separate functions, called only with the switch
     on); stale vehicle flags are cleared every frame; `CFE_Passable_Cell` uses the reserving unit.
   - Differences from CFE: the reserving unit is in an unsaved side table, rebuilt after a load from where
     each vehicle is, is heading and its track midpoint (CFE saves it, `0409a4f`/`5a38301`); the stale-flag
     cleanup runs from `LogicClass::AI` (CFE runs it inside `Map.Clean()`, which only its DLL calls each
     tick); no rank-3 veteran buggy crushing (needs veterancy, step 5); `Arbitrary_Smooth_Turn` not ported
     (CFE only uses it in disabled code).
   - Verified: builds, no warnings on changed lines. Instrumented runs (temporary code, removed): all 9 saves,
     switch off and on, 45 s each with the player's units hunting: no crash; too little traffic to compare
     (0–1 overlaps either way). Stress test on save 4, 30 light tanks in two groups swapping sides every 400
     frames, 4 group splits per setting, totals at frame 1200:
     A* off: fix off 215 same-cell pairs / 58 episodes / 181 arrivals; fix on 130 / 68 / 191.
     A* on: fix off 233 / 32 / 635, plus 121 frames with two stopped tanks stacked; fix on 46 / 20 / 686, 0.
     No orphan vehicle flags in any run. One run (A* on, fix off) produced no output at all; rerun was normal.
     Not verified: real play; that the rebuilt table after a load matches what CFE would have saved.
   - 4c (no new switch; active when `ASTAR_PATHING` and the harvester switches are on): A* harvester distances
     from CFE HEAD (CFE `6061399` and later). Refinery queue jump, load balancing and optimize measure the A*
     path length in cells; when A* returns 0 the straight-line cell distance is used up to 6 cells, beyond
     that the refinery counts as unreachable (left out of the choice; a waiting harvester counts as infinitely
     far). If no refinery is reachable, straight-line distances in cells are used. The cells-to-leptons
     conversion of `QUEUE_JUMP_CUTOFF` and the optimize weights runs only with A* off again, as in CFE.
   - Differences from CFE: the three copies of the A*-or-nearby distance code are one function,
     `UnitClass::CFE_AStar_Refinery_Distance`.
   - Verified: builds, no warnings on changed lines. Instrumented runs (temporary code, removed) on save 2
     with 2 extra refineries and 4 extra harvesters added (3 refineries, 9 harvesters): A* on, 12000 frames:
     114 refinery choices, 342 A* distances (A* path sum 3530 cells vs straight-line 3365), 0 unreachable,
     0 fallbacks, 6 queue jumps, all with the waiting harvester over the 4-cell cutoff and the newcomer closer.
     A* off, 4000 frames: 39 choices, all in leptons, 1 queue jump. No crash.
     Not verified: real play; an unreachable refinery (no test map had one); the AI's harvesters (CFE limits
     these features to human players in single player).
5. Veterancy, smarter SAMs/mammoths/aircraft/commandos – in progress: 5a–5c ported, play-test pending; 5d and 5e to do
   - Split: 5a small switches, 5b `SMARTER_SAMS`, 5c `SMARTER_AIRCRAFT`, 5d `SMARTER_CPU_COMMANDOS`, 5e veterancy.
   - 5a: `DISABLE_COMMANDO_AIRSTRIKES` (no enemy A10 reinforcements in single player until the player has a
     discovered building), `SMARTER_MAMMOTHS` (mammoths use missiles on infantry), `COMMANDO_GUARD` (0 off,
     1 multiplayer only, 2 always: a guarding human commando shoots infantry only).
   - Differences from CFE: with `COMMANDO_GUARD` not applying, the commando's threat code stays the original.
     CFE dropped the original line `TechnoClass::Greatest_Threat(threat | THREAT_INFANTRY | THREAT_BUILDINGS)`
     for all non-guarding commandos, unconditionally; here that only happens when the switch applies.
   - Verified: builds, no warnings on changed lines. Instrumented runs (temporary code, removed):
     Mammoth vs an enemy rifleman, 800 frames: off 14 cannon shots, 0 missiles; on 0 cannon shots, 14 missiles.
     Commando in guard next to an enemy light tank, 800 frames, riflemen added from frame 300: mode 0 never
     takes a target; mode 2 targets infantry on 375 frames and the tank on 0.
     Two A10s sent as an enemy reinforcement: switch on with player buildings, sent (saves 1, 2); on with no
     buildings, not sent (saves 3, 5, 6); off, sent on all four saves tried.
     Not verified: real play; a mission with owned but undiscovered buildings; mammoths against non-infantry
     (the new code only acts on infantry targets); `COMMANDO_GUARD=1` in multiplayer.
   - 5b `SMARTER_SAMS` + `SMARTER_SAM_AMMO` + `SMARTER_SAM_BALANCE_DELAY`: a SAM keeps firing until its ammo is
     out instead of reloading after 2 shots, and with shots left it looks for a new target for 2 seconds
     before going down.
   - Differences from CFE: ammo and lowering delay are in unsaved side tables (`SAMAmmo` and
     `SAMLoweringDelay` are saved in CFE); the ammo count stops at 0, so a SAM caught mid-volley by a load
     fires its next shot and reloads. The rank-3 quad-shot and `LocalROFBias` parts come with veterancy (5e).
   - Verified: builds, no warnings on changed lines. Instrumented runs (temporary code, removed) on save 2,
     one player SAM against up to 4 enemy Orcas, 1500 frames: off 2 shots; ammo 2, 4 shots in volleys of 2;
     ammo 6, a volley of 6 about 90 frames apart per pair, a new target for each pair; with one-hit Orcas
     the same, then a reload and a new volley.
     Not verified: real play; a load during a volley; `SMARTER_SAM_BALANCE_DELAY` other than 0.
   - 5c `SMARTER_AIRCRAFT`: new helicopters use a free pad, the primary if possible; helicopters without a pad
     fly in level with the primary pad; an aircraft with no ammo told to attack goes straight home; every 120
     frames of flight it re-picks its attack position, so it follows a moving target.
   - Differences from CFE: the recheck counter is in an unsaved side table (saved in CFE). CFE's check in
     `Mission_Move` for attack-moving aircraft without ammo comes with attack-move (step 7).
   - Verified: builds, no warnings on changed lines. Instrumented runs (temporary code, removed) on save 2 with
     the map revealed. Two pads, primary busy: off picks the primary, on the free one; both free or both
     busy: the primary either way. 4 helicopters with no pad: off arrive at random heights (35–59, pad at 52),
     on all at 52. Orca with no ammo: off goes on to pick a position, on heads home at once. Orca attacking a
     jeep on the move: off flies to the old position and first fires on frame 229 from 3759 leptons; on
     re-picks on frame 138 and first fires on frame 170 from 1240 leptons.
     Not verified: real play; the helicopter's firing accuracy; AI aircraft behaviour.
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
