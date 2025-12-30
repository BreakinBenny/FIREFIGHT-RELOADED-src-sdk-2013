// Created by Crowbar 0.72

$modelname "gibs/police_left_leg.mdl"
$body studio "police_left_leg_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 4.062 -0.654 17.6

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -6.054 0.311 -3.597 7.359 7.816 38.798

$definebone "ValveBiped.Bip01_L_Thigh" "" 3.890447 -0.533612 38.566914 87.014814 84.252583 84.244851 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.848166 0 0 0 1.959516 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 16.525244 0 0.000001 -2.336637 -61.400334 -1.599412 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879447 -0.000001 0.000001 -0.16908 -33.48211 -0.418202 0 0 0 0 0 0

$sequence ragdoll "police_left_leg_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "police_left_leg_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_L_Thigh"


	$jointmassbias "ValveBiped.Bip01_L_Calf" 2
	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 126 0.04

	$jointmassbias "ValveBiped.Bip01_L_Foot" 2
	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit -1 -1 0.04
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 0.04
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 0.04
}