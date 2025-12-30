// Created by Crowbar 0.72

$modelname "gibs/police_right_leg.mdl"
$body studio "police_right_leg_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -4.063 -0.614 17.587

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -6.09 -7.819 -3.624 7.323 -0.311 38.798

$definebone "ValveBiped.Bip01_R_Thigh" "" -3.890457 -0.533622 38.566921 87.014527 95.749444 95.75723 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.848169 0 0 0 1.958771 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 16.525249 0 0 2.336465 -61.399992 2.212248 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879454 -0.000001 0 -0.16908 -33.48211 -4.660038 0 0 0 0 0 0

$sequence ragdoll "police_right_leg_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "police_right_leg_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_R_Thigh"


	$jointmassbias "ValveBiped.Bip01_R_Calf" 2
	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -12 126 0.04

	$jointmassbias "ValveBiped.Bip01_R_Foot" 2
	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -21 6 0.04
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 0.04
}