// Created by Crowbar 0.72

$modelname "gibs/police_right_arm.mdl"
$body studio "police_right_arm_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -16.479 1.854 13.838

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -5.425 -27.563 0.449 1.722 -5.403 27.221

$definebone "ValveBiped.Bip01_R_UpperArm" "" -7.723804 1.95614 58.484608 49.713084 175.833822 176.81965 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692549 0 0.000004 0.000003 -3.459061 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481701 0 0.000015 6.108074 2.532072 -89.664285 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand" 2.676086 -1.712444 0 0 -89.999982 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger4" "ValveBiped.Bip01_R_Hand" 3.859667 -0.132565 1.193108 -9.083959 -50.36534 27.452699 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4" 1.312561 0.000004 -0.000001 -0.683653 -25.191351 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41" 0.72937 0.000002 0.000001 -0.351455 -13.995697 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 3.942289 0.050335 0.431044 -4.543843 -46.421326 8.880788 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.539101 0.000004 0 -0.33541 -14.196231 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 1.196327 -0.000006 0 -0.553649 -24.68079 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.883656 -0.050327 -0.431043 -1.512838 -33.672447 -4.921994 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719572 0.000004 0 -0.456877 -20.995237 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209183 0 0 -0.255597 -12.397432 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859827 -0.148937 -1.331281 2.209727 -30.481416 -20.306714 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719421 0 0 -0.378897 -20.796652 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099663 0.000002 0 -0.241275 -13.998106 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806017 -0.359837 -1.318333 38.595473 -45.490327 68.995579 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789785 -0.000003 0 -0.361017 13.095236 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.207001 0 0 -0.551068 20.792925 0.000002 0 0 0 0 0 0

$sequence ragdoll "police_right_arm_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "police_right_arm_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_R_UpperArm"


	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -120 4 0.04

	$jointmassbias "ValveBiped.Bip01_R_Hand" 4
	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -60 60 0.04
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -57 70 0.04
}