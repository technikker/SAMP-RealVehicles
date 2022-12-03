


// TODO rename player

#define MAX_VEHICLES_NAMES		MAX_PLAYER_VEHICLES * MAX_VEHICLE_NAME + 1 // +1 for the terminating null after the final ':'

enum pStat
{
	// stats that get saved [realvehicles]
	pVehiclesNames[MAX_VEHICLES_NAMES],
	pVehiclesLen,
	pMoneyOwed,
	
	// temp stats
	pCash,
	pBank,

	// realtime stats
	pName[MAX_PLAYER_NAME],
	pVehStatIDs[MAX_PLAYER_VEHICLES], // vehstatid
	// these vars are only used when a player is in a vehicle
	pVehicleID,
	Float: pDistanceThisVeh, // distance travelled by this playerid in this vehid
	Float: pDistanceRemaining, // distance left to travel on this tank of fuel
}
new playerStat[MAX_PLAYERS][pStat];
new temp_playerStat[pStat];

stock playerStat_pVehStatIds_clear(playerid)
{
	for(new i=0; i<MAX_PLAYER_VEHICLES; i++) playerStat[playerid][pVehStatIDs][i] = INVALID_VEHSTAT_ID;
}

stock playerStat_clear(playerid)
{
	if(!valid_index(playerid, MAX_PLAYERS)) return 1;

	playerStat[playerid][pVehiclesNames][0]		= '\0';
	playerStat[playerid][pVehiclesLen]			= 0;
	playerStat[playerid][pMoneyOwed]			= 0;

	playerStat[playerid][pName][0]				= '\0';
	playerStat_pVehStatIds_clear(playerid);
	playerStat[playerid][pVehicleID]			= INVALID_VEHICLE_ID;
	playerStat[playerid][pDistanceThisVeh]		= 0.0;
	playerStat[playerid][pDistanceRemaining]	= 0.0;
	
	return 0;
}

stock playerStat_init()
{
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++){
		playerStat_clear(playerid);
	}
	return;
}




// adds vehicle to list of player vehicles, doesn't set vehicle owner
// player must be online, vehicle does not need to be
// example "VEH 001:VEH 002:"
stock player_vehicle_add(playerstat[pStat], name[])
{
	if(playerstat[pVehiclesLen] == MAX_PLAYER_VEHICLES) return 1;
	if(strcount(playerstat[pVehiclesNames], name)) return 1; // they already have this vehicle
	strcat(playerstat[pVehiclesNames], name, MAX_VEHICLES_NAMES); // add vehicle name
	strcat(playerstat[pVehiclesNames], ":",  MAX_VEHICLES_NAMES); // add separator
	playerstat[pVehiclesLen]++;
	//printf("str3 --%s----%s--", name, playerstat[pVehiclesNames]);
	return 0;
}

// removes vehicle to list of player vehicles, doesn't unset vehicle owner
// player must be online, vehicle does not need to be
// example "VEH 001:VEH 002:"
stock player_vehicle_remove(playerstat[pStat], name[])
{
	if(playerstat[pVehiclesLen] == 0) return 1; // no vehicles to remove
	if(!strcount(playerstat[pVehiclesNames], name)) return 1; // vehicle to remove not theirs
	new temp[MAX_VEHICLE_NAME + 1];
	strcat(temp, name), strcat(temp, ":");
	strreplace(playerstat[pVehiclesNames], temp, ""); // remove vehicle name
	playerstat[pVehiclesLen]--;
	for(new i=0; i<MAX_PLAYER_VEHICLES; i++) playerstat[pVehStatIDs][i] = INVALID_VEHSTAT_ID; // reset the vehstatid list because we just messed up its order
	//printf("str3 --%s----%s--", name, playerstat[pVehiclesNames]);
	return 0;
}

// get the players nth vehicle
// player must be online
stock player_vehicle_get(playerid, position)
{
	new index = 0;
	new name[MAX_VEHICLE_NAME];

	do
	{
		index = token_by_delim(playerStat[playerid][pVehiclesNames], name, ':', index);
		if(index == (-1)) break; // fail
		if(position-- == 0) return name;
		index++; // skip the next ':'
	}
	while(playerStat[playerid][pVehiclesNames][index]);

	name[0] = '\0';
	return name; // fail
}

// get and save vehstatid for quicker searching of the players vehicles
stock player_vehstatid_get(playerid, position)
{
	// if id already known
	if(playerStat[playerid][pVehStatIDs][position] != INVALID_VEHSTAT_ID) return playerStat[playerid][pVehStatIDs][position];
	
	// if id not already known
	playerStat[playerid][pVehStatIDs][position] = vehicleStat_getid(player_vehicle_get(playerid, position)); // get it
	
	return playerStat[playerid][pVehStatIDs][position];
}



// TODO sql support
stock offline_player_give_money(playername[], ammount)
{
	// if player is online
	new playerid = getPlayerID(playername);
	if(playerid != INVALID_PLAYER_ID)
	{
		GivePlayerMoney(playerid, ammount);
		playerStat_save(playerid);
	}
	
	// if player is offline
	else
	{
		if(!playerdata_exists(playername)) return; // if player does not exist at all
		// load data into temp
		strcpy(temp_playerStat[pName], playername, MAX_PLAYER_NAME);
		playerStat_load(playername, INVALID_PLAYER_ID);
		// give them the mondy
		temp_playerStat[pMoneyOwed] += ammount;
		// save
		playerStat_save(temp_playerStat);
	}
}

// TODO sql support
stock offline_player_vehicle_sold(playername[], vehiclename[], saleprice)
{
	// if player is online
	new playerid = getPlayerID(playername);
	if(playerid != INVALID_PLAYER_ID)
	{
		player_vehicle_remove(playerStat[playerid], vehiclename); // take the vehicle
		GivePlayerMoney(playerid, saleprice); // give the money
		playerStat_save(playerStat[playerid]);
		return;
	}
	
	// if player is offline
	else
	{
		if(!playerdata_exists(playername)) return; // if player does not exist at all
		// load data into temp
		strcpy(temp_playerStat[pName], playername, MAX_PLAYER_NAME);
		playerStat_load(temp_playerStat[pName], INVALID_PLAYER_ID);
		// take the vehicle
		if(player_vehicle_remove(temp_playerStat, vehiclename)) return; // didn't remove vehicle, they didn't have it
		// give them the money
		temp_playerStat[pMoneyOwed] += saleprice;
		// save
		playerStat_save(temp_playerStat);
		return;
	}
}

// TODO sql support
stock offline_player_vehicle_give(playername[], vehiclename[])
{
	// if player is online
	new playerid = getPlayerID(playername);
	if(playerid != INVALID_PLAYER_ID)
	{
		player_vehicle_add(playerStat[playerid], vehiclename); // give the vehicle
		playerStat_save(playerStat[playerid]);
		return;
	}
	
	// if player is offline
	else
	{
		if(!playerdata_exists(playername)) return; // if player does not exist at all
		// load data into temp
		strcpy(temp_playerStat[pName], playername, MAX_PLAYER_NAME);
		playerStat_load(playername, INVALID_PLAYER_ID);
		// give the vehicle
		player_vehicle_add(temp_playerStat, vehiclename);
		// save
		playerStat_save(temp_playerStat);
		return;
	}
}


/*
CMD:pload(playerid, params[])
{
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid, playername, MAX_PLAYER_NAME);
	playerStat_load(playerid);
	return 1;
}

CMD:psave(playerid, params[])
{
	GetPlayerName(playerid, playerStat[playerid][pName], MAX_PLAYER_NAME);
	playerStat_save(playerid);
	return 1;
}

CMD:ptt(playerid, params[])
{
	printf("%d %s", playerStat[playerid][pMoneyOwed], playerStat[playerid][pVehiclesNames]);
	return 1;
}
*/


stock playerdata_exists(name[])
{
	if(primary_database == DATABASE_INI)
	{
		new fileName[256];
		format(fileName, sizeof(fileName), "%s%s.ini", PLAYER_STATS_LOCATION, name);
		// if file exists, or there is a stat in memory (name not case sensitive), return true.
		if(fexist(fileName) || getPlayerID(name) != INVALID_PLAYER_ID) return 1; // player already exist

		return 0; // does not exist
	}
	
	if(primary_database == DATABASE_SQL)
	{
		new query[50 + MAX_PLAYER_NAME];
		mysql_format(database[0], query, sizeof(query), "SELECT * FROM playerdata WHERE Name='%e'", name);
		new Cache:result = mysql_query(database[0], query);
		cache_set_active(result, database[0]);
		if(cache_get_row_count(database[0]) > 0)
		{
			cache_delete(result);
			return 1; // vehicle exists
		}

		return 0; // does not exist
	}
	
	return 0;
}



// TODO doesn't handle sql
stock playerStat_load(name[], playerid)
{
	#if defined DATABASE_ENABLE_INI
	//GetPlayerName(playerid, playerStat[playerid][pName], MAX_PLAYER_NAME);
	playerStat_load_ini(name, playerid);
	#endif
}



// TODO doesn't handle sql
stock playerStat_save(player_stat[pStat])
{
	#if defined DATABASE_ENABLE_INI
	//GetPlayerName(playerid, playerStat[playerid][pName], MAX_PLAYER_NAME);
	playerStat_save_ini(player_stat);
	#endif
}



//
// Load from INI handler
//

stock playerStat_load_ini(name[], playerid)
{
	if(!valid_index(playerid, MAX_PLAYERS)) playerid = INVALID_PLAYER_ID; // don't return, this means load into temp

	// Make the filename.
	new fileName[256];
	format(fileName, sizeof(fileName), "%s%s.ini", PLAYER_STATS_LOCATION, name);

	// quit if the file doesn't exist
	if(!fexist(fileName)){
		//print("file not exists!");
		return 0;
	}

	// parse INI file
	INI_ParseFile(fileName, "playerStat_load_vehicles", .bExtra = true, .extra = playerid); // load the vehicles stats to the unused id

	// swap out SOH chars for NULL chars (little y_ini thing, uses SOH instead of NULL, and it messes up our system)
	if(playerid != INVALID_PLAYER_ID && playerStat[playerid][pVehiclesNames][0] == 1)		playerStat[playerid][pVehiclesNames][0] = '\0';
	else if(temp_playerStat[pVehiclesNames][0] == 1)										temp_playerStat[pVehiclesNames][0] = '\0';

	return 1;
}



//
// Save to INI
//

// this function does not have error catching, so make sure the vehstatid passed is valid!
stock playerStat_save_ini(player_stat[pStat])
{
	if(!player_stat[pName][0]) return 0; // no name to save

	// Make the filename.
	new filename[MAX_FILENAME];
	format(filename, sizeof (filename), "%s%s.ini", PLAYER_STATS_LOCATION, player_stat[pName]);

	// "INI_Open" returns a variable with tag "INI".
	new INI:file = INI_Open(filename);

	//
	// y_ini supports tags, that is:
	//
	//  [tag]
	//  key = value
	//

	// write the data for [vehicles]
	INI_SetTag(file, "vehicles");
	INI_WriteString	(file, "pVehiclesNames",	player_stat[pVehiclesNames]);
	INI_WriteInt	(file, "pVehiclesLen",		player_stat[pVehiclesLen]);
	INI_WriteInt	(file, "pMoneyOwed",		player_stat[pMoneyOwed]);

	// Close the file.
	INI_Close(file);

	return 1;
}



//
// Load from INI
//

forward playerStat_load_vehicles(playerid, name[], value[]);
public playerStat_load_vehicles(playerid, name[], value[])
{
	// load into global temp
	if(playerid == INVALID_PLAYER_ID)
	{
		INI_String	("pVehiclesNames",			temp_playerStat[pVehiclesNames], MAX_VEHICLES_NAMES);
		INI_Int		("pVehiclesLen",			temp_playerStat[pVehiclesLen]);
		INI_Int		("pMoneyOwed",				temp_playerStat[pMoneyOwed]);
		return 0;
	}
	
	INI_String	("pVehiclesNames",			playerStat[playerid][pVehiclesNames], MAX_VEHICLES_NAMES);
	INI_Int		("pVehiclesLen",			playerStat[playerid][pVehiclesLen]);
	INI_Int		("pMoneyOwed",				playerStat[playerid][pMoneyOwed]);
	return 0;
}









// update the players vehicle-related statistics
forward playerStat_update(playerid);
public playerStat_update(playerid)
{
	if(IsPlayerConnected(playerid) && GetPlayerState(playerid)==PLAYER_STATE_DRIVER)
	{
		new vehstatid = vehicleLink[GetPlayerVehicleID(playerid)];

		// set the vars
		playerStat[playerid][pDistanceThisVeh] = vehicleStat[vehstatid][vTotalDistance] - distanceAtStart[playerid];
		playerStat[playerid][pDistanceRemaining] = vehicleStat[vehstatid][vFuel] / vehicleConsumption(vehicleStat[vehstatid][vVehicleID]) * 100; // this just works
	}
}









forward playerStat_load_player(playerid, name[], value[]);
public playerStat_load_player(playerid, name[], value[])
{
	INI_Int		("pCash",					playerStat[playerid][pCash]);
	INI_Int		("pBank",					playerStat[playerid][pBank]);
	return 0;
}

stock todo_playerMoney_load(name[], playerid)
{
	if(!valid_index(playerid, MAX_PLAYERS)) playerid = INVALID_PLAYER_ID; // don't return, this means load into temp

	// Make the filename.
	new fileName[256];
	format(fileName, sizeof(fileName), "%s%s.ini", PLAYER_STATS_LOCATION, name);

	// quit if the file doesn't exist
	if(!fexist(fileName)){
		//print("file not exists!");
		return 0;
	}
	
	playerStat[playerid][pCash] = 0;
	playerStat[playerid][pBank] = 0;

	// parse INI file
	INI_ParseFile(fileName, "playerStat_load_player", .bExtra = true, .extra = playerid); // load the vehicles stats to the unused id

	// swap out SOH chars for NULL chars (little y_ini thing, uses SOH instead of NULL, and it messes up our system)

	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, playerStat[playerid][pCash]);

	ResetPlayerBank(playerid);
	GivePlayerBank(playerid, playerStat[playerid][pBank]);

	return 1;
}

stock todo_playerMoney_save(playerid)
{
	if(!valid_index(playerid, MAX_PLAYERS)) return 0; // no name to save

	// Make the filename.
	new filename[MAX_FILENAME];
	format(filename, sizeof (filename), "%s%s.ini", PLAYER_STATS_LOCATION, playerStat[playerid][pName]);

	// "INI_Open" returns a variable with tag "INI".
	new INI:file = INI_Open(filename);

	//
	// y_ini supports tags, that is:
	//
	//  [tag]
	//  key = value
	//

	// write the data for [player]
	INI_SetTag(file, "player");
	INI_WriteInt	(file, "pCash",				GetPlayerMoney(playerid));
	INI_WriteInt	(file, "pBank",				GetPlayerBank(playerid));

	// Close the file.
	INI_Close(file);

	return 1;
}
