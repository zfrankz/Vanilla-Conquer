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
** Loads CFE_CONFIG_FILE_NAME, ported from CFE's REDALERT/CFEUTIL.CPP.
** Unlike CFE, no default INI is loaded and the user's file is never rewritten:
** a missing file or entry leaves the CFEPatchConfig default (the unmodified game).
*/
#include "function.h"
#include "cfeconfig.h"

#include <algorithm>
#include <limits>

CFEPatchConfig ActiveCFEPatchConfig;

namespace
{

    // Sets target only when the entry is present, so absent entries keep their default.
    struct SettingReader
    {
        const INIClass& ini;

        template <typename T> void operator()(T& target, const char* section, const char* entry) const
        {
            const int value = ini.Get_Int(section, entry, InvalidSetting);
            if (value != InvalidSetting)
                target = value;
        }

        static const int InvalidSetting;
    };

    const int SettingReader::InvalidSetting = std::numeric_limits<int>::min();

} // namespace

// Every entry of the file goes through here, so the reader and the hash can't disagree.
template <typename Visitor> static void Visit_CFE_Settings(CFEPatchConfig& cfg, const Visitor& GetSetting)
{
    // Features
    GetSetting(cfg.EnableVersionAnnounce, "FEATURES", "VERSION_ANNOUNCE");
    GetSetting(cfg.EnableASPathing, "FEATURES", "ASTAR_PATHING");
    GetSetting(cfg.EnableCellReservationFix, "FEATURES", "CELL_RESERVATION_FIX");
    GetSetting(cfg.EnableOOMRepair, "FEATURES", "OOM_REPAIR");
    GetSetting(cfg.EnableRallyPoints, "FEATURES", "RALLY_POINTS");
    GetSetting(cfg.EnableHarvyQueueJump, "FEATURES", "HARVY_QUEUE_JUMP");
    GetSetting(cfg.EnableHarvyLoadBalancing, "FEATURES", "HARVY_LOAD_BALANCING");
    GetSetting(cfg.EnableHarvyOptimize, "FEATURES", "HARVY_OPTIMIZE");
    GetSetting(cfg.EnableInstantCapture, "FEATURES", "INSTANT_CAPTURE");
    GetSetting(cfg.EnableAttackMove, "FEATURES", "ATTACK_MOVE");
    GetSetting(cfg.EnableChronoTankAttackMove, "FEATURES", "CHRONOTANK_ATTACK_MOVE");
    GetSetting(cfg.EnableQMoveLoops, "FEATURES", "QMOVE_LOOPS");
    GetSetting(cfg.EnableAirQMove, "FEATURES", "AIR_QMOVE");
    GetSetting(cfg.EnableAirQAMLRecall, "FEATURES", "AIR_QAML_RECALL");
    GetSetting(cfg.EnableSmarterAircraft, "FEATURES", "SMARTER_AIRCRAFT");
    GetSetting(cfg.EnableSmarterRepairBays, "FEATURES", "SMARTER_REPAIRBAY");
    GetSetting(cfg.BetterOreGrowth, "FEATURES", "BETTER_ORE_GROWTH");
    GetSetting(cfg.OreIndexBugfix, "FEATURES", "ORE_INDEX_BUGFIX");
    GetSetting(cfg.GemOverloadFix, "FEATURES", "GEM_OVERLOAD_FIX");
    GetSetting(cfg.EnableInaccuracyBugfix, "FEATURES", "INACCURACY_BUGFIX");
    GetSetting(cfg.EnableWideSupers, "FEATURES", "OPENRA_WIDE_SUPERS");
    GetSetting(cfg.EnableQuickSupers, "FEATURES", "OPENRA_QUICK_SUPERS");
    GetSetting(cfg.EnableSmarterChrono, "FEATURES", "SMARTER_CHRONO");
    GetSetting(cfg.EnableTimeQuakeFix, "FEATURES", "TIMEQUAKE_FIX");
    GetSetting(cfg.EnableSharedChronoDisasterOdds, "FEATURES", "USE_SHARED_CHRONO_ODDS");
    GetSetting(cfg.EnableCumulativeTimelineDamage, "FEATURES", "CUMULATIVE_TIMELINE_DAMAGE");
    GetSetting(cfg.EnableLongGapFix, "FEATURES", "LONG_BGAP_BUGFIX");
    GetSetting(cfg.EnableMIGAmmoFudge, "FEATURES", "MIG_AMMO_FUDGE");
    GetSetting(cfg.EnableBuildingCaptureAnnouncements, "FEATURES", "BUILDING_CAPTURE_ANNOUNCE");
    GetSetting(cfg.EnableSmarterSonar, "FEATURES", "SMARTER_SONAR");
    GetSetting(cfg.EnableBurningTreeFix, "FEATURES", "BURNING_TREE_FIX");
    GetSetting(cfg.EnableDeathsDoorFix, "FEATURES", "DEATHS_DOOR_FIX");
    GetSetting(cfg.EnableGeneralBugfixes, "FEATURES", "GENERAL_BUGFIXES");

    // Settings
    GetSetting(cfg.EnableVeterancy, "SETTINGS", "VETERANCY");
    GetSetting(cfg.EnableTanyaGuard, "SETTINGS", "TANYA_GUARD");
    GetSetting(cfg.SafeSabotage, "SETTINGS", "SAFE_SABOTAGE");
    GetSetting(cfg.HarvyQueueJumpCutoff, "SETTINGS", "QUEUE_JUMP_CUTOFF");
    GetSetting(cfg.HarvyOptimizeUnloadWaitWeight, "SETTINGS", "HARVY_OPTIMIZE_UNLOAD_WAIT_WEIGHT");
    GetSetting(cfg.HarvyOptimizeThrashingCutoff, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_CUTOFF");
    GetSetting(cfg.HarvyOptimizeThrashingNumerator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_NUMERATOR");
    GetSetting(cfg.HarvyOptimizeThrashingDenominator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_DENOMINATOR");
    GetSetting(cfg.HarvyOptimizeCommunalismNumerator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_NUMERATOR");
    GetSetting(cfg.HarvyOptimizeCommunalismDenominator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_DENOMINATOR");
    GetSetting(cfg.OreGrowthScale, "SETTINGS", "ORE_GROWTH_SCALE");
    GetSetting(cfg.OreToGemPromotionSlowFactor, "SETTINGS", "ORE_TO_GEM_SLOW_FACTOR");
    GetSetting(cfg.WallBuildLength, "SETTINGS", "WALL_BUILD_LENGTH");
    GetSetting(cfg.BuildingGapOffset, "SETTINGS", "BUILDING_GAP_OFFSET");
    GetSetting(cfg.OreCapacityOffset, "SETTINGS", "ORE_CAPACITY_OFFSET");
    GetSetting(cfg.WallsAlwaysHaveFullCost, "SETTINGS", "WALLS_HAVE_FULL_COST");
    GetSetting(cfg.InverseBuildingPreviewMode, "SETTINGS", "INVERSE_PREVIEW_MODE");
    GetSetting(cfg.SharedChronoDisasterOdds, "SETTINGS", "SHARED_CHRONO_ODDS");
    GetSetting(cfg.TurkeyFixMode, "SETTINGS", "TURKEY_FIX_MODE");
}

static void Normalize_CFE_Settings(CFEPatchConfig& cfg)
{
    // Clamp wall length between 1 (build at all) and 10 (buffer limits)
    cfg.WallBuildLength = std::max(1, std::min(cfg.WallBuildLength, 10));

    // Clamp mode settings to their legal ranges
    cfg.InverseBuildingPreviewMode = std::max(0, std::min(cfg.InverseBuildingPreviewMode, 2));
    cfg.SafeSabotage = std::max(0, std::min(cfg.SafeSabotage, 2));
    cfg.TurkeyFixMode = std::max(0, std::min(cfg.TurkeyFixMode, 2));

    // If A-Star isn't enabled, distance constants will be compared against leptons rather than cells, so convert here
    if (!cfg.EnableASPathing) {
        cfg.HarvyQueueJumpCutoff = Cell_To_Lepton(cfg.HarvyQueueJumpCutoff);
        cfg.HarvyOptimizeUnloadWaitWeight = Cell_To_Lepton(cfg.HarvyOptimizeUnloadWaitWeight);
        cfg.HarvyOptimizeThrashingCutoff = Cell_To_Lepton(cfg.HarvyOptimizeThrashingCutoff);
    }

    // Denominators can't be zero
    if (cfg.HarvyOptimizeThrashingDenominator == 0) {
        cfg.HarvyOptimizeThrashingDenominator = 1;
    }
    if (cfg.HarvyOptimizeCommunalismDenominator == 0) {
        cfg.HarvyOptimizeCommunalismDenominator = 1;
    }

    // Growth can't be negative
    if (cfg.OreGrowthScale < 0) {
        cfg.OreGrowthScale = 0;
    }

    // Don't multiply by 0
    if (cfg.OreToGemPromotionSlowFactor < 1) {
        cfg.OreToGemPromotionSlowFactor = 1;
    }

    // Compute complex weights here so they only have to be computed once
    cfg.HarvyOptimizeThrashingWeight = (cfg.HarvyOptimizeUnloadWaitWeight * cfg.HarvyOptimizeThrashingNumerator)
                                       / cfg.HarvyOptimizeThrashingDenominator;
    cfg.HarvyOptimizeCommunalismWeight = (cfg.HarvyOptimizeUnloadWaitWeight * cfg.HarvyOptimizeCommunalismNumerator)
                                         / cfg.HarvyOptimizeCommunalismDenominator;

    // Can't share chrono disaster odds if timequakes are disabled
    if (!cfg.EnableTimeQuakeFix) {
        cfg.SharedChronoDisasterOdds = false;
    }

    // Odds can't be less than 0; greater than 100% is allowed to counteract odds scaling
    if (cfg.SharedChronoDisasterOdds < 0) {
        cfg.SharedChronoDisasterOdds = 0;
    }
}

static void Populate_CFE_Patch_Config_From_INI(const INIClass& ini)
{
    const SettingReader reader = {ini};
    Visit_CFE_Settings(ActiveCFEPatchConfig, reader);
    Normalize_CFE_Settings(ActiveCFEPatchConfig);
}

namespace
{

    // FNV-1a over the settings that change the simulation. These ones only change what the local
    // player sees or hears, so peers may differ on them.
    struct SettingHasher
    {
        unsigned* hash;

        template <typename T> void operator()(T& target, const char* section, const char* entry) const
        {
            static const char* const local[] = {
                "VERSION_ANNOUNCE",
                "BUILDING_CAPTURE_ANNOUNCE",
                "INVERSE_PREVIEW_MODE",
            };
            for (unsigned i = 0; i < ARRAY_SIZE(local); i++) {
                if (strcmp(entry, local[i]) == 0) {
                    return;
                }
            }
            const int value = (int)target;
            for (int shift = 0; shift < 32; shift += 8) {
                *hash = (*hash ^ ((unsigned)value >> shift & 0xFF)) * 16777619u;
            }
        }
    };

} // namespace

static unsigned Hash_CFE_Settings(CFEPatchConfig& cfg)
{
    unsigned hash = 2166136261u;
    const SettingHasher hasher = {&hash};
    Visit_CFE_Settings(cfg, hasher);
    return hash;
}

/*
**	Peers must agree on these or the game desyncs, so joining compares them. The unmodified game
**	gives 0, which keeps it compatible with a Vanilla Conquer that doesn't have this file.
*/
unsigned CFE_Patch_Config_Hash()
{
    CFEPatchConfig defaults;
    Normalize_CFE_Settings(defaults);
    const unsigned hash = Hash_CFE_Settings(ActiveCFEPatchConfig);
    return hash == Hash_CFE_Settings(defaults) ? 0 : (hash | 1);
}

void Initialise_CFE_Patch_Config()
{
    INIClass ini;
    CCFileClass file(CFE_CONFIG_FILE_NAME);
    if (file.Is_Available()) {
        // Read_Line drops a last line with no line end, and people edit this file by hand.
        const int size = file.Size();
        char* text = new char[size + 1];
        const int length = file.Read(text, size);
        text[length] = '\n';
        BufferStraw straw(text, length + 1);
        ini.Load(straw);
        delete[] text;
    }
    Populate_CFE_Patch_Config_From_INI(ini);
}

/*
**	Random_Pick is capped at 32767, but the ore growth odds can go well beyond that. This uses the
**	synchronized random number generator, like Random_Pick.
*/
int CFE_Patch_FullRange_Random_Pick(int min, int max)
{
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    min = std::max(min, 0);
    max = std::max(max, 0);
    if (max == 0) {
        return 0;
    }
    int adjustedmax = (max - min) + 1;
    int ceiling = (INT_MAX / adjustedmax) * adjustedmax;
    int fullrangerandom = 0;
    do {
        int random_one = Scen.RandomNumber & 0x7fff; // This is a call: RandomClass has an int conversion operator.
        int random_two = Scen.RandomNumber & 0x7fff;
        fullrangerandom = (random_one << 16) | random_two;
        if (random_one > random_two) {
            fullrangerandom |= 0x8000;
        }
    } while (fullrangerandom > ceiling);
    return (fullrangerandom % adjustedmax) + min;
}
