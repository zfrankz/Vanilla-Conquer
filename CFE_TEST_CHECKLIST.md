# CFE port – play-test checklist

Timings below are in game frames. A frame lasts `GameSpeed`/60 seconds (`conquer.ini`), so 20 frames a second at
GameSpeed=3 and 15 at the default 4, if the computer keeps up.

Settings files live in `~/Library/Application Support/Vanilla-Conquer/vanillatd/`:
- `cfepatchredux.ini`: CFE switches (`1` on, `0` off). It currently turns on the 8 TD fixes below.
- `conquer.ini`, section `[Cheats]`: `Enabled=yes` turns on the F1–F6 cheats.
  `StartMission=9` there makes Start New Game begin at mission 9 of the side you pick (GDI 1–15, Nod 1–13).
  Red Alert has the same setting in `vanillara/redalert.ini` (Allies and Soviets 1–14).

The cheats help set up tests: F1 for money, F3 to see the whole map, F5 to build anything, F6 for instant build.

If something goes wrong, note the mission, what you did, and the switches that were on. If the game crashes, send the crash log.

## 1. Retest the crash (most important)
- [ ] **Burning tree:** set a tree on fire with a flame tank or a flamethrower soldier, then wait until the flames end.
  Expected: the tree crumbles and disappears, and the game keeps running.
  It crashed here before commit `c0407c42`.
- [ ] Do it again with several trees burning at once, and with a burning tree that is also hit by other weapons.

## 2. The 8 TD fixes (all on)
Some of these are hard to see. "No crash, game plays normally" is a useful result too.

- [ ] **`BURNING_TREE_FIX`:** a tree that has been set on fire burns down instead of staying as an indestructible obstacle.
  After it crumbles, units can drive through its spot.
- [ ] **`TIB_INDEX_BUGFIX`:**
  - Harvesters keep harvesting and delivering credits normally.
  - A nearly full harvester tops up to full in one scoop instead of coming back partly empty.
  - Tiberium fields still grow and spread.
- [ ] **`GDI9_FIX`:** play GDI mission 9 (the gunboat mission: "Take out Nod turrets along shore...").
  It applies to mission 9 only; mission 10 still needs every Nod building destroyed, as in CFE.
  The map has 8 Nod turrets. Only the 2 on the far (south) side of the water (map cells 2715 and 2717)
  are exempt. The 2 on the near shore just above the water (cells 2144 and 2146) still count; they are the
  briefing's "turrets along shore". Destroy those, the other 4 turrets, every other Nod building and every
  Nod unit (harvester included). Nod should then blow up about a second later and the mission end.
- [ ] **`DEATHS_DOOR_FIX`:**
  - Now and then a unit or building that would have died by a small margin survives with a sliver of health.
  - Big hits such as an airstrike, or hits that would kill outright, still kill.
  - The same unit isn't saved again and again.
- [ ] **`DEAD_BUILDING_SCATTER_FIX`:** destroy an enemy building with soldiers inside.
  The survivors run out and scatter first, then start shooting.
- [ ] **`DISTANCE_CALC_FIX`:** this changes the distance math used everywhere, so mostly look for anything odd.
  For example: units or defences that won't fire at something clearly in range, or that fire from too far away.
- [ ] **`NUKE_RADIUS_FIX`:** if you get a nuclear strike (Nod, Temple of Nod), use it on a group of buildings.
  The centre should take heavy damage and the edges noticeably less.
- [ ] **`GUNBOAT_BUGFIX`:** in a mission with a GDI gunboat on the river, the gunboat switches targets as it moves.
  It shouldn't keep firing at one target it can't reach.

## 2b. Harvester features (Phase 3 step 2)
Turn them on under `[FEATURES]` in `cfepatchredux.ini`: `HARVY_SELF_REPAIR=1`, `SMARTER_REFINERIES=1`,
`SMARTER_HARVESTERS=1`, `HARVY_QUEUE_JUMP=1`, `HARVY_LOAD_BALANCING=1`, `HARVY_OPTIMIZE=1`.
They only affect your own harvesters (in single player). Build 2–3 refineries and 4+ harvesters (F1, F5, F6 help).

- [ ] **Self-repair:** a damaged harvester slowly heals up to half health on its own.
- [ ] **Smarter refineries:** park a tank on a refinery's unloading spot (the strip below it). When a harvester
  heads in, the tank moves off within 75 game frames (about 4 seconds at your GameSpeed=3, 5 at the default 4).
- [ ] **Smarter harvesters:** a harvester that gets wedged while harvesting backs up or moves aside after
  60–120 game frames (two checks 60 frames apart: 3–6 seconds at your GameSpeed=3, 4–8 at the default 4) and carries on. A harvester that ran out of tiberium and went idle tries again about once a
  minute; one you parked yourself (Stop/Guard) stays parked.
- [ ] **Queue jump:** with a harvester driving to a refinery from far away, a full harvester right next to that
  refinery gets to unload first.
- [ ] **Load balancing / optimize:** harvesters spread over your refineries instead of queuing at one, and
  queued ones switch to a free refinery when it's quicker.
- [ ] With all six on: save, load, and keep playing.

## 2c. Tiberium and visceroids (Phase 3 step 3)
Add under `[FEATURES]`: `BETTER_TIB_GROWTH=1`, `TIB_RESURGENCE=1`, `TS_VISCEROID_SPAWN=1`; and under
`[SETTINGS]`: `TREE_VISCEROID_SPAWN_MODE=2` (mode 2 = also in single player).
Resurgence and tree visceroids only work with `BETTER_TIB_GROWTH=1`.

- [ ] **Better growth:** tiberium fields grow and spread at roughly the usual pace, more evenly; fields
  boxed in by other tiberium thicken instead of stalling.
- [ ] **Growth scale:** with `BETTER_TIB_GROWTH=0` and `TIB_GROWTH_SCALE=3` under `[SETTINGS]`, tiberium grows
  about three times as fast.
- [ ] **Resurgence:** harvest the field around a tiberium tree bare. After several minutes (default
  `TIB_RESURGE_DELAY=7200` game frames at the least: 6 minutes at your GameSpeed=3, 8 at the default 4;
  longer if some tiberium is left next to the tree) the tree puffs spores and tiberium bursts out around it.
- [ ] **Tree visceroids:** a tiberium tree completely surrounded by full tiberium (nowhere to spread)
  occasionally spawns a visceroid, with a squish sound.
- [ ] **TS visceroids:** infantry that die walking through tiberium sometimes leave a visceroid behind.
- [ ] Save and load with these on, then keep playing.

## 2d. A* pathfinding (Phase 3 step 4a)
Add under `[FEATURES]`: `ASTAR_PATHING=1`; and under `[SETTINGS]`: `INFANTRY_TIBERIUM_AVERSION=2` (CFE's default).

- [ ] **Routes:** send groups of tanks and soldiers across the map, around rocks, rivers and buildings. They should
  take sensible routes and get stuck or wander off less than before.
- [ ] **Traffic:** send many units through a narrow gap or over a bridge at once. They should queue and flow rather
  than bump into each other and turn back.
- [ ] **Tiberium aversion:** order soldiers (not chem warriors) to a point on the other side of a tiberium field.
  If there's a reasonable way around, they should take it instead of walking through.
- [ ] **Refineries / repair bays:** a harvester or unit returning to dock picks one it can actually reach.
- [ ] **Performance:** with lots of units moving, the game shouldn't slow down noticeably.
- [ ] With A* on: save, load, and keep playing.
- [ ] **Harvesters with A\* (step 4c):** with the six harvester switches from 2b also on, build 2–3 refineries
  with something in the way (a river, cliffs or a long wall). Harvesters should go to the refinery that is
  closest by road, not the one that looks closest in a straight line, and the 2b queue-jump and
  spreading-out behaviour should still work.

## 2e. Two units in one spot (Phase 3 step 4b)
Add under `[FEATURES]`: `CELL_RESERVATION_FIX=1`. It works with or without `ASTAR_PATHING`; the test showed the
clearest gain with both on.

- [ ] **Crowds:** send two big groups of tanks through each other, back and forth. Two vehicles should not end
  up parked on top of each other.
- [ ] **Diagonal crossings:** units crossing diagonally at a corner should wait for each other rather than pass
  through each other.
- [ ] **No stuck units:** units shouldn't freeze in place waiting for a cell nobody is using.
- [ ] With it on: save, load, and keep playing (traffic should carry on normally after the load).

## 2f. Mammoths and commandos (Phase 3 step 5a)
Add under `[FEATURES]`: `SMARTER_MAMMOTHS=1`, `DISABLE_COMMANDO_AIRSTRIKES=1`; and under `[SETTINGS]`:
`COMMANDO_GUARD=2` (2 = also in single player).

- [ ] **Mammoths:** a Mammoth Tank attacking infantry fires only its missiles, and vehicles and buildings still
  get the cannon.
- [ ] **Commando guard:** a commando left standing shoots enemy soldiers that come near, and ignores vehicles
  and buildings. Told to attack something, it does so as usual.
- [ ] **Airstrikes:** in a mission that starts without a base (commando missions), no enemy A10s arrive until
  you own a base. Missions with a base still get them.

## 2g. Smarter SAMs (Phase 3 step 5b)
Add under `[FEATURES]`: `SMARTER_SAMS=1`; and under `[SETTINGS]`: `SMARTER_SAM_AMMO=6`.
`SMARTER_SAM_AMMO` is the number of shots between reloads (default 2).

- [ ] **Volleys:** with `SMARTER_SAM_AMMO=6`, a SAM site under air attack fires 6 missiles before going back down.
- [ ] **Retargeting:** when its target dies and it has shots left, the SAM stays up for about 2 seconds and
  turns to the next aircraft instead of going down.
- [ ] **Balance:** `SMARTER_SAM_BALANCE_DELAY` adds game frames between pairs of shots. With 0, the test showed
  about 90 frames between pairs (at your GameSpeed=3, about 4.5 seconds).
- [ ] Save during an air attack, load, and keep playing.

## 2h. Smarter aircraft (Phase 3 step 5c)
Add under `[FEATURES]`: `SMARTER_AIRCRAFT=1`.

- [ ] **Free pads:** with two helipads and the primary one occupied, a new helicopter appears on the free one.
- [ ] **No pad free:** with every pad occupied, new helicopters fly in level with the primary pad, not from a
  random point along the map edge.
- [ ] **No ammo:** tell a helicopter with no missiles left to attack something: it goes home to rearm instead of
  flying off to the target.
- [ ] **Moving targets:** send a helicopter after a moving vehicle far away: it turns to follow as it flies.

## 2i. Smarter AI commandos (Phase 3 step 5d)
Add under `[FEATURES]`: `SMARTER_CPU_COMMANDOS=1`. Use a mission where Nod or GDI sends a commando at you, or play
against one in skirmish.

- [ ] **Bombs:** an enemy commando runs to your buildings and blows them up instead of shooting them.
- [ ] **Vehicles:** it ignores your vehicles.
- [ ] **Your commando:** yours still works as usual when told to destroy a building.

## 2j. Veterancy (Phase 3 step 5e)
Add under `[SETTINGS]`: `VETERANCY=2` (2 = also in single player).

- [ ] **Promotion:** a unit that keeps killing things gets promoted (a click sound and a flash), and selecting
  it shows 1–3 small yellow dots in the bottom-right corner of its selection box.
- [ ] **Harvesters:** a harvester also gets promoted after about 10 loads.
- [ ] **Healing:** a damaged rank 2 or 3 unit slowly heals on its own.
- [ ] **Save and load:** a promoted unit keeps its rank after save and load. An old save still loads.

## 3. Fixes off (original game)
- [ ] Rename `cfepatchredux.ini` (for example to `cfepatchredux.ini.off`) and play a mission.
  Everything should behave like the original game, and there should be no crashes.
- [ ] Optional: turn the fixes off one at a time (`=0`) to see what each one changes.

## 4. General
- [ ] Save a game with the fixes on, load it, and keep playing.
- [ ] Play at least one GDI and one Nod mission from start to finish with the fixes on.
- [ ] **War factory with F6 (instant build):** order vehicles quickly, one after another, and click "Ready" icons
  while the previous vehicle is still driving out. Every vehicle should come out; none should be cancelled.
  If the war factory is destroyed while a vehicle is "Ready", clicking it should still cancel it ("no factory").
- [ ] **Cheats, one case not yet tried:** press F4 with the mouse over an occupied spot (a building or another unit).
  Expected: the message "Cheat: can't spawn a commando there", and no crash.

## Results
| Test | Result | Notes |
|---|---|---|
| 1. Burning-tree crash | Pass (user, 2026-10-29) | Nothing odd seen; recheck if it turns up later |
| 2b. Harvester features | Pass (user, 2026-10-29) | Same |
| 2c. Tiberium and visceroids | Pass (user, 2026-10-29) | Same |
| Save/load, including a save from before the features | Pass (user, 2026-10-29) | Same |
| 3. Fixes off | Pass (user, 2026-10-29) | Same |
