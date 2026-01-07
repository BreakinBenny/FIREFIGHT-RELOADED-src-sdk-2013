// Created by Crowbar 0.72

$modelname "gibs/police_left_arm.mdl"
$body studio "police_left_arm_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 16.932 1.94 13.882

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -5.423 5.403 0.538 1.543 28.464 27.22

$definebone "ValveBiped.Bip01_L_UpperArm" "" 7.723858 1.95616 58.484581 49.713313 4.166435 3.180546 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692547 0 -0.000008 -0.000003 -3.459061 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.48167 0 -0.000023 -6.108475 2.532072 90.136743 0 0 0 0 0 0

$sequence ragdoll "police_left_arm_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "police_left_arm_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_L_UpperArm"


	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -120 4 0.04

	$jointmassbias "ValveBiped.Bip01_L_Hand" 4
	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -60 60 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -57 70 0.04
}