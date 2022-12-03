



CMD:veh1(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == 0) return 1;

	showDialog(vehicle, playerid, vehicleLink[vehicleid], "");

	return 1;
}



CMD:vrepair(playerid, params[])
{
	new idx,ammount,tmp[256];

	tmp = strtok(params, idx);

	// if not in a vehicle
	if(!IsPlayerInAnyVehicle(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must be in a vehicle to repair it!");
		return 1;
	}

	// if not at a petrol station
	if(!IsPlayerAtFuelStation(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must at a fuel station to repair!");
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	new vehstatid = vehicleLink[vehicleid];

	// if no params have been passed
	if(isnull(tmp)){
		// make them for it
		new playerCash = GetPlayerMoney(playerid);
		new invoice = floatround((vehicleStat[vehstatid][vDamage] * REPAIR_COST), floatround_round);
		if(playerCash-invoice < 0){ // if they can't afford
			SendClientMessage(playerid, COLOR_ERROR, "You can't afford to do this!");
			return 1;
		}
		GivePlayerMoney( playerid, (invoice * -1) );

		SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Now Repairing"); // tell the player they are being repaired
		repairVehicle(vehicleid, -1, 0);  // call the repair function
		return 1;
	}

	// if invalid params have been passed
	if(!isNumeric(tmp)){
		SendClientMessage(playerid, COLOR_ERROR, "Usage: /repair {percentage to repair}");
		return 1;
	}

	// if valid params have been passed
	ammount = strval(tmp);

	// if ammount is not a valid percentage
	if(ammount<0 || ammount>100){
		SendClientMessage(playerid, COLOR_ERROR, "{percentage to repair} must be an integer from 0 to 100!");
		return 1;
	}

	// make them for it
	new playerCash = GetPlayerMoney(playerid);
	new invoice = floatround( (ammount * REPAIR_COST), floatround_round );
	if(playerCash-invoice < 0){ // if they can't afford
		SendClientMessage(playerid, COLOR_ERROR, "You can't afford to do this!");
		return 1;
	}
	GivePlayerMoney( playerid, (invoice * -1) );

	// tell the player they are being repaired
	new string[256];
	format (string, sizeof (string), "Now repairing %d%%", ammount);
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);

	repairVehicle(vehicleid, ammount, 0); // call the repair function

	return 1;
}

CMD:vrefuel(playerid, params[])
{
	new idx;
	new tmp[256];

	new Float: ammount = -1.0;
	new fuelType = FUELTYPE_NONE;
	new reverse = false; // when set to true, removes fuel instead of adding it.

	new vehicleid = GetPlayerVehicleID(playerid);
	new vehstatid = vehicleLink[vehicleid];

	// if not in a vehicle
	if(!IsPlayerInAnyVehicle(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must be in a vehicle to refuel it!");
		return 1;
	}

	// if not at a petrol station
	if(!IsPlayerAtFuelStation(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must at a fuel station to refuel!");
		return 1;
	}


	// deal with the first param
	tmp = strtok(params, idx);

	// if no params has been passed
	if(isnull(tmp)){
		ammount = -1; // -1 will fill the tank to the top
	}
	// if params have been passed
	else{
		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /refuel {numerical ammount}");
			return 1;
		}

		ammount = strval(tmp); // convert the passed params to a value

		if(ammount<0){
			reverse = true; // if they entered a negative ammount, set reverse to true (to remove fuel from tank)
			ammount *= -1; // and invert ammount, because ammount must not be negative
		}
	}


	// deal with the second param
	tmp = strtok(params, idx);

	// if no params has been passed
	if(isnull(tmp)){
		fuelType = vehicleStat[vehstatid][vFuelTypeInTank]; // set the new fuel type to what is already in the tank
	}
	// if params have been passed for fuel type
	else{
		if(!strcmp (tmp, "battery", true)) fuelType = FUELTYPE_BATTERY; // if the passed param matches a fuel type
		else if(!strcmp (tmp, "regular", true)) fuelType = FUELTYPE_REGULAR;
		else if(!strcmp (tmp, "premium", true)) fuelType = FUELTYPE_PREMIUM;
		else if(!strcmp (tmp, "diesel", true)) fuelType = FUELTYPE_DIESEL;
		else if(!strcmp (tmp, "aviation", true)) fuelType = FUELTYPE_AVIATION;
		// if we don't recognize the param they passed
		else{
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /refuel {numerical ammount} {\"battery\" / \"regular\" / \"premium\" / \"diesel\" / \"aviation\"}");
			return 1;
		}
	}


	// check that the selected fuel type is compatible with their car
	if(!checkFuelVehicle(GetVehicleModel(vehicleid), fuelType)){
		new string[256];
		format (string, sizeof (string), "You cannot fill this vehicle with %s!", tmp);
		SendClientMessage(playerid, COLOR_ERROR, string);
		return 1;
	}


	// check if the player can pay for it
	new playerCash = GetPlayerMoney(playerid);
	new invoice = 0;
	new Float: fuelPrice = getFuelPrice(fuelType);
	new Float: tankCapacity = vehicleFuelTankCapacity(GetVehicleModel(vehicleid)); // find the tank capacity
	new Float: ammountToAdd;

	// if we are filling to a full tank
	if(ammount<0){
		// if we are to drain before filling
		if(fuelType != vehicleStat[vehstatid][vFuelTypeInTank]){
			ammountToAdd = tankCapacity; // add a full tank
		}
		else{
			ammountToAdd = tankCapacity - vehicleStat[vehstatid][vFuel]; // don't add whats already in the tank
		}
	}
	// if we are filling a set ammount
	else{
		ammountToAdd = ammount;

		// check for overfill and correct it
		if(fuelType != vehicleStat[vehstatid][vFuelTypeInTank]){ // if we are to drain before filling
			// if we are going to overfill
			if(ammount>tankCapacity){
				ammountToAdd = tankCapacity; // add a full tank
			}
		}
		else{
			// if we are going to overfill
			if(ammount > tankCapacity-vehicleStat[vehstatid][vFuelTypeInTank]){
				ammountToAdd = tankCapacity-vehicleStat[vehstatid][vFuelTypeInTank]; // add a full tank
			}
		}
	}

	invoice = floatround( (ammountToAdd * fuelPrice * REFUEL_COST), floatround_round );

	// if removing fuel, skip the payment part
	if(!reverse){
		if(playerCash-invoice < 0){ // if they can't afford
			SendClientMessage(playerid, COLOR_ERROR, "You can't afford to do this!");
			return 1;
		}

		GivePlayerMoney( playerid, (invoice * -1) ); // pay
	}


	// if we have selected a different fuel than what is already in the tank
	if(fuelType != vehicleStat[vehstatid][vFuelTypeInTank]){
		new string[256];
		format (string, sizeof (string), "The fuel you selected (%s) is different to the fuel in the tank. The tank will be emptied before it is refilled!", tmp);
		SendClientMessage(playerid, COLOR_ERROR, string);

		// set a timer to call the drainRefuel function
		refuelTimer[vehicleid] = SetTimerEx("drainRefuelVehicle", 100, 1, "dff", vehicleid, ammount, REFUEL_RATE );
	}
	else{
		refuelVehicle(vehicleid, ammount, 0, reverse); // call the refuel function
	}

	// set the fuel type to what we just put in the tank
	vehicleStat[vehstatid][vFuelTypeInTank] = fuelType;


	// tell the player they are being refueled
	new string[128];
	if(ammount<0){
		format(string, sizeof (string), "Now Refueling.");
	}
	else{
		if(reverse) ammount *= -1;
		format(string, sizeof (string), "Now Refueling %.0f Litres.", ammount);
	}
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);

	return 1;
}



CMD:vconnect(playerid, params[])
{
	#pragma unused playerid
	new vehstatid = vehicle_connect(params);

	//vehicleStat_logdump(vehstatid); // don't log dump, its not nice

	if(vehstatid==INVALID_VEHSTAT_ID){
		SendClientMessage(playerid, COLOR_ERROR, "Error: Vehicle does not exist, cannot connect!");
	}
	else{
		new string[256];
		format(string, sizeof(string), "Vehicle %s has been connected as statsID %d!", params, vehstatid);
		SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);
	}

	return 1;
}

CMD:vspawn(playerid, params[])
{
	#pragma unused playerid
	new vehstatid = strval(params);
	new vehicleid = vehicle_spawn(vehstatid);

	if(vehstatid==INVALID_VEHICLE_ID){
		SendClientMessage(playerid, COLOR_ERROR, "Error: Vehicle does not exist, cannot spawn!");
	}
	else{
		new string[256];
		format(string, sizeof(string), "Vehicle %s has been spawned as vehicleID %d!", params, vehicleid);
		SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);
	}

	return 1;
}



CMD:vgetid(playerid, params[])
{
	#pragma unused params

	if(!IsPlayerInAnyVehicle(playerid)){

		new string[256];
		format(string, sizeof(string), "You must be in a vehicle!");
		SendClientMessage(playerid, COLOR_ERROR, string);
		return 1;
	}

	new vehid = GetPlayerVehicleID(playerid);
	new vehstatid = vehicleLink[vehid];

	new string[256];
	format(string, sizeof(string), "vehicleID=%d, statsID=%d", vehid, vehstatid);
	SendClientMessage(playerid, COLOR_ERROR, string);

	//SetVehicleParamsForPlayer(vehid, playerid, 1, 0);

	return 1;
}



CMD:vfuelrescue(playerid, params[])
{
	#pragma unused params

	new vehid = GetPlayerVehicleID(playerid);

	new vehstatid = vehicleLink[vehid];

	// add 5 litres
	vehicleStat[vehstatid][vFuel] += 5;

	return 1;
}



CMD:vsaveall(playerid, params[])
{
	#pragma unused playerid
	#pragma unused params

	//if(!IsPlayerAdmin(playerid)) return 1;

	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++){
		if(vehicleStat[vehstatid][vModel]!=0){
			vehicledata_save(vehstatid);
		}
	}

	return 1;
}



// command /v
// params
// e = engine
// l = lights
// a = alarm
// d = doors
// h = hood
// b = boot
CMD:v(playerid, params[])
{
	new param1[20]; // would make this smaller (it only needs to be about 8), but strtok function will give us errors if we do :P
	new param2[20];
	new param2_val; // -1=toggle, 0=turn off, 1=turn on


	// if not in a vehicle
	if(!IsPlayerInAnyVehicle(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must be in a vehicle to do this!");
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);

	// scan the params
	sscanf(params, "s[20]s[20]", param1, param2);


	// if no params has been passed for the first param
	if( sscanf(params, "s[20] ", param1) ){
		vehicle_params_toggle(vehicleLink[vehicleid], 1, 0, 0, 0, 0, 0, 0); // toggle the engine

		// -TODO- add code to close boot, bonnet.

		return 1; // we have finnished
	}


	// param2
	// if param2 not a number
	if( sscanf(param2, "d", param2_val)){

		// if not a string either, no params have been passed
		if(isnull(param2)){
			param2_val = -1;
		}

		// if it is a string, check it
		else{
			if	 (strcmp(param2, "true", true) == 0)  param2_val = 1;
			else if(strcmp(param2, "false", true) == 0) param2_val = 0;
			else{
				SendClientMessage(playerid, COLOR_ERROR, "Usage: /v {param to set} {bool on/off}");
				return 1; // we have finnished
			}
		}
	}
	// parma2_val has been set

	// check param2_val is within its boundaries
	if(param2_val!=-1 && param2_val!=0 && param2_val!=1){
		SendClientMessage(playerid, COLOR_ERROR, "Usage: /v {param to set} {bool on/off}");
		return 1; // we have finnished
	}


	// if set to toggle
	if(param2_val == -1){
		if	 ( !strcmp(param1, "e", true) || !strcmp(param1, "engine", true) ){
			vehicle_params_toggle(vehicleLink[vehicleid], 1, 0, 0, 0, 0, 0, 0); // toggle the engine
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle engine toggled!");
		}
		else if( !strcmp(param1, "l", true) || !strcmp(param1, "lights", true)){
			vehicle_params_toggle(vehicleLink[vehicleid], 0, 1, 0, 0, 0, 0, 0); // toggle the lights
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle lights toggled!");
		}
		else if( !strcmp(param1, "a", true) || !strcmp(param1, "alarm", true)){
			vehicle_params_toggle(vehicleLink[vehicleid], 0, 0, 1, 0, 0, 0, 0); // toggle the alarm
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle alarm toggled!");
		}
		else if( !strcmp(param1, "d", true) || !strcmp(param1, "doors", true)){
			vehicle_params_toggle(vehicleLink[vehicleid], 0, 0, 0, 1, 0, 0, 0); // toggle the door lock
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle door lock toggled!");
		}
		else if( !strcmp(param1, "h", true) || !strcmp(param1, "hood", true)){
			vehicle_params_toggle(vehicleLink[vehicleid], 0, 0, 0, 0, 1, 0, 0); // toggle the bonnet
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle bonnet toggled!");
		}
		else if( !strcmp(param1, "b", true) || !strcmp(param1, "boot", true)){
			vehicle_params_toggle(vehicleLink[vehicleid], 0, 0, 0, 0, 0, 1, 0); // toggle the boot
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle boot toggled!");
		}

		else{ // they didn't enter a valid param
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /v {param to set}");
			//return 1; // we are done here
		}
	}

	else{ // if we are were given a second param
		if	 ( !strcmp(param1, "e", true) || !strcmp(param1, "engine", true)){
			vehicle_params_set(vehicleLink[vehicleid], param2_val, -1, -1, -1, -1, -1, -1); // engine on/off
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle engine set!");
		}
		else if( !strcmp(param1, "l", true) || !strcmp(param1, "lights", true)){
			vehicle_params_set(vehicleLink[vehicleid], -1, param2_val, -1, -1, -1, -1, -1); // lights on/off
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle lights set!");
		}
		else if( !strcmp(param1, "a", true) || !strcmp(param1, "alarm", true)){
			vehicle_params_set(vehicleLink[vehicleid], -1, -1, param2_val, -1, -1, -1, -1); // alarm on/off
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle alarm set!");
		}
		else if( !strcmp(param1, "d", true) || !strcmp(param1, "doors", true)){
			vehicle_params_set(vehicleLink[vehicleid], -1, -1, -1, param2_val, -1, -1, -1); // door lock on/off
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle door lock set!");
		}
		else if( !strcmp(param1, "h", true) || !strcmp(param1, "hood", true)){
			vehicle_params_set(vehicleLink[vehicleid], -1, -1, -1, -1, param2_val, -1, -1); // bonnet open/closed
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle bonnet set!");
		}
		else if( !strcmp(param1, "b", true) || !strcmp(param1, "boot", true)){
			vehicle_params_set(vehicleLink[vehicleid], -1, -1, -1, -1, -1, param2_val, -1); // boot open/closed
			SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "Vehicle boot set!");
		}

		else{ // they didn't enter a valid param
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /v {param to set} {bool on/off}");
			//return 1; // we are done here
		}
	}

	//SendClientMessage(playerid, COLOR_ERROR, "Failed to do anything!");
	return 1; // we are done
}









CMD:slapme(playerid, params[])
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPos(playerid, x, y, z+2);
	return 1;
}


CMD:gimme(playerid, params[])
{
	if(!IsPlayerAdmin(playerid)) return 1;
	GivePlayerMoney(playerid, 1000000);
	return 1;
}


