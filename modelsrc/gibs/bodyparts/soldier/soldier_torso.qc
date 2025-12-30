// Created by Crowbar 0.72

$modelname "gibs/soldier_torso.mdl"
$body studio "soldier_torso_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -0.015 0.274 10.124

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -9.3 -9.134 -3.433 8.757 9.109 23.676

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345127 -2.981901 4.944225 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.01833 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518559 0 0 0 5.74883 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942638 0 0 0 11.120882 0 0 0 0 0 0 0

$sequence ragdoll "soldier_torso_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_torso_physics.smd"
{
	$mass 40
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.bip01_pelvis"


	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -48 48 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -25 25 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -25 50 0.04
}