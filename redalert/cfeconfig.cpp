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

namespace {

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

static void Populate_CFE_Patch_Config_From_INI(const INIClass& ini)
{
    const SettingReader GetSetting = {ini};

    // Features
    GetSetting(ActiveCFEPatchConfig.EnableVersionAnnounce, "FEATURES", "VERSION_ANNOUNCE");
    GetSetting(ActiveCFEPatchConfig.EnableASPathing, "FEATURES", "ASTAR_PATHING");
    GetSetting(ActiveCFEPatchConfig.EnableOOMRepair, "FEATURES", "OOM_REPAIR");
    GetSetting(ActiveCFEPatchConfig.EnableRallyPoints, "FEATURES", "RALLY_POINTS");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyQueueJump, "FEATURES", "HARVY_QUEUE_JUMP");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyLoadBalancing, "FEATURES", "HARVY_LOAD_BALANCING");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyOptimize, "FEATURES", "HARVY_OPTIMIZE");
    GetSetting(ActiveCFEPatchConfig.EnableInstantCapture, "FEATURES", "INSTANT_CAPTURE");
    GetSetting(ActiveCFEPatchConfig.EnableAttackMove, "FEATURES", "ATTACK_MOVE");
    GetSetting(ActiveCFEPatchConfig.EnableChronoTankAttackMove, "FEATURES", "CHRONOTANK_ATTACK_MOVE");
    GetSetting(ActiveCFEPatchConfig.EnableQMoveLoops, "FEATURES", "QMOVE_LOOPS");
    GetSetting(ActiveCFEPatchConfig.EnableAirQMove, "FEATURES", "AIR_QMOVE");
    GetSetting(ActiveCFEPatchConfig.EnableAirQAMLRecall, "FEATURES", "AIR_QAML_RECALL");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterAircraft, "FEATURES", "SMARTER_AIRCRAFT");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterRepairBays, "FEATURES", "SMARTER_REPAIRBAY");
    GetSetting(ActiveCFEPatchConfig.BetterOreGrowth, "FEATURES", "BETTER_ORE_GROWTH");
    GetSetting(ActiveCFEPatchConfig.OreIndexBugfix, "FEATURES", "ORE_INDEX_BUGFIX");
    GetSetting(ActiveCFEPatchConfig.GemOverloadFix, "FEATURES", "GEM_OVERLOAD_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableInaccuracyBugfix, "FEATURES", "INACCURACY_BUGFIX");
    GetSetting(ActiveCFEPatchConfig.EnableWideSupers, "FEATURES", "OPENRA_WIDE_SUPERS");
    GetSetting(ActiveCFEPatchConfig.EnableQuickSupers, "FEATURES", "OPENRA_QUICK_SUPERS");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterChrono, "FEATURES", "SMARTER_CHRONO");
    GetSetting(ActiveCFEPatchConfig.EnableTimeQuakeFix, "FEATURES", "TIMEQUAKE_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableSharedChronoDisasterOdds, "FEATURES", "USE_SHARED_CHRONO_ODDS");
    GetSetting(ActiveCFEPatchConfig.EnableCumulativeTimelineDamage, "FEATURES", "CUMULATIVE_TIMELINE_DAMAGE");
    GetSetting(ActiveCFEPatchConfig.EnableLongGapFix, "FEATURES", "LONG_BGAP_BUGFIX");
    GetSetting(ActiveCFEPatchConfig.EnableMIGAmmoFudge, "FEATURES", "MIG_AMMO_FUDGE");
    GetSetting(ActiveCFEPatchConfig.EnableBuildingCaptureAnnouncements, "FEATURES", "BUILDING_CAPTURE_ANNOUNCE");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterSonar, "FEATURES", "SMARTER_SONAR");
    GetSetting(ActiveCFEPatchConfig.EnableBurningTreeFix, "FEATURES", "BURNING_TREE_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableDeathsDoorFix, "FEATURES", "DEATHS_DOOR_FIX");

    // Settings
    GetSetting(ActiveCFEPatchConfig.EnableVeterancy, "SETTINGS", "VETERANCY");
    GetSetting(ActiveCFEPatchConfig.EnableTanyaGuard, "SETTINGS", "TANYA_GUARD");
    GetSetting(ActiveCFEPatchConfig.SafeSabotage, "SETTINGS", "SAFE_SABOTAGE");
    GetSetting(ActiveCFEPatchConfig.HarvyQueueJumpCutoff, "SETTINGS", "QUEUE_JUMP_CUTOFF");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeUnloadWaitWeight, "SETTINGS", "HARVY_OPTIMIZE_UNLOAD_WAIT_WEIGHT");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingCutoff, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_CUTOFF");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingNumerator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_NUMERATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingDenominator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_DENOMINATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeCommunalismNumerator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_NUMERATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeCommunalismDenominator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_DENOMINATOR");
    GetSetting(ActiveCFEPatchConfig.OreGrowthScale, "SETTINGS", "ORE_GROWTH_SCALE");
    GetSetting(ActiveCFEPatchConfig.OreToGemPromotionSlowFactor, "SETTINGS", "ORE_TO_GEM_SLOW_FACTOR");
    GetSetting(ActiveCFEPatchConfig.WallBuildLength, "SETTINGS", "WALL_BUILD_LENGTH");
    GetSetting(ActiveCFEPatchConfig.BuildingGapOffset, "SETTINGS", "BUILDING_GAP_OFFSET");
    GetSetting(ActiveCFEPatchConfig.OreCapacityOffset, "SETTINGS", "ORE_CAPACITY_OFFSET");
    GetSetting(ActiveCFEPatchConfig.WallsAlwaysHaveFullCost, "SETTINGS", "WALLS_HAVE_FULL_COST");
    GetSetting(ActiveCFEPatchConfig.InverseBuildingPreviewMode, "SETTINGS", "INVERSE_PREVIEW_MODE");
    GetSetting(ActiveCFEPatchConfig.SharedChronoDisasterOdds, "SETTINGS", "SHARED_CHRONO_ODDS");
    GetSetting(ActiveCFEPatchConfig.TurkeyFixMode, "SETTINGS", "TURKEY_FIX_MODE");

    CFEPatchConfig& cfg = ActiveCFEPatchConfig;

    // Clamp wall length between 1 (build at all) and 10 (buffer limits)
    cfg.WallBuildLength = std::max(1, std::min(cfg.WallBuildLength, 10));

    // Clamp mode settings to their legal ranges
    cfg.InverseBuildingPreviewMode = std::max(0, std::min(cfg.InverseBuildingPreviewMode, 2));
    cfg.SafeSabotage = std::max(0, std::min(cfg.SafeSabotage, 2));

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

void Initialise_CFE_Patch_Config()
{
    INIClass ini;
    CCFileClass file(CFE_CONFIG_FILE_NAME);
    if (file.Is_Available()) {
        ini.Load(file);
    }
    Populate_CFE_Patch_Config_From_INI(ini);
}
