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

/*
** Single-player cheats for the legacy client (not part of CFE). Off unless [Cheats] Enabled=yes
** in conquer.ini; refused in multiplayer, where they would desync. Keys are rebindable there too.
*/
#ifndef TD_CHEATS_H
#define TD_CHEATS_H

class HouseClass;

void Init_Cheats();
bool Cheat_Key(KeyNumType input);
bool Cheat_Instant_Build(HouseClass const* house);
int Cheat_Start_Mission();

#endif
