//
// legacy support functions
//

// functions to handle
//CreateVehicle()
//DestroyVehicle()
//SetVehiclePos()
//SetVehicleZAngle()
//SetVehicleParamsEx()
//GetVehicleParamsEx()
//AddVehicleComponent()
//RemoveVehicleComponent()
//ChangeVehicleColor()
//ChangeVehiclePaintjob()
//SetVehicleHealth()
//GetVehicleHealth()
//SetVehicleNumberPlate()
//RepairVehicle()
//UpdateVehicleDamageStatus()
//SetVehicleVirtualWorld()
//GetVehicleVirtualWorld()

public rvCreateVehicle(modelid, Float:x, Float:y, Float:z, Float:r, color1, color2, respawn_delay, addsiren)
{
	#pragma unused respawn_delay

	if(addsiren) addsiren = VEHSTATUS_SIREN;

	new vehstatid = vehicle_create
	(
		randomNumberPlate(random(100000), 9), // seed, number of chars (always send 9)
		modelid, x, y, z, r, color1, color2, "", VEHSTATUS_GUEST | addsiren
	);

	return vehicle_spawn
	(
		vehstatid
	);
}
#if defined CreateVehicle
#undef CreateVehicle
#endif
#define CreateVehicle rvCreateVehicle



