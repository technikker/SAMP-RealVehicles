// =============================================================================
//                             car mods API INCLUDE
//                               modding_api.inc
// =============================================================================

// TODO some vehicle components just don't work, and there may not be a fix.

// call when a component is added to a vehicle by the game or another script (updates the vehicle stats).
public vehicle_component_added(vehstatid, componentid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	//new componenttype = GetVehicleComponentType(componentid);
	//switch(componenttype){
	//}

	for(new s=0; s<20; s++) {
		if(componentid == vcomp_spoiler[s]) {
			vehicleStat[vehstatid][vMod00] = componentid;
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_bscoop[s]) {
			vehicleStat[vehstatid][vMod01] = componentid;
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_rscoop[s]) {
			vehicleStat[vehstatid][vMod02] = componentid;
			return 1;
		}
	}
	// the next two are linked (left and right side skirts) if we see the one side, add the other too
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_lskirt[s]) {
			vehicleStat[vehstatid][vMod03] = vcomp_lskirt[s];
			vehicleStat[vehstatid][vMod04] = vcomp_rskirt[s];
			return 1;
		}
	}
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_rskirt[s]) {
			vehicleStat[vehstatid][vMod04] = vcomp_rskirt[s];
			vehicleStat[vehstatid][vMod03] = vcomp_lskirt[s];
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_lights[s]) {
			vehicleStat[vehstatid][vMod05] = componentid;
			return 1;
		}
	}
	for(new s=0; s<3; s++) {
		if(componentid == vcomp_nitro[s]) {
			vehicleStat[vehstatid][vMod06] = componentid;
			return 1;
		}
	}
	for(new s=0; s<28; s++) {
		if(componentid == vcomp_exhaust[s]) {
			vehicleStat[vehstatid][vMod07] = componentid;
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_wheels[s]) {
			vehicleStat[vehstatid][vMod08] = componentid;
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_base[s]) {
			vehicleStat[vehstatid][vMod09] = componentid;
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_hydraulics[s]) {
			vehicleStat[vehstatid][vMod10] = componentid;
			return 1;
		}
	}
	for(new s=0; s<23; s++) {
		if(componentid == vcomp_fbumper[s]) {
			vehicleStat[vehstatid][vMod11] = componentid;
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_fbbars[s]) {
			vehicleStat[vehstatid][vMod12] = componentid;
			return 1;
		}
	}
	for(new s=0; s<22; s++) {
		if(componentid == vcomp_rbumper[s]) {
			vehicleStat[vehstatid][vMod13] = componentid;
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_rbbars[s]) {
			vehicleStat[vehstatid][vMod14] = componentid;
			return 1;
		}
	}
	// these two are linked
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventr[s]) {
			vehicleStat[vehstatid][vMod15] = vcomp_bventr[s];
			vehicleStat[vehstatid][vMod16] = vcomp_bventl[s]; // if we see the right component, add the left one as well
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventl[s]) {
			vehicleStat[vehstatid][vMod16] = vcomp_bventl[s];
			vehicleStat[vehstatid][vMod15] = vcomp_bventr[s]; // opposite as above
			return 1;
		}
	}

	return 0;
}



public vehicle_component_add(vehstatid, componentid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	//new componenttype = GetVehicleComponentType(componentid);
	//switch(componenttype){
	//}

	for(new s=0; s<20; s++) {
		if(componentid == vcomp_spoiler[s]) {
			vehicleStat[vehstatid][vMod00] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_bscoop[s]) {
			vehicleStat[vehstatid][vMod01] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_rscoop[s]) {
			vehicleStat[vehstatid][vMod02] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_lskirt[s]) {
			vehicleStat[vehstatid][vMod03] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_rskirt[s]) {
			vehicleStat[vehstatid][vMod04] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_lights[s]) {
			vehicleStat[vehstatid][vMod05] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<3; s++) {
		if(componentid == vcomp_nitro[s]) {
			vehicleStat[vehstatid][vMod06] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<28; s++) {
		if(componentid == vcomp_exhaust[s]) {
			vehicleStat[vehstatid][vMod07] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_wheels[s]) {
			vehicleStat[vehstatid][vMod08] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_base[s]) {
			vehicleStat[vehstatid][vMod09] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_hydraulics[s]) {
			vehicleStat[vehstatid][vMod10] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<23; s++) {
		if(componentid == vcomp_fbumper[s]) {
			vehicleStat[vehstatid][vMod11] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_fbbars[s]) {
			vehicleStat[vehstatid][vMod12] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<22; s++) {
		if(componentid == vcomp_rbumper[s]) {
			vehicleStat[vehstatid][vMod13] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_rbbars[s]) {
			vehicleStat[vehstatid][vMod14] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventr[s]) {
			vehicleStat[vehstatid][vMod15] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventl[s]) {
			vehicleStat[vehstatid][vMod16] = componentid;
			if(vehicleid != INVALID_VEHICLE_ID) AddVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}

	return 0;
}



// get all the components the vehicle has and update the vehstatid with this information.
// try to avoid using this, the native function GetVehicleComponentInSlot has known bugs!
public vehicle_component_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	vehicleStat[vehstatid][vMod00] = GetVehicleComponentInSlot(vehicleid, 0);
	vehicleStat[vehstatid][vMod01] = GetVehicleComponentInSlot(vehicleid, 1);
	vehicleStat[vehstatid][vMod02] = GetVehicleComponentInSlot(vehicleid, 2);
	vehicleStat[vehstatid][vMod03] = GetVehicleComponentInSlot(vehicleid, 3);
	vehicleStat[vehstatid][vMod04] = GetVehicleComponentInSlot(vehicleid, 3);
	vehicleStat[vehstatid][vMod05] = GetVehicleComponentInSlot(vehicleid, 4);
	vehicleStat[vehstatid][vMod06] = GetVehicleComponentInSlot(vehicleid, 5);
	vehicleStat[vehstatid][vMod07] = GetVehicleComponentInSlot(vehicleid, 6);
	vehicleStat[vehstatid][vMod08] = GetVehicleComponentInSlot(vehicleid, 7);
	vehicleStat[vehstatid][vMod09] = GetVehicleComponentInSlot(vehicleid, 8);
	vehicleStat[vehstatid][vMod10] = GetVehicleComponentInSlot(vehicleid, 9);
	vehicleStat[vehstatid][vMod11] = GetVehicleComponentInSlot(vehicleid, 10);
	vehicleStat[vehstatid][vMod12] = GetVehicleComponentInSlot(vehicleid, 10);
	vehicleStat[vehstatid][vMod13] = GetVehicleComponentInSlot(vehicleid, 11);
	vehicleStat[vehstatid][vMod14] = GetVehicleComponentInSlot(vehicleid, 11);
	vehicleStat[vehstatid][vMod15] = GetVehicleComponentInSlot(vehicleid, 12);
	vehicleStat[vehstatid][vMod16] = GetVehicleComponentInSlot(vehicleid, 13);

	return 1;
}



// TODO side skirts don't work properly
// update the vehicle components to what is stored in the vehicle stats.
// returns -1 if vehicle is not spawned, else returns the number of components added
// to the vehicle.  Call this whenever a vehicle is spawned.
public vehicle_component_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	new componentsAdded = 0;
	if(vehicleStat[vehstatid][vMod00] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod01] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod01]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod02] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod02]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod03] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod03]);
		componentsAdded++;
		//printf("left side skirt %d", vehicleStat[vehstatid][vMod03]);
	}
	if(vehicleStat[vehstatid][vMod04] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod04]);
		componentsAdded++;
		//printf("right side skirt %d", vehicleStat[vehstatid][vMod04]);
	}
	if(vehicleStat[vehstatid][vMod05] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod05]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod06] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod06]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod07] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod07]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod08] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod08]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod09] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod09]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod10] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod10]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod11] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod11]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod12] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod12]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod13] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod13]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod14] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod14]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod15] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod15]);
		componentsAdded++;
	}
	if(vehicleStat[vehstatid][vMod16] != 0) {
		AddVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod16]);
		componentsAdded++;
	}

	//if(componentsAdded > 0){
	//	printf("name of modded vehicle -%s-", vehicleStat[vehstatid][vName]);
	//}

	return componentsAdded;
}



public vehicle_component_remove(vehstatid, componentid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	for(new s=0; s<20; s++) {
		if(componentid == vcomp_spoiler[s]) {
			vehicleStat[vehstatid][vMod00] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_bscoop[s]) {
			vehicleStat[vehstatid][vMod01] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_rscoop[s]) {
			vehicleStat[vehstatid][vMod02] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_lskirt[s]) {
			vehicleStat[vehstatid][vMod03] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<21; s++) {
		if(componentid == vcomp_rskirt[s]) {
			vehicleStat[vehstatid][vMod04] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_lights[s]) {
			vehicleStat[vehstatid][vMod05] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<3; s++) {
		if(componentid == vcomp_nitro[s]) {
			vehicleStat[vehstatid][vMod06] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<28; s++) {
		if(componentid == vcomp_exhaust[s]) {
			vehicleStat[vehstatid][vMod07] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<17; s++) {
		if(componentid == vcomp_wheels[s]) {
			vehicleStat[vehstatid][vMod08] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_base[s]) {
			vehicleStat[vehstatid][vMod09] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<1; s++) {
		if(componentid == vcomp_hydraulics[s]) {
			vehicleStat[vehstatid][vMod10] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<23; s++) {
		if(componentid == vcomp_fbumper[s]) {
			vehicleStat[vehstatid][vMod11] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_fbbars[s]) {
			vehicleStat[vehstatid][vMod12] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<22; s++) {
		if(componentid == vcomp_rbumper[s]) {
			vehicleStat[vehstatid][vMod13] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<4; s++) {
		if(componentid == vcomp_rbbars[s]) {
			vehicleStat[vehstatid][vMod14] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventr[s]) {
			vehicleStat[vehstatid][vMod15] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}
	for(new s=0; s<2; s++) {
		if(componentid == vcomp_bventl[s]) {
			vehicleStat[vehstatid][vMod16] = 0;
			if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, componentid);
			return 1;
		}
	}

	return 0;
}



public vehicle_component_removeall(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	if(vehicleStat[vehstatid][vMod00] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod01] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod02] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod03] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod04] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod05] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod06] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod07] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod08] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod09] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod10] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod11] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod12] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod13] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod14] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod15] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	if(vehicleStat[vehstatid][vMod16] != 0){
		if(vehicleid != INVALID_VEHICLE_ID) RemoveVehicleComponent(vehicleid, vehicleStat[vehstatid][vMod00]);
		vehicleStat[vehstatid][vMod00] = 0;
	}
	
	return 1;
}


