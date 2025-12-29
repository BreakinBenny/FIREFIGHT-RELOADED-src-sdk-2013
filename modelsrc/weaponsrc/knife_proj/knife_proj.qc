// Created by Crowbar 0.72

$modelname "weapons/knife_proj.mdl"
$body studio "knife_proj_w_Pulse_knife_reference.smd"

$surfaceprop "weapon"
$contents "solid"

$illumposition 0.081 -0.148 -0.066

$cdmaterials "models\weapons\knife\"

$cbox 0 0 0 0 0 0

$bbox 0 -0.716 -2.356 21.519 0.715 2.571

$sequence idle "anims\idle.smd" ACT_IDLE 1
$sequence throw "anims\throw.smd" ACT_RANGE_ATTACK_THROW 1 fps 90 loop