// Created by Crowbar 0.72

$modelname "gibs/soldier_shotgunner_right_leg.mdl"
$body studio "soldier_shotgunner_right_leg_soldier_right_leg_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -4.177 -0.516 9.269

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -6.681 -8.296 -3.631 7.717 -0.065 22.169

$definebone "ValveBiped.Bip01_R_Calf" "" -3.983585 0.391298 20.742977 85.061714 93.466836 93.479786 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 16.525251 0 0 2.336464 -61.399992 2.212248 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.87945 0 0 -0.16908 -33.48211 -4.660038 0 0 0 0 0 0

$sequence ragdoll "soldier_shotgunner_right_leg_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_shotgunner_right_leg_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_R_Calf"


	$jointmassbias "ValveBiped.Bip01_R_Foot" 2
	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -21 6 0.04
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 0.04
}