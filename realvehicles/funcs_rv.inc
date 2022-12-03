// calculate the fuel consumption for a vehicleid, return a value in Litres per 100km
// credit to Srdjan for origonal function (note - has been altered slightly)
Float:vehicleConsumption(vehid)
{
	//new vehid = vehicleStat[vehstatid][vVehicleID];

	if(vehicleType(GetVehicleModel(vehid) == 0 )) return -1.0; // if not a vehicle
	if(!vehicleStateEngine(vehid)) return 0.0; // if the engine is off

	new Float: speed [3], multiplier, final_speed, Float: health;
	GetVehicleVelocity (vehid, speed [0], speed [1], speed [2]);
	final_speed = floatround (floatsqroot (floatpower (speed [0], 2) + floatpower (speed [1], 2) + floatpower (speed [2], 2)) * 160.666667, floatround_round);
	GetVehicleHealth (vehid, health);

	switch (vehicleType(GetVehicleModel(vehid)))
	{
		case VEHTYPE_SPORT:
		{
			if (final_speed <= 80.0)
			{
				multiplier = floatround (final_speed / 10, floatround_round);
			}
			else
			{
				multiplier = - floatround (final_speed / 10, floatround_round);
			}

			return 9.5 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
		case VEHTYPE_COMMON:
		{
			if (final_speed <= 65.0)
			{
				multiplier = floatround (final_speed / 10, floatround_round);
			}
			else
			{
				multiplier = - floatround (final_speed / 10, floatround_round);
			}
			return 7.5 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
		case VEHTYPE_OFFROAD:
		{
			if (final_speed <= 85.0)
			{
				multiplier = floatround (final_speed / 10, floatround_round);
			}
			else
			{
				multiplier = - floatround (final_speed / 10, floatround_round);
			}
			return 10.5 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
		case VEHTYPE_TRUCK:
		{
			if (final_speed <= 75.0)
			{
				multiplier = floatround (final_speed / 10, floatround_round);
			}
			else
			{
				multiplier = - floatround (final_speed / 10, floatround_round);
			}
			return 15.5 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
		case VEHTYPE_BIKE:
		{
			if (final_speed <= 60.0)
			{
				multiplier = floatround (final_speed / 10, floatround_round);
			}
			else
			{
				multiplier = - floatround (final_speed / 10, floatround_round);
			}
			return 4.5 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
		case VEHTYPE_AIR:
		{
			multiplier = floatround (final_speed / 10, floatround_round);
			return 20.0 + multiplier * 0.02 + (10.0 - health / 100) / 2;
		}
	}
	return 0.0; // if vehicletype is 0
}



// are we at a gas station?
stock IsPlayerAtFuelStation(playerid)
{
	for (new i=0; i<sizeof(fuelStations); i++)
	{
		if (IsPlayerInRangeOfPoint (playerid, 10.0, fuelStations[i][X], fuelStations[i][Y], fuelStations[i][Z])) return 1;
	}
	return 0;
}



//------------------------------------------------------------------------------
//							 other fuctions
//------------------------------------------------------------------------------



new repairTimer[MAX_VEHICLES];

forward modifyVehicleDamage(vehicleid, Float: target, Float: rate);
public modifyVehicleDamage(vehicleid, Float: target, Float: rate)
{
	new vehstatid = vehicleLink[vehicleid];

	// if we have reached the target
	if(vehicleStat[vehstatid][vDamage]-rate <= target){
		vehicleStat[vehstatid][vDamage] = target; // correct for small errors

		// turn the engine back on
		vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);

		KillTimer(repairTimer[vehicleid]); // kill the repair timer
	}

	// but if we haven't...
	else{
		vehicleStat[vehstatid][vDamage] -= rate;

		// make sure engine stays off
		vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);
	}

	// if we repaired enough, fix the vehicles paneling
	if(vehicleStat[vehstatid][vDamage] < 10) RepairVehicle(vehicleid);
	// set the vehicle health from the vehicle damage
	new Float: vehicleHealth = (vehicleStat[vehstatid][vDamage] * -1 +100) * 750/100 + 250;
	SetVehicleHealth(vehicleid, vehicleHealth);

	return;
}



// repair a vehicleid - pass a negative ammount to repair completely,
// pass instant a value of false to repair using a timer (so it takes time to repair)
repairVehicle(vehicleid, Float: ammount, instant)
{
	new vehstatid = vehicleLink[vehicleid];

	// turn the engine off
	vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);

	// if the repair is to happen instantly
	if(instant){
		// if ammount is less than 0, repair fully
		if(ammount < 0) vehicleStat[vehstatid][vDamage] = 0;

		// if an ammount is specified
		else{
			if(vehicleStat[vehstatid][vDamage]-ammount < 0) vehicleStat[vehstatid][vDamage] = 0; // if ammount would over-repair, repair to 0 damage

			else vehicleStat[vehstatid][vDamage] -= ammount; // otherwise repair by ammount
		}

		// if we repaired enough, fix the vehicles paneling
		if(vehicleStat[vehstatid][vDamage] < 10) RepairVehicle(vehicleid);
		// set the vehicle health from the vehicle damage
		new Float: vehicleHealth = (vehicleStat[vehstatid][vDamage] * -1 +100) * 750/100 + 250;
		SetVehicleHealth(vehicleid, vehicleHealth);

		// turn the engine back on
		vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);
	}

	// if not using instant
	else{
		// if ammount is less than 0, repair fully
		if(ammount < 0){
			repairTimer[vehicleid] = SetTimerEx("modifyVehicleDamage", 100, 1, "dff", vehicleid, 0, REPAIR_RATE);
		}

		// if an ammount is specified
		else{
			// if ammount would over-repair, repair to 0 damage
			if(vehicleStat[vehstatid][vDamage]-ammount <= 0){
				repairTimer[vehicleid] = SetTimerEx("modifyVehicleDamage", 100, 1, "dff", vehicleid, 0, REPAIR_RATE);
			}

			else{ // otherwise repair by ammount
				new Float: target = vehicleStat[vehstatid][vDamage] - ammount; // target = current damage - ammount
				repairTimer[vehicleid] = SetTimerEx("modifyVehicleDamage", 100, 1, "dff", vehicleid, target, REPAIR_RATE); // repair to the target, instead of to 0 damage
			}
		}
	}

	return;
}


new refuelTimer[MAX_VEHICLES];

// empty the tank before initiating the refuel function loop
forward drainRefuelVehicle(vehicleid, Float: ammountToRefuel, Float: rate);
public drainRefuelVehicle(vehicleid, Float: ammountToRefuel, Float: rate)
{
	new vehstatid = vehicleLink[vehicleid];

	if(rate<0) rate *= -1; // rate must be positive;

	// check to see if we have reached the target (of 0)
	if(vehicleStat[vehstatid][vFuel]-rate <= 0){

		// stop the timer that is calling this function
		KillTimer(refuelTimer[vehicleid]);

		// call the refuel function to refuel the "ammountToRefuel"
		refuelVehicle(vehicleid, ammountToRefuel, 0, 0);

		return 1;
	}

	// alter the vehicle fuel by the ammount of rate
	vehicleStat[vehstatid][vFuel] -= rate;

	// make sure engine stays off
	vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);

	return 0;
}

// returns 1 when it reaches its target
forward addFuelToVehicle(vehicleid, Float: target, Float: rate, &Float: fuelAdded);
public addFuelToVehicle(vehicleid, Float: target, Float: rate, &Float: fuelAdded)
{
	new vehstatid = vehicleLink[vehicleid];

	// if we get a negative rate, drain the tank instead of filling it
	if(rate<0){
		// check to see if we have reached the target
		if(vehicleStat[vehstatid][vFuel]+rate <= target){
			// turn the engine back on
			vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);
			KillTimer(refuelTimer[vehicleid]); // stop the timer that is calling this function
			return 1;
		}
	}

	// if we get a positive rate
	else{
		// check to see if we have reached the target
		if(vehicleStat[vehstatid][vFuel]+rate >= target){
			// turn the engine back on
			vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);
			KillTimer(refuelTimer[vehicleid]); // stop the timer that is calling this function
			return 1;
		}
	}

	// alter the vehicle fuel by the ammount of rate
	vehicleStat[vehstatid][vFuel] += rate;
	fuelAdded += rate;

	// make sure engine stays off
	vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);

	return 0;
}

// 					variables to pass to this function
// "vehicleid" - the vehicles game id
// "ammount" - the ammount of fuel that we wish to add/remove from the tank.
//   use a value of -1 to completely fill/empty the fuel tank.
// "instant" - when true, the tank will be filled instantly, when false, the tank will be filled in real time
// "reverse" - when true, ammount will be subtracted from the tank, rather than added
refuelVehicle(vehicleid, Float: ammount, instant, reverse)
{
	new vehstatid = vehicleLink[vehicleid];

	// turn the engine off
	vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);

	new Float: tankCapacity = vehicleFuelTankCapacity(GetVehicleModel(vehicleid)); // find the tank capacity
	new Float: target; // the target fuel level to have in the tank at the end of the refill


	// if we want to empty the tank
	if(reverse){
		// if ammount is < 0, just empty the tank completely
		if(ammount<0){
			target = 0.0;
		}
		else{
			// if subtracting ammount would exceed the tank limits
			if(vehicleStat[vehstatid][vFuel]-ammount < 0.0) target = 0.0; // set the tank to empty

			// if it doesn't
			else target = vehicleStat[vehstatid][vFuel] - ammount; // remove ammount from the tank
		}
	}

	// otherwise fill the tank
	else{
		// if ammount is < 0, just fill the tank completely
		if(ammount<0){
			target = tankCapacity; // fill the tank to near capacity
		}
		else{
			// if adding ammount would exceed the tank limits
			if(vehicleStat[vehstatid][vFuel]+ammount >= tankCapacity) target = tankCapacity; // set the tank to near capacity

			// if it wouldn't
			else target = vehicleStat[vehstatid][vFuel] + ammount; // add ammount to the tank
		}
	}


	// if the refill is to happen instantly
	if(instant){

		// set the fuel level to the target we calculated
		vehicleStat[vehstatid][vFuel] = target;

		// turn the engine back on
		vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);
	}

	// if using realtime
	else{
		// if we want to empty the tank
		if(reverse){
			// set a timer to empty the tank to the "target" level
			refuelTimer[vehicleid] = SetTimerEx("addFuelToVehicle", 100, 1, "dff", vehicleid, target, (REFUEL_RATE * -1) );
		}

		// otherwise fill the tank
		else{
			// set a timer to fill the tank to the "target" level
			refuelTimer[vehicleid] = SetTimerEx("addFuelToVehicle", 100, 1, "dff", vehicleid, target, REFUEL_RATE );
		}
	}


	return;
}