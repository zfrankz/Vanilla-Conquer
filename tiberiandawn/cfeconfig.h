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
** CFE Patch Redux feature switches, ported from CFE's TIBERIANDAWN/EXTERNS.H.
** The in-class defaults are the unmodified game; values come from CFE_CONFIG_FILE_NAME.
*/
#ifndef TD_CFECONFIG_H
#define TD_CFECONFIG_H

#define CFE_CONFIG_FILE_NAME "CFEPATCHREDUX.INI"

struct CFEPatchConfig
{
    bool EnableVersionAnnounce = false;
    bool EnableASPathing = false;
    bool EnableOOMRepair = false;
    bool EnableRallyPoints = false;
    bool EnableHarvyQueueJump = false;
    bool EnableHarvyLoadBalancing = false;
    bool EnableHarvyOptimize = false;
    bool EnableHarvySelfRepair = false;
    bool DisableCommandoAirstrikes = false;
    bool WallsAlwaysHaveFullCost = true;
    bool EnableAttackMove = false;
    bool EnableQMove = false;
    bool EnableSmarterAircraft = false;
    bool EnableSmarterSAMs = false;
    bool EnableSmarterRepairBays = false;
    bool EnableSmarterMammoths = false;
    bool EnableSmarterHarvesters = false;
    bool EnableSmarterRefineries = false;
    bool SmarterCPUCommandos = false;
    bool EnableGunboatBugfix = false;
    bool BetterTibGrowth = false;
    bool TibIndexBugfix = false;
    bool TibResurgence = false;
    bool TreeVisceroidSpawnAnyway = false;
    bool TSVisceroidSpawn = false;
    bool AllowSandbagging = true;
    bool EnableNukeTank = false;
    bool DisableMPNeutralAttacks = false;
    bool EnableGDI9Fix = false;
    bool EnableBurningTreeFix = false;
    bool EnableNukeRadiusFix = false;
    bool EnableDeathsDoorFix = false;
    bool EnableNewSkirmishAI = false;
    bool EnableSellChanceFix = false;
    bool ForceLoanerCopters = false;
    bool AIvsWalls = false;
    bool EnableWallCrush = false;
    bool EnableAdvancedWallOwnership = false;
    bool EnableHuntFix = false;
    bool MCVAlwaysSpawnEngineer = false;
    bool SplitDualCapturables = false;
    bool BuildForbiddenSPCapturables = false;
    bool EnableDistanceCalcFix = false;
    bool ShroudRemover = false;
    bool DeadBuildingScatterFix = false;

    bool OverrideAPCReqs = false;
    bool OverrideWarFactoryHP = false;
    bool OverrideStealthTankCost = false;
    bool OverrideArtyTurn = false;
    bool OverrideMLRSReqs = false;
    bool OverrideMLRSHP = false;
    bool OverrideMLRSGDIOnly = false;
    bool OverrideMCVSight = false;
    bool OverrideChemGuyBuff = false;
    bool OverrideRocketGuyHP = false;
    bool OverrideRocketGuyCost = false;
    bool OverrideMinigunnerRange = false;
    bool ApplyMPBalanceToSP = false;
    bool MPUnitsInSP = false;
    bool JDExtraAdjustments = false;
    bool MPCivSight = false;

    int DefaultCustomMissionScenarioNumber = 1;
    int LegacyRenderDisableMode = 1;
    int EnableVeterancy = 0;
    int EnableCommandoGuard = 1;
    int SafeSabotage = 0;
    int InfantryTiberiumAversion = 0;
    int BuildingGap = 2;
    int HarvyQueueJumpCutoff = 4;
    int HarvyOptimizeUnloadWaitWeight = 13;
    int HarvyOptimizeThrashingCutoff = 5;
    int HarvyOptimizeThrashingNumerator = 2;
    int HarvyOptimizeThrashingDenominator = 3;
    int HarvyOptimizeThrashingWeight = 8;
    int HarvyOptimizeCommunalismNumerator = 9;
    int HarvyOptimizeCommunalismDenominator = 4;
    int HarvyOptimizeCommunalismWeight = 29;
    int TibGrowthScale = 1;
    int TibResurgeDelay = 7200;
    int TreeVisceroidSpawnMode = 0;
    int MeanerVisceroids = 0;
    int WallBuildLength = 5;
    int InverseBuildingPreviewMode = 0; // 0=off; 1= MP only; 2 = all game modes
    int SmarterSAMAmmo = 2;
    int SmarterSAMBalanceDelay = 0;
    int MPPreplacedUnitOrderMode =
        0; // 0=remastered vanilla - enter idle mode (guard or guard area); 1=honor orders map file; 2=original game vanilla - hunt for infantry, timed hunt for vehicles (aircraft not affected; they DO honor orders regardless)
    int BuildingDeathAnnounceMode =
        0; // 0=off; 1=owner notice only; 2=global notice; 3=global faction-based notice in SP; global faction-based notice in SP & MP
    int QuieterAirstrips =
        0; // 0=vanilla (announce "unit ready" & "reinf. arrived"); 1="unit ready" only; 2="reinf. arrived" only; 3="unit ready" only, but when cargo plane lands
    int SearchRangeFixMode = 0; // 0=off; 1=only fix THREAT_RANGE; 2=fix THREAT_RANGE and THREAT_AREA

    int OverrideWarFactoryHP_Value = 520;
    int OverrideStealthTankCost_Value = 700;
    int OverrideArtyTurn_Value = 4;
    int OverrideMLRSHP_Value = 120;
    int OverrideMCVSight_Value = 4;
    int OverrideConYardSight_Value = 6;
    int OverrideRocketGuyHP_Value = 35;
    int OverrideRocketGuyCost_Value = 250;
    int OverrideMinigunnerRange_Value = 512;
};

extern CFEPatchConfig ActiveCFEPatchConfig;

void Initialise_CFE_Patch_Config();

#endif
