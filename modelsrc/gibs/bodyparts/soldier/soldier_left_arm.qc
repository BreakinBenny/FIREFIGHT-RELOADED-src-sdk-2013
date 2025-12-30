// Created by Crowbar 0.72

$modelname "gibs/soldier_left_arm.mdl"
$body studio "soldier_left_arm_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 15.903 1.522 5.364

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -5.792 5.152 -8.242 2.748 26.659 18.965

$definebone "ValveBiped.Bip01_L_UpperArm" "" 7.723858 1.956158 58.484581 49.713313 4.166377 3.180546 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692562 0 0 -0.000003 -3.45906 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.481672 0 -0.000027 -6.108477 2.532072 90.136743 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.883696 -0.046776 0.431443 1.250767 -33.682289 4.921822 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 1.719578 0 0 0.316045 -20.997631 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.209187 -0.000002 0 0.176702 -12.398798 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.859859 -0.137955 1.332467 -2.449394 -30.465188 20.308434 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.719433 0 0.000001 0.233428 -20.798875 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 1.099663 0.000002 0.000001 0.148744 -13.999129 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 0.806046 -0.348923 1.321259 -38.931856 -45.224008 -69.147647 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.789782 0.000002 0.000004 0.423181 13.093407 0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.206993 0 0.000004 0.645485 20.79035 0.000003 0 0 0 0 0 0

$sequence ragdoll "soldier_left_arm_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "soldier_left_arm_physics.smd"
{
	$mass 9
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.Bip01_L_UpperArm"


	$jointmassbias "ValveBiped.Bip01_L_Forearm" 2
	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -120 4 0.04

	$jointmassbias "ValveBiped.Bip01_L_Hand" 4
	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -60 60 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -57 70 0.04
}