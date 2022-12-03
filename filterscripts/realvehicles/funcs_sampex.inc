// -----------------------------------------------------------------------------

// pass a value of 1 to turn a param on, 0 to turn it off, and -1 to leave it as it was and -2 to unset
// note that it is not advisable to unset, because the client will not re assume automatic control of the parameter
stock vehicleParamsSet(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective)
{
	new engine_state;
	new lights_state;
	new alarm_state;
	new doors_state;
	new bonnet_state;
	new boot_state;
	new objective_state;

	// get the old params state
	GetVehicleParamsEx(vehicleid, engine_state, lights_state, alarm_state, doors_state, bonnet_state, boot_state, objective_state);

	// set the vars to what the new state will be
	if(engine == 1) engine_state = 1; // if true turn on
	else if(engine == 0) engine_state = 0; // otherwise turn off
	else if(engine == -2) engine_state = -1; // unset

	if(lights == 1) lights_state = 1;
	else if(lights == 0)lights_state = 0;
	else if(lights == -2)lights_state = -1;

	if(alarm == 1) alarm_state = 1;
	else if(alarm == 0) alarm_state = 0;
	else if(alarm == -2) alarm_state = -1;

	if(doors == 1) doors_state = 1;
	else if(doors == 0) doors_state = 0;
	else if(doors == -2) doors_state = -1;

	if(bonnet == 1) bonnet_state = 1;
	else if(bonnet == 0) bonnet_state = 0;
	else if(bonnet == -2) bonnet_state = -1;

	if(boot == 1) boot_state = 1;
	else if(boot == 0) boot_state = 0;
	else if(boot == -2) boot_state = -1;

	if(objective == 1) objective_state = 1;
	else if(objective == 0) objective_state = 0;
	else if(objective == -2) objective_state = -1;

	// set the new params
	SetVehicleParamsEx(vehicleid, engine_state, lights_state, alarm_state, doors_state, bonnet_state, boot_state, objective_state);
}

// pass a value of 1 to toggle a param, 0 to leave as was
stock vehicleParamsToggle(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective)
{
    new engine_state;
	new lights_state;
	new alarm_state;
	new doors_state;
	new bonnet_state;
	new boot_state;
	new objective_state;

	// get the old params state
	GetVehicleParamsEx(vehicleid, engine_state, lights_state, alarm_state, doors_state, bonnet_state, boot_state, objective_state);

	// set the vars to what the new state will be
	if(engine){
		if(engine_state!=-1) toggle(engine_state); // don't if the engine has a value of unset (-1)
	}
	if(lights){
		if(lights_state!=-1) toggle(lights_state);
	}
	if(alarm){
		if(alarm_state!=-1) toggle(alarm_state);
	}
	if(doors){
		if(doors_state!=-1) toggle(doors_state);
	}
	if(bonnet){
		if(bonnet_state!=-1) toggle(bonnet_state);
	}
	if(boot){
		if(boot_state!=-1) toggle(boot_state);
	}
	if(objective){
		if(objective_state!=-1) toggle(objective_state);
	}

	// set the new params
	SetVehicleParamsEx(vehicleid, engine_state, lights_state, alarm_state, doors_state, bonnet_state, boot_state, objective_state);
	
	return;
}

// return 1 if engine is on, 0 if it is off
stock vehicleStateEngine(vehicleid)
{
    new engine_state;
	new lights_state;
	new alarm_state;
	new doors_state;
	new bonnet_state;
	new boot_state;
	new objective_state;

	// get the old params state
	// If a parameter is unset (SetVehicleParamsEx not used beforehand) the value will be -1 ('unset')
	GetVehicleParamsEx(vehicleid, engine_state, lights_state, alarm_state, doors_state, bonnet_state, boot_state, objective_state);

	if(engine_state == 1) return 1; // return "on"
	if(engine_state == 0) return 0; // return "off"
	return -1; // return "unset"
}

