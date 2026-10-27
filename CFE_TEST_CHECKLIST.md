# CFE port – play-test checklist

Settings files live in `~/Library/Application Support/Vanilla-Conquer/vanillatd/`:
- `cfepatchredux.ini`: CFE switches (`1` on, `0` off). It currently turns on the 8 TD fixes below.
- `conquer.ini`, section `[Cheats]`: `Enabled=yes` turns on the F1–F6 cheats.

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
  The map has 8 Nod turrets. Only the 2 southernmost ones (map cells 2715 and 2717, the pair furthest south,
  near the bottom middle of the map) are exempt: destroy the other 6, every other Nod building and every Nod unit
  (harvester included). Nod should then blow up about a second later and the mission end.
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

## 3. Fixes off (original game)
- [ ] Rename `cfepatchredux.ini` (for example to `cfepatchredux.ini.off`) and play a mission.
  Everything should behave like the original game, and there should be no crashes.
- [ ] Optional: turn the fixes off one at a time (`=0`) to see what each one changes.

## 4. General
- [ ] Save a game with the fixes on, load it, and keep playing.
- [ ] Play at least one GDI and one Nod mission from start to finish with the fixes on.
- [ ] **Cheats, one case not yet tried:** press F4 with the mouse over an occupied spot (a building or another unit).
  Expected: the message "Cheat: can't spawn a commando there", and no crash.

## Results
| Test | Result | Notes |
|---|---|---|
| | | |
