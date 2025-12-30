// Created by Crowbar 0.57

$modelname "blueheart.mdl"
$body axebane "heart.smd"

$texturegroup "astro"
{
	{ "blueheart" }
	{ "blueheart2" }
}

$surfaceprop "plastic"
$contents "solid"
$mostlyopaque
$cdmaterials "models\blueheart\"

$sequence idle "heart.smd" ACT_IDLE 1

$collisionmodel "heart.smd"
{
	$mass 150
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone "root"
}
