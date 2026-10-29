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
** Loads CFE_CONFIG_FILE_NAME, ported from CFE's TIBERIANDAWN/CFEUTIL.CPP.
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
    GetSetting(ActiveCFEPatchConfig.EnableRallyPoints, "FEATURES", "RALLY_POINTS");
    GetSetting(ActiveCFEPatchConfig.EnableASPathing, "FEATURES", "ASTAR_PATHING");
    GetSetting(ActiveCFEPatchConfig.EnableOOMRepair, "FEATURES", "OOM_REPAIR");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyQueueJump, "FEATURES", "HARVY_QUEUE_JUMP");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyLoadBalancing, "FEATURES", "HARVY_LOAD_BALANCING");
    GetSetting(ActiveCFEPatchConfig.EnableHarvyOptimize, "FEATURES", "HARVY_OPTIMIZE");
    GetSetting(ActiveCFEPatchConfig.EnableHarvySelfRepair, "FEATURES", "HARVY_SELF_REPAIR");
    GetSetting(ActiveCFEPatchConfig.DisableCommandoAirstrikes, "FEATURES", "DISABLE_COMMANDO_AIRSTRIKES");
    GetSetting(ActiveCFEPatchConfig.EnableAttackMove, "FEATURES", "ATTACK_MOVE");
    GetSetting(ActiveCFEPatchConfig.EnableQMove, "FEATURES", "Q_MOVE");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterAircraft, "FEATURES", "SMARTER_AIRCRAFT");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterSAMs, "FEATURES", "SMARTER_SAMS");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterRepairBays, "FEATURES", "SMARTER_REPAIRBAY");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterMammoths, "FEATURES", "SMARTER_MAMMOTHS");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterHarvesters, "FEATURES", "SMARTER_HARVESTERS");
    GetSetting(ActiveCFEPatchConfig.EnableSmarterRefineries, "FEATURES", "SMARTER_REFINERIES");
    GetSetting(ActiveCFEPatchConfig.SmarterCPUCommandos, "FEATURES", "SMARTER_CPU_COMMANDOS");
    GetSetting(ActiveCFEPatchConfig.EnableGunboatBugfix, "FEATURES", "GUNBOAT_BUGFIX");
    GetSetting(ActiveCFEPatchConfig.BetterTibGrowth, "FEATURES", "BETTER_TIB_GROWTH");
    GetSetting(ActiveCFEPatchConfig.TibIndexBugfix, "FEATURES", "TIB_INDEX_BUGFIX");
    GetSetting(ActiveCFEPatchConfig.TibResurgence, "FEATURES", "TIB_RESURGENCE");
    GetSetting(ActiveCFEPatchConfig.TreeVisceroidSpawnAnyway, "FEATURES", "TREE_VISCEROID_SPAWN_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.TSVisceroidSpawn, "FEATURES", "TS_VISCEROID_SPAWN");
    GetSetting(ActiveCFEPatchConfig.EnableNukeTank, "FEATURES", "NUKE_TANK");
    GetSetting(ActiveCFEPatchConfig.DisableMPNeutralAttacks, "FEATURES", "DISABLE_MP_NEUTRAL_ATTACKS");
    GetSetting(ActiveCFEPatchConfig.EnableGDI9Fix, "FEATURES", "GDI9_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableBurningTreeFix, "FEATURES", "BURNING_TREE_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableNukeRadiusFix, "FEATURES", "NUKE_RADIUS_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableDeathsDoorFix, "FEATURES", "DEATHS_DOOR_FIX");
    GetSetting(ActiveCFEPatchConfig.EnableSellChanceFix, "FEATURES", "SELL_CHANCE_FIX");
    GetSetting(ActiveCFEPatchConfig.ForceLoanerCopters, "FEATURES", "LOANER_COPTER_DEFAULT");
    GetSetting(ActiveCFEPatchConfig.AIvsWalls, "FEATURES", "AI_VS_WALLS");
    GetSetting(ActiveCFEPatchConfig.EnableWallCrush, "FEATURES", "WALL_CRUSH");
    GetSetting(ActiveCFEPatchConfig.EnableAdvancedWallOwnership, "FEATURES", "ADV_WALL_OWN");
    GetSetting(ActiveCFEPatchConfig.EnableHuntFix, "FEATURES", "HUNT_FIX");
    GetSetting(ActiveCFEPatchConfig.MCVAlwaysSpawnEngineer, "FEATURES", "MCV_ALWAYS_SPAWN_ENGINEER");
    GetSetting(ActiveCFEPatchConfig.SplitDualCapturables, "FEATURES", "SPLIT_DUAL_CAPTURABLES");
    GetSetting(ActiveCFEPatchConfig.BuildForbiddenSPCapturables, "FEATURES", "BUILD_FORBIDDEN_SP_CAPTURABLES");
    GetSetting(ActiveCFEPatchConfig.EnableDistanceCalcFix, "FEATURES", "DISTANCE_CALC_FIX");
    GetSetting(ActiveCFEPatchConfig.ShroudRemover, "FEATURES", "SHROUD_REMOVER");
    GetSetting(ActiveCFEPatchConfig.DeadBuildingScatterFix, "FEATURES", "DEAD_BUILDING_SCATTER_FIX");

    // Settings
    GetSetting(ActiveCFEPatchConfig.DefaultCustomMissionScenarioNumber, "SETTINGS", "CUSTOM_MISSION_DEFAULT_SCENARIO");
    GetSetting(ActiveCFEPatchConfig.LegacyRenderDisableMode, "SETTINGS", "LEGACY_RENDER_DISABLE_MODE");
    GetSetting(ActiveCFEPatchConfig.EnableVeterancy, "SETTINGS", "VETERANCY");
    GetSetting(ActiveCFEPatchConfig.EnableCommandoGuard, "SETTINGS", "COMMANDO_GUARD");
    GetSetting(ActiveCFEPatchConfig.SafeSabotage, "SETTINGS", "SAFE_SABOTAGE");
    GetSetting(ActiveCFEPatchConfig.InfantryTiberiumAversion, "SETTINGS", "INFANTRY_TIBERIUM_AVERSION");
    GetSetting(ActiveCFEPatchConfig.BuildingGap, "SETTINGS", "BUILDING_GAP");
    GetSetting(ActiveCFEPatchConfig.HarvyQueueJumpCutoff, "SETTINGS", "QUEUE_JUMP_CUTOFF");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeUnloadWaitWeight, "SETTINGS", "HARVY_OPTIMIZE_UNLOAD_WAIT_WEIGHT");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingCutoff, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_CUTOFF");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingNumerator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_NUMERATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeThrashingDenominator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_DENOMINATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeCommunalismNumerator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_NUMERATOR");
    GetSetting(ActiveCFEPatchConfig.HarvyOptimizeCommunalismDenominator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_DENOMINATOR");
    GetSetting(ActiveCFEPatchConfig.TibGrowthScale, "SETTINGS", "TIB_GROWTH_SCALE");
    GetSetting(ActiveCFEPatchConfig.TibResurgeDelay, "SETTINGS", "TIB_RESURGE_DELAY");
    GetSetting(ActiveCFEPatchConfig.TreeVisceroidSpawnMode, "SETTINGS", "TREE_VISCEROID_SPAWN_MODE");
    GetSetting(ActiveCFEPatchConfig.MeanerVisceroids, "SETTINGS", "MEANER_VISCEROIDS");
    GetSetting(ActiveCFEPatchConfig.WallBuildLength, "SETTINGS", "WALL_BUILD_LENGTH");
    GetSetting(ActiveCFEPatchConfig.WallsAlwaysHaveFullCost, "SETTINGS", "WALLS_HAVE_FULL_COST");
    GetSetting(ActiveCFEPatchConfig.InverseBuildingPreviewMode, "SETTINGS", "INVERSE_PREVIEW_MODE");
    GetSetting(ActiveCFEPatchConfig.SmarterSAMAmmo, "SETTINGS", "SMARTER_SAM_AMMO");
    GetSetting(ActiveCFEPatchConfig.SmarterSAMBalanceDelay, "SETTINGS", "SMARTER_SAM_BALANCE_DELAY");
    GetSetting(ActiveCFEPatchConfig.MPPreplacedUnitOrderMode, "SETTINGS", "MP_PREPLACED_UNIT_ORDER_MODE");
    GetSetting(ActiveCFEPatchConfig.BuildingDeathAnnounceMode, "SETTINGS", "BUILDING_DEATH_ANNOUNCE_MODE");
    GetSetting(ActiveCFEPatchConfig.QuieterAirstrips, "SETTINGS", "QUIETER_AIRSTRIPS");
    GetSetting(ActiveCFEPatchConfig.SearchRangeFixMode, "SETTINGS", "SEARCH_RANGE_FIX");

    // Balance
    GetSetting(ActiveCFEPatchConfig.OverrideAPCReqs, "BALANCE", "DO_OVERRIDE_APC_NOREQ");
    GetSetting(ActiveCFEPatchConfig.OverrideWarFactoryHP, "BALANCE", "DO_OVERRIDE_WFACTORY_HP");
    GetSetting(ActiveCFEPatchConfig.OverrideWarFactoryHP_Value, "BALANCE", "WFACTORY_HP_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideStealthTankCost, "BALANCE", "DO_OVERRIDE_STEALTHTANK_COST");
    GetSetting(ActiveCFEPatchConfig.OverrideStealthTankCost_Value, "BALANCE", "STEALTHTANK_COST_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideArtyTurn, "BALANCE", "DO_OVERRIDE_ARTY_TURN");
    GetSetting(ActiveCFEPatchConfig.OverrideArtyTurn_Value, "BALANCE", "ARTY_TURN_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideMLRSReqs, "BALANCE", "DO_MLRS_REQ_RADAR");
    GetSetting(ActiveCFEPatchConfig.OverrideMLRSHP, "BALANCE", "DO_OVERRIDE_MLRS_HP");
    GetSetting(ActiveCFEPatchConfig.OverrideMLRSHP_Value, "BALANCE", "MLRS_HP_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideMLRSGDIOnly, "BALANCE", "DO_MLRS_GDI_ONLY");
    GetSetting(ActiveCFEPatchConfig.OverrideMCVSight, "BALANCE", "DO_OVERRIDE_MCV_SIGHT");
    GetSetting(ActiveCFEPatchConfig.OverrideMCVSight_Value, "BALANCE", "MCV_SIGHT_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideConYardSight_Value, "BALANCE", "CONYARD_SIGHT_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideChemGuyBuff, "BALANCE", "DO_CHEM_GUY_BUFF");
    GetSetting(ActiveCFEPatchConfig.OverrideRocketGuyHP, "BALANCE", "DO_OVERRIDE_ROCKEYGUY_HP");
    GetSetting(ActiveCFEPatchConfig.OverrideRocketGuyHP_Value, "BALANCE", "ROCKEYGUY_HP_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideRocketGuyCost, "BALANCE", "DO_OVERRIDE_ROCKEYGUY_COST");
    GetSetting(ActiveCFEPatchConfig.OverrideRocketGuyCost_Value, "BALANCE", "ROCKEYGUY_COST_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.OverrideMinigunnerRange, "BALANCE", "DO_OVERRIDE_MINIGUNNER_RANGE");
    GetSetting(ActiveCFEPatchConfig.OverrideMinigunnerRange_Value, "BALANCE", "MINIGUNNER_RANGE_OVERRIDE");
    GetSetting(ActiveCFEPatchConfig.ApplyMPBalanceToSP, "BALANCE", "APPLY_BALANCE_TO_SINGLE_PLAYER");
    GetSetting(ActiveCFEPatchConfig.MPUnitsInSP, "BALANCE", "MULTIPLAYER_UNITS_IN_SINGLE_PLAYER");
    GetSetting(ActiveCFEPatchConfig.JDExtraAdjustments, "BALANCE", "JD_EXTRA_ADJUSTMENTS");
    GetSetting(ActiveCFEPatchConfig.MPCivSight, "BALANCE", "MP_CIV_SIGHT");

    CFEPatchConfig& cfg = ActiveCFEPatchConfig;

    // Clamp wall length between 1 (build at all) and 10 (buffer limits)
    cfg.WallBuildLength = std::max(1, std::min(cfg.WallBuildLength, 10));

    // Clamp mode settings to their legal ranges
    cfg.InverseBuildingPreviewMode = std::max(0, std::min(cfg.InverseBuildingPreviewMode, 2));
    cfg.MeanerVisceroids = std::max(0, std::min(cfg.MeanerVisceroids, 3));
    cfg.SafeSabotage = std::max(0, std::min(cfg.SafeSabotage, 2));
    cfg.MPPreplacedUnitOrderMode = std::max(0, std::min(cfg.MPPreplacedUnitOrderMode, 2));
    cfg.BuildingDeathAnnounceMode = std::max(0, std::min(cfg.BuildingDeathAnnounceMode, 4));
    cfg.QuieterAirstrips = std::max(0, std::min(cfg.QuieterAirstrips, 3));
    cfg.LegacyRenderDisableMode = std::max(0, std::min(cfg.LegacyRenderDisableMode, 2));
    cfg.SearchRangeFixMode = std::max(0, std::min(cfg.SearchRangeFixMode, 2));

    // If A-Star isn't enabled, distance constants will be compared against leptons rather than cells, so convert here.
    // The A-Star harvester distances are not ported yet, so the harvester code always compares leptons for now.
    {
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
    if (cfg.TibGrowthScale < 0) {
        cfg.TibGrowthScale = 0;
    }

    // SAMs can't have less than 1 ammo
    if (cfg.SmarterSAMAmmo < 1) {
        cfg.SmarterSAMAmmo = 1;
    }

    // Overall SAM delay can't be less than 1 tick
    if (cfg.SmarterSAMBalanceDelay + (TICKS_PER_SECOND * 3) < 1) {
        cfg.SmarterSAMBalanceDelay = (-1 * (TICKS_PER_SECOND * 3)) + 1;
    }

    // Scenario can't be less than 1
    if (cfg.DefaultCustomMissionScenarioNumber < 1) {
        cfg.DefaultCustomMissionScenarioNumber = 1;
    }

    // Compute complex weights here so they only have to be computed once
    cfg.HarvyOptimizeThrashingWeight = (cfg.HarvyOptimizeUnloadWaitWeight * cfg.HarvyOptimizeThrashingNumerator)
                                       / cfg.HarvyOptimizeThrashingDenominator;
    cfg.HarvyOptimizeCommunalismWeight = (cfg.HarvyOptimizeUnloadWaitWeight * cfg.HarvyOptimizeCommunalismNumerator)
                                         / cfg.HarvyOptimizeCommunalismDenominator;

    // Only allow building off walls if wall build distance isn't longer than building gap
    cfg.AllowSandbagging = cfg.WallBuildLength <= cfg.BuildingGap;

    // Clamp balance patch values above 0
    cfg.OverrideWarFactoryHP_Value = std::max(1, cfg.OverrideWarFactoryHP_Value);
    cfg.OverrideStealthTankCost_Value = std::max(1, cfg.OverrideStealthTankCost_Value);
    cfg.OverrideArtyTurn_Value = std::max(1, cfg.OverrideArtyTurn_Value);
    cfg.OverrideMLRSHP_Value = std::max(1, cfg.OverrideMLRSHP_Value);
    cfg.OverrideMCVSight_Value = std::max(1, cfg.OverrideMCVSight_Value);
    cfg.OverrideConYardSight_Value = std::max(1, cfg.OverrideConYardSight_Value);
    cfg.OverrideRocketGuyHP_Value = std::max(1, cfg.OverrideRocketGuyHP_Value);
    cfg.OverrideRocketGuyCost_Value = std::max(1, cfg.OverrideRocketGuyCost_Value);
    cfg.OverrideMinigunnerRange_Value = std::max(1, cfg.OverrideMinigunnerRange_Value);

    // JDExtraAdjustments should use JD's desired stealth tank price override
    if (cfg.JDExtraAdjustments) {
        cfg.OverrideStealthTankCost = true;
        cfg.OverrideStealthTankCost_Value = 800;
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
