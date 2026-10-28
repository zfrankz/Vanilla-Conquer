//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free
// software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed
// in the hope that it will be useful, but with permitted additional restrictions
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT
// distributed with this program. You should have received a copy of the
// GNU General Public License along with permitted additional restrictions
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

#include "function.h"
#include "cheats.h"

namespace {

enum CheatType
{
    CHEAT_CREDITS,
    CHEAT_REVEAL_MAP,
    CHEAT_REVEAL_SHROUD,
    CHEAT_SPAWN_COMMANDO,
    CHEAT_FULL_CATALOG,
    CHEAT_INSTANT_BUILD,
    CHEAT_COUNT
};

struct CheatKeyEntry
{
    char const* Name;
    KeyNumType Default;
};

const CheatKeyEntry CheatKeyEntries[CHEAT_COUNT] = {
    {"KeyCredits", KN_F1},
    {"KeyRevealMap", KN_F2},
    {"KeyRevealShroud", KN_F3},
    {"KeySpawnCommando", KN_F4},
    {"KeyFullCatalog", KN_F5},
    {"KeyInstantBuild", KN_F6},
};

const int CheatCredits = 5000;

bool CheatsEnabled = false;
KeyNumType CheatKeys[CHEAT_COUNT];
bool CheatInstantBuild = false;
int CheatStartMission = 1;

bool Cheats_Allowed()
{
    return CheatsEnabled && PlayerPtr != NULL && !Debug_Map
           && (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH);
}

void Cheat_Message(char const* text)
{
    char buffer[MAX_MESSAGE_LENGTH];
    snprintf(buffer, sizeof(buffer), "Cheat: %s", text);
    Session.Messages.Add_Message(NULL,
                                 0,
                                 buffer,
                                 PlayerPtr->RemapColor,
                                 TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW,
                                 TICKS_PER_SECOND * 4);
    Map.Flag_To_Redraw(false);
}

void Cheat_Spawn_Commando()
{
    COORDINATE coord = Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y());
    if (coord != 0) {
        InfantryClass* infantry = new InfantryClass(INFANTRY_TANYA, PlayerPtr->Class->House);
        if (infantry != NULL) {
            if (infantry->Unlimbo(Cell_Coord(Coord_Cell(coord)), DIR_N)) {
                infantry->Assign_Mission(MISSION_GUARD);
                Cheat_Message("Tanya spawned");
                return;
            }
            delete infantry;
        }
    }
    Cheat_Message("can't spawn Tanya there");
}

} // namespace

void Init_Cheats()
{
    INIClass ini;
    CCFileClass file(CONFIG_FILE_NAME);
    if (file.Is_Available()) {
        ini.Load(file);
    }

    static char const* const CHEATS = "Cheats";
    CheatsEnabled = ini.Get_Bool(CHEATS, "Enabled", false);
    for (int index = 0; index < CHEAT_COUNT; index++) {
        CheatKeys[index] =
            (KeyNumType)ini.Get_Int(CHEATS, CheatKeyEntries[index].Name, CheatKeyEntries[index].Default);
    }

    /*
    ** Allies and Soviets have 14 campaign missions each.
    */
    CheatStartMission = Bound(ini.Get_Int(CHEATS, "StartMission", 1), 1, 14);
}

bool Cheat_Key(KeyNumType input)
{
    if (!Cheats_Allowed() || input == KN_NONE) {
        return false;
    }

    /*
    ** Only an unmodified key press matches; releases carry KN_RLSE_BIT and never do.
    */
    KeyNumType key = KeyNumType(input & ~WWKEY_VK_BIT);
    int cheat = 0;
    while (cheat < CHEAT_COUNT && key != KeyNumType(CheatKeys[cheat] & ~WWKEY_VK_BIT)) {
        cheat++;
    }

    switch (cheat) {
    case CHEAT_CREDITS:
        PlayerPtr->Refund_Money(CheatCredits);
        Cheat_Message("+5000 credits");
        break;

    case CHEAT_REVEAL_MAP:
        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            Map.Map_Cell(cell, PlayerPtr);
        }
        Map.Flag_To_Redraw(true);
        Cheat_Message("map revealed");
        break;

    case CHEAT_REVEAL_SHROUD:
        Debug_Unshroud = !Debug_Unshroud;
        Map.Flag_To_Redraw(true);
        Cheat_Message(Debug_Unshroud ? "shroud off" : "shroud on");
        break;

    case CHEAT_SPAWN_COMMANDO:
        Cheat_Spawn_Commando();
        break;

    case CHEAT_FULL_CATALOG:
        PlayerPtr->DebugUnlockBuildables = !PlayerPtr->DebugUnlockBuildables;
        PlayerPtr->IsRecalcNeeded = true;
        Cheat_Message(PlayerPtr->DebugUnlockBuildables ? "full catalog on" : "full catalog off");
        break;

    case CHEAT_INSTANT_BUILD:
        CheatInstantBuild = !CheatInstantBuild;
        Cheat_Message(CheatInstantBuild ? "instant build on" : "instant build off");
        break;

    default:
        return false;
    }
    return true;
}

bool Cheat_Instant_Build(HouseClass const* house)
{
    return CheatInstantBuild && house == PlayerPtr && Cheats_Allowed();
}

/*
** Swaps the first campaign mission picked by "Start New Game" for mission StartMission of the
** same side, using the first of its map files that exists. Keeps the first mission if none does.
*/
void Cheat_Start_Mission()
{
    char const side = Scen.ScenarioName[2];
    if (!CheatsEnabled || CheatStartMission <= 1 || (side != 'G' && side != 'U')) {
        return;
    }

    char name[_MAX_FNAME + _MAX_EXT];
    for (char const* dir = "EW"; *dir != '\0'; dir++) {
        for (char var = 'A'; var <= 'D'; var++) {
            snprintf(name, sizeof(name), "SC%c%02d%c%c.INI", side, CheatStartMission, *dir, var);
            if (CCFileClass(name).Is_Available()) {
                Scen.Set_Scenario_Name(name);
                return;
            }
        }
    }
}
