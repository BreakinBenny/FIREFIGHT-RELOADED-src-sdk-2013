// Created by Crowbar 0.72

$modelname "weapons/v_oicw_remake.mdl"

$bodygroup "studio"
{
	studio "v_ociw_remake_ref.smd"
}
$bodygroup "Hands"
{
	studio "c_arms_hev.smd"
}

$surfaceprop "default"

$contents "solid"

$illumposition -3.153 -21.059 -7.793

$cdmaterials "models\Weapons\oicw\"
$cdmaterials "models\Weapons\V_hand\"

$attachment "muzzle" "v_weapon.sg552_Parent" 0 5.07 -22.27 rotate 90 0 0
$attachment "1" "v_weapon.sg552_Parent" 0 6.4 -4.8 rotate 5 168 0

$cbox 0 0 0 0 0 0

$bbox -1.795 -10.215 -14.167 43.913 3.91 -1.419

$bonemerge "ValveBiped.Bip01_Spine4"
$bonemerge "ValveBiped.Bip01_L_Clavicle"
$bonemerge "ValveBiped.Bip01_L_UpperArm"
$bonemerge "ValveBiped.Bip01_L_Forearm"
$bonemerge "ValveBiped.Bip01_L_Hand"
$bonemerge "ValveBiped.Bip01_L_Finger4"
$bonemerge "ValveBiped.Bip01_L_Finger41"
$bonemerge "ValveBiped.Bip01_L_Finger42"
$bonemerge "ValveBiped.Bip01_L_Finger3"
$bonemerge "ValveBiped.Bip01_L_Finger31"
$bonemerge "ValveBiped.Bip01_L_Finger32"
$bonemerge "ValveBiped.Bip01_L_Finger2"
$bonemerge "ValveBiped.Bip01_L_Finger21"
$bonemerge "ValveBiped.Bip01_L_Finger22"
$bonemerge "ValveBiped.Bip01_L_Finger1"
$bonemerge "ValveBiped.Bip01_L_Finger11"
$bonemerge "ValveBiped.Bip01_L_Finger12"
$bonemerge "ValveBiped.Bip01_L_Finger0"
$bonemerge "ValveBiped.Bip01_L_Finger01"
$bonemerge "ValveBiped.Bip01_L_Finger02"
$bonemerge "ValveBiped.Bip01_R_Clavicle"
$bonemerge "ValveBiped.Bip01_R_UpperArm"
$bonemerge "ValveBiped.Bip01_R_Forearm"
$bonemerge "ValveBiped.Bip01_R_Hand"
$bonemerge "ValveBiped.Bip01_R_Finger4"
$bonemerge "ValveBiped.Bip01_R_Finger41"
$bonemerge "ValveBiped.Bip01_R_Finger42"
$bonemerge "ValveBiped.Bip01_R_Finger3"
$bonemerge "ValveBiped.Bip01_R_Finger31"
$bonemerge "ValveBiped.Bip01_R_Finger32"
$bonemerge "ValveBiped.Bip01_R_Finger2"
$bonemerge "ValveBiped.Bip01_R_Finger21"
$bonemerge "ValveBiped.Bip01_R_Finger22"
$bonemerge "ValveBiped.Bip01_R_Finger1"
$bonemerge "ValveBiped.Bip01_R_Finger11"
$bonemerge "ValveBiped.Bip01_R_Finger12"
$bonemerge "ValveBiped.Bip01_R_Finger0"
$bonemerge "ValveBiped.Bip01_R_Finger01"
$bonemerge "ValveBiped.Bip01_R_Finger02"

$sequence "idle" {
	"v_anims\idle.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$sequence "reload" {
	"v_anims\reload.smd"
	activity "ACT_VM_RELOAD" 1
	{ event 5004 10 "Weapon_MP5.Clipout" }
	{ event 5004 40 "Weapon_MP5.Clipin" }
	{ event 5004 76 "Weapon_MP5.Slideback" }
	fadein 0.2
	fadeout 0.2
	fps 37
}

$sequence "draw" {
	"v_anims\draw.smd"
	activity "ACT_VM_DRAW" 1
	fadein 0.2
	fadeout 0.2
	fps 37
}

$sequence "shoot1" {
	"v_anims\shoot1.smd"
	activity "ACT_VM_PRIMARYATTACK" 1
	{ event 6001 0 "0" }
	{ event AE_MUZZLEFLASH 0 "SMG1 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "357 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 35
}

$sequence "shoot2" {
	"v_anims\shoot2.smd"
	activity "ACT_VM_RECOIL1" 1
	{ event 6001 0 "0" }
	{ event AE_MUZZLEFLASH 0 "SMG1 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "357 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 35
}

$sequence "shoot3" {
	"v_anims\shoot3.smd"
	activity "ACT_VM_RECOIL2" 1
	{ event 6001 0 "0" }
	{ event AE_MUZZLEFLASH 0 "SMG1 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "357 MUZZLE" }
    { event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 35
}
