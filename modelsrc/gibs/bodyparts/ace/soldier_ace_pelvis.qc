// Created by Crowbar 0.72

$modelname "gibs/soldier_ace_pelvis.mdl"
$body studio "soldier_ace_pelvis_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 0.046 0.927 9.447

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -8.049 -8.443 -2.398 6.2 8.539 21.3

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890452 -0.000004 -0.000003 2.970156 -89.700792 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0.000004 0.000007 2.9705 -90.299349 -89.999982 0 0 0 0 0 0

$sequence ragdoll "soldier_ace_pelvis_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_ace_pelvis_physics.smd"
{
	$mass 40
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.bip01_pelvis"

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -25 20 0.008
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 0.008
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 0.008

	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -25 39 0.008
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 0.008
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 0.008
}