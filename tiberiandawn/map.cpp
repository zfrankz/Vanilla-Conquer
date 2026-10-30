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

/* $Header:   F:\projects\c&c\vcs\code\map.cpv   2.17   16 Oct 1995 16:52:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAP.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : August 20, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MapClass::Cell_Distance -- Determines the distance between two cells.                     *
 *   MapClass::Cell_Region -- Determines the region from a specified cell number.              *
 *   MapClass::Cell_Threat -- Gets a houses threat value for a cell                            *
 *   MapClass::Close_Object -- Finds a clickable close object to the specified coordinate.     *
 *   MapClass::In_Radar -- Is specified cell in the radar map?                                 *
 *   MapClass::Init -- clears all cells                                                        *
 *   MapClass::Logic -- Handles map related logic functions.                                   *
 *   MapClass::One_Time -- Performs special one time initializations for the map.              *
 *   MapClass::Overlap_Down -- computes & marks object's overlap cells                         *
 *   MapClass::Overlap_Up -- Computes & clears object's overlap cells                          *
 *   MapClass::Overpass -- Performs any final cleanup to a freshly constructed map.            *
 *   MapClass::Pick_Up -- Removes specified object from the map.                               *
 *   MapClass::Place_Down -- Places the specified object onto the map.                         *
 *   MapClass::Place_Random_Crate -- Places a crate at random location on map.                 *
 *   MapClass::Read_Binary -- reads the map's binary image file                                *
 *   MapClass::Set_Map_Dimensions -- Initialize the map.                                       *
 *   MapClass::Sight_From -- Mark as visible the cells within a specified radius.              *
 *   MapClass::Validate -- validates every cell on the map                                     *
 *   MapClass::Write_Binary -- writes the map's binary image file                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "tile.h"

#define MCW MAP_CELL_W
int const MapClass::RadiusOffset[] = {
    /* 0  */ 0,
    /* 1  */ (-MCW * 1) - 1,
    (-MCW * 1) + 0,
    (-MCW * 1) + 1,
    -1,
    1,
    (MCW * 1) - 1,
    (MCW * 1) + 0,
    (MCW * 1) + 1,
    /* 2  */ (-MCW * 2) - 1,
    (-MCW * 2) + 0,
    (-MCW * 2) + 1,
    (-MCW * 1) - 2,
    (-MCW * 1) + 2,
    -2,
    2,
    (MCW * 1) - 2,
    (MCW * 1) + 2,
    (MCW * 2) - 1,
    (MCW * 2) + 0,
    (MCW * 2) + 1,
    /* 3  */ (-MCW * 3) - 1,
    (-MCW * 3) + 0,
    (-MCW * 3) + 1,
    (-MCW * 2) - 2,
    (-MCW * 2) + 2,
    (-MCW * 1) - 3,
    (-MCW * 1) + 3,
    -3,
    3,
    (MCW * 1) - 3,
    (MCW * 1) + 3,
    (MCW * 2) - 2,
    (MCW * 2) + 2,
    (MCW * 3) - 1,
    (MCW * 3) + 0,
    (MCW * 3) + 1,
    /* 4  */ (-MCW * 4) - 1,
    (-MCW * 4) + 0,
    (-MCW * 4) + 1,
    (-MCW * 3) - 3,
    (-MCW * 3) - 2,
    (-MCW * 3) + 2,
    (-MCW * 3) + 3,
    (-MCW * 2) - 3,
    (-MCW * 2) + 3,
    (-MCW * 1) - 4,
    (-MCW * 1) + 4,
    -4,
    4,
    (MCW * 1) - 4,
    (MCW * 1) + 4,
    (MCW * 2) - 3,
    (MCW * 2) + 3,
    (MCW * 3) - 3,
    (MCW * 3) - 2,
    (MCW * 3) + 2,
    (MCW * 3) + 3,
    (MCW * 4) - 1,
    (MCW * 4) + 0,
    (MCW * 4) + 1,
    /* 5  */ (-MCW * 5) - 1,
    (-MCW * 5) + 0,
    (-MCW * 5) + 1,
    (-MCW * 4) - 3,
    (-MCW * 4) - 2,
    (-MCW * 4) + 2,
    (-MCW * 4) + 3,
    (-MCW * 3) - 4,
    (-MCW * 3) + 4,
    (-MCW * 2) - 4,
    (-MCW * 2) + 4,
    (-MCW * 1) - 5,
    (-MCW * 1) + 5,
    -5,
    5,
    (MCW * 1) - 5,
    (MCW * 1) + 5,
    (MCW * 2) - 4,
    (MCW * 2) + 4,
    (MCW * 3) - 4,
    (MCW * 3) + 4,
    (MCW * 4) - 3,
    (MCW * 4) - 2,
    (MCW * 4) + 2,
    (MCW * 4) + 3,
    (MCW * 5) - 1,
    (MCW * 5) + 0,
    (MCW * 5) + 1,
    /* 6  */ (-MCW * 6) - 1,
    (-MCW * 6) + 0,
    (-MCW * 6) + 1,
    (-MCW * 5) - 3,
    (-MCW * 5) - 2,
    (-MCW * 5) + 2,
    (-MCW * 5) + 3,
    (-MCW * 4) - 4,
    (-MCW * 4) + 4,
    (-MCW * 3) - 5,
    (-MCW * 3) + 5,
    (-MCW * 2) - 5,
    (-MCW * 2) + 5,
    (-MCW * 1) - 6,
    (-MCW * 1) + 6,
    -6,
    6,
    (MCW * 1) - 6,
    (MCW * 1) + 6,
    (MCW * 2) - 5,
    (MCW * 2) + 5,
    (MCW * 3) - 5,
    (MCW * 3) + 5,
    (MCW * 4) - 4,
    (MCW * 4) + 4,
    (MCW * 5) - 3,
    (MCW * 5) - 2,
    (MCW * 5) + 2,
    (MCW * 5) + 3,
    (MCW * 6) - 1,
    (MCW * 6) + 0,
    (MCW * 6) + 1,
    /* 7  */ (-MCW * 7) - 1,
    (-MCW * 7) + 0,
    (-MCW * 7) + 1,
    (-MCW * 6) - 3,
    (-MCW * 6) - 2,
    (-MCW * 6) + 2,
    (-MCW * 6) + 3,
    (-MCW * 5) - 5,
    (-MCW * 5) - 4,
    (-MCW * 5) + 4,
    (-MCW * 5) + 5,
    (-MCW * 4) - 5,
    (-MCW * 4) + 5,
    (-MCW * 3) - 6,
    (-MCW * 3) + 6,
    (-MCW * 2) - 6,
    (-MCW * 2) + 6,
    (-MCW * 1) - 7,
    (-MCW * 1) + 7,
    -7,
    7,
    (MCW * 1) - 7,
    (MCW * 1) + 7,
    (MCW * 2) - 6,
    (MCW * 2) + 6,
    (MCW * 3) - 6,
    (MCW * 3) + 6,
    (MCW * 4) - 5,
    (MCW * 4) + 5,
    (MCW * 5) - 5,
    (MCW * 5) - 4,
    (MCW * 5) + 4,
    (MCW * 5) + 5,
    (MCW * 6) - 3,
    (MCW * 6) - 2,
    (MCW * 6) + 2,
    (MCW * 6) + 3,
    (MCW * 7) - 1,
    (MCW * 7) + 0,
    (MCW * 7) + 1,
    /* 8  */ (-MCW * 8) - 1,
    (-MCW * 8) + 0,
    (-MCW * 8) + 1,
    (-MCW * 7) - 3,
    (-MCW * 7) - 2,
    (-MCW * 7) + 2,
    (-MCW * 7) + 3,
    (-MCW * 6) - 5,
    (-MCW * 6) - 4,
    (-MCW * 6) + 4,
    (-MCW * 6) + 5,
    (-MCW * 5) - 6,
    (-MCW * 5) + 6,
    (-MCW * 4) - 6,
    (-MCW * 4) + 6,
    (-MCW * 3) - 7,
    (-MCW * 3) + 7,
    (-MCW * 2) - 7,
    (-MCW * 2) + 7,
    (-MCW * 1) - 8,
    (-MCW * 1) + 8,
    -8,
    8,
    (MCW * 1) - 8,
    (MCW * 1) + 8,
    (MCW * 2) - 7,
    (MCW * 2) + 7,
    (MCW * 3) - 7,
    (MCW * 3) + 7,
    (MCW * 4) - 6,
    (MCW * 4) + 6,
    (MCW * 5) - 6,
    (MCW * 5) + 6,
    (MCW * 6) - 5,
    (MCW * 6) - 4,
    (MCW * 6) + 4,
    (MCW * 6) + 5,
    (MCW * 7) - 3,
    (MCW * 7) - 2,
    (MCW * 7) + 2,
    (MCW * 7) + 3,
    (MCW * 8) - 1,
    (MCW * 8) + 0,
    (MCW * 8) + 1,
    /* 9  */ (-MCW * 9) - 1,
    (-MCW * 9) + 0,
    (-MCW * 9) + 1,
    (-MCW * 8) - 3,
    (-MCW * 8) - 2,
    (-MCW * 8) + 2,
    (-MCW * 8) + 3,
    (-MCW * 7) - 5,
    (-MCW * 7) - 4,
    (-MCW * 7) + 4,
    (-MCW * 7) + 5,
    (-MCW * 6) - 6,
    (-MCW * 6) + 6,
    (-MCW * 5) - 7,
    (-MCW * 5) + 7,
    (-MCW * 4) - 7,
    (-MCW * 4) + 7,
    (-MCW * 3) - 8,
    (-MCW * 3) + 8,
    (-MCW * 2) - 8,
    (-MCW * 2) + 8,
    (-MCW * 1) - 9,
    (-MCW * 1) + 9,
    -9,
    9,
    (MCW * 1) - 9,
    (MCW * 1) + 9,
    (MCW * 2) - 8,
    (MCW * 2) + 8,
    (MCW * 3) - 8,
    (MCW * 3) + 8,
    (MCW * 4) - 7,
    (MCW * 4) + 7,
    (MCW * 5) - 7,
    (MCW * 5) + 7,
    (MCW * 6) - 6,
    (MCW * 6) + 6,
    (MCW * 7) - 5,
    (MCW * 7) - 4,
    (MCW * 7) + 4,
    (MCW * 7) + 5,
    (MCW * 8) - 3,
    (MCW * 8) - 2,
    (MCW * 8) + 2,
    (MCW * 8) + 3,
    (MCW * 9) - 1,
    (MCW * 9) + 0,
    (MCW * 9) + 1,
    /* 10 */ (-MCW * 10) - 1,
    (-MCW * 10) + 0,
    (-MCW * 10) + 1,
    (-MCW * 9) - 3,
    (-MCW * 9) - 2,
    (-MCW * 9) + 2,
    (-MCW * 9) + 3,
    (-MCW * 8) - 5,
    (-MCW * 8) - 4,
    (-MCW * 8) + 4,
    (-MCW * 8) + 5,
    (-MCW * 7) - 7,
    (-MCW * 7) - 6,
    (-MCW * 7) + 6,
    (-MCW * 7) + 7,
    (-MCW * 6) - 7,
    (-MCW * 6) + 7,
    (-MCW * 5) - 8,
    (-MCW * 5) + 8,
    (-MCW * 4) - 8,
    (-MCW * 4) + 8,
    (-MCW * 3) - 9,
    (-MCW * 3) + 9,
    (-MCW * 2) - 9,
    (-MCW * 2) + 9,
    (-MCW * 1) - 10,
    (-MCW * 1) + 10,
    -10,
    10,
    (MCW * 1) - 10,
    (MCW * 1) + 10,
    (MCW * 2) - 9,
    (MCW * 2) + 9,
    (MCW * 3) - 9,
    (MCW * 3) + 9,
    (MCW * 4) - 8,
    (MCW * 4) + 8,
    (MCW * 5) - 8,
    (MCW * 5) + 8,
    (MCW * 6) - 7,
    (MCW * 6) + 7,
    (MCW * 7) - 7,
    (MCW * 7) - 6,
    (MCW * 7) + 6,
    (MCW * 7) + 7,
    (MCW * 8) - 5,
    (MCW * 8) - 4,
    (MCW * 8) + 4,
    (MCW * 8) + 5,
    (MCW * 9) - 3,
    (MCW * 9) - 2,
    (MCW * 9) + 2,
    (MCW * 9) + 3,
    (MCW * 10) - 1,
    (MCW * 10) + 0,
    (MCW * 10) + 1,
};

int const MapClass::RadiusCount[11] = {1, 9, 21, 37, 61, 89, 121, 161, 205, 253, 309};

CellClass* BlubCell;

/***********************************************************************************************
 * MapClass::One_Time -- Performs special one time initializations for the map.                *
 *                                                                                             *
 *    This routine is used by the game initialization function in order to perform any one     *
 *    time initializations required for the map. This includes allocation of the map and       *
 *    setting up its default dimensions.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine MUST be called once and only once.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/01/1994 BR : Added CellTriggers initialization                                         *
 *=============================================================================================*/
void MapClass::One_Time(void)
{
    GScreenClass::One_Time();

    XSize = MAP_CELL_W;
    YSize = MAP_CELL_H;
    Size = XSize * YSize;

    /*
    **	Allocate the cell array.
    */
    Alloc_Cells();

    /*
    **	Init the CellTriggers array to the required size.
    */
    CellTriggers.Resize(MAP_CELL_TOTAL);
}

////////////////////////////////////////////////////
// Added this function to allow the editor to setup the map without setting up the entire system. - 06/18/2019 JAS
void MapClass::One_Time_Editor(void)
{
    XSize = MAP_CELL_W;
    YSize = MAP_CELL_H;
    Size = XSize * YSize;

    /*
    **	Allocate the cell array.
    */
    Alloc_Cells();

    /*
    **	Init the CellTriggers array to the required size.
    */
    CellTriggers.Resize(MAP_CELL_TOTAL);
}
// End of change. - 06/15/2019 JAS
////////////////////////////////////////////////////

/***********************************************************************************************
 * MapClass::Init_Clear -- clears the map & buffers to a known state                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Init_Clear(void)
{
    GScreenClass::Init_Clear();
    Init_Cells();
    TiberiumScan = 0;
    IsForwardScan = true;
    TiberiumGrowthCount = 0;
    TiberiumSpreadCount = 0;
    Clear_CFE_Cell_State();
}

/***********************************************************************************************
 * MapClass::Alloc_Cells -- allocates the cell array                                           *
 *                                                                                             *
 * This routine should be called at One_Time, and after loading the Map object from a save     *
 * game, but prior to loading the cell objects.                                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Alloc_Cells(void)
{
    /*
    **	Assume that whatever the contents of the VectorClass are is garbage
    **	(it may have been loaded from a save-game file), so zero it out first.
    */
    new (&Array) VectorClass<CellClass>;
    Array.Resize(Size);
    Clear_CFE_Cell_State();
}

/***********************************************************************************************
 * MapClass::Free_Cells -- frees the cell array                                                *
 *                                                                                             *
 * This routine is used by the Load_Game routine to free the map's cell array before loading   *
 * the map object from disk; the array is then re-allocated & cleared before the cell objects  *
 * are loaded.                                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Free_Cells(void)
{
    Array.Clear();
}

/***********************************************************************************************
 * MapClass::Init_Cells -- Initializes the cell array to a fresh state.                        *
 *                                                                                             *
 * This routine is used by Init_Clear to set the cells to a known state; it's also used by     *
 * the Load_Game routine to init all cells before loading a set of cells from disk, so it      *
 * needs to be called separately from the other Init_xxx() routines.                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Init_Cells(void)
{
    TotalValue = 0;
    for (int index = 0; index < MAP_CELL_TOTAL; index++) {
        new (&Array[index]) CellClass;
    }
}

/***********************************************************************************************
 * MapClass::Set_Map_Dimensions -- Set map dimensions.                                         *
 *                                                                                             *
 *    This routine is used to set the legal limits and position of the                         *
 *    map as it relates to the overall map array. Typically, this is                           *
 *    called by the scenario loading code.                                                     *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y coordinate of the "upper left" corner                         *
 *                   of the map.                                                               *
 *                                                                                             *
 *          w,h   -- The width and height of the legal map.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
    MapCellX = x;
    MapCellY = y;
    MapCellWidth = w;
    MapCellHeight = h;
}

/***********************************************************************************************
 * MapClass::Sight_From -- Mark as visible the cells within a specified radius.                *
 *                                                                                             *
 *    This routine is used to reveal the cells around a specific location.                     *
 *    Typically, as a unit moves or is deployed, this routine will be                          *
 *    called. Since it deals with MANY cells, it needs to be extremely                         *
 *    fast.                                                                                    *
 *                                                                                             *
 * INPUT:   house		-- Player to perform the visibility update for                            *
 *                                                                                             *
 * 			cell     -- The coordinate that the sighting originates from.                      *
 *                                                                                             *
 *          sightrange-- The distance in cells that sighting extends.                          *
 *                                                                                             *
 *          incremental-- Is this an incremental sighting. In other                            *
 *                      words, has this function been called before where                      *
 *                      the center coordinate is no more than one cell                         *
 *                      distant from the last time?                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1992 JLB : Created.                                                                 *
 *   03/08/1994 JLB : Updated to use sight table and incremental flag.                         *
 *   05/18/1994 JLB : Converted to member function.                                            *
 *   03/06/2019 ST  : Added HouseClass pointer parameter for new multiplayer code              *
 *=============================================================================================*/
void MapClass::Sight_From(HouseClass* house, CELL cell, int sightrange, bool incremental)
{
    int xx;         // Center cell X coordinate (bounds checking).
    int const* ptr; // Offset pointer.
    int count;      // Counter for number of offsets to process.
#ifdef REMASTER_BUILD
    // Added. ST - 3/6/2019 1:50PM
    if ((house == NULL || house->IsHuman == false) && !ShareAllyVisibility) {
        return;
    }
#endif
    /*
    **	Units that are off-map cannot sight.
    */
    if (!In_Radar(cell))
        return;
    if (!sightrange || sightrange > 10)
        return;

    /*
    **	Determine logical cell coordinate for center scan point.
    */
    xx = Cell_X(cell);

    /*
    **	Incremental scans only scan the outer rings. Full scans
    **	scan all internal cells as well.
    */
    count = RadiusCount[sightrange];
    ptr = &RadiusOffset[0];
    if (incremental) {
        if (sightrange > 1) {
            ptr += RadiusCount[sightrange - 2];
            count -= RadiusCount[sightrange - 2];
        }
    }

    /*
    **	Process all offsets required for the desired scan.
    */
    while (count--) {
        CELL newcell; // New cell with offset.
        int xdiff;    // New cell's X coordinate distance from center.

        newcell = cell + *ptr++;

        /*
        **	Determine if the map edge has been wrapped. If so,
        **	then don't process the cell.
        */
        if ((unsigned)newcell >= MAP_CELL_TOTAL)
            continue;
        xdiff = Cell_X(newcell) - xx;
        xdiff = ABS(xdiff);
        if (xdiff > sightrange)
            continue;
        if (Distance(newcell, cell) > sightrange)
            continue;

        /*
        **	Map the cell. For incremental scans, then update
        **	adjacent cells as well. For full scans, just update
        **	the cell itself.
        */
        // Pass the house through, instead of assuming it's the local player. ST - 3/6/2019 10:26AM
        // Map.Map_Cell(newcell, PlayerPtr);
        Map.Map_Cell(newcell, house, true);
    }
}

/***********************************************************************************************
 * MapClass::Cell_Distance -- Determines the distance between two cells.                       *
 *                                                                                             *
 *    This routine will return with the calculated "straight line"                             *
 *    distance between the two cells specified. It uses the dragon strike                      *
 *    method of distance calculation.                                                          *
 *                                                                                             *
 * INPUT:   cell1    -- First cell.                                                            *
 *                                                                                             *
 *          cell2    -- Second cell.                                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the "cell" distance between the two cells                             *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/29/1994 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
int MapClass::Cell_Distance(CELL cell1, CELL cell2)
{
    register int x, y; // Difference on X and Y axis.

    x = Cell_X(cell1) - Cell_X(cell2);
    y = Cell_Y(cell1) - Cell_Y(cell2);

    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;

    if (x > y) {
        return (x + (y >> 1));
    }
    return (y + (x >> 1));
}

/***********************************************************************************************
 * MapClass::In_Radar -- Is specified cell in the radar map?                                   *
 *                                                                                             *
 *    This determines if the specified cell can be within the navigable                        *
 *    bounds of the map. Technically, this means, any cell that can be                         *
 *    scanned by radar. If a cell returns false from this function, then                       *
 *    the player could never move to or pass over this cell.                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  bool; Is this cell possible to be displayed on radar?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *   05/01/1994 JLB : Speeded up.                                                              *
 *=============================================================================================*/
bool MapClass::In_Radar(CELL cell) const
{
    /*
    ** Updated to use the macro so it supports both map size configurations
    */
    if ((unsigned)cell > MAP_CELL_TOTAL)
        return (false);
    return ((unsigned)(Cell_X(cell) - MapCellX) < (unsigned)MapCellWidth
            && (unsigned)(Cell_Y(cell) - MapCellY) < (unsigned)MapCellHeight);
}

/***********************************************************************************************
 * MapClass::Place_Down -- Places the specified object onto the map.                           *
 *                                                                                             *
 *    This routine is used to place an object onto the map. It updates the "occupier" of the   *
 *    cells that this object covers. The cells are determined from the Occupy_List function    *
 *    provided by the object. Only one cell can have an occupier and this routine is the only  *
 *    place that sets this condition.                                                          *
 *                                                                                             *
 * INPUT:   cell     -- The cell to base object occupation around.                             *
 *                                                                                             *
 *          object   -- The object to place onto the map.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Place_Down(CELL cell, ObjectClass* object)
{
    if (!object)
        return;

    short const* list = object->Occupy_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Occupy_Down(object);
            (*this)[newcell].Recalc_Attributes();
            (*this)[newcell].Redraw_Objects();
        }
    }

    list = object->Overlap_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Overlap_Down(object);
            (*this)[newcell].Redraw_Objects();
        }
    }
}

/***********************************************************************************************
 * MapClass::Pick_Up -- Removes specified object from the map.                                 *
 *                                                                                             *
 *    The object specified is removed from the map by this routine. This will remove the       *
 *    occupation flag for all the cells that the object covers. The cells that are covered     *
 *    are determined from the Occupy_List function.                                            *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the object is centered about.                            *
 *                                                                                             *
 *          object   -- Pointer to the object that will be removed.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Pick_Up(CELL cell, ObjectClass* object)
{
    if (!object)
        return;

    short const* list = object->Occupy_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Occupy_Up(object);
            (*this)[newcell].Recalc_Attributes();
            (*this)[newcell].Redraw_Objects();
        }
    }

    list = object->Overlap_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Overlap_Up(object);
            (*this)[newcell].Redraw_Objects();
        }
    }
}

/***********************************************************************************************
 * MapClass::Overlap_Down -- computes & marks object's overlap cells                           *
 *                                                                                             *
 * This routine is just like Place_Down, but it doesn't mark the cell's Occupier.              *
 * This routine is used to implement MARK_OVERLAP_DOWN, which is useful for changing           *
 * an object's render size, but not its logical size (ie when it's selected or an              *
 * animation is attached to it).                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell -- The cell to base object overlap around.                                        *
 *    object   -- The object to place onto the map.                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Overlap_Down(CELL cell, ObjectClass* object)
{
    if (!object)
        return;

    short const* list = object->Overlap_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Overlap_Down(object);
            (*this)[newcell].Redraw_Objects();
        }
    }
}

/***********************************************************************************************
 * MapClass::Overlap_Up -- Computes & clears object's overlap cells                            *
 *                                                                                             *
 * This routine is just like Pick_Up, but it doesn't mark the cell's Occupier.                 *
 * This routine is used to implement MARK_OVERLAP_UP, which is useful for changing             *
 * an object's render size, but not its logical size (ie when it's selected or an              *
 * animation is attached to it).                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell -- The cell to base object overlap around.                                        *
 *    object   -- The object to place onto the map.                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Overlap_Up(CELL cell, ObjectClass* object)
{
    if (!object)
        return;

    short const* list = object->Overlap_List();
    while (*list != REFRESH_EOL) {
        CELL newcell = cell + *list++;
        if ((unsigned)newcell < MAP_CELL_TOTAL) {
            (*this)[newcell].Overlap_Up(object);
            (*this)[newcell].Redraw_Objects();
        }
    }
}

/***********************************************************************************************
 * MapClass::Overpass -- Performs any final cleanup to a freshly constructed map.              *
 *                                                                                             *
 *    This routine will clean up anything necessary with the presumption that the map has      *
 *    been freshly created. Such things to clean up include various tiberium concentrations.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the total credit value of the tiberium on the map.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *   02/13/1995 JLB : Returns total tiberium worth.                                            *
 *   02/15/1995 JLB : Optimal scan.                                                            *
 *=============================================================================================*/
int MapClass::Overpass(void)
{
    int value = 0;

    /*
    **	Smooth out Tiberium. Cells that are not surrounded by other tiberium
    **	will be reduced in density.
    */
    for (int y = 0; y < MapCellHeight - 1; y++) {
        for (int x = 0; x < MapCellWidth; x++) {
            value += (*this)[(MapCellY + y) * MAP_CELL_W + (MapCellX + x)].Tiberium_Adjust(true);
        }
    }
    return (value);
}

/***********************************************************************************************
 * MapClass::Read_Binary -- reads the map's binary image file                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for scenario                                                   *
 *      crc       ptr to CRC value to update                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = success, 0 = failure                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 BR : Created.                                                                  *
 *   01/08/1995 JLB : Fixup any obsolete icons detected.                                       *
 *=============================================================================================*/
bool MapClass::Read_Binary(char const* root, uint32_t* crc)
{
    char fname[_MAX_FNAME + _MAX_EXT];

    /*
    **	Filename = INI name with BIN extension.
    */
    sprintf(fname, "%s.BIN", root);

    return Read_Binary_File(fname, crc);
}

#ifdef MEGAMAPS
/***********************************************************************************************
 * MapClass::Read_Binary_Big -- reads the map's binary image file                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for scenario                                                   *
 *      crc       ptr to CRC value to update                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = success, 0 = failure                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 BR : Created.                                                                  *
 *   01/08/1995 JLB : Fixup any obsolete icons detected.                                       *
 *=============================================================================================*/
bool MapClass::Read_Binary_Big(char const* fname, uint32_t* crc)
{
    CCFileClass file;
    int i;
    char* map;
    void* rawmap;
    void const* shape;
    CellClass* cellptr = NULL;

    /*
    **	Create object & open file.
    */
    file.Set_Name(fname);
    if (!file.Is_Available()) {
        return (false);
    }
    file.Open(READ);

    /*
    **	Loop through all cells and set all to the clear tile.
    */
    for (i = 0; i < MAP_CELL_TOTAL; i++) {
        cellptr = &Map[i];
        cellptr->TType = (TemplateType)255;
        cellptr->TIcon = 0;
        cellptr->Recalc_Attributes();
    }

    /*
    **	Loop through all cells.
    */
    struct
    {
        CELL Cell;           // cell number on the map.
        TemplateType TType;  // Template type.
        unsigned char TIcon; // Template icon number.
    } temp;

    int file_size = file.Size() / sizeof(temp);
    cellptr = &Map[0];
    for (i = 0; i < MAP_CELL_TOTAL; i++) {

        if (i >= file_size)
            break;
        if (file.Read(&temp, sizeof(temp)) != sizeof(temp))
            break;
        if (temp.TType == (TemplateType)255) {
            temp.TType = TEMPLATE_NONE;
        }

        cellptr = &Map[temp.Cell];

        /*
        **	Verify that the template type actually contains the template number specified. If
        **	an illegal icon was specified, then replace it with clear terrain.
        */
        if (temp.TType != TEMPLATE_CLEAR1 && temp.TType != TEMPLATE_NONE) {
            TemplateTypeClass const& ttype = TemplateTypeClass::As_Reference(temp.TType);
            shape = ttype.Get_Image_Data();
            if (shape) {
                rawmap = Get_Icon_Set_Map(shape);
                if (rawmap) {
                    map = (char*)rawmap;
                    if ((temp.TIcon >= (ttype.Width * ttype.Height)) || (map[temp.TIcon] == -1)) {
                        temp.TIcon = 0;
                        temp.TType = TEMPLATE_NONE;
                    }
                }
            }
        }

        cellptr->TType = temp.TType;
        cellptr->TIcon = temp.TIcon;
        cellptr->Recalc_Attributes();

#ifndef DEMO
        Add_CRC(crc, (unsigned int)cellptr->TType);
        Add_CRC(crc, (unsigned int)cellptr->TIcon);
#endif

        cellptr++;
    }

    /*
    **	Close the file.
    */
    file.Close();

    return (i == MAP_CELL_TOTAL);
}
#endif

/***********************************************************************************************
 * MapClass::Read_Binary_File -- reads the map's binary image file						  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      fname     file path for scenario																		  *
 *      crc       ptr to CRC value to update                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = success, 0 = failure                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/2019 JAS : Created.                                                                  *
 *=============================================================================================*/
bool MapClass::Read_Binary_File(char const* fname, uint32_t* crc)
{
    CCFileClass file;

    int i;
    char* map;
    void* rawmap;
    void const* shape;
    CellClass* cellptr = NULL;

#ifdef MEGAMAPS
    if (MapBinaryVersion == MAP_VERSION_MEGA) {
        return Read_Binary_Big(fname, crc);
    }
#endif

    /*
    **	Create object & open file.
    */
    file.Set_Name(fname);
    if (!file.Is_Available()) {
        return (false);
    }
    file.Open(READ);

#ifdef MEGAMAPS
    /*
    **	Loop through all cells and set all to the clear tile.
    */
    for (i = 0; i < MAP_CELL_TOTAL; i++) {
        cellptr = &Map[i];
        cellptr->TType = (TemplateType)255;
        cellptr->TIcon = 0;
        cellptr->Recalc_Attributes();
    }
#endif

    /*
    **	We need to handle the orignal maps differently now as MAP_CELL_TOTAL is larger
    **  and will break all maps made the old way. This new piece of code will offset the
    **  cells of the smaller maps to be within the new MAP_CELL_TOTAL range.
    */
#ifdef MEGAMAPS
    /*
    **	Loop through all cells. The helper funtion will convert the old map binary cell
    **  to a position in the new big map array.
    */
    for (i = 0; i < (MAP_CELL_TOTAL / 4); i++) {
        CELL cell = Confine_Old_Cell(i);
        cellptr = &Map[cell];
#else
    /*
    **	Loop through all cells.
    */
    cellptr = &Map[0];
    for (i = 0; i < MAP_CELL_TOTAL; i++) {
#endif
#pragma pack(push, 1)
        struct
        {
            TemplateType TType;  // Template type.
            unsigned char TIcon; // Template icon number.
        } temp;
#pragma pack(pop)

        if (file.Read(&temp, sizeof(temp)) != sizeof(temp))
            break;
        if (temp.TType == (TemplateType)255) {
            temp.TType = TEMPLATE_NONE;
        }

        /*
        **	Verify that the template type actually contains the template number specified. If
        **	an illegal icon was specified, then replace it with clear terrain.
        */
        if (temp.TType != TEMPLATE_CLEAR1 && temp.TType != TEMPLATE_NONE) {
            shape = TemplateTypeClass::As_Reference(temp.TType).Get_Image_Data();
            if (shape) {
                rawmap = Get_Icon_Set_Map(shape);
                if (rawmap) {
                    map = (char*)rawmap;
                    if (map[temp.TIcon] == -1) {
                        temp.TIcon = 0;
                        temp.TType = TEMPLATE_NONE;
                    }
                }
            }
        }

        cellptr->TType = temp.TType;
        cellptr->TIcon = temp.TIcon;
        cellptr->Recalc_Attributes();

#ifndef DEMO
        Add_CRC(crc, (uint32_t)cellptr->TType);
        Add_CRC(crc, (uint32_t)cellptr->TIcon);
#endif

#ifndef MEGAMAPS
        cellptr++;
#endif
    }

    /*
    **	Close the file.
    */
    file.Close();

#ifdef MEGAMAPS
    return (i == (MAP_CELL_TOTAL / 4));
#else
    return (i == MAP_CELL_TOTAL);
#endif
}

/***********************************************************************************************
 * MapClass::Write_Binary -- writes the map's binary image file                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for scenario                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = success, 0 = failure                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 BR : Created.                                                                  *
 *=============================================================================================*/
bool MapClass::Write_Binary(char const* root)
{
#ifdef MEGAMAPS
    if (MapBinaryVersion == MAP_VERSION_MEGA) {
        return Write_Binary_Big(root);
    }
#endif

    CCFileClass* file;
    char fname[_MAX_FNAME + _MAX_EXT];
    int i;

    /*
    **	Filename = INI name with BIN extension.
    */
    sprintf(fname, "%s.BIN", root);

    /*
    **	Create object & open file.
    */
    file = new CCFileClass(fname);
    file->Open(WRITE);

    /*
    **	Loop through all cells.
    */
#ifdef MEGAMAPS
    for (i = 0; i < (MAP_CELL_TOTAL / 4); i++) {
        /*
        **	Work out which cell in the larger array we need data from.
        */
        CELL cell = Confine_Old_Cell(i);
#else
    for (i = 0; i < MAP_CELL_TOTAL; i++) {
        CELL cell = i;
#endif
        /*
        **	Save TType.
        */
        if (file->Write(&(Map[cell].TType), sizeof(TemplateType)) != sizeof(TemplateType)) {
            file->Close();
            delete file;
            return (false);
        }

        /*
        **	Save TIcon.
        */
        if (file->Write(&(Map[cell].TIcon), sizeof(unsigned char)) != sizeof(unsigned char)) {
            file->Close();
            delete file;
            return (false);
        }
    }

    /*
    **	Close the file.
    */
    file->Close();
    delete file;

    return (true);
}

#ifdef MEGAMAPS
/***********************************************************************************************
 * MapClass::Write_Binary_Big -- writes the map's binary image file                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for scenario                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = success, 0 = failure                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 BR : Created.                                                                  *
 *=============================================================================================*/
bool MapClass::Write_Binary_Big(char const* root)
{
    CCFileClass* file;
    char fname[_MAX_FNAME + _MAX_EXT];
    int i;

    /*
    **	Filename = INI name with BIN extension.
    */
    sprintf(fname, "%s.BIN", root);

    /*
    **	Create object & open file.
    */
    file = new CCFileClass(fname);
    file->Open(WRITE);

    /*
    **	Loop through all cells.
    */
    for (i = 0; i < MAP_CELL_TOTAL; i++) {
#pragma pack(push, 1)
        struct
        {
            CELL Cell;           // cell number on the map.
            TemplateType TType;  // Template type.
            unsigned char TIcon; // Template icon number.
        } temp;
#pragma pack(pop)
        /*
        **	To cut down the size of the scenario binary don't save clear tiles.
        **  Read_Binary handles this by initialising the map data with clear tiles for us.
        */
        if (Map[i].TType == TEMPLATE_NONE || Map[i].TType == TEMPLATE_CLEAR1) {
            continue;
        }

        temp.Cell = i;
        temp.TType = Map[i].TType;
        temp.TIcon = Map[i].TIcon;

        /*
        **	Save cell data.
        */
        if (file->Write(&temp, sizeof(temp)) != sizeof(temp)) {
            file->Close();
            delete file;
            return (false);
        }
    }

    /*
    **	Close the file.
    */
    file->Close();
    delete file;

    return (true);
}
#endif

/***********************************************************************************************
 * MapClass::Logic -- Handles map related logic functions.                                     *
 *                                                                                             *
 *    Manages tiberium growth and spread.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/11/1995 JLB : Created.                                                                 *
 *   07/09/1995 JLB : Handles two directional scan.                                            *
 *   08/01/1995 JLB : Gives stronger weight to blossom trees.                                  *
 *=============================================================================================*/
void MapClass::Logic(void)
{
    if (Debug_Force_Crash) {
        *((int*)0) = 1;
    }

    /*
    **	Bail early if there is no allowed growth or spread of Tiberium.
    */
    if (!Special.IsTGrowth && !Special.IsTSpread)
        return;

    /*
    **	Scan another block of the map in order to accumulate the potential
    **	Tiberium cells that can grow or spread.
    */
    int subcount = MAP_CELL_TOTAL / 136;
    int index;
    for (index = TiberiumScan; index < MAP_CELL_TOTAL; index++) {
        CELL cell = index;
        if (!IsForwardScan)
            cell = (MAP_CELL_TOTAL - 1) - index;
        CellClass* ptr = &(*this)[cell];

        if (Special.IsTGrowth && ptr->Land_Type() == LAND_TIBERIUM && ptr->OverlayData < 11) {
            if (TiberiumGrowthCount < sizeof(TiberiumGrowth) / sizeof(TiberiumGrowth[0])) {
                TiberiumGrowth[TiberiumGrowthCount++] = cell;
            } else {
                TiberiumGrowth[Random_Pick(0, TiberiumGrowthCount - 1)] = cell;
            }
        }

        /*
        **	Heavy Tiberium growth can spread.
        */
        TerrainClass* terrain = ptr->Cell_Terrain();
        const int spreadthreshhold = ActiveCFEPatchConfig.TibIndexBugfix ? 5 : 6;
        if (Special.IsTSpread && (ptr->Land_Type() == LAND_TIBERIUM && ptr->OverlayData > spreadthreshhold)
            || (terrain && terrain->Class->IsTiberiumSpawn)) {

            int tries = 1;
            if (terrain)
                tries = 3;
            for (int i = 0; i < tries; i++) {
                if (TiberiumSpreadCount < sizeof(TiberiumSpread) / sizeof(TiberiumSpread[0])) {
                    TiberiumSpread[TiberiumSpreadCount++] = cell;
                } else {
                    TiberiumSpread[Random_Pick(0, TiberiumSpreadCount - 1)] = cell;
                }
            }
        }
        subcount--;
        if (!subcount)
            break;
    }
    TiberiumScan = index;

    if (TiberiumScan >= MAP_CELL_TOTAL) {
        int tries = 1;
        if (Special.IsTFast || GameToPlay != GAME_NORMAL)
            tries = 2;

        /*
        ** Use the Tiberium setting as a multiplier on growth rate. ST - 7/1/2020 3:05PM
        */
        if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
            if (MPlayerTiberium > 1) {
                tries += (MPlayerTiberium - 1) << 1;
            }
        }
        TiberiumScan = 0;
        IsForwardScan = (IsForwardScan == false);

        /*
        **	Growth logic.
        */
        if (TiberiumGrowthCount) {
            for (int i = 0; i < tries; i++) {
                int pick = Random_Pick(0, TiberiumGrowthCount - 1);
                CELL cell = TiberiumGrowth[pick];
                CellClass* newcell = &(*this)[cell];
                if (newcell->Land_Type() == LAND_TIBERIUM && newcell->OverlayData < 12 - 1) {
                    newcell->OverlayData++;
                }
                TiberiumGrowth[pick] = TiberiumGrowth[TiberiumGrowthCount - 1];
                TiberiumGrowthCount--;
                if (TiberiumGrowthCount <= 0) {
                    break;
                }
            }
        }
        TiberiumGrowthCount = 0;

        /*
        **	Spread logic.
        */
        if (TiberiumSpreadCount) {
            for (int i = 0; i < tries; i++) {
                int pick = Random_Pick(0, TiberiumSpreadCount - 1);
                CELL cell = TiberiumSpread[pick];

                /*
                **	Find a pseudo-random adjacent cell that doesn't contain any tiberium.
                */
                if (Map.In_Radar(cell)) {
                    FacingType offset = Random_Pick(FACING_N, FACING_NW);
                    for (FacingType index = FACING_N; index < FACING_COUNT; index++) {
                        CellClass* newcell = (*this)[cell].Adjacent_Cell(index + offset);

                        if (newcell && newcell->Cell_Object() == NULL && newcell->Land_Type() == LAND_CLEAR
                            && newcell->Overlay == OVERLAY_NONE) {
                            bool found = false;

                            switch (newcell->TType) {
                            case TEMPLATE_BRIDGE1:
                            case TEMPLATE_BRIDGE2:
                            case TEMPLATE_BRIDGE3:
                            case TEMPLATE_BRIDGE4:
                                break;

                            default:
                                found = true;
                                new OverlayClass(Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12),
                                                 newcell->Cell_Number());
                                newcell->OverlayData = ActiveCFEPatchConfig.TibIndexBugfix ? 0 : 1;
                                break;
                            }
                            if (found)
                                break;
                        }
                    }
                }
                TiberiumSpread[pick] = TiberiumSpread[TiberiumSpreadCount - 1];
                TiberiumSpreadCount--;
                if (TiberiumSpreadCount <= 0) {
                    break;
                }
            }
        }
        TiberiumSpreadCount = 0;
    }
}

/***********************************************************************************************
 * MapClass::Cell_Region -- Determines the region from a specified cell number.                *
 *                                                                                             *
 *    Use this routine to determine what region a particular cell lies in.                     *
 *                                                                                             *
 * INPUT:   cell  -- The cell number to examine.                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the region that the specified cell occupies.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int MapClass::Cell_Region(CELL cell)
{
    return ((Cell_X(cell) / REGION_WIDTH) + 1) + (((Cell_Y(cell) / REGION_HEIGHT) + 1) * MAP_REGION_WIDTH);
}

/***************************************************************************
 * MapClass::Cell_Threat -- Gets a houses threat value for a cell          *
 *                                                                         *
 * INPUT:   CELL        cell    - the cell number to check                 *
 *            HouseType house   - the house to check                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/25/1995 PWG : Created.                                             *
 *=========================================================================*/
int MapClass::Cell_Threat(CELL cell, HousesType house)
{
    int threat = HouseClass::As_Pointer(house)->Regions[Map.Cell_Region(Map[cell].Cell_Number())].Threat_Value();
    // using function for IsVisible so we have different results for different players - JAS 2019/09/30
    if (!threat && Map[cell].Is_Visible(house)) {
        threat = 1;
    }
    return (threat);
}

/***********************************************************************************************
 * MapClass::Place_Random_Crate -- Places a crate at random location on map.                   *
 *                                                                                             *
 *    This routine will place a crate at a random location on the map. This routine will only  *
 *    make a limited number of attempts to place and if unsuccessful, it will not place any.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was a crate successfully placed?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Place_Random_Crate(void)
{
    int old = ScenarioInit;
    ScenarioInit = 0;
    for (int index = 0; index < 100; index++) {
        int x = Random_Pick(0, MapCellWidth - 1);
        int y = Random_Pick(0, MapCellHeight - 1);
        CELL cell = XY_Cell(MapCellX + x, MapCellY + y);

        CellClass* ptr = &(*this)[cell];
        if (ptr->Is_Generally_Clear() && ptr->Overlay == OVERLAY_NONE) {
            ptr->Overlay = OVERLAY_WOOD_CRATE;
            ptr->OverlayData = 0;
            ptr->Redraw_Objects();
            ScenarioInit = old;
            return (true);
        }
    }
    ScenarioInit = old;
    return (false);
}

/***************************************************************************
 * MapClass::Validate -- validates every cell on the map                   *
 *                                                                         *
 * This is a debugging routine, designed to detect memory trashers that    *
 * alter the map.  This routine is slow, but thorough.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = map is OK, false = an error was found                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/08/1995 BRR : Created.                                             *
 *=========================================================================*/
int MapClass::Validate(void)
{
    CELL cell;
    TemplateType ttype;
    unsigned char ticon;
    // TemplateTypeClass const *tclass;
    // unsigned char map[13*8];
    OverlayType overlay;
    SmudgeType smudge;
    ObjectClass* obj;
    LandType land;
    int i;

    BlubCell = &((*this)[797]);

    if (BlubCell->Overlapper[1]) {
        obj = BlubCell->Overlapper[1];
        if (obj) {
            if (obj->IsInLimbo)
                obj = obj;
        }
    }

    /*------------------------------------------------------------------------
    Check every cell on the map, even those that aren't displayed,
    in the hopes of detecting a memory trasher.
    ------------------------------------------------------------------------*/
    for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
        /*.....................................................................
        Validate Template & Icon data
        .....................................................................*/
        ttype = (*this)[cell].TType;
        ticon = (*this)[cell].TIcon;
        if (ttype >= TEMPLATE_COUNT && ttype != TEMPLATE_NONE)
            return (false);

            /*.....................................................................
            To validate the icon value, we have to get a copy of the template's
            "icon map"; this map will have 0xff's in spots where there is no
            icon.  If the icon value is out of range or points to an invalide spot,
            return an error.
            .....................................................................*/
#if (0)
        if (ttype != TEMPLATE_NONE) {
            tclass = &TemplateTypeClass::As_Reference(ttype);
            ticon = (*this)[cell].TIcon;
            Mem_Copy(Get_Icon_Set_Map(tclass->Get_Image_Data()), map, tclass->Width * tclass->Height);
            if (ticon < 0 || ticon >= (tclass->Width * tclass->Height) || map[ticon] == 0xff)
                return (false);
        }
#endif
        /*.....................................................................
        Validate Overlay
        .....................................................................*/
        overlay = (*this)[cell].Overlay;
        if (overlay < OVERLAY_NONE || overlay >= OVERLAY_COUNT)
            return (false);

        /*.....................................................................
        Validate Smudge
        .....................................................................*/
        smudge = (*this)[cell].Smudge;
        if (smudge < SMUDGE_NONE || smudge >= SMUDGE_COUNT)
            return (false);

        /*.....................................................................
        Validate LandType
        .....................................................................*/
        land = (*this)[cell].Land_Type();
        if (land < LAND_CLEAR || land >= LAND_COUNT)
            return (false);

        /*.....................................................................
        Validate Occupier
        .....................................................................*/
        obj = (*this)[cell].Cell_Occupier();
        if (obj) {

            volatile TARGET target = obj->As_Target(); // This will do some internal verification

            if (!obj->IsActive) {
                return false;
            }

            if (obj->IsInLimbo) {
                return false;
            }

            if (((unsigned int)Coord_Cell(obj->Coord) > 4095)) {
                return (false);
            }
        }

        /*.....................................................................
        Validate Overlappers
        .....................................................................*/
        for (i = 0; i < ARRAY_SIZE((*this)[cell].CellClass::Overlapper); i++) {
            obj = (*this)[cell].Overlapper[i];
            if (obj) {

                volatile TARGET target = obj->As_Target(); // This will do some internal verification

                if (!obj->IsActive) {
                    return false;
                }

                if (obj->IsInLimbo) {
                    return false;
                }

                if (((unsigned int)Coord_Cell(obj->Coord) > 4095)) {
                    return (false);
                }
            }
        }
    }

    return (true);
}

/***********************************************************************************************
 * MapClass::Clean -- Clean up dangling pointers caused by bugs in the originl code.           *
 *                                                                                             *
 *   Ideally, we'd fix the underlying cause of the overlappers not being cleared               *
 *   but we can afford the CPU time now                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/14/2020 11:48AM ST : Created.                                                           *
 *=============================================================================================*/
void MapClass::Clean(void)
{
    CELL cell;
    ObjectClass* obj;
    int i;
#ifndef NDEBUG
    char debug_message[256];
#endif
    bool active_fail = false;
    bool limbo_fail = false;
    const char* type_text = NULL;
    const char* ini_name = NULL;
    AbstractClass abstract_object;
    unsigned int abstract_vtable = *(unsigned int*)&abstract_object;

    /*------------------------------------------------------------------------
    Check every cell on the map, even those that aren't displayed.
    ------------------------------------------------------------------------*/
    for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {

        /*.....................................................................
        Validate Occupier
        .....................................................................*/
        (*this)[cell].Cell_Occupier();

        /*.....................................................................
        Validate Overlappers
        .....................................................................*/
        for (i = 0; i < ARRAY_SIZE((*this)[cell].Overlapper); i++) {
            obj = (*this)[cell].Overlapper[i];
            if (obj) {

                if (!obj->IsActive) {
                    (*this)[cell].Overlapper[i] = NULL;
                    active_fail = true;
                }

                if (!active_fail && obj->IsInLimbo) {
                    (*this)[cell].Overlapper[i] = NULL;
                    limbo_fail = true;
                }

                if (active_fail || limbo_fail) {
#ifndef NDEBUG
                    /*
                    ** This object is likely deleted.
                    */
                    if (abstract_vtable == *(unsigned int*)obj) {
                        type_text = "Abstract";
                        ini_name = "UNKNOWN";
                    } else {

                        RTTIType type = obj->What_Am_I();

                        switch (type) {
                        default:
                            type_text = "Unknown";
                            break;

                        case RTTI_INFANTRY:
                            type_text = "Infantry";
                            break;

                        case RTTI_UNIT:
                            type_text = "Unit";
                            break;

                        case RTTI_AIRCRAFT:
                            type_text = "Aircraft";
                            break;

                        case RTTI_BUILDING:
                            type_text = "Building";
                            break;

                        case RTTI_BULLET:
                            type_text = "Bullet";
                            break;

                        case RTTI_ANIM:
                            type_text = "Anim";
                            break;

                        case RTTI_SMUDGE:
                            type_text = "Smudge";
                            break;

                        case RTTI_TERRAIN:
                            type_text = "Terrain";
                            break;
                        }

                        ini_name = obj->Class_Of().IniName;
                    }

                    snprintf(debug_message,
                             sizeof(debug_message) - 1,
                             "Cleaned %s overlapper in cell %08X. Type=%s, IniName=%s",
                             active_fail ? "inactive" : "limbo",
                             cell,
                             type_text,
                             ini_name);
                    GlyphX_Debug_Print(debug_message);
#endif // NDEBUG
                }
            }
        }
    }
}

/***********************************************************************************************
 * MapClass::Close_Object -- Finds a clickable close object to the specified coordinate.       *
 *                                                                                             *
 *    This routine is used by the mouse input processing code to find a clickable object       *
 *    close to coordinate specified. This is for targeting as well as selection determination. *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to scan for close object from.                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an object that is nearby the specified coordinate.       *
 *                                                                                             *
 * WARNINGS:   There could be a cloaked object at the location, but it won't be considered     *
 *             if it is not owned by the player.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass* MapClass::Close_Object(COORDINATE coord) const
{
    ObjectClass* object = 0;
    int distance = 0;
    CELL cell = Coord_Cell(coord);

    /*
    **	Scan through current and adjacent cells, looking for the
    **	closest object (within reason) to the specified coordinate.
    */
    static int _offsets[] = {
        0, -1, 1, -MAP_CELL_W, MAP_CELL_W, MAP_CELL_W - 1, MAP_CELL_W + 1, -(MAP_CELL_W - 1), -(MAP_CELL_W + 1)};
    for (int index = 0; index < (sizeof(_offsets) / sizeof(_offsets[0])); index++) {

        /*
        **	Examine the cell for close object. Make sure that the cell actually is a
        **	legal one.
        */
        CELL newcell = cell + _offsets[index];
        if (In_Radar(newcell)) {

            /*
            **	Search through all objects that occupy this cell and then
            **	find the closest object. Check against any previously found object
            **	to ensure that it is actually closer.
            */
            ObjectClass* o = (*this)[newcell].Cell_Occupier();
            while (o) {

                /*
                **	Special case check to ignore cloaked object if not allied with the player.
                */
                // Changed for multiplayer. ST - 3/13/2019 5:38PM
                if (!o->Is_Techno() || !((TechnoClass*)o)->Is_Cloaked(PlayerPtr)) {
                    // if (!o->Is_Techno() || ((TechnoClass *)o)->IsOwnedByPlayer || ((TechnoClass *)o)->Cloak !=
                    // CLOAKED) {
                    int d = -1;
                    if (o->What_Am_I() == RTTI_BUILDING) {
                        d = Distance(coord, Cell_Coord(newcell));
                        if (d > 0x00C0)
                            d = -1;
                    } else {
                        d = Distance(coord, o->Center_Coord());
                    }
                    if (d >= 0 && (!object || d < distance)) {
                        distance = d;
                        object = o;
                    }
                }
                o = o->Next;
            }
        }
    }

    /*
    ** Handle aircraft selection separately, since they aren't tracked in cells while flying
    */
    for (int index = 0; index < Aircraft.Count(); index++) {
        AircraftClass* aircraft = Aircraft.Ptr(index);

        if (aircraft->In_Which_Layer() != LAYER_GROUND) {
            if (!aircraft->Is_Cloaked(PlayerPtr)) {
                int d = Distance(
                    coord, Coord_Add(aircraft->Center_Coord(), XY_Coord(0, -Pixel_To_Lepton(aircraft->Altitude))));
                if (d >= 0 && (!object || d < distance)) {
                    distance = d;
                    object = aircraft;
                }
            }
        }
    }

    /*
    **	Only return the object if it is within 1/4 cell distance from the specified
    **	coordinate.
    */
    if (object && distance > 0xC0) {
        object = 0;
    }
    return (object);
}

#ifdef USE_RA_AI

/*
** Nearby_Location pulled in from RA for AI. ST - 7/24/2019 5:54PM
*/
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

/***********************************************************************************************
 * MapClass::Nearby_Location -- Finds a generally clear location near a specified cell.        *
 *                                                                                             *
 *    This routine is used to find a location that probably will be ok to move to that is      *
 *    located as close as possible to the specified cell. The computer uses this when it has   *
 *    determined the ideal location for an object, but then needs to give a valid movement     *
 *    destination to a unit.                                                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell that scanning should radiate out from.                           *
 *                                                                                             *
 *          zone  -- The zone that must be matched to find a legal location (value of -1 means *
 *                   any zone will do).                                                        *
 *                                                                                             *
 *                                                                                             *
 *          check -- The type of zone to check against. Only valid if a zone value is given.   *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is generally clear (legal to move to) that is close     *
 *          to the specified cell.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL MapClass::Nearby_Location(CELL cell) const //, SpeedType speed, int zone, MZoneType check) const
{
    CELL topten[10];
    int count = 0;
    int xx = Cell_X(cell);
    int yy = Cell_Y(cell);

    /*
    **	Determine the limits of the scanning in the four directions so that
    **	it won't scan past the edge of the world.
    */
    int left = MapCellX;
    int right = MapCellX + MapCellWidth - 1;
    int top = MapCellY;
    int bottom = MapCellY + MapCellHeight - 1;

    /*
    **	Radiate outward from the specified location, looking for the closest
    **	location that is generally clear.
    */
    for (int radius = 0; radius < MAP_CELL_W; radius++) {
        CELL newcell;
        CellClass const* cellptr;

        /*
        **	Scan the top and bottom rows of the "box".
        */
        for (int x = xx - radius; x <= xx + radius; x++) {
            if (x >= left && x <= right) {
                int y = yy - radius;
                if (y >= top) {
                    newcell = XY_Cell(x, y);
                    cellptr = &Map[newcell];
                    if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(false, false)) {
                        topten[count++] = newcell;
                    }
                }
                if (count == ARRAY_SIZE(topten))
                    break;

                y = yy + radius;
                if (y <= bottom) {
                    newcell = XY_Cell(x, y);
                    cellptr = &Map[newcell];
                    if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(false, false)) {
                        topten[count++] = newcell;
                    }
                }
                if (count == ARRAY_SIZE(topten))
                    break;
            }
        }

        if (count == ARRAY_SIZE(topten))
            break;

        /*
        **	Scan the left and right columns of the "box".
        */
        for (int y = yy - radius; y <= yy + radius; y++) {
            if (y >= top && y <= bottom) {
                int x = xx - radius;
                if (x >= left) {
                    newcell = XY_Cell(x, y);
                    cellptr = &Map[newcell];
                    if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(false, false)) {
                        topten[count++] = newcell;
                    }
                }
                if (count == ARRAY_SIZE(topten))
                    break;

                x = xx + radius;
                if (x <= right) {
                    newcell = XY_Cell(x, y);
                    cellptr = &Map[newcell];
                    if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(false, false)) {
                        topten[count++] = newcell;
                    }
                }
                if (count == ARRAY_SIZE(topten))
                    break;
            }
        }

        if (count > 0)
            break;
    }

    if (count > 0) {
        return (topten[Frame % count]);
    }
    return (0);
}

#endif // USE_RA_AI

/*
**	CFE "tiberium resurgence" per-cell state. CFE keeps these in CellClass; here they are kept
**	outside it so sizeof(CellClass), and so the save format, stays the same. They are not saved:
**	after a load, resurgence build-up starts over.
*/
static int TibResurgenceBuildup[MAP_CELL_TOTAL];
static int TibResurgenceCountdown[MAP_CELL_TOTAL];

/*
**	CFE "ReservingVehicle" (CELL_RESERVATION_FIX), kept outside CellClass for the same reason.
**	Not saved either: Rebuild_Vehicle_Reservations() fills it in again after a load.
*/
static FootClass* ReservingVehicles[MAP_CELL_TOTAL];

void MapClass::Clear_CFE_Cell_State(void)
{
    memset(TibResurgenceBuildup, 0, sizeof(TibResurgenceBuildup));
    memset(TibResurgenceCountdown, 0, sizeof(TibResurgenceCountdown));
    memset(ReservingVehicles, 0, sizeof(ReservingVehicles));
}

FootClass*& CellClass::Reserving_Vehicle(void) const
{
    return ReservingVehicles[Cell_Number()];
}

/***********************************************************************************************
 * MapClass::Cleanup_Vehicle_Reservations -- Clears vehicle flags that nothing holds any more.  *
 *                                                                                             *
 *    CFE: vehicle occupy/reserve flags sometimes stay set after the unit has gone. Every     *
 *    frame, clear the flag in any cell with no vehicle in it unless its live reserving unit    *
 *    is headed there or will pass through it on a two-cell track.                             *
 *=============================================================================================*/
void MapClass::Cleanup_Vehicle_Reservations(void)
{
    /*
    **	Check the playable area plus one row around it: units can drive on and off the map.
    */
    int maxx = MapCellX + MapCellWidth;
    int maxy = MapCellY + MapCellHeight;
    int minx = MapCellX - 1;
    int miny = MapCellY - 1;

    for (int index = 0; index < MAP_CELL_TOTAL; index++) {
        CELL cell = index;
        int cellx = Cell_X(cell);
        int celly = Cell_Y(cell);
        if (cellx > maxx || celly > maxy || cellx < minx || celly < miny) {
            continue;
        }
        CellClass* cellptr = &(*this)[cell];
        FootClass* someunit = cellptr->Reserving_Vehicle();

        if (!someunit && !cellptr->Flag.Occupy.Vehicle) {
            continue;
        }
        if (cellptr->Cell_Unit() || cellptr->Cell_Aircraft()) {
            continue;
        }
        if (someunit && someunit->IsActive) {
            if (Coord_Cell(someunit->Head_To_Coord()) == cell) {
                continue;
            }
            if (someunit->What_Am_I() == RTTI_UNIT && ((DriveClass*)someunit)->Track_Midpoint_Cell() == cell) {
                continue;
            }
        }
        cellptr->Reserving_Vehicle() = NULL;
        cellptr->Flag.Occupy.Vehicle = false;
    }
}

/***********************************************************************************************
 * MapClass::Rebuild_Vehicle_Reservations -- Refills the reserving-unit table after a load.    *
 *                                                                                             *
 *    The table isn't saved. Each vehicle claims the flagged cells it is in, heading to, or     *
 *    passing through, so the flags aren't taken for orphans and cleared.                      *
 *=============================================================================================*/
static void Claim_Vehicle_Reservation(CELL cell, FootClass* unit)
{
    if ((unsigned)cell < MAP_CELL_TOTAL) {
        CellClass* cellptr = &Map[cell];
        if (cellptr->Flag.Occupy.Vehicle && !cellptr->Reserving_Vehicle()) {
            cellptr->Reserving_Vehicle() = unit;
        }
    }
}

void MapClass::Rebuild_Vehicle_Reservations(void)
{
    if (!ActiveCFEPatchConfig.EnableCellReservationFix) {
        return;
    }
    for (int index = 0; index < Units.Count(); index++) {
        UnitClass* unit = Units.Ptr(index);
        if (unit && unit->IsActive && !unit->IsInLimbo) {
            Claim_Vehicle_Reservation(Coord_Cell(unit->Coord), unit);
            if (unit->Head_To_Coord()) {
                Claim_Vehicle_Reservation(Coord_Cell(unit->Head_To_Coord()), unit);
            }
            Claim_Vehicle_Reservation(unit->Track_Midpoint_Cell(), unit);
        }
    }
    for (int index = 0; index < Aircraft.Count(); index++) {
        AircraftClass* aircraft = Aircraft.Ptr(index);
        if (aircraft && aircraft->IsActive && !aircraft->IsInLimbo) {
            CELL cell = Coord_Cell(aircraft->Coord);
            if ((unsigned)cell < MAP_CELL_TOTAL && Map[cell].Cell_Aircraft() == aircraft) {
                Claim_Vehicle_Reservation(cell, aircraft);
            }
        }
    }
}

/***********************************************************************************************
 * CFE_Random_Pick_Full_Range -- Random_Pick for ranges wider than 15 bits.                    *
 *                                                                                             *
 *    Scen.RandomNumber (behind Random_Pick) only yields 15 significant bits. CFE uses rand()  *
 *    here because Random_Pick uses rand() in its build; VC's doesn't, so this combines two     *
 *    15-bit draws from the same game generator, rejecting the uneven top end.                 *
 *=============================================================================================*/
static int CFE_Random_Pick_Full_Range(int minval, int maxval)
{
    if (minval > maxval) {
        int temp = minval;
        minval = maxval;
        maxval = temp;
    }
    if (maxval - minval < 0x8000) {
        return Random_Pick(minval, maxval);
    }

    unsigned range = (unsigned)(maxval - minval) + 1;
    unsigned limit = (0x40000000u / range) * range;
    unsigned value;
    do {
        value = ((unsigned)Random_Pick(0, 0x7FFF) << 15) | (unsigned)Random_Pick(0, 0x7FFF);
    } while (value >= limit);
    return minval + (int)(value % range);
}

/***********************************************************************************************
 * MapClass::CFE_Logic -- CFE Patch Redux "better tiberium growth".                            *
 *                                                                                             *
 *    Replaces the original block-by-block scan with a per-frame pass over the whole map in    *
 *    which every growable or spreading cell has the same small chance to act. On average the  *
 *    amount of growth matches the original rate (one growth phase per 137 frames). A spreader *
 *    with no free neighbour grows a neighbour or itself instead of wasting its turn.          *
 *=============================================================================================*/
void MapClass::CFE_Logic(void)
{
    /*
    **	Bail early if there is no allowed growth or spread of Tiberium.
    */
    if (!Special.IsTGrowth && !Special.IsTSpread)
        return;

    /*
    **	Scan the whole map for cells that can grow or spread. Tiberium trees are listed three
    **	times, as in the original logic.
    */
    static CELL cangrowlist[MAP_CELL_TOTAL];
    static CELL canspreadlist[MAP_CELL_TOTAL];
    int growlistsize = 0;
    int spreadlistsize = 0;
    const int spreadthreshhold = ActiveCFEPatchConfig.TibIndexBugfix ? 5 : 6;

    for (int index = 0; index < MAP_CELL_TOTAL; index++) {
        CELL cell = index;
        if (!Map.In_Radar(cell)) {
            continue;
        }
        CellClass* ptr = &(*this)[cell];
        TerrainClass* terrain = ptr->Cell_Terrain();
        if (Special.IsTGrowth && ptr->Land_Type() == LAND_TIBERIUM && ptr->OverlayData < 11
            && growlistsize < MAP_CELL_TOTAL) {
            cangrowlist[growlistsize++] = cell;
        }
        if (Special.IsTSpread && ptr->Land_Type() == LAND_TIBERIUM && ptr->OverlayData > spreadthreshhold
            && spreadlistsize < MAP_CELL_TOTAL) {
            canspreadlist[spreadlistsize++] = cell;
        } else if (terrain && terrain->Class->IsTiberiumSpawn) {
            for (int i = 0; i < 3; i++) {
                if (spreadlistsize < MAP_CELL_TOTAL) {
                    canspreadlist[spreadlistsize++] = cell;
                }
            }
        }
    }

    /*
    **	Growth strength: doubled for fast growth and multiplayer, then scaled by the multiplayer
    **	tiberium setting or TIB_GROWTH_SCALE, and by map area on megamaps.
    */
    int growthstrength = 1;
    if (Special.IsTFast || GameToPlay != GAME_NORMAL) {
        growthstrength = 2;
    }
    if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
        int scalefactor = MPlayerTiberium;
        if (ActiveCFEPatchConfig.TibGrowthScale > scalefactor) {
            scalefactor = ActiveCFEPatchConfig.TibGrowthScale;
        }
        growthstrength += (scalefactor - 1) * 2;
    } else {
        growthstrength *= ActiveCFEPatchConfig.TibGrowthScale;
    }
    if (MapBinaryVersion == MAP_VERSION_MEGA) {
        /*
        **	CFE stores this factor when the map loads; it is derived only from the map size, so
        **	compute it here. Integer division, as in CFE, so it is a whole number.
        */
        float megafactor = ((MapCellWidth + 2) * (MapCellHeight + 2)) / 4096;
        if (megafactor < 1.0f) {
            megafactor = 1.0f;
        }
        float megagrowstrength = (growthstrength * megafactor) + 0.5f;
        growthstrength = megagrowstrength;
    }

    /*
    **	Grow tiberium. The original logic took 137 frames per growth phase.
    */
    int maxroll = (growlistsize * 137) - 1;
    for (int i = 0; i < growlistsize; i++) {
        if (CFE_Random_Pick_Full_Range(0, maxroll) < growthstrength) {
            (*this)[cangrowlist[i]].OverlayData++;
        }
    }

    /*
    **	Spread tiberium.
    */
    maxroll = (spreadlistsize * 137) - 1;
    for (int i = 0; i < spreadlistsize; i++) {
        if (CFE_Random_Pick_Full_Range(0, maxroll) < growthstrength) {
            CELL cell = canspreadlist[i];
            if (Map.In_Radar(cell)) {
                CellClass* newcell = NULL;
                CellClass* backupcell = NULL;
                bool foundideal = false;
                bool foundbackup = false;

                /*
                **	Starting from a random direction, look for an empty neighbour; remember the
                **	first neighbour with tiberium that can still grow as a fallback.
                */
                FacingType offset = Random_Pick(FACING_N, FACING_NW);
                for (FacingType j = FACING_N; j < FACING_COUNT; j++) {
                    newcell = (*this)[cell].Adjacent_Cell(j + offset);
                    if (newcell && Map.In_Radar(newcell->Cell_Number()) && newcell->Cell_Object() == NULL
                        && newcell->Land_Type() == LAND_CLEAR && newcell->Overlay == OVERLAY_NONE
                        && newcell->TType != TEMPLATE_BRIDGE1 && newcell->TType != TEMPLATE_BRIDGE2
                        && newcell->TType != TEMPLATE_BRIDGE3 && newcell->TType != TEMPLATE_BRIDGE4) {
                        foundideal = true;
                        break;
                    } else if (!foundbackup && newcell && Map.In_Radar(newcell->Cell_Number())
                               && newcell->Land_Type() == LAND_TIBERIUM && newcell->OverlayData < 11) {
                        backupcell = newcell;
                        foundbackup = true;
                    }
                }

                if (foundideal) {
                    new OverlayClass(Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12), newcell->Cell_Number());
                    newcell->OverlayData = ActiveCFEPatchConfig.TibIndexBugfix ? 0 : 1;
                } else if (foundbackup) {
                    backupcell->OverlayData++;
                } else {
                    CellClass* thiscell = &(*this)[cell];
                    if (thiscell->Land_Type() == LAND_TIBERIUM && thiscell->OverlayData < 11) {
                        thiscell->OverlayData++;
                    } else if (ActiveCFEPatchConfig.TreeVisceroidSpawnMode > 0
                               && (GameToPlay != GAME_NORMAL || ActiveCFEPatchConfig.TreeVisceroidSpawnMode > 1)
                               && (GameToPlay == GAME_NORMAL || Special.IsVisceroids
                                   || ActiveCFEPatchConfig.TreeVisceroidSpawnAnyway)) {
                        /*
                        **	CFE tree visceroid spawn: a tiberium tree that couldn't spread or grow
                        **	at all may spawn a visceroid. 5% chance plus 1% per failure since the
                        **	last spawn, scaled down by growth strength so the spawn rate doesn't
                        **	rise with faster growth. (CFE keeps the failure count in MapClass,
                        **	saved; here it is a static, not saved.)
                        */
                        static unsigned char frustration_count = 0;
                        TerrainClass* thisterrain = thiscell->Cell_Terrain();
                        if (thisterrain && thisterrain->Class->IsTiberiumSpawn) {
                            frustration_count++;
                            if (CFE_Random_Pick_Full_Range(0, (100 * growthstrength) - 1) < frustration_count + 4) {
                                UnitClass* unit = new UnitClass(UNIT_VICE, HOUSE_JP);
                                if (unit) {
                                    if (Scan_Place_Object(unit, cell)) {
                                        unit->Do_Squidgy_Spawn();
                                        frustration_count = 0;
                                    } else {
                                        delete unit;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /*
    **	CFE "tiberium resurgence": a tiberium tree with little tiberium around it builds up for
    **	TIB_RESURGE_DELAY frames on average, then (starting with a sporing animation) bursts out
    **	new tiberium around itself for 480 frames, slowing down over that time.
    */
    if (ActiveCFEPatchConfig.TibResurgence) {
        for (int index = 0; index < MAP_CELL_TOTAL; index++) {
            CELL cell = index;
            if (!Map.In_Radar(cell)) {
                continue;
            }
            CellClass* ptr = &(*this)[cell];
            TerrainClass* terrain = ptr->Cell_Terrain();
            if (terrain && terrain->Class->IsTiberiumSpawn) {
                int& buildup = TibResurgenceBuildup[cell];
                int& countdown = TibResurgenceCountdown[cell];

                if (countdown == 0) {
                    /*
                    **	Build up while there is little tiberium next to the tree.
                    */
                    int nearbytibcount = 0;
                    bool toomuch = false;
                    for (FacingType j = FACING_N; j < FACING_COUNT; j++) {
                        CellClass* newcell = (*this)[cell].Adjacent_Cell(j);
                        if (newcell && Map.In_Radar(newcell->Cell_Number()) && newcell->Land_Type() == LAND_TIBERIUM) {
                            nearbytibcount += newcell->OverlayData;
                            if (ActiveCFEPatchConfig.TibIndexBugfix) {
                                nearbytibcount++;
                            }
                            if (nearbytibcount > 8) {
                                toomuch = true;
                                break;
                            }
                        }
                    }
                    if (!toomuch) {
                        buildup = buildup + 8 - Random_Pick(0, nearbytibcount);
                        if (buildup >= 8 * ActiveCFEPatchConfig.TibResurgeDelay) {
                            buildup = 0;
                            countdown = 480;
                        }
                    }
                } else if (countdown > 0 && (countdown < 480 || terrain->Is_Sporing())) {
                    /*
                    **	Spread until the countdown expires; wait for a sporing animation to start.
                    */
                    countdown--;
                    FacingType offset = Random_Pick(FACING_N, FACING_NW);
                    for (FacingType j = FACING_N; j < FACING_COUNT; j++) {
                        // Decrease the odds over time so that growth bursts, then slows down.
                        if (Random_Pick(0, 638 - countdown) > 9) {
                            continue;
                        }
                        FacingType facingindex = j + offset;
                        CellClass* newcell = (*this)[cell].Adjacent_Cell(facingindex);
                        if (newcell && Map.In_Radar(newcell->Cell_Number()) && newcell->Cell_Object() == NULL
                            && newcell->TType != TEMPLATE_BRIDGE1 && newcell->TType != TEMPLATE_BRIDGE2
                            && newcell->TType != TEMPLATE_BRIDGE3 && newcell->TType != TEMPLATE_BRIDGE4) {
                            if (newcell->Land_Type() == LAND_CLEAR && newcell->Overlay == OVERLAY_NONE) {
                                new OverlayClass(Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12),
                                                 newcell->Cell_Number());
                                newcell->OverlayData = ActiveCFEPatchConfig.TibIndexBugfix ? 0 : 1;
                            } else if (newcell->Land_Type() == LAND_TIBERIUM) {
                                if (newcell->OverlayData < 11) {
                                    newcell->OverlayData++;
                                } else {
                                    /*
                                    **	That neighbour is full: try the cells beyond it, favouring
                                    **	the one straight ahead.
                                    */
                                    int maxroll = 3;
                                    for (FacingType k = facingindex - FACING_NE; k <= facingindex + FACING_NE; k++) {
                                        maxroll--;
                                        if (maxroll) {
                                            if (Random_Pick(0, maxroll) != 0) {
                                                continue;
                                            }
                                        }
                                        CellClass* newnewcell = newcell->Adjacent_Cell(k);
                                        if (newnewcell && Map.In_Radar(newnewcell->Cell_Number())
                                            && newnewcell->Cell_Object() == NULL && newnewcell->TType != TEMPLATE_BRIDGE1
                                            && newnewcell->TType != TEMPLATE_BRIDGE2
                                            && newnewcell->TType != TEMPLATE_BRIDGE3
                                            && newnewcell->TType != TEMPLATE_BRIDGE4) {
                                            if (newnewcell->Land_Type() == LAND_CLEAR
                                                && newnewcell->Overlay == OVERLAY_NONE) {
                                                new OverlayClass(Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12),
                                                                 newnewcell->Cell_Number());
                                                newnewcell->OverlayData = ActiveCFEPatchConfig.TibIndexBugfix ? 0 : 1;
                                                break;
                                            } else if (newnewcell->Land_Type() == LAND_TIBERIUM
                                                       && newnewcell->OverlayData < 11) {
                                                newnewcell->OverlayData++;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/***********************************************************************************************
 * MapClass::Process_Tiberium_Death -- Maybe spawns a visceroid where infantry died.           *
 *                                                                                             *
 *    CFE Patch Redux "TS visceroid spawn" (how == 1: died walking through tiberium). The      *
 *    chance starts at 1 in 20 and rises with each such death until a visceroid appears. CFE's *
 *    other cases (killed by a visceroid or a buffed chem soldier) come with meaner visceroids *
 *    and the balance options.                                                                 *
 *=============================================================================================*/
bool MapClass::Process_Tiberium_Death(CELL where, COORDINATE specifically_where, unsigned char how)
{
    /*
    **	CFE keeps this count in MapClass (saved); here it is not saved.
    */
    static unsigned char offal_count = 0;

    if (how == 1) {
        offal_count++;
        if (Random_Pick(0, 19) < offal_count) {
            UnitClass* unit = new UnitClass(UNIT_VICE, HOUSE_JP);
            if (unit) {
                if (unit->Unlimbo(specifically_where) || Scan_Place_Object(unit, where)) {
                    unit->Do_Squidgy_Spawn();
                    offal_count = 0;
                    return true;
                }
                delete unit;
            }
        }
    }
    return false;
}

/***********************************************************************************************
 * MapClass::Flip_Walls -- Reassigns the walls around a building that changed hands.           *
 *                                                                                             *
 * INPUT:   centercell -- The cell at the center of the building.                              *
 *                                                                                             *
 *          gainhouse  -- The house that gained a building, or HOUSE_NONE.                     *
 *                                                                                             *
 *          losehouse  -- The house that lost a building, or HOUSE_NONE.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *=============================================================================================*/
void MapClass::Flip_Walls(CELL centercell, HousesType gainhouse, HousesType losehouse)
{
    int centerx = Cell_X(centercell);
    int centery = Cell_Y(centercell);
    for (int y = centery - 16; y <= centery + 16; y++) {
        if (y < 0 || y >= MAP_CELL_H) {
            continue;
        }
        for (int x = centerx - 16; x <= centerx + 16; x++) {
            if (x < 0 || x >= MAP_CELL_W) {
                continue;
            }
            CELL cell = XY_Cell(x, y);
            if (In_Radar(cell)) {
                (*this)[cell].Flip_Wall(gainhouse, losehouse);
            }
        }
    }
}

/***********************************************************************************************
 * MapClass::Scan_For_Overlay -- Looks along a straight line for a clear path to an overlay.   *
 *                                                                                             *
 * INPUT:   origin       -- The starting cell.                                                 *
 *                                                                                             *
 *          dir          -- The direction to search in.                                        *
 *                                                                                             *
 *          overlay      -- The overlay to search for.                                         *
 *                                                                                             *
 *          maxlength    -- The number of cells to search.                                     *
 *                                                                                             *
 *          limittohouse -- Only accept an overlay on a cell owned by whichhouse?              *
 *                                                                                             *
 *          whichhouse   -- The house for limittohouse.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the number of clear cells before the overlay, or -1 if not found.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *=============================================================================================*/
int MapClass::Scan_For_Overlay(CELL origin,
                               FacingType dir,
                               OverlayType overlay,
                               int maxlength,
                               bool limittohouse,
                               HousesType whichhouse) const
{
    if ((limittohouse && whichhouse == HOUSE_NONE) || !In_Radar(origin)) {
        return (-1);
    }

    CELL current = origin;
    for (int i = 0; i < maxlength; ++i) {
        current = Adjacent_Cell(current, dir);
        if (!In_Radar(current)) {
            break;
        }
        CellClass const& cell = (*this)[current];
        if (cell.Overlay == overlay) {
            return ((!limittohouse || whichhouse == cell.Owner) ? i : -1);
        }
        if (!cell.Is_Generally_Clear()) {
            break;
        }
    }
    return (-1);
}
