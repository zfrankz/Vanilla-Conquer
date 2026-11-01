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
    GetSetting(cfg.EnableRallyPoints, "FEATURES", "RALLY_POINTS");
    GetSetting(cfg.EnableASPathing, "FEATURES", "ASTAR_PATHING");
    GetSetting(cfg.EnableCellReservationFix, "FEATURES", "CELL_RESERVATION_FIX");
    GetSetting(cfg.EnableOOMRepair, "FEATURES", "OOM_REPAIR");
    GetSetting(cfg.EnableHarvyQueueJump, "FEATURES", "HARVY_QUEUE_JUMP");
    GetSetting(cfg.EnableHarvyLoadBalancing, "FEATURES", "HARVY_LOAD_BALANCING");
    GetSetting(cfg.EnableHarvyOptimize, "FEATURES", "HARVY_OPTIMIZE");
    GetSetting(cfg.EnableHarvySelfRepair, "FEATURES", "HARVY_SELF_REPAIR");
    GetSetting(cfg.DisableCommandoAirstrikes, "FEATURES", "DISABLE_COMMANDO_AIRSTRIKES");
    GetSetting(cfg.EnableAttackMove, "FEATURES", "ATTACK_MOVE");
    GetSetting(cfg.EnableQMove, "FEATURES", "Q_MOVE");
    GetSetting(cfg.EnableSmarterAircraft, "FEATURES", "SMARTER_AIRCRAFT");
    GetSetting(cfg.EnableSmarterSAMs, "FEATURES", "SMARTER_SAMS");
    GetSetting(cfg.EnableSmarterRepairBays, "FEATURES", "SMARTER_REPAIRBAY");
    GetSetting(cfg.EnableSmarterMammoths, "FEATURES", "SMARTER_MAMMOTHS");
    GetSetting(cfg.EnableSmarterHarvesters, "FEATURES", "SMARTER_HARVESTERS");
    GetSetting(cfg.EnableSmarterRefineries, "FEATURES", "SMARTER_REFINERIES");
    GetSetting(cfg.SmarterCPUCommandos, "FEATURES", "SMARTER_CPU_COMMANDOS");
    GetSetting(cfg.EnableGunboatBugfix, "FEATURES", "GUNBOAT_BUGFIX");
    GetSetting(cfg.BetterTibGrowth, "FEATURES", "BETTER_TIB_GROWTH");
    GetSetting(cfg.TibIndexBugfix, "FEATURES", "TIB_INDEX_BUGFIX");
    GetSetting(cfg.TibResurgence, "FEATURES", "TIB_RESURGENCE");
    GetSetting(cfg.TreeVisceroidSpawnAnyway, "FEATURES", "TREE_VISCEROID_SPAWN_OVERRIDE");
    GetSetting(cfg.TSVisceroidSpawn, "FEATURES", "TS_VISCEROID_SPAWN");
    GetSetting(cfg.EnableNukeTank, "FEATURES", "NUKE_TANK");
    GetSetting(cfg.DisableMPNeutralAttacks, "FEATURES", "DISABLE_MP_NEUTRAL_ATTACKS");
    GetSetting(cfg.EnableGDI9Fix, "FEATURES", "GDI9_FIX");
    GetSetting(cfg.EnableBurningTreeFix, "FEATURES", "BURNING_TREE_FIX");
    GetSetting(cfg.EnableNukeRadiusFix, "FEATURES", "NUKE_RADIUS_FIX");
    GetSetting(cfg.EnableDeathsDoorFix, "FEATURES", "DEATHS_DOOR_FIX");
    GetSetting(cfg.EnableSellChanceFix, "FEATURES", "SELL_CHANCE_FIX");
    GetSetting(cfg.ForceLoanerCopters, "FEATURES", "LOANER_COPTER_DEFAULT");
    GetSetting(cfg.AIvsWalls, "FEATURES", "AI_VS_WALLS");
    GetSetting(cfg.EnableWallCrush, "FEATURES", "WALL_CRUSH");
    GetSetting(cfg.EnableAdvancedWallOwnership, "FEATURES", "ADV_WALL_OWN");
    GetSetting(cfg.EnableHuntFix, "FEATURES", "HUNT_FIX");
    GetSetting(cfg.MCVAlwaysSpawnEngineer, "FEATURES", "MCV_ALWAYS_SPAWN_ENGINEER");
    GetSetting(cfg.SplitDualCapturables, "FEATURES", "SPLIT_DUAL_CAPTURABLES");
    GetSetting(cfg.BuildForbiddenSPCapturables, "FEATURES", "BUILD_FORBIDDEN_SP_CAPTURABLES");
    GetSetting(cfg.EnableDistanceCalcFix, "FEATURES", "DISTANCE_CALC_FIX");
    GetSetting(cfg.ShroudRemover, "FEATURES", "SHROUD_REMOVER");
    GetSetting(cfg.EnableGeneralBugfixes, "FEATURES", "GENERAL_BUGFIXES");
    GetSetting(cfg.DeadBuildingScatterFix, "FEATURES", "DEAD_BUILDING_SCATTER_FIX");

    // Settings
    GetSetting(cfg.DefaultCustomMissionScenarioNumber, "SETTINGS", "CUSTOM_MISSION_DEFAULT_SCENARIO");
    GetSetting(cfg.LegacyRenderDisableMode, "SETTINGS", "LEGACY_RENDER_DISABLE_MODE");
    GetSetting(cfg.EnableVeterancy, "SETTINGS", "VETERANCY");
    GetSetting(cfg.EnableCommandoGuard, "SETTINGS", "COMMANDO_GUARD");
    GetSetting(cfg.SafeSabotage, "SETTINGS", "SAFE_SABOTAGE");
    GetSetting(cfg.InfantryTiberiumAversion, "SETTINGS", "INFANTRY_TIBERIUM_AVERSION");
    GetSetting(cfg.BuildingGap, "SETTINGS", "BUILDING_GAP");
    GetSetting(cfg.HarvyQueueJumpCutoff, "SETTINGS", "QUEUE_JUMP_CUTOFF");
    GetSetting(cfg.HarvyOptimizeUnloadWaitWeight, "SETTINGS", "HARVY_OPTIMIZE_UNLOAD_WAIT_WEIGHT");
    GetSetting(cfg.HarvyOptimizeThrashingCutoff, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_CUTOFF");
    GetSetting(cfg.HarvyOptimizeThrashingNumerator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_NUMERATOR");
    GetSetting(cfg.HarvyOptimizeThrashingDenominator, "SETTINGS", "HARVY_OPTIMIZE_THRASHING_DENOMINATOR");
    GetSetting(cfg.HarvyOptimizeCommunalismNumerator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_NUMERATOR");
    GetSetting(cfg.HarvyOptimizeCommunalismDenominator, "SETTINGS", "HARVY_OPTIMIZE_COMMUNALISM_DENOMINATOR");
    GetSetting(cfg.TibGrowthScale, "SETTINGS", "TIB_GROWTH_SCALE");
    GetSetting(cfg.TibResurgeDelay, "SETTINGS", "TIB_RESURGE_DELAY");
    GetSetting(cfg.TreeVisceroidSpawnMode, "SETTINGS", "TREE_VISCEROID_SPAWN_MODE");
    GetSetting(cfg.MeanerVisceroids, "SETTINGS", "MEANER_VISCEROIDS");
    GetSetting(cfg.WallBuildLength, "SETTINGS", "WALL_BUILD_LENGTH");
    GetSetting(cfg.WallsAlwaysHaveFullCost, "SETTINGS", "WALLS_HAVE_FULL_COST");
    GetSetting(cfg.InverseBuildingPreviewMode, "SETTINGS", "INVERSE_PREVIEW_MODE");
    GetSetting(cfg.SmarterSAMAmmo, "SETTINGS", "SMARTER_SAM_AMMO");
    GetSetting(cfg.SmarterSAMBalanceDelay, "SETTINGS", "SMARTER_SAM_BALANCE_DELAY");
    GetSetting(cfg.MPPreplacedUnitOrderMode, "SETTINGS", "MP_PREPLACED_UNIT_ORDER_MODE");
    GetSetting(cfg.BuildingDeathAnnounceMode, "SETTINGS", "BUILDING_DEATH_ANNOUNCE_MODE");
    GetSetting(cfg.QuieterAirstrips, "SETTINGS", "QUIETER_AIRSTRIPS");
    GetSetting(cfg.SearchRangeFixMode, "SETTINGS", "SEARCH_RANGE_FIX");

    // Balance
    GetSetting(cfg.OverrideAPCReqs, "BALANCE", "DO_OVERRIDE_APC_NOREQ");
    GetSetting(cfg.OverrideWarFactoryHP, "BALANCE", "DO_OVERRIDE_WFACTORY_HP");
    GetSetting(cfg.OverrideWarFactoryHP_Value, "BALANCE", "WFACTORY_HP_OVERRIDE");
    GetSetting(cfg.OverrideStealthTankCost, "BALANCE", "DO_OVERRIDE_STEALTHTANK_COST");
    GetSetting(cfg.OverrideStealthTankCost_Value, "BALANCE", "STEALTHTANK_COST_OVERRIDE");
    GetSetting(cfg.OverrideArtyTurn, "BALANCE", "DO_OVERRIDE_ARTY_TURN");
    GetSetting(cfg.OverrideArtyTurn_Value, "BALANCE", "ARTY_TURN_OVERRIDE");
    GetSetting(cfg.OverrideMLRSReqs, "BALANCE", "DO_MLRS_REQ_RADAR");
    GetSetting(cfg.OverrideMLRSHP, "BALANCE", "DO_OVERRIDE_MLRS_HP");
    GetSetting(cfg.OverrideMLRSHP_Value, "BALANCE", "MLRS_HP_OVERRIDE");
    GetSetting(cfg.OverrideMLRSGDIOnly, "BALANCE", "DO_MLRS_GDI_ONLY");
    GetSetting(cfg.OverrideMCVSight, "BALANCE", "DO_OVERRIDE_MCV_SIGHT");
    GetSetting(cfg.OverrideMCVSight_Value, "BALANCE", "MCV_SIGHT_OVERRIDE");
    GetSetting(cfg.OverrideConYardSight_Value, "BALANCE", "CONYARD_SIGHT_OVERRIDE");
    GetSetting(cfg.OverrideChemGuyBuff, "BALANCE", "DO_CHEM_GUY_BUFF");
    GetSetting(cfg.OverrideRocketGuyHP, "BALANCE", "DO_OVERRIDE_ROCKEYGUY_HP");
    GetSetting(cfg.OverrideRocketGuyHP_Value, "BALANCE", "ROCKEYGUY_HP_OVERRIDE");
    GetSetting(cfg.OverrideRocketGuyCost, "BALANCE", "DO_OVERRIDE_ROCKEYGUY_COST");
    GetSetting(cfg.OverrideRocketGuyCost_Value, "BALANCE", "ROCKEYGUY_COST_OVERRIDE");
    GetSetting(cfg.OverrideMinigunnerRange, "BALANCE", "DO_OVERRIDE_MINIGUNNER_RANGE");
    GetSetting(cfg.OverrideMinigunnerRange_Value, "BALANCE", "MINIGUNNER_RANGE_OVERRIDE");
    GetSetting(cfg.ApplyMPBalanceToSP, "BALANCE", "APPLY_BALANCE_TO_SINGLE_PLAYER");
    GetSetting(cfg.MPUnitsInSP, "BALANCE", "MULTIPLAYER_UNITS_IN_SINGLE_PLAYER");
    GetSetting(cfg.JDExtraAdjustments, "BALANCE", "JD_EXTRA_ADJUSTMENTS");
    GetSetting(cfg.MPCivSight, "BALANCE", "MP_CIV_SIGHT");
}

static void Normalize_CFE_Settings(CFEPatchConfig& cfg)
{
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
                "BUILDING_DEATH_ANNOUNCE_MODE",
                "QUIETER_AIRSTRIPS",
                "INVERSE_PREVIEW_MODE",
                "LEGACY_RENDER_DISABLE_MODE",
                "CUSTOM_MISSION_DEFAULT_SCENARIO",
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
