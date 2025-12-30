// Created by Crowbar 0.72

$modelname "gibs/soldier_right_arm.mdl"
$body studio "soldier_right_arm_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -15.959 1.843 5.395

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -6.463 -26.666 -8.243 2.782 -5.26 19.028

$definebone "ValveBiped.Bip01_R_UpperArm" "" -7.723803 1.956144 58.484608 49.713143 175.833754 176.81965 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692551 0 0 -0.000003 -3.459061 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481697 0 0.000015 6.108067 2.532073 -89.664285 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.88365 -0.050327 -0.431043 -1.512838 -33.672447 -4.921994 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719582 0 0 -0.456876 -20.99524 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209183 -0.000002 -0.000001 -0.255596 -12.397435 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859821 -0.148937 -1.331281 2.209726 -30.481413 -20.306712 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719419 0.000004 0 -0.378898 -20.79665 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099667 0.000004 -0.000001 -0.241272 -13.998104 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806015 -0.359837 -1.318334 38.595469 -45.490327 68.995579 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789783 -0.000001 0.000002 -0.361021 13.09524 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.206999 0.000001 0 -0.551069 20.792928 -0.000003 0 0 0 0 0 0

$sequence ragdoll "soldier_right_arm_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_right_arm_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_R_UpperArm"


	$jointmassbias "ValveBiped.Bip01_R_Forearm" 2
	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -120 4 0.04
}