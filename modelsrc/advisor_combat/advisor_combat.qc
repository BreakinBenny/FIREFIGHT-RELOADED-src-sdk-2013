// Created by Crowbar 0.72

$modelname "Advisor_combat.mdl"

$bodygroup "studio"
{
	studio "advisor_combat_advisor_referenceN.smd"
}
$bodygroup "arms"
{
	studio "advisor_combat_advisor_arms_reference.smd"
	blank
}


$surfaceprop "alienflesh"

$contents "solid"

$illumposition 0.04 -0.33 1.077
$eyeposition 0 0 80

$mostlyopaque

$cdmaterials "models\combine_advisor\"

$attachment "attach_l_bottom_front_jet" "advisor.l_bottom_front_jet" -0.02 3.3 -0.03 rotate 0 90 0
$attachment "attach_l_front_top_jet" "advisor.l_front_top_jet" 0 2.8 0.21 rotate 0 90 0
$attachment "attach_l_front_top_jet1" "advisor.l_front_top_jet1" -0.05 2.73 0.53 rotate 0 90 0
$attachment "attach_l_rear_bottom_jet" "advisor.l_rear_bottom_jet" 2.84 0.01 -0.01 rotate 0 0 0
$attachment "attach_mask_end_l" "advisor.bodyFat" 2.69 -3.7 12.31 rotate 0 -90 0
$attachment "attach_mask_end_r" "advisor.bodyFat" -1.47 -3.7 12.26 rotate 0 -90 0
$attachment "attach_mask_start_l" "advisor.mask" 2.7 -5.8 0.61 rotate 0 -90 0
$attachment "attach_mask_start_r" "advisor.mask" -2.36 -5.93 0.57 rotate 0 -90 0
$attachment "attach_mouth" "advisor.head" 10.84 3 0 rotate 0 0 0
$attachment "attach_r_bottom_top_jet" "advisor.r_bottom_top_jet" 0 3.11 0.01 rotate 0 90 0
$attachment "attach_r_front_top_jet" "advisor.r_front_top_jet" -0.02 2.8 0.21 rotate 0 90 0
$attachment "attach_r_rear_bottom_jet" "advisor.r_rear_bottom_jet" 3.34 -0.1 0 rotate 0 0 0
$attachment "attach_r_rear_top_jet" "advisor.r_rear_top_jet" 0.16 2.87 -0.29 rotate 0 90 0
$attachment "eyes" "advisor.head" 8 -6 0 rotate 0 0 0
$attachment "attach_podhose1" "advisor.bodyFat" 6.81 -2.31 -5.79 rotate 0 0 0
$attachment "attach_podhose2" "advisor.bodyFat" -5.15 -3.82 -6 rotate 0 0 0
$attachment "attach_podhose3" "advisor.bodyFat2" 2.08 0 3.7 rotate 0 0 0
$attachment "tongue_end" "advisor.tongue_10" 6 0 0 rotate 35 180 0
$attachment "__illumPosition" "advisor.head" 0 0 0 rotate 0 0 0

$cbox 0 0 0 0 0 0

$bbox -67.334 -90.676 -39.396 67.993 90.755 41.55

$definebone "advisor.body" "" 0 0 8.148932 1.423571 0.631457 88.341209 0 0 0 0 0 0
$definebone "advisor.tail1" "advisor.body" 0 -2.930103 -26.487471 66.417839 -88.786688 -86.526366 0 0 0 0 0 0
$definebone "advisor.tail2" "advisor.tail1" 27.397987 -0.000001 0 0 9.756556 -0.000001 0 0 0 0 0 0
$definebone "advisor.tailFat1" "advisor.tail1" 8.076069 22.058969 -0.407542 -89.999982 -8.657044 0 0 0 0 0 0 0
$definebone "advisor.tailFat2" "advisor.tail1" 13.756657 0.53698 25.874573 3.567293 91.666145 -89.533193 0 0 0 0 0 0
$definebone "advisor.tailFat3" "advisor.tail1" 13.220615 -1.817121 -24.072386 2.881061 87.22252 -89.412073 0 0 0 0 0 0
$definebone "advisor.tailFat" "advisor.tail1" 16.858845 -22.605181 -0.433444 -89.999982 0.183117 0 0 0 0 0 0 0
$definebone "advisor.bodyFat3" "advisor.body" 32.215031 0.506242 -2.300251 0 -89.107713 0 0 0 0 0 0 0
$definebone "advisor.bodyFat2" "advisor.body" -0.437291 27.788012 0 0 0.892324 0 0 0 0 0 0 0
$definebone "advisor.bodyFat1" "advisor.body" -30.214748 -2.408854 -0.08451 0 94.510714 0 0 0 0 0 0 0
$definebone "advisor.bodyFat" "advisor.body" 0 -28.315166 0 0 0 0 0 0 0 0 0 0
$definebone "advisor.neck1" "advisor.body" 0 -6.148932 19.999992 -71.207079 -89.999982 -89.999982 0 0 0 0 0 0
$definebone "advisor.neck2" "advisor.neck1" 22.561018 0.000003 0 0 -1.494331 0 0 0 0 0 0 0
$definebone "advisor.head" "advisor.neck2" 10.198025 0 0 0 1.218855 0 0 0 0 0 0 0
$definebone "advisor.camera" "advisor.head" 6.922688 3.329456 12.444248 -67.064371 -1.043124 -91.818834 0 0 0 0 0 0
$definebone "advisor.top_lens" "advisor.camera" -1.063606 9.662201 -1.66358 -0.000001 -0.000007 0.000001 0 0 0 0 0 0
$definebone "advisor.bottom_lens" "advisor.camera" -1.034004 9.744011 0.657837 -0.000001 -0.000007 0.000001 0 0 0 0 0 0
$definebone "advisor.mask" "advisor.head" 11.72559 0.474465 -0.311427 -89.999982 169.990049 0 0 0 0 0 0 0
$definebone "advisor.tongue_1" "advisor.head" -69.881157 -2.563123 -0.057629 0.878402 4.246881 -80.593732 0 0 0 0 0 0
$definebone "advisor.tongue_2" "advisor.tongue_1" 5.39999 0 0.000001 -0.000003 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.tongue_3" "advisor.tongue_2" 5.399989 0 0.000005 -0.000003 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.tongue_4" "advisor.tongue_3" 5.399991 0 0.000002 -0.000003 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.tongue_5" "advisor.tongue_4" 5.399988 0 0.000002 -0.000061 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.tongue_6" "advisor.tongue_5" 5.399985 0.000001 0.000004 -0.00006 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.tongue_7" "advisor.tongue_6" 5.39998 0 0.000003 -0.000063 0.000001 -0.000002 0 0 0 0 0 0
$definebone "advisor.tongue_8" "advisor.tongue_7" 5.399977 0.000001 0.000005 -0.000062 0.000001 -0.000002 0 0 0 0 0 0
$definebone "advisor.tongue_9" "advisor.tongue_8" 5.399977 0.000001 0.000003 -0.000063 0.000001 -0.000002 0 0 0 0 0 0
$definebone "advisor.tongue_10" "advisor.tongue_9" 5.399979 0.000001 0.000006 -0.000064 0.000001 -0.000001 0 0 0 0 0 0
$definebone "advisor.mouth" "advisor.head" 9.172108 3.691774 -0.127551 -0.528898 26.914014 1.464824 0 0 0 0 0 0
$definebone "advisor.head_fat_1" "advisor.head" 6.910618 -8.440711 -0.466711 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_2" "advisor.head" 6.688198 -4.371861 6.768445 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_8" "advisor.head" 6.6255 -3.981935 -7.458954 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_7" "advisor.head" 7.472057 3.303181 -9.894845 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_6" "advisor.head" 8.921673 9.369276 -8.238168 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_5" "advisor.head" 10.153728 13.563323 0.107412 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_3" "advisor.head" 7.567486 2.782489 8.307478 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.head_fat_4" "advisor.head" 8.951427 8.958187 5.556417 0 0.000003 0 0 0 0 0 0 0
$definebone "advisor.neckFat2" "advisor.neck1" 11.579376 -3.699903 -22.509211 -4.588245 -86.889104 -83.425863 0 0 0 0 0 0
$definebone "advisor.neckFat3" "advisor.neck1" 13.471937 -26.098545 -1.231855 -88.289935 164.886654 31.953684 0 0 0 0 0 0
$definebone "advisor.neckFat1" "advisor.neck1" 12.092398 -4.953053 19.40259 5.092907 -86.787348 -97.334929 0 0 0 0 0 0
$definebone "advisor.neckFat" "advisor.neck1" 18.602739 21.12649 0 -89.999982 -174.386641 0 0 0 0 0 0 0
$definebone "advisor.r_front_top_jet" "advisor.body" -19.114607 20.329763 10.97989 -5.471728 -139.289004 168.040426 0 0 0 0 0 0
$definebone "advisor.r_rear_top_jet" "advisor.body" -18.264963 11.67857 -29.056177 5.345868 59.921195 -14.346922 0 0 0 0 0 0
$definebone "advisor.l_front_top_jet" "advisor.body" 17.832436 21.235771 11.413651 0.541028 143.364687 167.266524 0 0 0 0 0 0
$definebone "advisor.l_bottom_front_jet" "advisor.body" 15.182141 -24.982206 16.557426 9.087511 -157.154525 4.392695 0 0 0 0 0 0
$definebone "advisor.r_bottom_top_jet" "advisor.body" -14.276615 -25.926432 16.303633 6.814589 -22.184236 177.935826 0 0 0 0 0 0
$definebone "advisor.r_rear_bottom_jet" "advisor.body" -14.56122 -27.470682 -20.126251 -10.448745 -108.36356 -29.551042 0 0 0 0 0 0
$definebone "advisor.l_rear_bottom_jet" "advisor.body" 16.351473 -27.281498 -19.737764 -13.117984 -62.653107 -173.653421 0 0 0 0 0 0
$definebone "advisor.l_front_top_jet1" "advisor.body" 18.4666 12.63843 -28.558079 3.754577 126.944625 -169.382175 0 0 0 0 0 0
$definebone "advisor.l_clavicle" "advisor.body" 8.027829 20.653341 22.993221 -18.256898 66.320727 9.093584 0 0 0 0 0 0
$definebone "advisor.l_shoulder" "advisor.l_clavicle" 7.522324 -0.493925 -0.913219 -7.691101 -130.435166 -26.029417 0 0 0 0 0 0
$definebone "advisor.l_up_arm" "advisor.l_shoulder" 47.693588 -0.470371 0.900616 -23.598983 111.54188 -20.528391 0 0 0 0 0 0
$definebone "advisor.l_low_arm" "advisor.l_up_arm" 52.499996 0.000002 -0.000005 0 -109.627617 -0.000003 0 0 0 0 0 0
$definebone "advisor.l_hand" "advisor.l_low_arm" 34.89698 -0.36834 -0.137398 0.861155 -1.234321 0.257485 0 0 0 0 0 0
$definebone "advisor.l_thumb_1" "advisor.l_hand" 9.941532 -1.809998 -0.253163 -6.917323 -60.380587 -1.379283 0 0 0 0 0 0
$definebone "advisor.l_thumb_2" "advisor.l_thumb_1" 9.848 0.000008 0.000006 -0.000002 40.940755 -0.000004 0 0 0 0 0 0
$definebone "advisor.l_finger_2_1" "advisor.l_hand" 13.283329 -0.308411 -0.835837 -5.190542 -24.912321 -5.139377 0 0 0 0 0 0
$definebone "advisor.l_finger_2_2" "advisor.l_finger_2_1" 14.102995 0 0.000008 -0.000006 -46.310875 -0.000001 0 0 0 0 0 0
$definebone "advisor.l_finger_1_1" "advisor.l_hand" 13.232742 -0.155586 0.465384 -6.541061 -24.790279 -6.984759 0 0 0 0 0 0
$definebone "advisor.l_finger_1_2" "advisor.l_finger_1_1" 14.102996 0 0.000011 -0.000004 -46.310875 -0.000001 0 0 0 0 0 0
$definebone "advisor.l_needle" "advisor.l_up_arm" 48.818165 3.389593 -0.352083 2.415994 8.577064 -38.74696 0 0 0 0 0 0
$definebone "advisor.r_clavicle" "advisor.body" -9.56763 20.221861 22.78665 -17.682562 116.883156 169.628049 0 0 0 0 0 0
$definebone "advisor.r_shoulder" "advisor.r_clavicle" 7.522316 -0.493926 0.913215 7.691101 -130.435166 26.029421 0 0 0 0 0 0
$definebone "advisor.r_up_arm" "advisor.r_shoulder" 47.693562 -0.47036 -0.900621 23.268615 111.405624 20.92729 0 0 0 0 0 0
$definebone "advisor.r_low_arm" "advisor.r_up_arm" 52.499947 -0.000006 0.000015 0.000004 -109.627617 0.000003 0 0 0 0 0 0
$definebone "advisor.r_hand" "advisor.r_low_arm" 34.896957 -0.368332 0.137401 -0.861151 -1.23432 -0.257485 0 0 0 0 0 0
$definebone "advisor.r_thumb_1" "advisor.r_hand" 9.941528 -1.809998 0.253165 6.917324 -60.380587 1.379278 0 0 0 0 0 0
$definebone "advisor.r_thumb_2" "advisor.r_thumb_1" 9.847988 -0.000015 -0.000006 0.000005 40.940813 0 0 0 0 0 0 0
$definebone "advisor.r_finger_2_1" "advisor.r_hand" 13.283318 -0.308411 0.835842 5.190545 -24.912312 5.139372 0 0 0 0 0 0
$definebone "advisor.r_finger_2_2" "advisor.r_finger_2_1" 14.102979 0 -0.00001 0.000004 -46.310858 -0.000004 0 0 0 0 0 0
$definebone "advisor.r_finger_1_1" "advisor.r_hand" 13.232735 -0.155579 -0.465379 6.541062 -24.790269 6.984756 0 0 0 0 0 0
$definebone "advisor.r_finger_1_2" "advisor.r_finger_1_1" 14.102981 -0.000008 -0.000002 0.000004 -46.310868 -0.000003 0 0 0 0 0 0
$definebone "advisor.r_needle" "advisor.r_up_arm" 45.173393 2.839865 0.196569 -2.057095 8.45193 6.103492 0 0 0 0 0 0

$weightlist "head" {
	"advisor.head" 1
	"advisor.neck2" 1
}

$Sequence "look_params" "advisor_combat_anims\look.smd" hidden 

$Animation aCenter "advisor_combat_anims\look.smd" subtract "look_params" 0 frames 1 1 weightlist head
$Animation aUp "advisor_combat_anims\look.smd" subtract "look_params" 0 frames 4 4 weightlist head
$Animation aDown "advisor_combat_anims\look.smd" subtract "look_params" 0 frames 5 5 weightlist head
$Animation aLeft "advisor_combat_anims\look.smd" subtract "look_params" 0 frames 2 2 weightlist head
$Animation aRight "advisor_combat_anims\look.smd" subtract "look_params" 0 frames 3 3 weightlist head

$sequence "aim_yaw" {
	"aLeft"
	"aCenter"
	"aRight"
	blendcenter aCenter
	blendwidth 3 blend "aim_yaw" -90 90
	delta
	hidden
	autoplay
}

$sequence "aim_pitch" {
	"aUp"
	"aCenter"
	"aDown"
	blendcenter aCenter
	blendwidth 3 blend "aim_pitch" -90 90
	delta
	hidden
	autoplay
}

$sequence "ragdoll" {
	"advisor_combat_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "attackR" {
	"advisor_combat_anims\attackR.smd"
	activity "ACT_MELEE_ATTACK1" 1
	{ event 4 16 "" }
	fadein 0.2
	fadeout 0.2
	fps 90
}

$sequence "attackL" {
	"advisor_combat_anims\attackL.smd"
	activity "ACT_MELEE_ATTACK1" 1
	{ event 3 16 "" }
	fadein 0.2
	fadeout 0.2
	fps 90
}

$includemodel "advisor_animations.mdl"

$collisionjoints "advisor_combat_physics.smd"
{
	$mass 200
	$inertia 5
	$damping 0.01
	$rotdamping 1.5
	$rootbone "advisor.body"
	$concave
	$maxconvexpieces 7
	$concaveperjoint


	$jointconstrain "advisor.r_up_arm" x limit -50 48 0.2
	$jointconstrain "advisor.r_up_arm" y limit -19 70 0.2
	$jointconstrain "advisor.r_up_arm" z limit -79 73 0.2

	$jointconstrain "advisor.r_low_arm" x limit 0 0 0
	$jointconstrain "advisor.r_low_arm" y limit 0 0 0
	$jointconstrain "advisor.r_low_arm" z limit 0 133 0.2

	$jointconstrain "advisor.r_hand" x limit -23 23 0.2
	$jointconstrain "advisor.r_hand" y limit -28 28 0.2
	$jointconstrain "advisor.r_hand" z limit -46 46 0.2

	$jointconstrain "advisor.r_finger_2_1" x limit 0 0 0
	$jointconstrain "advisor.r_finger_2_1" y limit 0 0 0
	$jointconstrain "advisor.r_finger_2_1" z limit 0 0 0

	$jointconstrain "advisor.r_finger_2_2" x limit 0 0 0
	$jointconstrain "advisor.r_finger_2_2" y limit 0 0 0
	$jointconstrain "advisor.r_finger_2_2" z limit 0 0 0

	$jointconstrain "advisor.r_thumb_1" x limit 0 0 0
	$jointconstrain "advisor.r_thumb_1" y limit 0 0 0
	$jointconstrain "advisor.r_thumb_1" z limit 0 0 0

	$jointconstrain "advisor.r_thumb_2" x limit 0 0 0
	$jointconstrain "advisor.r_thumb_2" y limit 0 0 0
	$jointconstrain "advisor.r_thumb_2" z limit 0 0 0

	$jointconstrain "advisor.r_finger_1_1" x limit 0 0 0
	$jointconstrain "advisor.r_finger_1_1" y limit 0 0 0
	$jointconstrain "advisor.r_finger_1_1" z limit 0 0 0

	$jointconstrain "advisor.r_finger_1_2" x limit 0 0 0
	$jointconstrain "advisor.r_finger_1_2" y limit 0 0 0
	$jointconstrain "advisor.r_finger_1_2" z limit 0 0 0

	$jointconstrain "advisor.r_needle" x limit 0 0 0
	$jointconstrain "advisor.r_needle" y limit 0 0 0
	$jointconstrain "advisor.r_needle" z limit 0 0 0

	$jointconstrain "advisor.l_shoulder" x limit 0 0 0
	$jointconstrain "advisor.l_shoulder" y limit 0 0 0
	$jointconstrain "advisor.l_shoulder" z limit 0 0 0

	$jointconstrain "advisor.l_up_arm" x limit -50 48 0.2
	$jointconstrain "advisor.l_up_arm" y limit -19 70 0.2
	$jointconstrain "advisor.l_up_arm" z limit -79 73 0.2

	$jointconstrain "advisor.l_low_arm" x limit 0 0 0
	$jointconstrain "advisor.l_low_arm" y limit 0 0 0
	$jointconstrain "advisor.l_low_arm" z limit 0 133 0.2

	$jointconstrain "advisor.l_hand" x limit -23 23 0.2
	$jointconstrain "advisor.l_hand" y limit -28 28 0.2
	$jointconstrain "advisor.l_hand" z limit -46 46 0.2

	$jointconstrain "advisor.l_finger_2_1" x limit 0 0 0
	$jointconstrain "advisor.l_finger_2_1" y limit 0 0 0
	$jointconstrain "advisor.l_finger_2_1" z limit 0 0 0

	$jointconstrain "advisor.l_thumb_1" x limit 0 0 0
	$jointconstrain "advisor.l_thumb_1" y limit 0 0 0
	$jointconstrain "advisor.l_thumb_1" z limit 0 0 0

	$jointconstrain "advisor.l_thumb_2" x limit 0 0 0
	$jointconstrain "advisor.l_thumb_2" y limit 0 0 0
	$jointconstrain "advisor.l_thumb_2" z limit 0 0 0

	$jointconstrain "advisor.l_finger_2_2" x limit 0 0 0
	$jointconstrain "advisor.l_finger_2_2" y limit 0 0 0
	$jointconstrain "advisor.l_finger_2_2" z limit 0 0 0

	$jointconstrain "advisor.l_finger_1_1" x limit 0 0 0
	$jointconstrain "advisor.l_finger_1_1" y limit 0 0 0
	$jointconstrain "advisor.l_finger_1_1" z limit 0 0 0

	$jointconstrain "advisor.l_needle" x limit 0 0 0
	$jointconstrain "advisor.l_needle" y limit 0 0 0
	$jointconstrain "advisor.l_needle" z limit 0 0 0

	$jointconstrain "advisor.l_finger_1_2" x limit 0 0 0
	$jointconstrain "advisor.l_finger_1_2" y limit 0 0 0
	$jointconstrain "advisor.l_finger_1_2" z limit 0 0 0

	$jointconstrain "advisor.tail1" x limit -10 10 0.2
	$jointconstrain "advisor.tail1" y limit -10 10 0.2
	$jointconstrain "advisor.tail1" z limit -10 10 0.2

	$jointconstrain "advisor.neck1" x limit -10 10 0.2
	$jointconstrain "advisor.neck1" y limit -10 10 0.2
	$jointconstrain "advisor.neck1" z limit -10 10 0.2
}

$keyvalues
{
	Cables 
	{
		"Cable"		 
		{
			"StartAttachment" "attach_mask_start_l"
			"EndAttachment" "attach_mask_end_l"
			"Width" "1s"
			"Material" "cable/cable"
			"NumSegments" "15"
			"Length" "170"
		}
		"Cable"		 
		{
			"StartAttachment" "attach_mask_start_r"
			"EndAttachment" "attach_mask_end_r"
			"Width" "1"
			"Material" "cable/cable"
			"NumSegments" "15"
			"Length" "170"
		}
	}
}
