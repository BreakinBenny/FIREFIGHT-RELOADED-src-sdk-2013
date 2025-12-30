// Created by Crowbar 0.72

$modelname "gibs/soldier_prisonguard_head.mdl"
$body studio "soldier_prisonguard_head_soldier_head_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition -0.651 -0.276 8.193

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -7.446 -5.748 1.216 7.996 4.446 15.171

$definebone "Props" "" 0.000006 0.780074 64.613853 -80.100012 -89.999873 89.99975 0 0 0 0 0 0

$sequence idle "soldier_prisonguard_head_anims\idle.smd"

$collisionmodel "soldier_prisonguard_head_soldier_head_reference.smd"
{
	$mass 2
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone " "
	$concave
	$maxconvexpieces 5

}

$keyvalues
{
	prop_data 
	{
		"base" "Flesh.Small"
	}
}