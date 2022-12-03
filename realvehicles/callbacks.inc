// =============================================================================
//                                 callbacks.inc
//                                 SAMP callbacks
// =============================================================================

// TODO
// make new callbacks
// OnPlayerJackPlayer(playerid, driverid) return false to prevent jacking
// OnPlayerEnterVehicleSeat(playerid, vehicleid, seatid)
// 
// deny the changing of colour of faction owned vehicles
//
// this adds new callbacks into the script - 
// OnPlayerEnteredVehicle and OnPlayerExitedVehicle
// add this to documentation

/*
// multiple use of a callback example

public OnPlayerUpdate(playerid)
{
	// call the next occurrence of this callback (if it exists)
	if (funcidx("_OnPlayerUpdate") != -1)
    {
        return CallLocalFunction("_OnPlayerUpdate", "");
    }

	return 1;
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate _OnPlayerUpdate
forward _OnPlayerUpdate(playerid);

// we may now utilise the callback OnPlayerUpdate again
// later on in our script, and both occurrences will be
// executed when the script runs.
*/

/*
// multiple use of a callback (Don Walt's method) example
// this method of doing it should be slightly faster, as it
// doesn't have to call any extra functions before calling
// the original callback. It does it by using compiler macros
// instead of calling a function.

// _OnPlayerUpdate
public OnPlayerUpdate(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined _OnPlayerUpdate
		returnValue = _OnPlayerUpdate();
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate _OnPlayerUpdate
#if defined _OnPlayerUpdate
	forward _OnPlayerUpdate();
#endif

// we may now utilise the callback OnPlayerUpdate again
// later on in our script, and both occurrences will be
// executed when the script runs.
*/



//------------------------------------------------------------------------------
//							 multiplayer fuctions
//------------------------------------------------------------------------------

forward LoadDefaultVehicles();
public LoadDefaultVehicles()
{
	// for testing in LS
	//loadVehiclesFromCSV("vehicles/spawnlists/ls_gen_inner.txt", false);
	//loadVehiclesFromCSV("vehicles/spawnlists/ls_gen_outer.txt", false);

	loadVehiclesFromCSV("vehicles/spawnlists/bone.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/flint.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/ls_airport.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/ls_gen_inner.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/ls_gen_outer.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/ls_law.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/lv_airport.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/lv_gen.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/lv_law.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/pilots.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/red_county.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/sf_airport.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/sf_gen.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/sf_law.txt", false);
	//loadVehiclesFromCSV("vehicles/spawnlists/sf_train.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/tierra.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/trains.txt", false);
	//loadVehiclesFromCSV("vehicles/spawnlists/trains_platform.txt", false);
	loadVehiclesFromCSV("vehicles/spawnlists/whetstone.txt", false);

	return 0;
}



//------------------------------------------------------------------------------
//									timed fuctions
//------------------------------------------------------------------------------

// -TODO- write a better update timer function
//new bool updateLag = false;
// period - time elapsed since last call to this function
forward globalStats_update(period);
public globalStats_update(period)
{
	#pragma unused period
	// TODO use gettime to dynamically calculate the periods
	// TODO add a lag checker

	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++)	vehicleStat_update(vehstatid);
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++)		playerStat_update(playerid);
	vehStatsDisplayProf_update();

	return;
}



//------------------------------------------------------------------------------
//									callbacks
//------------------------------------------------------------------------------

// prove that the function can daisy chain
// rvx_OnGameModeInit
public OnGameModeInit()
{	
	print(" ");
	print(" ");
	print(" ----------------------------------------- ");
	print("--     Realistic Vehicle Performace      --");
	print("--             Daisy-Chained             --");
	print(" ----------------------------------------- ");
	print(" ");

	print("Pawned Daisy-Chain by Don_Walt");

	// call the other callback
	new returnValue = 1;
	#if defined rvx_OnGameModeInit
		returnValue = rvx_OnGameModeInit();
	#endif
	return returnValue;
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit rvx_OnGameModeInit
#if defined rvx_OnGameModeInit
	forward rvx_OnGameModeInit();
#endif

//------------------------------------------------------------------------------

// rv_OnGameModeInit
public OnGameModeInit()
{	
	print("Manual Vehicle Engine and Lights");
	ManualVehicleEngineAndLights();
	
	print("Loading RealVehicles:");
	
	print(" - Connecting to database...");
	dbinfo_load();
	database_connect(0);
	mysql_log(LOG_ERROR | LOG_WARNING, LOG_TYPE_TEXT);

	print(" - Setting up timers...");
	SetTimerEx("globalStats_update", 500, 1, "d", 500);

	print(" - Initializing variables...");
	vehicleStat_init();
	vehicleLink_init();
	playerStat_init();
	
	print(" - Initializing TextDraws...");
	vehStatsDisplayProf_init();
	
	print(" - Loading configuration files...");
	loadHardnessCSV();
	
	print(" - Loaded RealVehicles!");
	print(" ");

	
	// TODO some debugging stuff below
	vehicleHardness_logdump();
	

	print(" ");
	print(" ");
	print(" ----------------------------------------- ");
	print("--  Realistic Vehicle Performace Loaded  --");
	print("--         by Don Walt \"Enforcer\"        --");
	print(" ----------------------------------------- ");
	print(" ");

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnGameModeInit
		returnValue = rv_OnGameModeInit();
	#endif
	return returnValue;
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit rv_OnGameModeInit
#if defined rv_OnGameModeInit
	forward rv_OnGameModeInit();
#endif

//------------------------------------------------------------------------------

// rv_OnGameModeExit
public OnGameModeExit()
{
	print("Unloading RealVehicles:");
	
	print(" - Deleting TextDraws...");
	vehStatsDisplayProf_delete();

	print(" - Saving Vehicles...");
	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++){
		if(valid_modelid(vehicleStat[vehstatid][vModel])){
			if(vehicleStat[vehstatid][vVehicleID] != INVALID_VEHICLE_ID){ // if vehicle spawned
				// get the position and set as park position
				GetVehiclePos(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vParkPositionX], vehicleStat[vehstatid][vParkPositionY], vehicleStat[vehstatid][vParkPositionZ]);
				GetVehicleZAngle(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vParkPositionR]);
				//vehicle_virtualworld_get(vehicleStat[vehstatid][vVehicleID]); // ac do this one
				vehicle_health_get(vehicleStat[vehstatid][vVehicleID]);
				vehicle_damagestatus_get(vehicleStat[vehstatid][vVehicleID]);
				//vehicle_params_get(vehicleStat[vehstatid][vVehicleID]); // no, let the ac do this one.
			}
			vehicledata_save(vehstatid);
		}
	}
	
	print(" - Disconnecting from database...");
	database_disconnect(0);
	
	print(" - RealVehicles Unloaded!");
	print(" ");

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnGameModeExit
		returnValue = rv_OnGameModeExit();
	#endif
	return returnValue;
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit rv_OnGameModeExit
#if defined rv_OnGameModeExit
	forward rv_OnGameModeExit();
#endif

//------------------------------------------------------------------------------

// rv_OnPlayer_Connect
public OnPlayerConnect(playerid)
{
	// mapicon styles
	// 0: MAPICON_LOCAL					Display in the player's local area
	// 1: MAPICON_GLOBAL				Display always
	// 2: MAPICON_LOCAL_CHECKPOINT		Display in the player's local area and has a checkpoint marker
	// 3: MAPICON_GLOBAL_CHECKPOINT		Display always and has a checkpoint marker
	
	// clear the stats ready to load for them
	playerStat_clear(playerid);
	// get the players name
	strcpy(playerStat[playerid][pName], getPlayerName(playerid), MAX_PLAYER_NAME);
	// if they have an account, load it
	if(playerdata_exists( playerStat[playerid][pName] )) playerStat_load(playerStat[playerid][pName], playerid);
	// if they don't, make them one
	else 
	{
		playerStat_save(playerStat[playerid]); // if they've never logged in before save them an account
	}
	
	todo_playerMoney_load(playerStat[playerid][pName], playerid);

	// set petrol station map icons
	new iconid = 100;
	// for every fuel station
	for (new i=0; i<sizeof(fuelStations); i++)
	{
		iconid--;
		SetPlayerMapIcon(playerid, iconid, fuelStations[i][X], fuelStations[i][Y], fuelStations[i][Z], 19, 0, MAPICON_LOCAL);
	}

	vehStatsDisplayProf_init_player(playerid);

	// call the other callback
	#if defined rv_OnPlayer_Connect
	return rv_OnPlayer_Connect(playerid);
	#else
	return 1; // Allow other scripts to process OnPlayerConnect()
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect rv_OnPlayer_Connect
#if defined rv_OnPlayer_Connect
	forward rv_OnPlayer_Connect(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerDisconnect
public OnPlayerDisconnect(playerid, reason)
{
	playerStat_save(playerStat[playerid]);
	
	todo_playerMoney_save(playerid);
	
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerDisconnect
		returnValue = rv_OnPlayerDisconnect(playerid, reason);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect rv_OnPlayerDisconnect
#if defined rv_OnPlayerDisconnect
	forward rv_OnPlayerDisconnect(playerid, reason);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerSpawn
public OnPlayerSpawn(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerSpawn
		returnValue = rv_OnPlayerSpawn(playerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn rv_OnPlayerSpawn
#if defined rv_OnPlayerSpawn
	forward rv_OnPlayerSpawn(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerDeath
public OnPlayerDeath(playerid, killerid, reason)
{
	// if killer was in a vehicle, increment the vehicles kills
	if(IsPlayerConnected(killerid)){
		new vehicleid = GetPlayerVehicleID(killerid);
		if(vehicleid != INVALID_VEHICLE_ID){
			vehicleStat[vehicleLink[vehicleid]][vKills]++;
		}
	}
	
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerDeath
		returnValue = rv_OnPlayerDeath(playerid, killerid, reason);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath rv_OnPlayerDeath
#if defined rv_OnPlayerDeath
	forward rv_OnPlayerDeath(playerid, killerid, reason);
#endif

//------------------------------------------------------------------------------

// TODO fix this to do all the stuff it needs to do
// rv_OnVehicleSpawn
public OnVehicleSpawn(vehicleid)
{
	//SendClientMessageToAll(0xFF0000FF, "OnVehicleSpawn");
	
	// player could be vehicle cheating, check for inconsistencies
	if(vehicleLink[vehicleid]==INVALID_VEHSTAT_ID){

		print("Attempt to spawn a vehicle without a vehstatid!");
		DestroyVehicle(vehicleid);

		return 1; // further occurrences of the callback won't run!
	}
	
	new vehstatid = vehicleLink[vehicleid];

	// must set to spawned BEFORE spawning the vehicle!
	if(!vehicleStat[vehstatid][vSpawned])
	{
		DestroyVehicle(vehicleid);
		return 1;
	}

	// don't spawn if health is 0, health must be reset before respawn.
	if(vehicleStat[vehstatid][vHealth] < 250.0){
		DestroyVehicle(vehicleid);
		return 1;
	}

	// update the vehicle to its stats
	vehicle_paintjob_update(vehicleid);
	vehicle_component_update(vehicleid);
	vehicle_virtualworld_update(vehicleid);
	vehicle_health_update(vehicleid);
	vehicle_damagestatus_update(vehicleid);
	vehicle_params_update(vehicleid);

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleSpawn
		returnValue = rv_OnVehicleSpawn(vehicleid);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehicleSpawn
	#undef OnVehicleSpawn
#else
	#define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn rv_OnVehicleSpawn
#if defined rv_OnVehicleSpawn
	forward rv_OnVehicleSpawn(vehicleid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehicleDeath
public OnVehicleDeath(vehicleid, killerid)
{
	// increment kills of the vehicle the killing player was in
	if(IsPlayerConnected(killerid)){
		new targetid = GetPlayerVehicleID(killerid);
		if(targetid != INVALID_PLAYER_ID){
			vehicleStat[vehicleLink[targetid]][vKills]++;
		}
	}

	vehicleStat[vehicleLink[vehicleid]][vVehicleID]		= INVALID_VEHICLE_ID;
	vehicleStat[vehicleLink[vehicleid]][vSpawned]		= 0;
	// park at spawn location
	vehicleStat[vehicleLink[vehicleid]][vParkPositionX]	= vehicleStat[vehicleLink[vehicleid]][vSpawnPositionX];
	vehicleStat[vehicleLink[vehicleid]][vParkPositionY]	= vehicleStat[vehicleLink[vehicleid]][vSpawnPositionY];
	vehicleStat[vehicleLink[vehicleid]][vParkPositionZ]	= vehicleStat[vehicleLink[vehicleid]][vSpawnPositionZ];
	vehicleStat[vehicleLink[vehicleid]][vParkPositionR]	= vehicleStat[vehicleLink[vehicleid]][vSpawnPositionR];
	vehicleStat[vehicleLink[vehicleid]][vDeath]++; // increment
	// health = max_health
	vehicleStat[vehicleLink[vehicleid]][vHealth]		= vehicleStat[vehicleLink[vehicleid]][vHealthMax];
	// reset all exterior damage visuals
	vehicleStat[vehicleLink[vehicleid]][vParams]		= 0;
	vehicleStat[vehicleLink[vehicleid]][vDamageStatus0]	= 0;
	vehicleStat[vehicleLink[vehicleid]][vDamageStatus1]	= 0;
	vehicleStat[vehicleLink[vehicleid]][vDamageStatus2]	= 0;
	vehicleStat[vehicleLink[vehicleid]][vDamageStatus3]	= 0;

	// delete the vehicle id's link
	new vehstatid = vehicleLink[vehicleid];
	vehicleLink[vehicleid] = INVALID_VEHSTAT_ID; // unlink the vehicleid from the vehicle
	DestroyVehicle(vehicleid); // stop the vehicleid from re-spawning, we do that in our own time

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleDeath
		returnValue = rv_OnVehicleDeath(vehicleid, killerid);
	#endif

	// re-spawn the vehicle
	vehicle_spawn(vehstatid);

	return returnValue;
}
#if defined _ALS_OnVehicleDeath
	#undef OnVehicleDeath
#else
	#define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath rv_OnVehicleDeath
#if defined rv_OnVehicleDeath
	forward rv_OnVehicleDeath(vehicleid, killerid);
#endif

//------------------------------------------------------------------------------

// gets called when a player presses 'f', ie the moment they give the command to
// enter a vehicle, NOT when they reach the drivers seat, therefore OnPlayerExitVehicle
// will not be called if they cancel (getting in to the vehicle seat).
// rv_OnPlayerEnterVehicle
public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	// player could be vehicle cheating, check for inconsistencies
	if(vehicleLink[vehicleid]==INVALID_VEHSTAT_ID){
		
	}
	
	else
	{
		// if vehicle is locked, they shouldn't have been able to get in it
		if(vehicleStat[vehicleLink[vehicleid]][vParams] & VEHPARAM_LOCK) RemovePlayerFromVehicle(playerid);
	}

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerEnterVehicle
		returnValue = rv_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerEnterVehicle
	#undef OnPlayerEnterVehicle
#else
	#define _ALS_OnPlayerEnterVehicle
#endif
#define OnPlayerEnterVehicle rv_OnPlayerEnterVehicle
#if defined rv_OnPlayerEnterVehicle
	forward rv_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerExitVehicle
public OnPlayerExitVehicle(playerid, vehicleid)
{
	//SendClientMessage(playerid, 0xFF0000FF, "rv_OnPlayerExitVehicle(playerid, vehicleid);");

	// only if they were the driver
	if( GetPlayerState(playerid) == PLAYER_STATE_DRIVER ){
		// turn engine off
		vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1);
		//vehicleStat[ vehicleLink[vehicleid] ][vParams] &= ~VEHPARAM_ENGINE;
	}

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerExitVehicle
		returnValue = rv_OnPlayerExitVehicle(playerid, vehicleid);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif
#define OnPlayerExitVehicle rv_OnPlayerExitVehicle
#if defined rv_OnPlayerExitVehicle
	forward rv_OnPlayerExitVehicle(playerid, vehicleid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerStateChange
public OnPlayerStateChange(playerid, newstate, oldstate)
{
	// onPlayerEnterDriverSeat
	//if(IsPlayerInAnyVehicle(playerid)){
	if(newstate == PLAYER_STATE_DRIVER && oldstate != PLAYER_STATE_DRIVER){
		new vehicleid = GetPlayerVehicleID(playerid);
		//new seatid = GetPlayerVehicleSeat(playerid);

		// set up the players driving statistics
		playerStat[playerid][pVehicleID] = vehicleid;
		distanceAtStart[playerid] = vehicleStat[ vehicleLink[vehicleid] ][vTotalDistance];
		vehStatsDisplayProf_show(playerid); // show the speedo display
		// turn engine on
		vehicle_params_set(vehicleLink[vehicleid], 1, -1, -1, -1, -1, -1, -1);
		//vehicleStat[ vehicleLink[vehicleid] ][vEngine] = true;
		// remember the last player in the vehicle
		strcopy(vehicleStat[vehicleLink[vehicleid]][vLastDriver], MAX_PLAYER_NAME+1, getPlayerName(playerid));
		
		// call the OnPlayerEnteredVehicle callback
		#if defined OnPlayerEnteredVehicle
			OnPlayerEnteredVehicle(playerid, vehicleid, seatid);
		#endif
	}

	// on player exit driver seat
	else if(newstate != PLAYER_STATE_DRIVER && oldstate == PLAYER_STATE_DRIVER){
		//new vehicleid = GetPlayerVehicleID(playerid);

		// turn engine off - this must be done OnPlayerExitVehicle, (where the player is actually still in the vehicle,
		// or it don't work (the player is already out of the vehicle in this function)

		// unset the players driving statistics
		distanceAtStart[playerid] = 0;

		// hide the speedo display
		vehStatsDisplayProf_hide(playerid);
		
		// call the OnPlayerExitedVehicle callback
		#if defined OnPlayerExitedVehicle
			OnPlayerExitedVehicle(playerid, playerStat[playerid][pVehicleID]);
		#endif
		
		playerStat[playerid][pVehicleID] = INVALID_VEHICLE_ID;
	}

	//print("playerstatechange");

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerStateChange
		returnValue = rv_OnPlayerStateChange(playerid, newstate, oldstate);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange rv_OnPlayerStateChange
#if defined rv_OnPlayerStateChange
	forward rv_OnPlayerStateChange(playerid, newstate, oldstate);
#endif
// for the new callbacks
#if defined OnPlayerEnteredVehicle
	forward OnPlayerEnteredVehicle(playerid, vehicleid, seatid);
#endif
#if defined OnPlayerExitedVehicle
	forward OnPlayerExitedVehicle(playerid, vehicleid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerEnterCheckpoint
public OnPlayerEnterCheckpoint(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerEnterCheckpoint
		returnValue = rv_OnPlayerEnterCheckpoint(playerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerEnterCheckpoint
	#undef OnPlayerEnterCheckpoint
#else
	#define _ALS_OnPlayerEnterCheckpoint
#endif
#define OnPlayerEnterCheckpoint rv_OnPlayerEnterCheckpoint
#if defined rv_OnPlayerEnterCheckpoint
	forward rv_OnPlayerEnterCheckpoint(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerLeaveCheckpoint
public OnPlayerLeaveCheckpoint(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerLeaveCheckpoint
		returnValue = rv_OnPlayerLeaveCheckpoint(playerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerLeaveCheckpoint
	#undef OnPlayerLeaveCheckpoint
#else
	#define _ALS_OnPlayerLeaveCheckpoint
#endif
#define OnPlayerLeaveCheckpoint rv_OnPlayerLeaveCheckpoint
#if defined rv_OnPlayerLeaveCheckpoint
	forward rv_OnPlayerLeaveCheckpoint(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerEnterRaceCheckpoint
public OnPlayerEnterRaceCheckpoint(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerEnterRaceCheckpoint
		returnValue = rv_OnPlayerEnterRaceCheckpoint(playerid);
	#endif
	return returnValue;
}
//#if defined _ALS_OnPlayerEnterRaceCheckpoint
	#undef OnPlayerEnterRaceCheckpoint
//#else
//	#define _ALS_OnPlayerEnterRaceCheckpoint
//#endif
#define OnPlayerEnterRaceCheckpoint rv_OnPlayerEnterRaceCheckpoint
#if defined rv_OnPlayerEnterRaceCheckpoint
	forward rv_OnPlayerEnterRaceCheckpoint(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerLeaveRaceCheckpoint
public OnPlayerLeaveRaceCheckpoint(playerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerLeaveRaceCheckpoint
		returnValue = rv_OnPlayerLeaveRaceCheckpoint(playerid);
	#endif
	return returnValue;
}
//#if defined _ALS_OnPlayerLeaveRaceCheckpoint
	#undef OnPlayerLeaveRaceCheckpoint
//#else
//	#define _ALS_OnPlayerLeaveRaceCheckpoint
//#endif
#define OnPlayerLeaveRaceCheckpoint rv_OnPlayerLeaveRaceCheckpoint
#if defined rv_OnPlayerLeaveRaceCheckpoint
	forward rv_OnPlayerLeaveRaceCheckpoint(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehicleMod
public OnVehicleMod(playerid, vehicleid, componentid)
{
	// player could be vehicle cheating, check for inconsistencies
	if(vehicleLink[vehicleid]==INVALID_VEHSTAT_ID){
		return 0;
	}

	vehicle_component_added(vehicleLink[vehicleid], componentid);

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleMod
		returnValue = rv_OnVehicleMod(playerid, vehicleid, componentid);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehicleMod
	#undef OnVehicleMod
#else
	#define _ALS_OnVehicleMod
#endif
#define OnVehicleMod rv_OnVehicleMod
#if defined rv_OnVehicleMod
	forward rv_OnVehicleMod(playerid, vehicleid, componentid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehiclePaintjob
public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	// player could be vehicle cheating, check for inconsistencies
	if(vehicleLink[vehicleid]==INVALID_VEHSTAT_ID){
		return 1;
	}

	vehicleStat[vehicleLink[vehicleid]][vPaintJob] = paintjobid;
	
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehiclePaintjob
		returnValue = rv_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehiclePaintjob
	#undef OnVehiclePaintjob
#else
	#define _ALS_OnVehiclePaintjob
#endif
#define OnVehiclePaintjob rv_OnVehiclePaintjob
#if defined rv_OnVehiclePaintjob
	forward rv_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehicleRespray
// The callback name is deceptive, this callback is called when a player exits a mod shop,
// regardless of whether the vehicle's colors were changed, and is NEVER called for pay 'n'
// spray garages.
public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	// player could be vehicle cheating, check for inconsistencies
	if(vehicleLink[vehicleid]==INVALID_VEHSTAT_ID){
		return 0;
	}

	vehicleStat[vehicleLink[vehicleid]][vColor1] = color1;
	vehicleStat[vehicleLink[vehicleid]][vColor2] = color2;

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleRespray
		returnValue = rv_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehicleRespray
	#undef OnVehicleRespray
#else
	#define _ALS_OnVehicleRespray
#endif
#define OnVehicleRespray rv_OnVehicleRespray
#if defined rv_OnVehicleRespray
	forward rv_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerKeyStateChange
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerKeyStateChange
		returnValue = rv_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange rv_OnPlayerKeyStateChange
#if defined rv_OnPlayerKeyStateChange
	forward rv_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

//------------------------------------------------------------------------------

// rv_OnPlayerUpdate
public OnPlayerUpdate(playerid)
{
	// player is in a vehicle, and is the driver
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
		new vehicleid = GetPlayerVehicleID(playerid);

		if(vehicleStat[ vehicleLink[vehicleid] ][vFuel] > 0.2){ // if more than 200ml in the fuel tank
			vehicle_params_get(vehicleLink[vehicleid]);
		}
		else{ // if out of fuel
			vehicle_params_set(vehicleLink[vehicleid], 0, -1, -1, -1, -1, -1, -1); // turn engine off
		}
	}

	// call the other callback
	new returnValue = 1;
	#if defined rv_OnPlayerUpdate
		returnValue = rv_OnPlayerUpdate(playerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate rv_OnPlayerUpdate
#if defined rv_OnPlayerUpdate
	forward rv_OnPlayerUpdate(playerid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehicleStreamIn
public OnVehicleStreamIn(vehicleid, forplayerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleStreamIn
		returnValue = rv_OnVehicleStreamIn(vehicleid, forplayerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehicleStreamIn
	#undef OnVehicleStreamIn
#else
	#define _ALS_OnVehicleStreamIn
#endif
#define OnVehicleStreamIn rv_OnVehicleStreamIn
#if defined rv_OnVehicleStreamIn
	forward rv_OnVehicleStreamIn(vehicleid, forplayerid);
#endif

//------------------------------------------------------------------------------

// rv_OnVehicleStreamOut
public OnVehicleStreamOut(vehicleid, forplayerid)
{
	// call the other callback
	new returnValue = 1;
	#if defined rv_OnVehicleStreamOut
		returnValue = rv_OnVehicleStreamOut(vehicleid, forplayerid);
	#endif
	return returnValue;
}
#if defined _ALS_OnVehicleStreamOut
	#undef OnVehicleStreamOut
#else
	#define _ALS_OnVehicleStreamOut
#endif
#define OnVehicleStreamOut rv_OnVehicleStreamOut
#if defined rv_OnVehicleStreamOut
	forward rv_OnVehicleStreamOut(vehicleid, forplayerid);
#endif

//------------------------------------------------------------------------------


