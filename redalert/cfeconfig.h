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
** CFE Patch Redux feature switches, ported from CFE's REDALERT/EXTERNS.H.
** The in-class defaults are the unmodified game; values come from CFE_CONFIG_FILE_NAME.
*/
#ifndef RA_CFECONFIG_H
#define RA_CFECONFIG_H

#define CFE_CONFIG_FILE_NAME "CFEPATCHREDUX_RA.INI"

struct CFEPatchConfig
{
    bool EnableVersionAnnounce = false;
    bool EnableASPathing = false;
    bool EnableOOMRepair = false;
    bool EnableRallyPoints = false;
    bool EnableHarvyQueueJump = false;
    bool EnableHarvyLoadBalancing = false;
    bool EnableHarvyOptimize = false;
    bool EnableInstantCapture = false;
    bool WallsAlwaysHaveFullCost = true;
    bool EnableAttackMove = false;
    bool EnableChronoTankAttackMove = false;
    bool EnableQMoveLoops = false;
    bool EnableAirQMove = false;
    bool EnableAirQAMLRecall = false;
    bool EnableSmarterAircraft = false;
    bool EnableSmarterRepairBays = false;
    bool BetterOreGrowth = false;
    bool OreIndexBugfix = false;
    bool GemOverloadFix = false;
    bool EnableInaccuracyBugfix = false;
    bool EnableWideSupers = false;
    bool EnableQuickSupers = false;
    bool EnableSmarterChrono = false;
    bool EnableTimeQuakeFix = false;
    bool EnableSharedChronoDisasterOdds = false;
    bool EnableCumulativeTimelineDamage = false;
    bool EnableLongGapFix = false;
    bool EnableMIGAmmoFudge = false;
    bool EnableBuildingCaptureAnnouncements = false;
    bool EnableSmarterSonar = false;
    bool EnableBurningTreeFix = false;
    bool EnableDeathsDoorFix = false;
    bool EnableNewSkirmishAI = false;

    int EnableVeterancy = 0;
    int EnableTanyaGuard = 0;
    int SafeSabotage = 0;
    int HarvyQueueJumpCutoff = 4;
    int HarvyOptimizeUnloadWaitWeight = 6;
    int HarvyOptimizeThrashingCutoff = 5;
    int HarvyOptimizeThrashingNumerator = 2;
    int HarvyOptimizeThrashingDenominator = 3;
    int HarvyOptimizeThrashingWeight = 4;
    int HarvyOptimizeCommunalismNumerator = 9;
    int HarvyOptimizeCommunalismDenominator = 4;
    int HarvyOptimizeCommunalismWeight = 13;
    int OreGrowthScale = 1;
    int OreToGemPromotionSlowFactor = 8;
    int WallBuildLength = 5;
    int InverseBuildingPreviewMode = 0; // 0=off; 1= MP only; 2 = all game modes
    int BuildingGapOffset = 0;
    int OreCapacityOffset = 0;
    int SharedChronoDisasterOdds = 20;
    int TurkeyFixMode = 0; // 0=no bonus (buggy original RA behavior, 11/18/2024 Remastered patch behavior); 1=90% build time (intended original RA behavior, Remastered launch behavior, insanely overpowered); 2=110% build speed bonus from extra production structures (CFE patch rebalance)
};

extern CFEPatchConfig ActiveCFEPatchConfig;

void Initialise_CFE_Patch_Config();

#endif
