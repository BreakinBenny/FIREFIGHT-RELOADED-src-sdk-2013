// Created by Crowbar 0.72

$modelname "gibs/police_head.mdl"
$body studio "police_head_reference.smd"

$surfaceprop "flesh"
$contents "solid"

$illumposition 0.058 -1.846 7.37

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -4.684 -4.495 0.797 8.374 4.607 13.939

$definebone "Props" "" -0.170348 0.859042 65.835785 0 0 89.999982 0 0 0 0 0 0

$sequence idle "police_head_anims\idle.smd"

$collisionmodel "police_head_reference.smd"
{
	$mass 2
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone " "
	$concave
	$maxconvexpieces 1

}

$keyvalues
{
	prop_data 
	{
		"base" "Flesh.Small"
	}
}