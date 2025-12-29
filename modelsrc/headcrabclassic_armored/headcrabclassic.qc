// Created by Crowbar 0.72

$modelname "headcrabclassic_armored.mdl"
$body studio "headcrabclassic_reference.smd"
$body armor "armored_headcrab_shell_prop.smd"

$lod 12
{
	replacemodel "headcrabclassic_reference.smd" "headcrabclassic_reference_lod1.smd"
	nofacial
}
$lod 20
{
	replacemodel "headcrabclassic_reference.smd" "headcrabclassic_reference_lod2.smd"
    replacemodel "armored_headcrab_shell_prop.smd" "armored_headcrab_shell_prop_lod1.smd"
	replacebone "HeadcrabClassic.FangL_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.FangL_Bone2" "HeadcrabClassic.FangL_Bone1"
	replacebone "HeadcrabClassic.FangR_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.HoleControlL" "HeadcrabClassic.SpineControl"
	replacebone "HeadcrabClassic.HoleControlR" "HeadcrabClassic.SpineControl"
	nofacial
}
$lod 30
{
	replacemodel "headcrabclassic_reference.smd" "headcrabclassic_reference_lod3.smd"
        replacemodel "armored_headcrab_shell_prop.smd" "armored_headcrab_shell_prop_lod2.smd"
	replacebone "HeadcrabClassic.FangL_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.FangL_Bone2" "HeadcrabClassic.FangL_Bone1"
	replacebone "HeadcrabClassic.FangR_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.HoleControlL" "HeadcrabClassic.SpineControl"
	replacebone "HeadcrabClassic.HoleControlR" "HeadcrabClassic.SpineControl"
	nofacial
}
$shadowlod
{
	replacemodel "headcrabclassic_reference.smd" "headcrabclassic_reference_lod4.smd"
    replacemodel "armored_headcrab_shell_prop.smd" "armored_headcrab_shell_prop_lod3.smd"
	replacebone "HeadcrabClassic.FangL_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.FangL_Bone2" "HeadcrabClassic.FangL_Bone1"
	replacebone "HeadcrabClassic.FangR_Bone1" "HeadcrabClassic.HeadControl"
	replacebone "HeadcrabClassic.HoleControlL" "HeadcrabClassic.SpineControl"
	replacebone "HeadcrabClassic.HoleControlR" "HeadcrabClassic.SpineControl"
	nofacial
}

$surfaceprop "alienflesh"
$contents "solid"

$illumposition -0.126 0.163 7.813

$eyeposition 0 0 16

$cdmaterials "models\headcrab_classic\"
$cdmaterials "models\headcrab_armor\"

$cbox 0 0 0 0 0 0

$bbox -14.381 -9.086 -0.492 14.056 8.833 16.117

$hboxset "default"
$hbox 1 "HeadcrabClassic.BodyControl"	    -4.90   -1.75   -8.17    15.90    9.75    8.17
$hbox 0 "HeadcrabClassic.BodyControl" -3.5 -6 -7.17 13.5 4 7.17
$hbox 0 "HeadcrabClassic.ForeArmL_Bone" 0 -0.5 -1 8 1.5 1
$hbox 0 "HeadcrabClassic.UpperArmL_Bone" -0.5 -1 -1 8.5 1 1
$hbox 0 "HeadcrabClassic.UpperArmR_Bone" -0.5 -1 -1 8.5 1 1
$hbox 0 "HeadcrabClassic.ForeArmR_Bone" 0 -1.5 -1 8 0.5 1
$hbox 0 "HeadcrabClassic.CalfR_Bone" -0.25 -0.5 -1 4.25 1.5 1
$hbox 0 "HeadcrabClassic.CalfL_Bone" -0.25 -1 -1 4.25 1 1

$definebone "HeadcrabClassic.BodyControl" "" 0 8.692983 10.684172 0 -89.999982 89.999982 0 0 0 0 0 0
$definebone "HeadcrabClassic.SpineControl" "HeadcrabClassic.BodyControl" 5.836933 -0.820595 0.000175 0 0 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.HeadControl" "HeadcrabClassic.SpineControl" 3.70716 -1.779552 -0.00162 0 0 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.FangL_Bone1" "HeadcrabClassic.HeadControl" 1.884797 0.225918 -3.244098 -7.359758 -99.621492 -49.057731 0 0 0 0 0 0
$definebone "HeadcrabClassic.FangL_Bone2" "HeadcrabClassic.FangL_Bone1" 3.698876 0 0 0.000002 -38.081928 0.000001 0 0 0 0 0 0
$definebone "HeadcrabClassic.FangR_Bone1" "HeadcrabClassic.HeadControl" 1.884668 0.225917 3.247675 7.360101 -99.621205 -130.939945 0 0 0 0 0 0
$definebone "HeadcrabClassic.HoleControlL" "HeadcrabClassic.SpineControl" -1.562495 -4.663179 -3.756904 0 0 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.HoleControlR" "HeadcrabClassic.SpineControl" -1.562644 -4.663178 3.756911 0 0 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.UpperArmL_Bone" "HeadcrabClassic.SpineControl" 2.630715 -3.155085 -5.016712 4.648579 -0.518756 3.763244 0 0 0 0 0 0
$definebone "HeadcrabClassic.ForeArmL_Bone" "HeadcrabClassic.UpperArmL_Bone" 8.255675 0 0 0.000001 -53.959222 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.UpperArmR_Bone" "HeadcrabClassic.SpineControl" 2.630415 -3.155085 5.016869 -4.648464 -0.518813 176.236776 0 0 0 0 0 0
$definebone "HeadcrabClassic.ForeArmR_Bone" "HeadcrabClassic.UpperArmR_Bone" 8.255673 0 0 0 53.959225 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.HipControl" "HeadcrabClassic.BodyControl" -0.450924 -0.675699 0 0 0 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.ThighR_Bone" "HeadcrabClassic.HipControl" 0.000009 0 6.164912 -17.983584 -103.757998 -173.780831 0 0 0 0 0 0
$definebone "HeadcrabClassic.CalfR_Bone" "HeadcrabClassic.ThighR_Bone" 6.098812 0 0 0.000001 -0.040852 0 0 0 0 0 0 0
$definebone "HeadcrabClassic.ThighL_Bone" "HeadcrabClassic.HipControl" -0.000009 0 -6.164912 17.983584 -103.758053 173.796704 0 0 0 0 0 0
$definebone "HeadcrabClassic.CalfL_Bone" "HeadcrabClassic.ThighL_Bone" 6.098812 0 0 0.000001 -0.040853 0.000001 0 0 0 0 0 0

$sequence Idle01 "headcrabclassic_anims\Idle01.smd" ACT_IDLE 2 loop
$sequence IdleB "headcrabclassic_anims\IdleB.smd" ACT_IDLE 1 loop

$sequence Run1 {
	"headcrabclassic_anims\Run1.smd"
	activity "ACT_RUN" 1
	{ event AE_CL_PLAYSOUND 5 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 11 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 17 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 22 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 28 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 33 "NPC_Headcrab.Footstep" }
	loop
	walkframe 33 LX LY
}

$sequence "TurnLeft" {
	"headcrabclassic_anims\TurnLeft.smd"
	activity "ACT_TURN_LEFT" 1
	{ event AE_CL_PLAYSOUND 7 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 8 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 12 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 16 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 23 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 25 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 30 "NPC_Headcrab.Footstep" }
	loop
	walkframe 30
}

$sequence "TurnRight" {
	"headcrabclassic_anims\TurnRight.smd"
	activity "ACT_TURN_RIGHT" 1
	{ event AE_CL_PLAYSOUND 7 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 8 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 12 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 16 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 23 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 24 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 28 "NPC_Headcrab.Footstep" }
	{ event AE_CL_PLAYSOUND 30 "NPC_Headcrab.Footstep" }
	loop
}

$sequence "jumpattack_broadcast" {
	"headcrabclassic_anims\jumpattack_broadcast.smd"
	activity "ACT_RANGE_ATTACK1" 1
	{ event AE_HEADCRAB_JUMP_TELEGRAPH 0 "" }
	{ event AE_HEADCRAB_JUMPATTACK 5 "" }
}

$sequence "Drown" {
	"headcrabclassic_anims\Drown.smd"
	activity "ACT_HEADCRAB_DROWN" 1
	loop
}

$sequence "canal5b_sewer_jump" {
	"headcrabclassic_anims\canal5b_sewer_jump.smd"
	walkframe 51 LX LY LZ
}

$sequence "headcrabbed" {
	"headcrabclassic_anims\headcrabbed.smd"
}

$sequence "headcrabbedpost" {
	"headcrabclassic_anims\headcrabbedpost.smd"
	loop
}

$sequence "BurrowIn" {
	"headcrabclassic_anims\BurrowIn.smd"
	activity "ACT_HEADCRAB_BURROW_IN" 1
	{ event AE_HEADCRAB_BURROW_IN 25 "" }
	{ event AE_HEADCRAB_BURROW_IN_FINISH 42 "" }
}

$sequence "BurrowOut" {
	"headcrabclassic_anims\BurrowOut.smd"
	activity "ACT_HEADCRAB_BURROW_OUT" 1
	{ event AE_HEADCRAB_BURROW_OUT 0 "" }
}

$sequence "BurrowIdle" {
	"headcrabclassic_anims\BurrowIdle.smd"
	activity "ACT_HEADCRAB_BURROW_IDLE" 1
	loop
}

$sequence "cannisterDeploy_A" {
	"headcrabclassic_anims\cannisterDeploy_A.smd"
	activity "ACT_HEADCRAB_CRAWL_FROM_CANISTER_LEFT" 1
	walkframe 15 LX LY LZ LZR
	walkframe 20 LX LY LZ LZR
	walkframe 25 LX LY LZ LZR
	walkframe 30 LX LY LZ LZR
	walkframe 40 LX LY LZ LZR
	walkframe 46 LX LY LZ LZR
}

$sequence "cannisterDeploy_B" {
	"headcrabclassic_anims\cannisterDeploy_B.smd"
	activity "ACT_HEADCRAB_CRAWL_FROM_CANISTER_CENTER" 1
	walkframe 15 LX LY LZ LZR
	walkframe 20 LX LY LZ LZR
	walkframe 25 LX LY LZ LZR
	walkframe 30 LX LY LZ LZR
	walkframe 40 LX LY LZ LZR
	walkframe 46 LX LY LZ LZR
}

$sequence "cannisterDeploy_C" {
	"headcrabclassic_anims\cannisterDeploy_C.smd"
	activity "ACT_HEADCRAB_CRAWL_FROM_CANISTER_RIGHT" 1
	walkframe 20 LX LY LZ LZR
	walkframe 25 LX LY LZ LZR
	walkframe 30 LX LY LZ LZR
	walkframe 40 LX LY LZ LZR
	walkframe 46 LX LY LZ LZR
}

$sequence "testDeploy_A" {
	"headcrabclassic_anims\testDeploy_A.smd"
}

$sequence "testDeploy_b" {
	"headcrabclassic_anims\testDeploy_b.smd"
}

$sequence "testDeploy_C" {
	"headcrabclassic_anims\testDeploy_C.smd"
}

$sequence ragdoll "headcrabclassic_anims\ragdoll.smd" ACT_DIERAGDOLL 1

$collisionjoints "headcrab_phys.smd"
{
	$mass 5
	$inertia 5
	$damping 0.01
	$rotdamping 1.5
	$rootbone " "


	$jointconstrain "HeadcrabClassic.ThighL_Bone" x limit -37 12 1
	$jointconstrain "HeadcrabClassic.ThighL_Bone" y limit -39 46 1
	$jointconstrain "HeadcrabClassic.ThighL_Bone" z limit -44 46 1

	$jointconstrain "HeadcrabClassic.ThighR_Bone" x limit -30 19 1
	$jointconstrain "HeadcrabClassic.ThighR_Bone" y limit -39 47 1
	$jointconstrain "HeadcrabClassic.ThighR_Bone" z limit -44 46 1

	$jointconstrain "HeadcrabClassic.CalfR_Bone" x limit 0 0 0
	$jointconstrain "HeadcrabClassic.CalfR_Bone" y limit 0 0 0
	$jointconstrain "HeadcrabClassic.CalfR_Bone" z limit -77 57 1

	$jointconstrain "HeadcrabClassic.UpperArmR_Bone" x limit -37 64 1
	$jointconstrain "HeadcrabClassic.UpperArmR_Bone" y limit -53 23 1
	$jointconstrain "HeadcrabClassic.UpperArmR_Bone" z limit -61 55 1

	$jointconstrain "HeadcrabClassic.ForeArmR_Bone" x limit 0 0 0
	$jointconstrain "HeadcrabClassic.ForeArmR_Bone" y limit 0 0 0
	$jointconstrain "HeadcrabClassic.ForeArmR_Bone" z limit -84 39 1

	$jointconstrain "HeadcrabClassic.UpperArmL_Bone" x limit -64 35 1
	$jointconstrain "HeadcrabClassic.UpperArmL_Bone" y limit -23 39 1
	$jointconstrain "HeadcrabClassic.UpperArmL_Bone" z limit -44 53 1

	$jointconstrain "HeadcrabClassic.ForeArmL_Bone" x limit 0 0 0
	$jointconstrain "HeadcrabClassic.ForeArmL_Bone" y limit 0 0 0
	$jointconstrain "HeadcrabClassic.ForeArmL_Bone" z limit -70 48 1

	$jointconstrain "HeadcrabClassic.CalfL_Bone" x limit 0 0 0
	$jointconstrain "HeadcrabClassic.CalfL_Bone" y limit 0 0 0
	$jointconstrain "HeadcrabClassic.CalfL_Bone" z limit -95 57 1
	$noselfcollisions
}