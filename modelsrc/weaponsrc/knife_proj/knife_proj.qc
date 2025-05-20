// Created by Crowbar 0.72

$modelname "weapons/knife_proj.mdl"

$bodygroup "studio"
{
	studio "knife_proj_w_Pulse_knife_reference.smd"
}


$surfaceprop "weapon"

$contents "solid"

$illumposition 0.081 -0.148 -0.066

$cdmaterials "models\weapons\knife\"

$cbox 0 0 0 0 0 0

$bbox 0 -0.716 -2.356 21.519 0.715 2.571

$sequence "idle" {
	"anims\idle.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 90
	loop
}

$sequence "throw" {
	"anims\throw.smd"
	activity "ACT_RANGE_ATTACK_THROW" 1
	fadein 0.2
	fadeout 0.2
	fps 90
	loop
}