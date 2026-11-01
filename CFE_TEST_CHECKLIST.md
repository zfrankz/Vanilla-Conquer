# CFE port – play-test checklist

Timings below are in game frames. A frame lasts `GameSpeed`/60 seconds (`conquer.ini`), so 20 frames a second at
GameSpeed=3 and 15 at the default 4, if the computer keeps up.

Settings files live in `~/Library/Application Support/Vanilla-Conquer/vanillatd/`:
- `cfepatchredux.ini`: CFE switches (`1` on, `0` off). It currently turns on the 8 TD fixes below.
- `conquer.ini`, section `[Cheats]`: `Enabled=yes` turns on the F1–F6 cheats.
  `StartMission=9` there makes Start New Game begin at mission 9 of the side you pick (GDI 1–15, Nod 1–13).
  Red Alert has the same setting in `vanillara/redalert.ini` (Allies and Soviets 1–14).

  `SkipStartupMovie=yes` under `[Intro]` skips the logo movie at each start (TD in `conquer.ini`, RA in `redalert.ini`).

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
- [ ] **Rank 3 specials** (three yellow dots): light and medium tanks fire twice; mammoths fire four shots; guard
  towers, obelisks, grenadiers and artillery reach a cell further; a commando or Nod APC turns stealthy when
  standing still; buggies run infantry over; a GDI APC heals its passengers; helicopters repair on the pad.

## 2k. Wall crushing (Phase 3 step 6a)
Add under `[FEATURES]`: `WALL_CRUSH=1`.

- [ ] **Crushing:** a Mammoth or other tracked vehicle drives straight through enemy sandbags, chain-link, barbed
  wire and wooden fences, flattening them. Concrete walls and your own walls still stop it.

## 2l. AI against walls (Phase 3 step 6b)
Add under `[FEATURES]`: `AI_VS_WALLS=1`, `HUNT_FIX=1`.

- [ ] **Walls:** wall in part of your base. Enemy units shoot their way through instead of giving up or
  only going around.
- [ ] **Hunting:** enemy units in hunt mode still find you, with no slowdown.

## 2m. Wall ownership (Phase 3 step 6c)
Add under `[FEATURES]`: `ADV_WALL_OWN=1`.

- [ ] **Map walls:** walls around an enemy base count as theirs; enemy units don't shoot them, yours can be
  ordered to.
- [ ] **Changing hands:** capture or destroy the buildings by a wall and it goes to whoever's base it's now in.

## 2n. TS-style walls (Phase 3 step 6d)
Add under `[SETTINGS]`: `WALL_BUILD_LENGTH=5`, and `WALLS_HAVE_FULL_COST=0` for free extra pieces (`1` charges each).

- [ ] **Gap filling:** with a wall built, place another of the same kind in line, up to 5 cells away: the gap fills.
  The placement cursor shows the cells it will cover.
- [ ] **Single piece:** hold Ctrl while placing: only one piece, and the cursor is one cell.
- [ ] **Selling:** selling a wall piece refunds a smaller share with free extra pieces.

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

## 2o. Attack-move (`ATTACK_MOVE=1` under `[FEATURES]`)

Any mission or skirmish with a mixed group.
- Select armed units and Shift+click the ground past some enemies: they stop to fight, then carry on to the spot.
- Same with a plain click: they ignore enemies on the way, as usual.
- Same on the radar map.
- Press Stop (S) mid-way, or give a plain order: the attack-move ends.
- Helicopters: Shift+click: they attack on the way, then go home. With `SMARTER_AIRCRAFT=1` and no ammo, they don't leave.
- Save mid-way, load: units carry on.
- With the switch off, Shift+click is a forced move, as in the original game.

## 2p. Rally points (`RALLY_POINTS=1` under `[FEATURES]`)

Any mission with a weapons factory and barracks.
- Select the factory and click the ground: a green line shows the point while it stays selected.
- Build a unit: it drives there. Harvesters ignore it.
- Same with barracks / hand of Nod, helipad and repair bay (a repaired vehicle goes to the point).
- Press Stop with the building selected: the point goes away.
- Save, load: the point is still there.
- Sell or lose the building, or have it captured: no stray line.

## 2q. Q-move (`Q_MOVE=1` under `[FEATURES]`)

Any mission with a few units; `ATTACK_MOVE=1` as well for the last items.
- Select units, hold Q and click three spots: they go through them in order.
- Hold Q and click the first spot again: they keep looping.
- Press Stop, or give a plain click: the route is dropped.
- Save with a route under way, load: they carry on.
- Hold Q + Shift and click: same route, fighting on the way.
- Helicopters on a Q + Shift loop: out of ammo they go home, reload and restart it.

## 2r. Balance patch (`[BALANCE]` section, `APPLY_BALANCE_TO_SINGLE_PLAYER=1`)

Set the `DO_OVERRIDE_*` lines, `DO_CHEM_GUY_BUFF=1` and `MULTIPLAYER_UNITS_IN_SINGLE_PLAYER=1`; a Nod mission 11 or later for the lab.
- Prices and health in the sidebar: stealth tank 700, rocket soldier 250.
- With a refinery, the structures column offers an icon reading "Chem. Warrior": that is the bio lab (no art of its own). Build and place it.
- With the lab and a barracks (Hand of Nod), the chem soldier appears.
- Chem soldier kills: now and then a visceroid appears where the victim died.
- Capture a bio lab with an engineer.
- Without `APPLY_...`: the campaign plays as the original.

## 2s. JD's adjustments (`JD_EXTRA_ADJUSTMENTS=1` under `[BALANCE]`)

- GDI mission 5 or 6: the rocket soldier and sandbags are offered.
- Riflemen, jeeps and buggies reveal more shroud.
- Radar needs only a power plant; the war factory and airstrip need a barracks (not a refinery).

## 2t. Small options (Phase 3 step 9)

Under `[FEATURES]`, except the three marked `[SETTINGS]`.
- `OOM_REPAIR=1`: start repairing a building, run out of credits: the wrench stays and repair resumes with money.
- `VERSION_ANNOUNCE=1`: a few seconds into a new mission, "CFE Patch Redux version 1.9".
- `SHROUD_REMOVER=1`: no shroud in a new mission.
- `MCV_ALWAYS_SPAWN_ENGINEER=1`: sell your own construction yard: an engineer comes out.
- `[SETTINGS]` `BUILDING_DEATH_ANNOUNCE_MODE=1`: lose a building: "structure lost". Mode 2: also for enemy ones you have seen.
- `[SETTINGS]` `QUIETER_AIRSTRIPS=2` with an airstrip: order a vehicle: only "reinforcements have arrived" when it lands.
- `[SETTINGS]` `SAFE_SABOTAGE=1`: blow up an enemy building with C4: no crew comes out.

## 2u. Building gap (`BUILDING_GAP=2` under `[SETTINGS]`)

- Place a building leaving a one cell gap: allowed. With `BUILDING_GAP=1`: refused, as the original.

## 2v. Smarter repair bays (`SMARTER_REPAIRBAY=1` under `[FEATURES]`)

- Send a damaged vehicle: when fully repaired it drives off by itself, to the rally point if there is one.
- Send a second vehicle while the bay is busy: the cursor shows "enter", it waits and goes in after the first.
- Send a helicopter: when repaired it just moves off the bay. With `RALLY_POINTS=1` and a rally point set on the bay, it flies to a free helipad.

## 2w. Search range (`SEARCH_RANGE_FIX=2` under `[SETTINGS]`)

- Enemies in a mission still don't react from further than usual until they start fighting.
- Your own units in guard may react slightly further out, depending on weapon range and facing.

## 2x. Meaner visceroids (`MEANER_VISCEROIDS=3` under `[SETTINGS]`; `TS_VISCEROID_SPAWN=1` or a late mission)

- Visceroids wander the map and hunt instead of sitting still, and take noticeably more damage to kill.
- Infantry they kill sometimes become visceroids.
- When one dies, a green splash and tiberium appears around it.

## 2y. Original game bugfixes (`GENERAL_BUGFIXES=1` under `[FEATURES]`)

- Two engineers on one building: one goes in, the other survives.
- Load 3 infantry into a Chinook: all board and the door closes.
- Mission with crates: an explosive crate still works, and nothing stays stuck on that cell.
- A building destroyed while it is still rising leaves the power bar as it was before it.

## 2z. Missing voices

These data lack 10 EVA voices: `NOCASH1`, `MOCASH1`, `INCOME1`, `ENEMYA`, `NOBUILD1`, `NEEDHARV`, `TRANSSEE`, `TRANLOAD`,
`ENMYAPP1`, `REPAIR1`. Nothing is said for them, and this used to crash on the second repair in a row.

- Repair two damaged vehicles in a row on a repair bay: no crash, no spoken "repairing".

## RA 1. Small fixes (`cfepatchredux_ra.ini` in the `vanillara` folder)
Play any Red Alert mission with the switches of that file on.
- [ ] `VERSION_ANNOUNCE`: about 5 s in, "CFE Patch Redux RA version 1.9" shows top left.
- [ ] `INSTANT_CAPTURE`: an engineer captures a full-health enemy building at once.
- [ ] `BUILDING_CAPTURE_ANNOUNCE`: you hear "building captured" when you take a building and when you lose one.
- [ ] `BURNING_TREE_FIX`: trees set alight by a flamethrower burn down and leave a passable cell.
- [ ] `DEATHS_DOOR_FIX`: units on low health sometimes survive a hit at 1 hp.
- [ ] `ORE_INDEX_BUGFIX`: ore fields look more varied at mission start; harvesters still fill up normally.
- [ ] `OOM_REPAIR`: a building repairing with no money pauses and resumes when money arrives.
- [ ] `SAFE_SABOTAGE`: selling a building under capture gives no enemy survivors.
- [ ] `SMARTER_SONAR`: a sonar pulse surfaces enemy subs, not yours or your allies'.
- [ ] `MIG_AMMO_FUDGE`: a MiG fires 4 missiles and shows 4 ammo pips.
- [ ] `INACCURACY_BUGFIX`: artillery and V2 shots scatter a bit more than before.

## RA 2. Harvesters (`HARVY_QUEUE_JUMP`, `HARVY_LOAD_BALANCING`, `HARVY_OPTIMIZE`)
Mission with 2 refineries and 3-4 harvesters.
- [ ] Harvesters spread between refineries instead of all queueing at one.
- [ ] A harvester close to a refinery can take the place of one still far away.
- [ ] No harvester stays idle, full, or shuttles endlessly between refineries.
- [ ] Destroy the refinery a harvester is heading to: it picks another one.

## RA 3. Ore (`BETTER_ORE_GROWTH`, `ORE_GROWTH_SCALE`, `GEM_OVERLOAD_FIX`)
- [ ] Ore spreads steadily around fields and mines, and never over a mine or a tree.
- [ ] Long missions: some gems appear next to mines.
- [ ] A nearly full harvester that reaches gems leaves a bit of ore behind.
- [ ] Optional: `ORE_GROWTH_SCALE=3` makes fields grow visibly faster; `ORE_TO_GEM_SLOW_FACTOR=1` gives more gems.

## RA 4. Pathing (`ASTAR_PATHING`, `CELL_RESERVATION_FIX`)
- [ ] Send a big group across a base and through a bridge: routes look sensible, no long freezes.
- [ ] Two groups crossing: no two vehicles stopped on the same spot.
- [ ] Ships in a narrow strait: they pass or wait, without getting stuck.
- [ ] Save mid-move, load, and carry on: nothing freezes.
- [ ] Big groups with A* on arrive in reasonable time: a stress test got fewer arrivals with A* (35) than without (52), unexplained. Note any jam.
- [ ] Harvesters and repair-bound units pick the closest by road, not in a straight line across a river.

## RA 5. Veterancy and smarter units (`VETERANCY=2`, `TANYA_GUARD=2`, `SMARTER_AIRCRAFT`, `SMARTER_REPAIRBAY`)
- [ ] Units that fight gain yellow pips when selected (one per rank), and a beep on promotion.
- [ ] Rank 3 specials: light/medium tank double shot, heavy/mammoth extra volley, tesla chain lightning, stealth Tanya.
- [ ] Tanya on guard shoots enemy infantry on her own.
- [ ] Aircraft with no ammo go home, and don't take off to attack.
- [ ] Click a vehicle on a busy repair bay: it queues, and leaves cleanly when done.
- [ ] Save and load: pips and ranks stay.
- [ ] Rank 3 infantry: rifleman and Yak/ranger bullets hit tanks harder, grenadier +1 cell of range, rocket soldier runs faster, flamethrower barely hurts allies, medic heals itself, mechanic gives XP.
- [ ] Rank 3 Tanya: goes stealth, fires once half decloaked, enemy dogs reveal her, cell triggers still fire in missions.
- [ ] Rank 3 dogs: a howl sets nearby dogs on the same target.
- [ ] Rank 3 vehicles: mammoth iron curtain when hit (20 s, every 2 min), artillery +1 range, V2 reloads faster, phase transport heals passengers, APC keeps passengers in a chrono shift, minelayer regains ammo, its mines hit harder.
- [ ] Rank 3 ships: destroyer reveals nearby subs, submarine and gunboat double shot, cruiser extra volley, missile sub submerges sooner.
- [ ] Rank 3 aircraft: repair for free on the pad, helicopters reload twice as fast, MiG and Yak two shots per ammo pip.
- [ ] Rank 3 defenses: pillbox, AA gun, flame tower and SAM +1 range, turret double shot, Tesla coil chain lightning.
- [ ] Super nukes don't exist: a missile silo never ranks up.
- [ ] Tesla chain lightning force-fired at the ground doesn't crash.
- [ ] An old save (made before this port) loads and plays.

## RA 6. Attack-move and Q-move (`ATTACK_MOVE`, `QMOVE_LOOPS`, `AIR_QMOVE`, `AIR_QAML_RECALL`, `CHRONOTANK_ATTACK_MOVE`)
- [ ] Shift+click on the ground: units move and stop to fight what they meet, then carry on.
- [ ] Hold Q and click points: units follow them in order. Click the first point again: they loop.
- [ ] Shift+Q: a queued attack-move.
- [ ] A minelayer with Shift+click: goes there, lays its mines around and returns.
- [ ] Helicopters and MiGs with queues; out of ammo they rearm and return to the loop.
- [ ] A chronotank with Shift+click teleports when charged.
- [ ] Stop (S) halts a queue at once.
- [ ] Boats on attack-move shoot for a moment and move on, without getting stuck turning.
- [ ] Dogs on attack-move carry on after a kill.
- [ ] A new order or a death ends the attack-move cleanly; dying units don't twitch.
- [ ] With `SMARTER_AIRCRAFT`, an aircraft with no ammo refuses the attack-move and goes home.
- [ ] Several selected units take the same queue, and helicopters take a loop together.

## RA 7. Rally points, walls, gap (`RALLY_POINTS`, `WALL_BUILD_LENGTH=5`, `BUILDING_GAP_OFFSET`)
- [ ] Select a factory and click the ground: a green line shows the rally point, and new units go there. Stop (S) clears it.
- [ ] Repair bay with a rally point: repaired units go there; helicopters go to a pad.
- [ ] Place a wall in line with another up to 5 cells away: the cursor covers the gap and it fills in. Ctrl places one piece.
- [ ] With a gap offset, buildings can go further from the base.
- [ ] Optional, not in your ini: `LONG_BGAP_BUGFIX=1`. Alone it narrows placement to 1 cell; use it with an offset.
- [ ] Rally points on barracks, kennel, war factory, sub pen and shipyard.
- [ ] Rally point onto a unit or building: it's dropped when the target dies, and when the factory is captured.
- [ ] Harvesters ignore rally points.
- [ ] Minelayer re-armed on a bay with a rally point goes there.
- [ ] Fixed wing plane on a repair bay with no free airstrip stays put and doesn't crash.
- [ ] Selling a wall piece refunds money; with `WALLS_HAVE_FULL_COST=0` extra pieces are free and refund less.
- [ ] Rally point survives save and load.

## RA 8. Supers and chrono (`OPENRA_WIDE_SUPERS`, `OPENRA_QUICK_SUPERS`, `SMARTER_CHRONO`, `TIMEQUAKE_FIX`)
- [ ] Iron curtain on a group: up to 5 units glow, for 20 s; recharge 2 min.
- [ ] Chronosphere on a group: up to 5 vehicles go together and return after 20 s.
- [ ] A chronoshifted unit is deselected for others and enemy shots miss it.
- [ ] Sometimes a timequake shakes the map after a chrono shift.
- [ ] A chrono-shifted vehicle returning plays the sound at both ends and drops enemy target locks.
- [ ] A chronotank shifting itself doesn't animate the Chronosphere and doesn't discharge it.
- [ ] A timequake crate does something with `TIMEQUAKE_FIX`.
- [ ] A vortex appears now and then; shifting while it's hidden can wake it.
- [ ] Optional, not in your ini: `USE_SHARED_CHRONO_ODDS=1`, `CUMULATIVE_TIMELINE_DAMAGE=1` (the odds climb fast).

## RA 9. Unflagged fixes (`GENERAL_BUGFIXES`)
- [ ] MiGs out of ammo during a hunt go home in one smooth turn.
- [ ] A building destroyed while still rising leaves the power bar as it was.
- [ ] Sell a building and a construction yard: "structure sold" plays only for the building, no crash.
- [ ] Units draw in the right order around big buildings (no vehicle over a tree it should be behind).
- [ ] An engineer ordered to a new building goes at once, even while moving.
- [ ] Computer engineers go for buildings they can really capture.
- [ ] Capture an ally's construction yard and build a helipad and an airstrip: "new construction options" isn't repeated, and each offers only its own aircraft.
- [ ] A very long sidebar (F5 cheat, many factories) doesn't crash.
- [ ] With a building ready to place, hold the cursor next to a building and have it destroyed: the placement stops being valid without moving the mouse.
- [ ] A nuke from a house with no silo left doesn't crash.

## RA 10. Everything together
- [ ] A full campaign mission with your ini, saving and loading midway.
- [ ] A skirmish against the computer with your ini, with several factories, ships and aircraft.
- [ ] With the ini moved away: the game plays as the original.
- [ ] Multiplayer: not ready (Phase 5). Both players would need the same ini.
