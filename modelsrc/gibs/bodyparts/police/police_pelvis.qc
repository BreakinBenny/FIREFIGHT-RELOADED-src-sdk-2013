// Created by Crowbar 0.72

$modelname "gibs/police_pelvis.mdl"
$body studio "police_pelvis_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -0.002 0.635 11.473

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -7.429 -8.427 -0.568 6.163 8.427 23.508

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345127 -2.981901 4.944225 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0.000004 0.000007 2.9705 -90.299349 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.848166 0 0 0 1.958771 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890452 -0.000004 -0.000003 2.970156 -89.700792 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.848167 0 0 0 1.959516 0 0 0 0 0 0 0

$sequence ragdoll "police_pelvis_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "police_pelvis_physics.smd"
{
	$mass 40
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.bip01_pelvis"


	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -25 39 0.04
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 0.04
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 0.04

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -25 20 0.04
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 0.04
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 0.04
}