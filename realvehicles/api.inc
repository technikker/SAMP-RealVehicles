// =============================================================================
//									api.inc
// =============================================================================

// vehicleid == only works on spawned vehicles
// vehstatid == only works on connected vehicles
// vehname   == works on all vehicles, even offline ones
//
//	vehicleid
//	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
//	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
//	new vehstatid = vehicleLink[vehicleid];
//
//	vehstatid
//	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
//	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
//	new vehicleid = vehicleStat[vehstatid][vVehicleID];
//
//	vehname
//	if(!vehicle_exists(vehname)) return 0;

// TODO do some defines for getting variables from the stats, eg getVehicleModel





//-----------------------------------------------------------------------------

// creates and connects a new vehicle, return the vehstatid
// does not spawn the vehicle
// status (bitmask), value (price)
public vehicle_create(plate[], modelid, Float:park_x, Float:park_y, Float:park_z, Float:park_r, color1, color2, owner[], status)
{
	// check if the modelid is valid
	if(modelid < 400 || modelid >= 400+sizeof(modelidBasicPrice)) return INVALID_VEHSTAT_ID;

	// check the plate/name is valid
	if(isnull(plate)) return INVALID_VEHSTAT_ID;				// null is an invalid plate

	// no whitespace characters (space or tab) (use '_' instead)
	// invalid characters:  < > : \ / | * ? [ ] = % $ + , ; ~ "
	for(new i=0; plate[i]; i++) if(!validFileNameCharacter(plate[i])) return INVALID_VEHSTAT_ID;

	// return a fail if the plate is too long (more than 32 chars, not including null)
	for(new i=0; plate[i]!='\0'; i++) if(i>=32) return INVALID_VEHSTAT_ID;

	// process the plate into the vehicle name
	new name[MAX_VEHICLE_PLATE]; strcopy(name, sizeof(name), plate);
	removeEmbeddedColors(name); // remove embedded colours
	for(new i=0; name[i]!='\0'; i++) if(i>=9) return INVALID_VEHSTAT_ID; // return a fail if the name is too long (more than 9 chars, not including null)

	// check if name is already in use
	if(vehicle_exists(name)) return INVALID_VEHSTAT_ID;

	// get a new id
	new vehstatid = vehicleStat_newid();
	if(vehstatid == INVALID_VEHSTAT_ID){
		print("max vehicles exceeded!");
		return INVALID_VEHSTAT_ID;
	}

	// assign a value (price) for the vehicle
	new value = getVehicleModelPrice(modelid);

	// if we get this far, we have a valid plate that is not already used. set up the stats...
	vehicleStat[vehstatid][vVehicleID]			= INVALID_VEHICLE_ID;
	// general
	strcopy(vehicleStat[vehstatid][vName],		MAX_VEHICLE_NAME,	name);
	strcopy(vehicleStat[vehstatid][vPlate],		MAX_VEHICLE_PLATE,	plate);
	vehicleStat[vehstatid][vStatus]				= status;
	vehicleStat[vehstatid][vValue]				= value;
	vehicleStat[vehstatid][vModel]				= modelid;
	vehicleStat[vehstatid][vColor1]				= color1;
	vehicleStat[vehstatid][vColor2]				= color2;
	vehicleStat[vehstatid][vPaintJob]			= -1;
	vehicleStat[vehstatid][vMod00]				= 0;
	vehicleStat[vehstatid][vMod01]				= 0;
	vehicleStat[vehstatid][vMod02]				= 0;
	vehicleStat[vehstatid][vMod03]				= 0;
	vehicleStat[vehstatid][vMod04]				= 0;
	vehicleStat[vehstatid][vMod05]				= 0;
	vehicleStat[vehstatid][vMod06]				= 0;
	vehicleStat[vehstatid][vMod07]				= 0;
	vehicleStat[vehstatid][vMod08]				= 0;
	vehicleStat[vehstatid][vMod09]				= 0;
	vehicleStat[vehstatid][vMod10]				= 0;
	vehicleStat[vehstatid][vMod11]				= 0;
	vehicleStat[vehstatid][vMod12]				= 0;
	vehicleStat[vehstatid][vMod13]				= 0;
	vehicleStat[vehstatid][vMod14]				= 0;
	vehicleStat[vehstatid][vMod15]				= 0;
	vehicleStat[vehstatid][vMod16]				= 0;
	vehicleStat[vehstatid][vHealthMax]			= VehicleHardness(modelid);
	// position
	vehicleStat[vehstatid][vWorldID]			= 0;
	vehicleStat[vehstatid][vSpawned]			= 0;
	vehicleStat[vehstatid][vSpawnPositionX]		= park_x;
	vehicleStat[vehstatid][vSpawnPositionY]		= park_y;
	vehicleStat[vehstatid][vSpawnPositionZ]		= park_z;
	vehicleStat[vehstatid][vSpawnPositionR]		= park_r;
	vehicleStat[vehstatid][vParkPositionX]		= vehicleStat[vehstatid][vSpawnPositionX];
	vehicleStat[vehstatid][vParkPositionY]		= vehicleStat[vehstatid][vSpawnPositionY];
	vehicleStat[vehstatid][vParkPositionZ]		= vehicleStat[vehstatid][vSpawnPositionZ];
	vehicleStat[vehstatid][vParkPositionR]		= vehicleStat[vehstatid][vSpawnPositionR];
	// statistics
	vehicleStat[vehstatid][vLastDriver][0]		= '\0';
	strcopy(vehicleStat[vehstatid][vOwner],		MAX_PLAYER_NAME+1,	owner);
	vehicleStat[vehstatid][vKills]				= 0;
	vehicleStat[vehstatid][vDeath]				= 0;
	// the realtime variables
	vehicleStat[vehstatid][vParams]				= 0;
	vehicleStat[vehstatid][vHealth]				= vehicleStat[vehstatid][vHealthMax];
	vehicleStat[vehstatid][vDamageStatus0]		= 0;
	vehicleStat[vehstatid][vDamageStatus1]		= 0;
	vehicleStat[vehstatid][vDamageStatus2]		= 0;
	vehicleStat[vehstatid][vDamageStatus3]		= 0;
	// extended stats
	vehicleStat[vehstatid][vTotalDistance]		= 0;
	vehicleStat[vehstatid][vFuel]				= random( vehicleFuelTankCapacity(modelid) );
	vehicleStat[vehstatid][vFuelTypeInTank]		= getDefaultFuelType(modelid);

	// SAVE
	vehicledata_save(vehstatid);

	return vehstatid; // success
}

//-----------------------------------------------------------------------------

public vehicle_delete(name[])
{
	for(new i=0; name[i]; i++) if(i >= MAX_VEHICLE_NAME-1) return -1; // someone is trying to crash the database
	if(!vehicle_exists(name)) return 1; // vehicle must exist

	// if connected, disconnect
	new vehstatid = vehicleStat_getid(name);
	if(vehstatid != INVALID_VEHSTAT_ID){
		vehicle_disconnect(vehstatid);
	}

	// delete
	new query[55 + MAX_VEHICLE_NAME]; mysql_format(database[0], query, sizeof(query), "DELETE FROM vehicledata WHERE Name='%e' LIMIT 1;", name);
	//mysql_query(database[0], query, false);
	mysql_pquery(database[0], query, "", "");

	return 0; // success
}

//-----------------------------------------------------------------------------

// get values from vehicle and store in vehstatid
public vehicle_params_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES))			return 1;	// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID)	return 1;	// id not in use
	new vehstatid = vehicleLink[vehicleid];

	new engine;
	new lights;
	new alarm;
	new doors;
	new bonnet;
	new boot;
	new objective;
	new bool:unset = false;

	GetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID], engine, lights, alarm, doors, bonnet, boot, objective);

	// set the vars to what the new state will be
	if(engine == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_ENGINE; // if false turn off
	else if(engine == 0)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ENGINE; // if not "unset" turn on
	else if(engine == -1){ // if status is unset
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ENGINE;
	}

	if(lights == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_LIGHTS;
	else if(lights == 0)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LIGHTS;
	else if(lights == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LIGHTS;
	}

	if(alarm == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_ALARM;
	else if(alarm == 0)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ALARM;
	else if(alarm == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ALARM;
	}

	if(doors == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_LOCK;
	else if(doors == 0)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LOCK;
	else if(doors == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LOCK;
	}

	if(bonnet == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_BONNET;
	else if(bonnet == 0)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BONNET;
	else if(bonnet == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BONNET;
	}

	if(boot == 1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_BOOT;
	else if(boot == 0)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BOOT;
	else if(boot == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BOOT;
	}

	if(objective == 1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_OBJ;
	else if(objective == 0)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_OBJ;
	else if(objective == -1){
		unset = true;
		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_OBJ;
	}

	// set the new params
	if(unset){
	//	SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID],					vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS,	vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK,	vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT,	vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ);
		new bool:params[7] = {false, false, false, false, false, false, false};
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	params[0] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	params[1] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM)	params[2] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		params[3] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET)	params[4] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT)		params[5] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ)		params[6] = true;
		SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID], params[0], params[1],	params[2], params[3], params[4], params[5],	params[6]);
	}

	return 0; // SUCCESS
}

//-----------------------------------------------------------------------------

// 1 for on, 0 for off, and -1 to leave as is.
public vehicle_params_set(vehstatid, engine, lights, alarm, doors, bonnet, boot, objective)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return 1;	// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id not in use

	// set the vars to what the new state will be
	if(engine == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ENGINE; // if false turn off
	else if(engine != -1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_ENGINE; // if not "unset" turn on

	if(lights == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LIGHTS;
	else if(lights != -1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_LIGHTS;

	if(alarm == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_ALARM;
	else if(alarm != -1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_ALARM;

	if(doors == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_LOCK;
	else if(doors != -1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_LOCK;

	if(bonnet == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BONNET;
	else if(bonnet != -1)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_BONNET;

	if(boot == 0)				vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_BOOT;
	else if(boot != -1)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_BOOT;

	if(objective == 0)			vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] & ~VEHPARAM_OBJ;
	else if(objective != -1)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] | VEHPARAM_OBJ;

	// set the new params
	if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID){
	//	SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID],					vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS,	vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK,	vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT,	vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ);
		new bool:params[7] = {false, false, false, false, false, false, false};
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	params[0] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	params[1] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM)	params[2] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		params[3] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET)	params[4] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT)		params[5] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ)		params[6] = true;
		SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID], params[0], params[1],	params[2], params[3], params[4], params[5],	params[6]);
	}

	return 0; // SUCCESS
}

//-----------------------------------------------------------------------------

// pass a value of 1 to toggle a param, 0 to leave as was
public vehicle_params_toggle(vehstatid, engine, lights, alarm, doors, bonnet, boot, objective)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return 1;	// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id not in use

	// set the vars to what the new state will be
	if(engine)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_ENGINE;
	if(lights)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_LIGHTS;
	if(alarm)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_ALARM;
	if(doors)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_LOCK;
	if(bonnet)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_BONNET;
	if(boot)		vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_BOOT;
	if(objective)	vehicleStat[vehstatid][vParams] = vehicleStat[vehstatid][vParams] ^ VEHPARAM_OBJ;

	// set the new params
	if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID){
		new bool:params[7] = {false, false, false, false, false, false, false};
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	params[0] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	params[1] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM)	params[2] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		params[3] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET)	params[4] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT)		params[5] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ)		params[6] = true;
		SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID], params[0], params[1],	params[2], params[3], params[4], params[5],	params[6]);
	}

	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_params_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES))			return 1;	// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID)	return 1;	// id not in use
	new vehstatid = vehicleLink[vehicleid];

	// set the new params
	if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID){
	//	SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID],					vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS,	vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK,	vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET,
	//					   vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT,	vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ);
		new bool:params[7] = {false, false, false, false, false, false, false};
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	params[0] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	params[1] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM)	params[2] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		params[3] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET)	params[4] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT)		params[5] = true;
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ)		params[6] = true;
		SetVehicleParamsEx(vehicleStat[vehstatid][vVehicleID], params[0], params[1],	params[2], params[3], params[4], params[5],	params[6]);
	}

	return 0;
}

//-----------------------------------------------------------------------------

// 1==on
// 0==off
// -1==leave
// warning - using this could desync the vehicle on the client with the server AC
public vehicle_params_set_forplayer(vehicleid, playerid, doors, objective)
{
	if(!valid_index(vehicleid, MAX_VEHICLES))			return 1;	// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID)	return 1;	// id not in use

	new bool:params[2] = {false, false};
	if(doors == -1)		if(vehicleStat[vehicleLink[vehicleid]][vParams] & VEHPARAM_LOCK)	params[0] = true; // if "leave alone" set it to the server value
	else if(doors)		params[0] = true; // if true, turn it on
	if(objective == -1)	if(vehicleStat[vehicleLink[vehicleid]][vParams] & VEHPARAM_OBJ)		params[1] = true;
	else if(objective)	params[1] = true;

	// set the new params
	SetVehicleParamsForPlayer(vehicleid, playerid, params[1], params[0]);

	return 0; // SUCCESS
}

//-----------------------------------------------------------------------------

public vehicle_respawn(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	//new vehstatid = vehicleLink[vehicleid];

	SetVehicleToRespawn(vehicleid);
	
	// TODO
	// other stuff done by the callback
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_color_get(vehicleid, &color1, &color2)
{
	if(!valid_index(vehicleid, MAX_VEHICLES))			return -1;	// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID)	return 1;	// id not in use

	color1 = vehicleStat[vehicleLink[vehicleid]][vColor1];
	color2 = vehicleStat[vehicleLink[vehicleid]][vColor2];
	return 0;
}

//-----------------------------------------------------------------------------

// pass -2 as a colour to leave the colour as is
public vehicle_color_set(vehstatid, color1, color2)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return 1;	// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id not in use
	if(color1 >= -1  &&  color1 < 256) vehicleStat[vehstatid][vColor1] = color1;
	if(color2 >= -1  &&  color2 < 256) vehicleStat[vehstatid][vColor2] = color2;
	vehicle_color_update(vehicleStat[vehstatid][vVehicleID]);
	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_color_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES))			return 1;	// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID)	return 1;	// id not in use
	ChangeVehicleColor(vehicleid, vehicleStat[vehicleLink[vehicleid]][vColor1], vehicleStat[vehicleLink[vehicleid]][vColor2]);
	return 0;
}

//-----------------------------------------------------------------------------

// TODO fix stuff up from here

// there is no get feature for this
// 0, 1, 2 are paintjob id's, -1 or 3 will unset the paintjob
public vehicle_paintjob_set(vehstatid, paintjobid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	if(paintjobid<-1 || paintjobid>3) return 0;						// invalid PJ id
	if(paintjobid == 3) paintjobid = -1;
	vehicleStat[vehstatid][vPaintJob] = paintjobid;
	vehicle_paintjob_update(vehicleStat[vehstatid][vVehicleID]);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_paintjob_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new paintjobid = vehicleStat[vehicleLink[vehicleid]][vPaintJob];
	if(paintjobid == -1) paintjobid = 3;
	ChangeVehiclePaintjob(vehicleid, paintjobid);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_health_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	GetVehicleHealth(vehicleid, vehicleStat[vehstatid][vHealth]);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_health_set(vehstatid, Float:health)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	vehicleStat[vehstatid][vHealth] = health;

	if(vehicleid != INVALID_VEHICLE_ID){ // IF SPAWNED
		SetVehicleHealth(vehicleid, vehicleStat[vehstatid][vHealth]);
	}

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_health_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	SetVehicleHealth(vehicleid, vehicleStat[vehstatid][vHealth]);
	return 1;
}

//-----------------------------------------------------------------------------

// TODO
/*public vehicle_trailer_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	//new vehstatid = vehicleLink[vehicleid];

	return 0;
}

public vehicle_trailer_attach(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	//new vehstatid = vehicleLink[vehicleid];

	return 0;
}

public vehicle_trailer_detach(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	//new vehicleid = vehicleStat[vehstatid][vVehicleID];

	return 0;
}*/

//-----------------------------------------------------------------------------

// set the name and numberplate of a vehicle
// supports embedded colours
// pass to function the new numberplate (including embedded colours)
// function will set the numberplate and the name (the plate without embedded colours)
// return 1 if vehicle not valid, -1 if the newplate is not valid, and 0 if successful
public vehicle_name_set(vehstatid, newplate[])
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 1;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 1;	// id not in use
	// return a fail if the plate is too long (more than 32 chars, not including null)
	for(new i=0; newplate[i]!='\0'; i++){
		if(i >= MAX_VEHICLE_PLATE -1) return -1; // -1 to leave the last char as NULL
	}

	// process the plate into the vehicle name
	new newname[MAX_VEHICLE_PLATE];
	format(newname, sizeof(newname), newplate);
	removeEmbeddedColors(newname); // remove embedded colours
	// return a fail if the name is too long
	for(new i=0; newname[i]!='\0'; i++){
		if(i >= MAX_VEHICLE_NAME - 1) return -1;
	}

	// check that no invalid characters are used
	for(new i=0; newname[i]; i++){
		if(!validFileNameCharacter(newname[i])) return -1;
		if(newname[i] >= 32) continue;
		else break; // break on non printing characters
	}

	// change the plate to the new one
	strcopy(vehicleStat[vehstatid][vPlate], MAX_VEHICLE_PLATE, newplate);
	
	// change the vehicle name in the player data
	new playerid = getPlayerID(vehicleStat[vehstatid][vOwner]);
	if(playerid != INVALID_PLAYER_ID)
	{
		player_vehicle_remove(playerStat[playerid], vehicleStat[vehstatid][vName]);
		player_vehicle_add(playerStat[playerid], newname);
	}
	// todo update offline player

	// rename in stored data as well as memory
	vehicledata_rename(vehstatid, newname);
	
	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_model_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	// vehicle is spawned
	vehicleStat[vehstatid][vModel] = GetVehicleModel(vehicleid); // get the model of spawned vehicle
	return vehicleStat[vehstatid][vModel];
}

//-----------------------------------------------------------------------------

public vehicle_model_set(vehstatid, modelid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];
	if(modelid<400 || modelid>=612) return 0; // modelid out of range

	// if vehicle is spawned
	if(vehicleStat[vehstatid][vSpawned]){

		new Float:xPos;
		new Float:yPos;
		new Float:zPos;
		new Float:rPos;
		GetVehiclePos(vehicleid, xPos, yPos, zPos);
		GetVehicleZAngle(vehicleid, rPos);

		vehicle_despawn(vehstatid); // despawn
		vehicleStat[vehstatid][vModel] = modelid; // change model
		vehicle_spawnEx(vehstatid, xPos, yPos, zPos, rPos); // respawn
	}
	else{
		vehicleStat[vehstatid][vModel] = modelid;
	}

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_model_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	if(vehicleStat[vehstatid][vSpawned]){
		new Float:xPos;
		new Float:yPos;
		new Float:zPos;
		new Float:rPos;
		GetVehiclePos(vehicleid, xPos, yPos, zPos);
		GetVehicleZAngle(vehicleid, rPos);

		vehicle_despawn(vehstatid); // despawn
		vehicle_spawnEx(vehstatid, xPos, yPos, zPos, rPos); // respawn

		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_repair(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	//new vehicleid = vehicleStat[vehstatid][vVehicleID];

	if(vehicle_health_set(vehstatid, 1000.0) && vehicle_damagestatus_set(vehstatid, 0, 0, 0, 0)){
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_damagestatus_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	GetVehicleDamageStatus(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
						   vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_damagestatus_set(vehstatid, panels, doors, lights, tires)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	vehicleStat[vehstatid][vDamageStatus0] = panels;
	vehicleStat[vehstatid][vDamageStatus1] = doors;
	vehicleStat[vehstatid][vDamageStatus2] = lights;
	vehicleStat[vehstatid][vDamageStatus3] = lights;

	if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID){
		UpdateVehicleDamageStatus(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
								  vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);
	}
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_damagestatus_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	UpdateVehicleDamageStatus(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
							  vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_virtualworld_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	vehicleStat[vehstatid][vWorldID] = GetVehicleVirtualWorld(vehicleStat[vehstatid][vVehicleID]);
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_virtualworld_set(vehstatid, worldid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	vehicleStat[vehstatid][vWorldID] = worldid;

	if(vehicleStat[vehstatid][vSpawned]){
		SetVehicleVirtualWorld(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vWorldID]);
	}
	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_virtualworld_update(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];
	SetVehicleVirtualWorld(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vWorldID]);
	return 1;
}

//-----------------------------------------------------------------------------





// -----------------------------------------------------------------------------
//
//		 Extended API (features that are introduced by this include)
//
// -----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

public vehicle_connect(name[])
{
	// check if vehicle is already connected
	new vehstatid = vehicleStat_getid(name);
	if(vehstatid != INVALID_VEHSTAT_ID){
		return vehstatid; // return the vehstatid of the vehicle (that is already connected)
	}

	// attempt to load the vehicle
	vehstatid = vehicledata_load(name);
	if(vehstatid == INVALID_VEHSTAT_ID) return INVALID_VEHSTAT_ID; // if we failed to connect
	
	// if marked as spawned, spawn it
	if(vehicleStat[vehstatid][vSpawned]) vehicle_spawn(vehstatid);

	// TODO check vars are in range and fix if they are not
	//printf("owner -%s-", vehicleStat[vehstatid][vOwner]);

	return vehstatid; // return the id
}

//-----------------------------------------------------------------------------

public vehicle_disconnect(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 1;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 1;	// id not in use

	// if spawned
	if(vehicleStat[vehstatid][vSpawned]){
		vehicle_despawn(vehstatid);
		//vehicleStat[vehstatid][vSpawned] = 1; // set it to spawned so it will spawn automatically
	}

	// if vehicle has an owner
	if(vehicleStat[vehstatid][vOwner][0])
	{
		new playerid = getPlayerID(vehicleStat[vehstatid][vOwner]);
		// if that owner is connected
		if(playerid != INVALID_PLAYER_ID) playerStat_pVehStatIds_clear(playerid); // clear the list of player vehstatids since we just messed it up
	}
	
	// save and clear from memory
	vehicledata_save(vehstatid);
	vehicleStat_clear(vehstatid);

	//printf("Vehicle %s left the server!", vehicleStat[vehstatid][vName]);

	return 0;
}

//-----------------------------------------------------------------------------

public vehicle_spawn(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return INVALID_VEHICLE_ID;			// vehstatid out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return INVALID_VEHICLE_ID;	// vehstatid not in use
	
	// we can't spawn if already spawned
	if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID) return vehicleStat[vehstatid][vVehicleID];
	
	// we can't spawn if the health is too low!
	if(vehicleStat[vehstatid][vHealth] < 250.0) vehicleStat[vehstatid][vHealth] = VehicleHardness(vehicleStat[vehstatid][vModel]);

	// create a vehicle at this vehicle's park location
	new vehicleid;
	
	// if train must use AddStaticVehicleEx, as CreateVehicle wont work for it
	if(IsTrain(vehicleStat[vehstatid][vModel]))
	{
		vehicleid = AddStaticVehicleEx
		(
			vehicleStat[vehstatid][vModel],			vehicleStat[vehstatid][vParkPositionX],	vehicleStat[vehstatid][vParkPositionY],
			vehicleStat[vehstatid][vParkPositionZ],	vehicleStat[vehstatid][vParkPositionR],	vehicleStat[vehstatid][vColor1],
			vehicleStat[vehstatid][vColor2],		VEHICLE_RESPAWN_DELAY,					vehicleStat[vehstatid][vStatus] & VEHSTATUS_SIREN
		);
	}
	
	// otherwise use CreateVehicle as AddStaticVehicleEx can't create vehicles outside of OnGameModeInit
	else
	{
		vehicleid = CreateVehicle
		(
			vehicleStat[vehstatid][vModel],			vehicleStat[vehstatid][vParkPositionX],	vehicleStat[vehstatid][vParkPositionY],
			vehicleStat[vehstatid][vParkPositionZ],	vehicleStat[vehstatid][vParkPositionR],	vehicleStat[vehstatid][vColor1],
			vehicleStat[vehstatid][vColor2],		VEHICLE_RESPAWN_DELAY,					vehicleStat[vehstatid][vStatus] & VEHSTATUS_SIREN
		);
	}

	// link from the vehicleid and the vehstatid
	vehicleLink[vehicleid] = vehstatid;

	// if spawn failed
	if(vehicleid == INVALID_VEHICLE_ID){
		// undo the link between the vehicleid and the vehstatid
		vehicleLink[vehicleid] = INVALID_VEHSTAT_ID;
		print("fail");
		return INVALID_VEHICLE_ID; // return fail
	}

	// link from vehstatid to vehicleid
	vehicleStat[vehstatid][vVehicleID] = vehicleid;

	SetVehicleNumberPlate(vehicleid, vehicleStat[vehstatid][vPlate]); // set the number plate
	//SetVehicleNumberPlate(vehicleid, "test{ff0000} 010"); // ~r~ type embedded colours don't work with SetVehicleNumberPlate
	vehicleStat[vehstatid][vSpawned] = 1; // spawned is true
	SetVehicleToRespawn(vehicleid); // show the number plate

	// use the callback OnVehicleSpawn to set the rest of the vehicle stats to the vehicle

	return vehicleid;
}

//-----------------------------------------------------------------------------

// spawn at a position
public vehicle_spawnEx(vehstatid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:spawn_r)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	vehicleStat[vehstatid][vParkPositionX]		= spawn_x;
	vehicleStat[vehstatid][vParkPositionY]		= spawn_y;
	vehicleStat[vehstatid][vParkPositionZ]		= spawn_z;
	vehicleStat[vehstatid][vParkPositionR]		= spawn_r;

	return vehicle_spawn(vehstatid);
}

//-----------------------------------------------------------------------------

public vehicle_despawn(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;		// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	// park
	vehicle_virtualworld_get(vehicleid);
	vehicle_health_get(vehicleid);
	vehicle_damagestatus_get(vehicleid);
	vehicle_params_get(vehicleid);
	GetVehiclePos(vehicleid, vehicleStat[vehstatid][vParkPositionX], vehicleStat[vehstatid][vParkPositionY], vehicleStat[vehstatid][vParkPositionZ]);
	GetVehicleZAngle(vehicleid, vehicleStat[vehstatid][vParkPositionR]);

	// despawn
	vehicleStat[vehstatid][vSpawned] = 0; // not spawned anymore
	vehicleStat[vehstatid][vVehicleID] = INVALID_VEHICLE_ID;
	vehicleLink[vehicleid] = INVALID_VEHSTAT_ID;
	DestroyVehicle(vehicleid);

	//vehicledata_save(vehstatid);

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_park(vehstatid) // stationary position, also save the vehicle color
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(valid_modelid(vehicleStat[vehstatid][vModel])) return 0;		// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	vehicle_virtualworld_get(vehicleid);
	vehicle_health_get(vehicleid);
	vehicle_damagestatus_get(vehicleid);
	vehicle_params_get(vehicleid);

	GetVehiclePos(vehicleid, vehicleStat[vehstatid][vParkPositionX], vehicleStat[vehstatid][vParkPositionY], vehicleStat[vehstatid][vParkPositionZ]);
	GetVehicleZAngle(vehicleid, vehicleStat[vehstatid][vParkPositionR]);

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_parkEx(vehstatid, Float:park_x, Float:park_y, Float:park_z, Float:park_r)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(valid_modelid(vehicleStat[vehstatid][vModel])) return 0;		// id not in use

	vehicleStat[vehstatid][vParkPositionX]	= park_x;
	vehicleStat[vehstatid][vParkPositionY]	= park_y;
	vehicleStat[vehstatid][vParkPositionZ]	= park_z;
	vehicleStat[vehstatid][vParkPositionR]	= park_r;

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_kill(vehicleid) // blow it up
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	vehicleStat[vehstatid][vHealth] = 0.0;
	vehicle_health_update(vehicleid);

	return 1;
}

//-----------------------------------------------------------------------------

public vehicle_reposition(vehicleid, Float:x, Float:y, Float:z, Float:r)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return 0;				// index out of range
	if(vehicleLink[vehicleid] == INVALID_VEHSTAT_ID) return 0;		// id not in use
	new vehstatid = vehicleLink[vehicleid];

	vehicleStat[vehstatid][vParkPositionX] = x;
	vehicleStat[vehstatid][vParkPositionY] = y;
	vehicleStat[vehstatid][vParkPositionZ] = z;
	vehicleStat[vehstatid][vParkPositionR] = r;

	SetVehiclePos(vehicleid, x, y, z);
	SetVehicleZAngle(vehicleid, r);

	return 1;
}

//-----------------------------------------------------------------------------

// TODO (has checks to keep vars withing bounds)
/*public vehicle_fuel_set(vehstatid, Float:fuel)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	return 0;
}*/

//-----------------------------------------------------------------------------

/*public vehicle_fueltype_set(vehstatid, fuelTypeID)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	return 0;
}*/

//-----------------------------------------------------------------------------

public vehicle_maxhealth_set(vehstatid, Float:maxhealth)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	if(maxhealth<0) return 0;
	vehicleStat[vehstatid][vHealthMax] = maxhealth;
	return 1;
}

//-----------------------------------------------------------------------------

// TODO fix these
// return 0 for success
// vehicle must be online, player does not need to be
public vehicle_owner_set(vehstatid, playername[])
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 1;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 1;	// id not in use

	// check is real player
	if(!playerdata_exists(playername)) return 1; // player not real

	// set vehicle to player
	offline_player_vehicle_give(playername, vehicleStat[vehstatid][vName]);
	// set player to vehicle
	strcpy(vehicleStat[vehstatid][vOwner], playername, MAX_PLAYER_NAME);
	
	vehicledata_save(vehstatid); // save vehicle data or player will lose vehicle if server crashes

	return 0;
}



// vehicle must be online, but player does not need to be.
// return 0 for success
public vehicle_owner_unset(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 1;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 1;	// id not in use
	if(isnull(vehicleStat[vehstatid][vOwner])) return 1; // no owner

	// remove vehicle from player stats
	offline_player_vehicle_sold(vehicleStat[vehstatid][vOwner], vehicleStat[vehstatid][vName], 0);
	// remove player from vehicle stats
	vehicleStat[vehstatid][vOwner][0] = '\0';

	return 0;
}



// return true if a vehicle exists
public vehicle_exists(name[])
{
	return vehicledata_exists(name);
}



public vehicleid_get(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return INVALID_VEHSTAT_ID; // index out of range
	return vehicleStat[vehstatid][vVehicleID];
}



public vehstatid_get(vehicleid)
{
	if(!valid_index(vehicleid, MAX_VEHICLES)) return INVALID_VEHICLE_ID; // index out of range
	return vehicleLink[vehicleid];
}



