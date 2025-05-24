// Created by Crowbar 0.72

$modelname "Weapons/w_katana.mdl"

$bodygroup "studio"
{
	studio "w_katana_w_fblade_reference.smd"
}


$surfaceprop "weapon"

$contents "solid"

$illumposition 5.609 21.762 -2.523

$cdmaterials "models\Weapons\F_BLADE\"

$texturegroup "skinfamilies"
{
	{ "fire_blade_1024_256"          }
	{ "fire_blade_1024_256_bloodied" }
	{ "fire_blade_1024_256_bloodied_alien" }
}

$cbox 0 0 0 0 0 0

$bbox -5.5 -5.5 -11.233 5.5 5.5 44

$definebone "ValveBiped.Bip01_R_Hand" "" -0.631371 -6.659214 -2.082509 -69.697163 135.426259 -43.251609 0 0 0 0 0 0


$sequence "idle" {
	"w_katana_anims\idle.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$sequence "attack1" {
	"w_katana_anims\attack1.smd"
	activity "ACT_MELEE_ATTACK1" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$collisionmodel "w_katana_w_fblade_reference.smd"
{
	$mass 3
	$inertia 1
	$damping 0
	$rotdamping 0
	$rootbone " "

}
