// Created by Crowbar 0.72

$modelname "gibs/soldier_shotgunner_left_leg.mdl"
$body studio "soldier_shotgunner_left_leg_soldier_left_leg_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 4.319 -0.492 9.779

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -6.729 0.203 -3.621 7.706 8.439 23.18

$definebone "ValveBiped.Bip01_L_Calf" "" 3.983527 0.3912 20.742964 85.061311 86.535246 86.522412 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 16.525248 0 0 -2.336636 -61.400334 -1.599412 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879447 -0.000001 0.000001 -0.169079 -33.48211 -0.418202 0 0 0 0 0 0

$sequence ragdoll "soldier_shotgunner_left_leg_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_shotgunner_left_leg_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_L_Calf"


	$jointmassbias "ValveBiped.Bip01_L_Foot" 2
	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit -1 -1 0.04
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 0.04
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 0.04
}