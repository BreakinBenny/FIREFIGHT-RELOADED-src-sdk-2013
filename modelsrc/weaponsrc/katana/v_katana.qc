// Created by Crowbar 0.72

$modelname "Weapons/v_katana.mdl"

$bodygroup "studio"
{
	studio "v_katana_v_fblade_v_f_blade_hand.smd"
}
$bodygroup "katana"
{
	studio "v_katana_v_fblade_v_f_blade_katana.smd"
}


$surfaceprop "default"

$contents "solid"

$illumposition -17.883 33.268 -17.386

$mostlyopaque

$cdmaterials "models\Weapons\v_HAND\"
$cdmaterials "models\Weapons\F_BLADE\"

$texturegroup "skinfamilies"
{
	{ "fire_blade_1024_256"          }
	{ "fire_blade_1024_256_bloodied" }
	{ "fire_blade_1024_256_bloodied_alien" }
}

$cbox 0 0 0 0 0 0

$bbox -100.894 -54.787 -68 35.556 19.583 34.174

$definebone "ValveBiped" "" 0 0 0 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01" "ValveBiped" -0.000005 38.566917 0.533613 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01" 0.000005 3.345127 -2.981901 4.944225 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.018333 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518566 0 0 0 5.74883 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942638 0 0 0 11.120882 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 2.033348 1.000767 1.93766 -73.711873 168.619243 91.480562 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.02814 0.000004 0 -23.385787 -73.476334 -90.452673 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692551 -0.000002 0 0 -70.144472 0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.481674 -0.000004 -0.000032 -6.108474 2.532071 90.13675 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger4" "ValveBiped.Bip01_L_Hand" 3.859704 -0.142405 -1.191967 8.685981 -50.424527 -26.821128 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger41" "ValveBiped.Bip01_L_Finger4" 1.312562 0.000002 0.000004 -0.000003 -25.194107 -0.000006 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger42" "ValveBiped.Bip01_L_Finger41" 0.729363 0 0 -0.000004 -13.997305 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 3.942327 0.046771 -0.431438 4.194965 -46.461378 -8.696128 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 1.539093 -0.000002 0.000004 -0.000003 -14.197726 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31" 1.19632 0 0.000004 -0.000004 -24.683137 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.883687 -0.046771 0.43145 1.250764 -33.682296 4.921819 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 1.719576 0.000002 0.000004 0.31604 -20.997639 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.209183 0.000002 -0.000004 0.176697 -12.398801 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.85986 -0.137949 1.332474 -2.449399 -30.465198 20.308432 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.719429 0.000002 -0.000004 0.233418 -20.798887 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 1.099666 0 0 0.148736 -13.999138 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 0.806046 -0.348923 1.321262 -38.921372 -45.197206 -70.052927 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.789783 -0.000002 0 -0.000005 13.0934 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.206997 0.000004 0 -0.000005 20.790345 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.033356 1.000772 -1.93761 73.71205 168.618792 -95.596578 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028144 0 0 78.953585 -88.125723 91.732603 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692555 0 0 0 -18.575697 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481701 -0.000004 0 0.483175 2.499182 -105.41617 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger4" "ValveBiped.Bip01_R_Hand" 3.859674 -0.132568 1.193108 5.226347 -81.537856 11.563776 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4" 1.312563 -0.000001 0 -10.831141 -69.651325 -0.487301 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41" 0.729364 -0.000002 0.000004 5.910917 -39.344844 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 3.942297 0.050328 0.431042 7.722036 -76.205609 -1.298784 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.539096 0 -0.000004 -0.000002 -63.850828 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 1.196325 0 0 0 -79.202348 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.883659 -0.050328 -0.431046 3.14462 -60.997428 -0.938279 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719578 0 -0.000004 -0.428346 -86.032831 0.15894 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209183 -0.000002 0 -0.171371 -74.923623 0.189653 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859831 -0.148935 -1.331287 2.476322 -54.376495 -6.346829 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719425 -0.000004 0 -0.162722 -97.740642 0.342173 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099667 -0.000001 0.000004 -0.220474 -58.231413 0.098094 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806017 -0.359833 -1.318333 19.237976 -37.336611 70.867195 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789783 -0.000004 0 -10.929853 -0.18111 -23.59767 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.206997 0 0 2.447678 41.853541 0.290493 0 0 0 0 0 0
$definebone "ValveBiped.SparkL_1" "ValveBiped.Bip01_L_Forearm" 0.198902 -0.657009 0.005067 -89.939009 93.240647 89.999702 0 0 0 0 0 0
$definebone "ValveBiped.SparkR_1" "ValveBiped.Bip01_R_Forearm" 1.917045 -0.231709 -0.067282 -73.914306 93.024513 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.UpperBodyParent" "ValveBiped" 0.000156 39.987263 -0.385905 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.UpperBody" "ValveBiped.UpperBodyParent" -0.000161 -1.420341 14.468496 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.SpineControlParent" "ValveBiped.UpperBody" 0.000005 3.345127 -2.981901 4.766608 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.SpineControl" "ValveBiped.SpineControlParent" 0.000004 0.000001 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.SpineControl1" "ValveBiped.SpineControl" 4.018311 -0.012456 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.SpineControl2" "ValveBiped.SpineControl1" 3.516724 -0.113778 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.SpineControl3" "ValveBiped.SpineControl2" 8.921982 0.607576 0 20.000008 0 0 0 0 0 0 0 0
$definebone "ValveBiped.null2" "ValveBiped.SpineControl3" -57.2575 -15.556101 0 -89.999982 -85.233439 0 0 0 0 0 0 0
$definebone "ValveBiped.null" "ValveBiped.null2" -1.38447 60.269463 29.922504 42.575069 8.361748 5.415139 0 0 0 0 0 0
$definebone "ValveBiped.null1" "ValveBiped.null" 0 7.412292 0.753151 0.00001 0.000003 0.000003 0 0 0 0 0 0

$bonemerge "ValveBiped"
$bonemerge "ValveBiped.Bip01"
$bonemerge "ValveBiped.Bip01_Spine"
$bonemerge "ValveBiped.Bip01_Spine1"
$bonemerge "ValveBiped.Bip01_Spine2"
$bonemerge "ValveBiped.Bip01_Spine4"
$bonemerge "ValveBiped.Bip01_L_Clavicle"
$bonemerge "ValveBiped.Bip01_L_UpperArm"
$bonemerge "ValveBiped.Bip01_L_Forearm"
$bonemerge "ValveBiped.Bip01_R_Clavicle"
$bonemerge "ValveBiped.Bip01_R_UpperArm"
$bonemerge "ValveBiped.Bip01_R_Forearm"
$bonemerge "ValveBiped.SparkL_1"
$bonemerge "ValveBiped.SparkR_1"


$sequence "idle01" {
	"v_katana_anims\idle01.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$sequence "draw" {
	"v_katana_anims\draw.smd"
	activity "ACT_VM_DRAW" 1
	{ event 5004 1 "Katana.Equip" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 45
}

$sequence "misscenter1" {
	"v_katana_anims\misscenter1.smd"
	activity "ACT_VM_MISSCENTER" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 60
	loop
}

$sequence "misscenter2" {
	"v_katana_anims\misscenter2.smd"
	activity "ACT_VM_MISSCENTER2" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 60
	loop
}

$sequence "hitcenter1" {
	"v_katana_anims\hitcenter1.smd"
	activity "ACT_VM_HITCENTER" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 60
	loop
}

$sequence "hitcenter2" {
	"v_katana_anims\hitcenter2.smd"
	activity "ACT_VM_HITCENTER2" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 60
	loop
}

$sequence "hitkill1" {
	"v_katana_anims\hitkill1.smd"
	activity "ACT_VM_HITKILL" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 60
	loop
}

$sequence "holster" {
	"v_katana_anims\holster.smd"
	activity "ACT_VM_HOLSTER" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}
