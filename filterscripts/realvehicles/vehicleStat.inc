// =============================================================================
//                                vehicleStat.inc
//                    functions operating the vehicleStat index
// =============================================================================

/*
CREATE TABLE `saadat`.`vehicledata` (
  `ID` INT NOT NULL AUTO_INCREMENT,
  `Name` VARCHAR(10) NOT NULL,
  `Plate` VARCHAR(33) NOT NULL,
  `Model` INT NOT NULL,
  `Color1` INT NOT NULL DEFAULT 0,
  `Color2` INT NOT NULL DEFAULT 0,
  `PaintJob` INT NOT NULL DEFAULT -1,
  `Mod00` INT NOT NULL DEFAULT 0,
  `Mod01` INT NOT NULL DEFAULT 0,
  `Mod02` INT NOT NULL DEFAULT 0,
  `Mod03` INT NOT NULL DEFAULT 0,
  `Mod04` INT NOT NULL DEFAULT 0,
  `Mod05` INT NOT NULL DEFAULT 0,
  `Mod06` INT NOT NULL DEFAULT 0,
  `Mod07` INT NOT NULL DEFAULT 0,
  `Mod08` INT NOT NULL DEFAULT 0,
  `Mod09` INT NOT NULL DEFAULT 0,
  `Mod10` INT NOT NULL DEFAULT 0,
  `Mod11` INT NOT NULL DEFAULT 0,
  `Mod12` INT NOT NULL DEFAULT 0,
  `Mod13` INT NOT NULL DEFAULT 0,
  `Mod14` INT NOT NULL DEFAULT 0,
  `Mod15` INT NOT NULL DEFAULT 0,
  `Mod16` INT NOT NULL DEFAULT 0,
  `Armour` FLOAT NOT NULL DEFAULT 0,
  `WorldID` INT NOT NULL DEFAULT 0,
  `Spawned` INT NOT NULL DEFAULT 0,
  `SpawnPositionX` FLOAT NOT NULL DEFAULT 0,
  `SpawnPositionY` FLOAT NOT NULL DEFAULT 0,
  `SpawnPositionZ` FLOAT NOT NULL DEFAULT 0,
  `SpawnPositionR` FLOAT NOT NULL DEFAULT 0,
  `ParkPositionX` FLOAT NOT NULL DEFAULT 0,
  `ParkPositionY` FLOAT NOT NULL DEFAULT 0,
  `ParkPositionZ` FLOAT NOT NULL DEFAULT 0,
  `ParkPositionR` FLOAT NOT NULL DEFAULT 0,
  `LastPlayer` VARCHAR(25) NULL,
  `Faction` VARCHAR(45) NULL,
  `Owner` VARCHAR(25) NULL,
  `Kills` INT NOT NULL DEFAULT 0,
  `Death` INT NOT NULL DEFAULT 0,
  `Health` FLOAT NOT NULL DEFAULT 0,
  `DamageStatus0` INT NOT NULL DEFAULT 0,
  `DamageStatus1` INT NOT NULL DEFAULT 0,
  `DamageStatus2` INT NOT NULL DEFAULT 0,
  `DamageStatus3` INT NOT NULL DEFAULT 0,
  `VehicleParams` INT NOT NULL DEFAULT 0,
  `Engine` INT NOT NULL DEFAULT 0,
  `Lights` INT NOT NULL DEFAULT 0,
  `Alarm` INT NOT NULL DEFAULT 0,
  `vLock` INT NOT NULL DEFAULT 0,
  `Bonnet` INT NOT NULL DEFAULT 0,
  `Boot` INT NOT NULL DEFAULT 0,
  `Objective` INT NOT NULL DEFAULT 0,
  `TotalDistance` FLOAT NOT NULL DEFAULT 0,
  `Fuel` FLOAT NOT NULL DEFAULT 0,
  `FuelTypeInTank` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `Name_UNIQUE` (`Name` ASC),
  UNIQUE INDEX `ID_UNIQUE` (`ID` ASC),
  UNIQUE INDEX `Plate_UNIQUE` (`Plate` ASC));
*/



// find an unused vehstatid, and return it
stock vehicleStat_newid()
{
	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++){
		if(vehicleStat[vehstatid][vModel] == -1){
			vehicleStat[vehstatid][vModel] = -2; // so this function won't get it again
			return vehstatid;
		}
	}

	return INVALID_VEHSTAT_ID;
}



stock vehicleStat_changeID(newID, oldID)
{
	vehicleStat[newID][vVehicleID]			= vehicleStat[oldID][vVehicleID];

	vehicleStat[newID][vName]				= vehicleStat[oldID][vName];
	vehicleStat[newID][vPlate]				= vehicleStat[oldID][vPlate];
	vehicleStat[newID][vStatus]				= vehicleStat[oldID][vStatus];
	vehicleStat[newID][vValue]				= vehicleStat[oldID][vValue];
	vehicleStat[newID][vModel]				= vehicleStat[oldID][vModel];
	vehicleStat[newID][vColorScheme]		= vehicleStat[oldID][vColorScheme];
	vehicleStat[newID][vColor1]				= vehicleStat[oldID][vColor1];
	vehicleStat[newID][vColor2]				= vehicleStat[oldID][vColor2];
	vehicleStat[newID][vPaintJob]			= vehicleStat[oldID][vPaintJob];
	vehicleStat[newID][vMod00]				= vehicleStat[oldID][vMod00];
	vehicleStat[newID][vMod01]				= vehicleStat[oldID][vMod01];
	vehicleStat[newID][vMod02]				= vehicleStat[oldID][vMod02];
	vehicleStat[newID][vMod03]				= vehicleStat[oldID][vMod03];
	vehicleStat[newID][vMod04]				= vehicleStat[oldID][vMod04];
	vehicleStat[newID][vMod05]				= vehicleStat[oldID][vMod05];
	vehicleStat[newID][vMod06]				= vehicleStat[oldID][vMod06];
	vehicleStat[newID][vMod07]				= vehicleStat[oldID][vMod07];
	vehicleStat[newID][vMod08]				= vehicleStat[oldID][vMod08];
	vehicleStat[newID][vMod09]				= vehicleStat[oldID][vMod09];
	vehicleStat[newID][vMod10]				= vehicleStat[oldID][vMod10];
	vehicleStat[newID][vMod11]				= vehicleStat[oldID][vMod11];
	vehicleStat[newID][vMod12]				= vehicleStat[oldID][vMod12];
	vehicleStat[newID][vMod13]				= vehicleStat[oldID][vMod13];
	vehicleStat[newID][vMod14]				= vehicleStat[oldID][vMod14];
	vehicleStat[newID][vMod15]				= vehicleStat[oldID][vMod15];
	vehicleStat[newID][vMod16]				= vehicleStat[oldID][vMod16];
	vehicleStat[newID][vHealthMax]			= vehicleStat[oldID][vHealthMax];

	vehicleStat[newID][vWorldID]			= vehicleStat[oldID][vWorldID];
	vehicleStat[newID][vSpawned]			= vehicleStat[oldID][vSpawned];
	vehicleStat[newID][vSpawnPositionX]		= vehicleStat[oldID][vSpawnPositionX];
	vehicleStat[newID][vSpawnPositionY]		= vehicleStat[oldID][vSpawnPositionY];
	vehicleStat[newID][vSpawnPositionZ]		= vehicleStat[oldID][vSpawnPositionZ];
	vehicleStat[newID][vSpawnPositionR]		= vehicleStat[oldID][vSpawnPositionR];
	vehicleStat[newID][vParkPositionX]		= vehicleStat[oldID][vParkPositionX];
	vehicleStat[newID][vParkPositionY]		= vehicleStat[oldID][vParkPositionY];
	vehicleStat[newID][vParkPositionZ]		= vehicleStat[oldID][vParkPositionZ];
	vehicleStat[newID][vParkPositionR]		= vehicleStat[oldID][vParkPositionR];

	// statistics
	vehicleStat[newID][vLastDriver]			= vehicleStat[oldID][vLastDriver];
	vehicleStat[newID][vOwner]				= vehicleStat[oldID][vOwner];
	vehicleStat[newID][vKills]				= vehicleStat[oldID][vKills];
	vehicleStat[newID][vDeath]				= vehicleStat[oldID][vDeath];

	// the realtime variables
	vehicleStat[newID][vParams]				= vehicleStat[oldID][vParams];
	vehicleStat[newID][vHealth]				= vehicleStat[oldID][vHealth];
	vehicleStat[newID][vDamageStatus0]		= vehicleStat[oldID][vDamageStatus0];
	vehicleStat[newID][vDamageStatus1]		= vehicleStat[oldID][vDamageStatus1];
	vehicleStat[newID][vDamageStatus2]		= vehicleStat[oldID][vDamageStatus2];
	vehicleStat[newID][vDamageStatus3]		= vehicleStat[oldID][vDamageStatus3];

	// extended stats
	vehicleStat[newID][vTotalDistance]		= vehicleStat[oldID][vTotalDistance];
	vehicleStat[newID][vFuel]				= vehicleStat[oldID][vFuel];
	vehicleStat[newID][vFuelTypeInTank]		= vehicleStat[oldID][vFuelTypeInTank];

	// in game stats
	vehicleStat[newID][vDamage]				= vehicleStat[oldID][vDamage];
	vehicleStat[newID][vSpeedActual]		= vehicleStat[oldID][vSpeedActual];
	vehicleStat[newID][vSpeedGround]		= vehicleStat[oldID][vSpeedGround];
	vehicleStat[newID][vHeading]			= vehicleStat[oldID][vHeading];
	vehicleStat[newID][vAttitude]			= vehicleStat[oldID][vAttitude];
	vehicleStat[newID][vAltitude]			= vehicleStat[oldID][vAltitude];

	// reset the old id so it can be used again
	vehicleStat_clear(oldID);

	return;
}



// initialize the vehicleStat
stock vehicleStat_init()
{
	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++)
	{
		vehicleStat_clear(vehstatid);
	}

	return;
}



// call this function before disconnecting a vehicle from the game
stock vehicleStat_clear(vehstatid)
{
	vehicleStat[vehstatid][vVehicleID]			= INVALID_VEHICLE_ID;

	vehicleStat[vehstatid][vName][0]			= '\0';
	vehicleStat[vehstatid][vPlate][0]			= '\0';
	vehicleStat[vehstatid][vStatus]				= 0;
	vehicleStat[vehstatid][vValue]				= 0;
	vehicleStat[vehstatid][vModel]				= -1;
	vehicleStat[vehstatid][vColor1]				= -1;
	vehicleStat[vehstatid][vColor2]				= -1;
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
	vehicleStat[vehstatid][vHealthMax]			= 0;

	vehicleStat[vehstatid][vWorldID]			= 0;
	vehicleStat[vehstatid][vSpawned]			= 0;
	vehicleStat[vehstatid][vSpawnPositionX]		= 0;
	vehicleStat[vehstatid][vSpawnPositionY]		= 0;
	vehicleStat[vehstatid][vSpawnPositionZ]		= 0;
	vehicleStat[vehstatid][vSpawnPositionR]		= 0;
	vehicleStat[vehstatid][vParkPositionX]		= 0;
	vehicleStat[vehstatid][vParkPositionY]		= 0;
	vehicleStat[vehstatid][vParkPositionZ]		= 0;
	vehicleStat[vehstatid][vParkPositionR]		= 0;

	// statistics
	vehicleStat[vehstatid][vLastDriver][0]		= '\0';
	vehicleStat[vehstatid][vOwner][0]			= '\0';
	vehicleStat[vehstatid][vKills]				= 0;
	vehicleStat[vehstatid][vDeath]				= 0;

	// the realtime variables
	vehicleStat[vehstatid][vParams]				= 0;
	vehicleStat[vehstatid][vHealth]				= 0.0;
	vehicleStat[vehstatid][vDamageStatus0]		= 0;
	vehicleStat[vehstatid][vDamageStatus1]		= 0;
	vehicleStat[vehstatid][vDamageStatus2]		= 0;
	vehicleStat[vehstatid][vDamageStatus3]		= 0;

	// extended stats
	vehicleStat[vehstatid][vTotalDistance]		= 0;
	vehicleStat[vehstatid][vFuel]				= 0;
	vehicleStat[vehstatid][vFuelTypeInTank]		= 0;

	// in game stats
	vehicleStat[vehstatid][vDamage]				= 0;
	vehicleStat[vehstatid][vSpeedActual]		= 0;
	vehicleStat[vehstatid][vSpeedGround]		= 0;
	vehicleStat[vehstatid][vHeading]			= 0;
	vehicleStat[vehstatid][vAttitude]			= 0;
	vehicleStat[vehstatid][vAltitude]			= 0;

	return;
}







// find the vehstatid of a vehicle, case sensitive
stock vehicleStat_getid(name[])
{
	if(isnull(name)) return INVALID_VEHSTAT_ID;
	for(new vehstatid=0; vehstatid<MAX_VEHICLES; vehstatid++){
		// if the numplate matches another numplate
		if( strmatch(vehicleStat[vehstatid][vName], name) ){
			return vehstatid;
		}
	}
	return INVALID_VEHSTAT_ID;
}





stock vehicledata_exists(name[])
{
	if(primary_database == DATABASE_INI)
	{
		new fileName[256];
		format(fileName, sizeof(fileName), "%s%s.ini", VEHICLE_STATS_LOCATION, name);
		// if file exists, or there is a stat in memory (name not case sensitive), return true.
		if(fexist(fileName) || vehicleStat_getid(name) != INVALID_VEHSTAT_ID) return 1; // vehicle already exist
		
		return 0; // does not exist
	}
	
	if(primary_database == DATABASE_SQL)
	{
		
		new query[50 + MAX_VEHICLE_NAME];
		mysql_format(database[0], query, sizeof(query), "SELECT * FROM vehicledata WHERE Name='%e'", name);
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

stock vehicledata_save(vehstatid)
{
	if(vehicleStat[vehstatid][vStatus] & VEHSTATUS_GUEST) return; // don't save if it's a guest vehicle
	
	#if defined DATABASE_ENABLE_INI
	//print("saving vehicles to ini...");
	vehicleStat_save_ini(vehstatid);
	#endif
	
	#if defined DATABASE_ENABLE_SQL
	//print("saving vehicles to sql...");
	vehicleStat_save_sql(vehstatid);
	#endif
	
	return;
}

stock vehicledata_load(name[])
{
	new vehstatid = vehicleStat_newid(); // get an unused id for the joining vehicle to connect as
	if(vehstatid == INVALID_VEHSTAT_ID) return INVALID_VEHSTAT_ID; // failed to get a good id, server full?

	switch(primary_database)
	{
		case DATABASE_INI:
			if(!vehicleStat_load_ini(name, vehstatid)) return vehstatid; // sucess
			
		case DATABASE_SQL:
			if(!vehicleStat_load_sql(name, vehstatid)) return vehstatid; // sucess
	}
	
	vehicleStat_clear(vehstatid);

	return INVALID_VEHSTAT_ID;
}

// TODO this doesn't handle SQL
stock vehicledata_rename(vehstatid, newname[])
{
	// get the old filename of the vehicle
	new vehicleFile[MAX_FILE_NAME];
	format(vehicleFile, sizeof(vehicleFile), "%s%s.ini", VEHICLE_STATS_LOCATION, vehicleStat[vehstatid][vName]);

	if(fexist(vehicleFile)){
		fremove(vehicleFile); // delete the old file
	}
	
	// change name and resave
	strcopy(vehicleStat[vehstatid][vName], MAX_VEHICLE_NAME, newname);
	vehicledata_save(vehstatid);
}




stock vehicleStat_load_ini(name[], vehstatid)
{
	// Make the filename.
	new fileName[256];
	format(fileName, sizeof(fileName), "%s%s.ini", VEHICLE_STATS_LOCATION, name);
	
	// quit if the file doesn't exist
	if(!fexist(fileName)){
		//print("file not exists!");
		return 1;
	}

	// parse INI file
	INI_ParseFile(fileName, "vehicleStat_load_%s", .bExtra = true, .extra = vehstatid); // load the vehicles stats to the unused id

	// swap out SOH chars for NULL chars (little y_ini thing, uses SOH instead of NULL, and it messes up our system)
	if(vehicleStat[vehstatid][vName][0] == 1)		vehicleStat[vehstatid][vName][0] = '\0';
	if(vehicleStat[vehstatid][vPlate][0] == 1)		vehicleStat[vehstatid][vPlate][0] = '\0';
	if(vehicleStat[vehstatid][vLastDriver][0] == 1)	vehicleStat[vehstatid][vLastDriver][0] = '\0';
	if(vehicleStat[vehstatid][vOwner][0] == 1)		vehicleStat[vehstatid][vOwner][0] = '\0';

	return 0;
}

// this function does not have error catching, so make sure the vehstatid passed is valid!
stock vehicleStat_save_ini(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
    if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	// update some vars
	if(vehicleid != INVALID_VEHICLE_ID){
		// get the color
		GetVehicleColor(vehicleid, vehicleStat[vehstatid][vColor1], vehicleStat[vehstatid][vColor2]);

		// get the position and set as park position
		GetVehiclePos(vehicleid, vehicleStat[vehstatid][vParkPositionX], vehicleStat[vehstatid][vParkPositionY], vehicleStat[vehstatid][vParkPositionZ]);
		GetVehicleZAngle(vehicleid, vehicleStat[vehstatid][vParkPositionR]);

		vehicle_virtualworld_get(vehicleid);
		vehicle_health_get(vehicleid);
		vehicle_damagestatus_get(vehicleid);
		vehicle_params_get(vehicleid);
	}

	// Make the filename.
	new filename[MAX_FILENAME];
	format(filename, sizeof (filename), "%s%s.ini", VEHICLE_STATS_LOCATION, vehicleStat[vehstatid][vName]);

	// "INI_Open" returns a variable with tag "INI".
	new INI:file = INI_Open(filename);

	//
	// y_ini supports tags, that is:
	//
	//  [tag]
	//  key = value
	//
	
	// write the data for [info]
	INI_SetTag(file, "info");
	INI_WriteString	(file, "vName",				vehicleStat[vehstatid][vName]);
	INI_WriteString	(file, "vPlate",			vehicleStat[vehstatid][vPlate]);
	INI_WriteInt	(file, "vStatus",			vehicleStat[vehstatid][vStatus]);
	INI_WriteInt	(file, "vValue",			vehicleStat[vehstatid][vValue]);
	INI_WriteInt	(file, "vModel",			vehicleStat[vehstatid][vModel]);
	INI_WriteInt	(file, "vColor1",			vehicleStat[vehstatid][vColor1]);
	INI_WriteInt	(file, "vColor2",			vehicleStat[vehstatid][vColor2]);
	INI_WriteInt	(file, "vPaintJob",			vehicleStat[vehstatid][vPaintJob]);
	INI_WriteInt	(file, "vMod00",			vehicleStat[vehstatid][vMod00]);
	INI_WriteInt	(file, "vMod01",			vehicleStat[vehstatid][vMod01]);
	INI_WriteInt	(file, "vMod02",			vehicleStat[vehstatid][vMod02]);
	INI_WriteInt	(file, "vMod03",			vehicleStat[vehstatid][vMod03]);
	INI_WriteInt	(file, "vMod04",			vehicleStat[vehstatid][vMod04]);
	INI_WriteInt	(file, "vMod05",			vehicleStat[vehstatid][vMod05]);
	INI_WriteInt	(file, "vMod06",			vehicleStat[vehstatid][vMod06]);
	INI_WriteInt	(file, "vMod07",			vehicleStat[vehstatid][vMod07]);
	INI_WriteInt	(file, "vMod08",			vehicleStat[vehstatid][vMod08]);
	INI_WriteInt	(file, "vMod09",			vehicleStat[vehstatid][vMod09]);
	INI_WriteInt	(file, "vMod10",			vehicleStat[vehstatid][vMod10]);
	INI_WriteInt	(file, "vMod11",			vehicleStat[vehstatid][vMod11]);
	INI_WriteInt	(file, "vMod12",			vehicleStat[vehstatid][vMod12]);
	INI_WriteInt	(file, "vMod13",			vehicleStat[vehstatid][vMod13]);
	INI_WriteInt	(file, "vMod14",			vehicleStat[vehstatid][vMod14]);
	INI_WriteInt	(file, "vMod15",			vehicleStat[vehstatid][vMod15]);
	INI_WriteInt	(file, "vMod16",			vehicleStat[vehstatid][vMod16]);
	INI_WriteFloat	(file, "vHealthMax",		vehicleStat[vehstatid][vHealthMax]);

	// position
	INI_SetTag(file, "position");
	INI_WriteInt	(file, "vWorldID",			vehicleStat[vehstatid][vWorldID]);
	INI_WriteInt	(file, "vSpawned",			vehicleStat[vehstatid][vSpawned]);
	INI_WriteFloat	(file, "vSpawnPositionX",	vehicleStat[vehstatid][vSpawnPositionX]);
	INI_WriteFloat	(file, "vSpawnPositionY",	vehicleStat[vehstatid][vSpawnPositionY]);
	INI_WriteFloat	(file, "vSpawnPositionZ",	vehicleStat[vehstatid][vSpawnPositionZ]);
	INI_WriteFloat	(file, "vSpawnPositionR",	vehicleStat[vehstatid][vSpawnPositionR]);
	INI_WriteFloat	(file, "vParkPositionX",	vehicleStat[vehstatid][vParkPositionX]);
	INI_WriteFloat	(file, "vParkPositionY",	vehicleStat[vehstatid][vParkPositionY]);
	INI_WriteFloat	(file, "vParkPositionZ",	vehicleStat[vehstatid][vParkPositionZ]);
	INI_WriteFloat	(file, "vParkPositionR",	vehicleStat[vehstatid][vParkPositionR]);

	// statistics
	INI_SetTag(file, "statistics");
	INI_WriteString	(file, "vLastDriver",		vehicleStat[vehstatid][vLastDriver]);
	INI_WriteString	(file, "vOwner",			vehicleStat[vehstatid][vOwner]);
	INI_WriteInt	(file, "vKills",			vehicleStat[vehstatid][vKills]);
	INI_WriteInt	(file, "vDeath",			vehicleStat[vehstatid][vDeath]);

	// realtime
	INI_SetTag(file, "realtime");
	INI_WriteInt	(file, "vParams",			vehicleStat[vehstatid][vParams]);
	INI_WriteFloat	(file, "vHealth",			vehicleStat[vehstatid][vHealth]);
	INI_WriteInt	(file, "vDamageStatus0",	vehicleStat[vehstatid][vDamageStatus0]);
	INI_WriteInt	(file, "vDamageStatus1",	vehicleStat[vehstatid][vDamageStatus1]);
	INI_WriteInt	(file, "vDamageStatus2",	vehicleStat[vehstatid][vDamageStatus2]);
	INI_WriteInt	(file, "vDamageStatus3",	vehicleStat[vehstatid][vDamageStatus3]);

	INI_WriteFloat	(file, "vTotalDistance",	vehicleStat[vehstatid][vTotalDistance]);
	INI_WriteFloat	(file, "vFuel",				vehicleStat[vehstatid][vFuel]);
	INI_WriteInt	(file, "vFuelTypeInTank",	vehicleStat[vehstatid][vFuelTypeInTank]);

	// Close the file.
	INI_Close(file);

	return 1;
}


//
// Load from INI
//

forward vehicleStat_load_info(vehstatid, name[], value[]);
public vehicleStat_load_info(vehstatid, name[], value[])
{
	INI_String	("vName",			vehicleStat[vehstatid][vName], MAX_VEHICLE_NAME);
	INI_String	("vPlate",			vehicleStat[vehstatid][vPlate], MAX_VEHICLE_PLATE);
	INI_Int		("vStatus",			vehicleStat[vehstatid][vStatus]);
	INI_Int		("vValue",			vehicleStat[vehstatid][vValue]);
	INI_Int		("vModel",			vehicleStat[vehstatid][vModel]);
	INI_Int		("vColor1",			vehicleStat[vehstatid][vColor1]);
	INI_Int		("vColor2",			vehicleStat[vehstatid][vColor2]);
	INI_Int		("vPaintJob",		vehicleStat[vehstatid][vPaintJob]);
	INI_Int		("vMod00",			vehicleStat[vehstatid][vMod00]);
	INI_Int		("vMod01",			vehicleStat[vehstatid][vMod01]);
	INI_Int		("vMod02",			vehicleStat[vehstatid][vMod02]);
	INI_Int		("vMod03",			vehicleStat[vehstatid][vMod03]);
	INI_Int		("vMod04",			vehicleStat[vehstatid][vMod04]);
	INI_Int		("vMod05",			vehicleStat[vehstatid][vMod05]);
	INI_Int		("vMod06",			vehicleStat[vehstatid][vMod06]);
	INI_Int		("vMod07",			vehicleStat[vehstatid][vMod07]);
	INI_Int		("vMod08",			vehicleStat[vehstatid][vMod08]);
	INI_Int		("vMod09",			vehicleStat[vehstatid][vMod09]);
	INI_Int		("vMod10",			vehicleStat[vehstatid][vMod10]);
	INI_Int		("vMod11",			vehicleStat[vehstatid][vMod11]);
	INI_Int		("vMod12",			vehicleStat[vehstatid][vMod12]);
	INI_Int		("vMod13",			vehicleStat[vehstatid][vMod13]);
	INI_Int		("vMod14",			vehicleStat[vehstatid][vMod14]);
	INI_Int		("vMod15",			vehicleStat[vehstatid][vMod15]);
	INI_Int		("vMod16",			vehicleStat[vehstatid][vMod16]);
	INI_Float	("vHealthMax",		vehicleStat[vehstatid][vHealthMax]);
	return 0;
}

forward vehicleStat_load_position(vehstatid, name[], value[]);
public vehicleStat_load_position(vehstatid, name[], value[])
{
	INI_Int		("vWorldID",		vehicleStat[vehstatid][vWorldID]);
	INI_Int		("vSpawned",		vehicleStat[vehstatid][vSpawned]);
	INI_Float	("vSpawnPositionX",	vehicleStat[vehstatid][vSpawnPositionX]);
	INI_Float	("vSpawnPositionY",	vehicleStat[vehstatid][vSpawnPositionY]);
	INI_Float	("vSpawnPositionZ",	vehicleStat[vehstatid][vSpawnPositionZ]);
	INI_Float	("vSpawnPositionR",	vehicleStat[vehstatid][vSpawnPositionR]);
	INI_Float	("vParkPositionX",	vehicleStat[vehstatid][vParkPositionX]);
	INI_Float	("vParkPositionY",	vehicleStat[vehstatid][vParkPositionY]);
	INI_Float	("vParkPositionZ",	vehicleStat[vehstatid][vParkPositionZ]);
	INI_Float	("vParkPositionR",	vehicleStat[vehstatid][vParkPositionR]);
	return 0;
}

forward vehicleStat_load_statistics(vehstatid, name[], value[]);
public vehicleStat_load_statistics(vehstatid, name[], value[])
{
	INI_String	("vLastDriver",		vehicleStat[vehstatid][vLastDriver], MAX_PLAYER_NAME+1);
	INI_String	("vOwner",			vehicleStat[vehstatid][vOwner], MAX_PLAYER_NAME+1);
	INI_Int		("vKills",			vehicleStat[vehstatid][vKills]);
	INI_Int		("vDeath",			vehicleStat[vehstatid][vDeath]);
	return 0;
}

forward vehicleStat_load_realtime(vehstatid, name[], value[]);
public vehicleStat_load_realtime(vehstatid, name[], value[])
{
	INI_Int		("vParams",			vehicleStat[vehstatid][vParams]);
	INI_Float	("vHealth",			vehicleStat[vehstatid][vHealth]);
	INI_Int		("vDamageStatus0",	vehicleStat[vehstatid][vDamageStatus0]);
	INI_Int		("vDamageStatus1",	vehicleStat[vehstatid][vDamageStatus1]);
	INI_Int		("vDamageStatus2",	vehicleStat[vehstatid][vDamageStatus2]);
	INI_Int		("vDamageStatus3",	vehicleStat[vehstatid][vDamageStatus3]);

	INI_Float	("vTotalDistance",	vehicleStat[vehstatid][vTotalDistance]);
	INI_Float	("vFuel",			vehicleStat[vehstatid][vFuel]);
	INI_Int		("vFuelTypeInTank",	vehicleStat[vehstatid][vFuelTypeInTank]);
	return 0;
}



// log a vehicles data for debugging
stock vehicleStat_logdump(vehstatid)
{
	if(vehstatid==INVALID_VEHSTAT_ID) return 1; // id is invalid

	printf("\n ------------------------------------------------------------------------------");
	printf("Vehicle Statistics Log Dump (vehstatid id %d)", vehstatid);
	printf("vName:			%s", vehicleStat[vehstatid][vName]);
	printf("vPlate:			%s", vehicleStat[vehstatid][vPlate]);
	printf("vStatus:		%i", vehicleStat[vehstatid][vStatus]);
	printf("vValue:			%i", vehicleStat[vehstatid][vValue]);
	printf("vModel:			%i", vehicleStat[vehstatid][vModel]);
	printf("vColor1:		%i", vehicleStat[vehstatid][vColor1]);
	printf("vColor2:		%i", vehicleStat[vehstatid][vColor2]);
	printf("vPaintJob:		%i", vehicleStat[vehstatid][vPaintJob]);
	printf("vMod00:			%i", vehicleStat[vehstatid][vMod00]);
	printf("vMod01:			%i", vehicleStat[vehstatid][vMod01]);
	printf("vMod02:			%i", vehicleStat[vehstatid][vMod02]);
	printf("vMod03:			%i", vehicleStat[vehstatid][vMod03]);
	printf("vMod04:			%i", vehicleStat[vehstatid][vMod04]);
	printf("vMod05:			%i", vehicleStat[vehstatid][vMod05]);
	printf("vMod06:			%i", vehicleStat[vehstatid][vMod06]);
	printf("vMod07:			%i", vehicleStat[vehstatid][vMod07]);
	printf("vMod08:			%i", vehicleStat[vehstatid][vMod08]);
	printf("vMod09:			%i", vehicleStat[vehstatid][vMod09]);
	printf("vMod10:			%i", vehicleStat[vehstatid][vMod10]);
	printf("vMod11:			%i", vehicleStat[vehstatid][vMod11]);
	printf("vMod12:			%i", vehicleStat[vehstatid][vMod12]);
	printf("vMod13:			%i", vehicleStat[vehstatid][vMod13]);
	printf("vMod14:			%i", vehicleStat[vehstatid][vMod14]);
	printf("vMod15:			%i", vehicleStat[vehstatid][vMod15]);
	printf("vMod16:			%i", vehicleStat[vehstatid][vMod16]);
	printf("vHealthMax:		%f", vehicleStat[vehstatid][vHealthMax]);

	printf("vWorldID:		%i", vehicleStat[vehstatid][vWorldID]);
	printf("vSpawned:		%i", vehicleStat[vehstatid][vSpawned]);
	printf("vSpawnPositionX:	%f", vehicleStat[vehstatid][vSpawnPositionX]);
	printf("vSpawnPositionY:	%f", vehicleStat[vehstatid][vSpawnPositionY]);
	printf("vSpawnPositionZ:	%f", vehicleStat[vehstatid][vSpawnPositionZ]);
	printf("vSpawnPositionR:	%f", vehicleStat[vehstatid][vSpawnPositionR]);
	printf("vParkPositionX:		%f", vehicleStat[vehstatid][vParkPositionX]);
	printf("vParkPositionY:		%f", vehicleStat[vehstatid][vParkPositionY]);
	printf("vParkPositionZ:		%f", vehicleStat[vehstatid][vParkPositionZ]);
	printf("vParkPositionR:		%f", vehicleStat[vehstatid][vParkPositionR]);

	printf("vLastDriver:		%s", vehicleStat[vehstatid][vLastDriver]);
	printf("vOwner:				%s", vehicleStat[vehstatid][vOwner]);
	printf("vKills:				%i", vehicleStat[vehstatid][vKills]);
	printf("vDeath:				%i", vehicleStat[vehstatid][vDeath]);

	printf("vParams:			%i", vehicleStat[vehstatid][vParams]);
	printf("vHealth:			%f", vehicleStat[vehstatid][vHealth]);
	printf("vDamageStatus0:		%i", vehicleStat[vehstatid][vDamageStatus0]);
	printf("vDamageStatus1:		%i", vehicleStat[vehstatid][vDamageStatus1]);
	printf("vDamageStatus2:		%i", vehicleStat[vehstatid][vDamageStatus2]);
	printf("vDamageStatus3:		%i", vehicleStat[vehstatid][vDamageStatus3]);

	printf("vTotalDistance:		%f", vehicleStat[vehstatid][vTotalDistance]);
	printf("vFuel:				%f", vehicleStat[vehstatid][vFuel]);
	printf("vFuelTypeInTank:	%i", vehicleStat[vehstatid][vFuelTypeInTank]);
	printf(" ------------------------------------------------------------------------------");

	return 0;
}



forward vehicleStat_load_sql(name[], vehstatid);
public vehicleStat_load_sql(name[], vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return -1;	// index out of range - error code -1, fatal error.
    if(valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id already in use

	new query[55];
	mysql_format(database[0], query, sizeof(query), "SELECT * FROM vehicledata WHERE Name='%e';", name);
	new Cache:result = mysql_query(database[0], query, true);
	cache_set_active(result, database[0]);

	if(cache_get_row_count(database[0]) < 1) return 2; // vehicle not registered
	if(cache_get_row_count(database[0]) > 1) return 3; // very big fuck up

	new const row = 0;
	//cache_get_field_content(row, const field_name[], destination[], connectionHandle = 1, max_len = sizeof(destination))

	cache_get_field_content			(row,	"Name",				vehicleStat[vehstatid][vName],				database[0],	MAX_VEHICLE_NAME);
	cache_get_field_content			(row,	"Plate",			vehicleStat[vehstatid][vPlate],				database[0],	MAX_VEHICLE_PLATE);
	vehicleStat[vehstatid][vStatus]			=	cache_get_field_content_int		(row,	"Status",			database[0]);
	vehicleStat[vehstatid][vValue]			=	cache_get_field_content_int		(row,	"Value",			database[0]);
	vehicleStat[vehstatid][vModel]			=	cache_get_field_content_int		(row,	"Model",			database[0]);
	vehicleStat[vehstatid][vColor1]			=	cache_get_field_content_int		(row,	"Color1",			database[0]);
	vehicleStat[vehstatid][vColor2]			=	cache_get_field_content_int		(row,	"Color2",			database[0]);
	vehicleStat[vehstatid][vPaintJob]		=	cache_get_field_content_int		(row,	"PaintJob",			database[0]);
	vehicleStat[vehstatid][vMod00]			=	cache_get_field_content_int		(row,	"Mod00",			database[0]);
	vehicleStat[vehstatid][vMod01]			=	cache_get_field_content_int		(row,	"Mod01",			database[0]);
	vehicleStat[vehstatid][vMod02]			=	cache_get_field_content_int		(row,	"Mod02",			database[0]);
	vehicleStat[vehstatid][vMod03]			=	cache_get_field_content_int		(row,	"Mod03",			database[0]);
	vehicleStat[vehstatid][vMod04]			=	cache_get_field_content_int		(row,	"Mod04",			database[0]);
	vehicleStat[vehstatid][vMod05]			=	cache_get_field_content_int		(row,	"Mod05",			database[0]);
	vehicleStat[vehstatid][vMod06]			=	cache_get_field_content_int		(row,	"Mod06",			database[0]);
	vehicleStat[vehstatid][vMod07]			=	cache_get_field_content_int		(row,	"Mod07",			database[0]);
	vehicleStat[vehstatid][vMod08]			=	cache_get_field_content_int		(row,	"Mod08",			database[0]);
	vehicleStat[vehstatid][vMod09]			=	cache_get_field_content_int		(row,	"Mod09",			database[0]);
	vehicleStat[vehstatid][vMod10]			=	cache_get_field_content_int		(row,	"Mod10",			database[0]);
	vehicleStat[vehstatid][vMod11]			=	cache_get_field_content_int		(row,	"Mod11",			database[0]);
	vehicleStat[vehstatid][vMod12]			=	cache_get_field_content_int		(row,	"Mod12",			database[0]);
	vehicleStat[vehstatid][vMod13]			=	cache_get_field_content_int		(row,	"Mod13",			database[0]);
	vehicleStat[vehstatid][vMod14]			=	cache_get_field_content_int		(row,	"Mod14",			database[0]);
	vehicleStat[vehstatid][vMod15]			=	cache_get_field_content_int		(row,	"Mod15",			database[0]);
	vehicleStat[vehstatid][vMod16]			=	cache_get_field_content_int		(row,	"Mod16",			database[0]);
	vehicleStat[vehstatid][vHealthMax]		=	cache_get_field_content_float	(row,	"HealthMax",		database[0]);

	vehicleStat[vehstatid][vWorldID]		=	cache_get_field_content_int		(row,	"WorldID",			database[0]);
	vehicleStat[vehstatid][vSpawned]		=	cache_get_field_content_int		(row,	"Spawned",			database[0]);
	vehicleStat[vehstatid][vSpawnPositionX]	=	cache_get_field_content_float	(row,	"SpawnPositionX",	database[0]);
	vehicleStat[vehstatid][vSpawnPositionY]	=	cache_get_field_content_float	(row,	"SpawnPositionY",	database[0]);
	vehicleStat[vehstatid][vSpawnPositionZ]	=	cache_get_field_content_float	(row,	"SpawnPositionZ",	database[0]);
	vehicleStat[vehstatid][vSpawnPositionR]	=	cache_get_field_content_float	(row,	"SpawnPositionR",	database[0]);
	vehicleStat[vehstatid][vParkPositionX]	=	cache_get_field_content_float	(row,	"ParkPositionX",	database[0]);
	vehicleStat[vehstatid][vParkPositionY]	=	cache_get_field_content_float	(row,	"ParkPositionY",	database[0]);
	vehicleStat[vehstatid][vParkPositionZ]	=	cache_get_field_content_float	(row,	"ParkPositionZ",	database[0]);
	vehicleStat[vehstatid][vParkPositionR]	=	cache_get_field_content_float	(row,	"ParkPositionR",	database[0]);
	
	cache_get_field_content			(row,	"LastDriver",		vehicleStat[vehstatid][vLastDriver],		database[0],	MAX_PLAYER_NAME+1);
	cache_get_field_content			(row,	"Owner",			vehicleStat[vehstatid][vOwner],				database[0],	MAX_PLAYER_NAME+1);
	vehicleStat[vehstatid][vKills]			=	cache_get_field_content_int		(row,	"Kills",			database[0]);
	vehicleStat[vehstatid][vDeath]			=	cache_get_field_content_int		(row,	"Death",			database[0]);
	
	vehicleStat[vehstatid][vParams]			=	cache_get_field_content_int		(row,	"VehicleParams",	database[0]);
	vehicleStat[vehstatid][vHealth]			=	cache_get_field_content_float	(row,	"Health",			database[0]);
	vehicleStat[vehstatid][vDamageStatus0]	=	cache_get_field_content_int		(row,	"DamageStatus0",	database[0]);
	vehicleStat[vehstatid][vDamageStatus1]	=	cache_get_field_content_int		(row,	"DamageStatus1",	database[0]);
	vehicleStat[vehstatid][vDamageStatus2]	=	cache_get_field_content_int		(row,	"DamageStatus2",	database[0]);
	vehicleStat[vehstatid][vDamageStatus3]	=	cache_get_field_content_int		(row,	"DamageStatus3",	database[0]);

	vehicleStat[vehstatid][vTotalDistance]	=	cache_get_field_content_float	(row,	"TotalDistance",	database[0]);
	vehicleStat[vehstatid][vFuel]			=	cache_get_field_content_float	(row,	"Fuel",				database[0]);
	vehicleStat[vehstatid][vFuelTypeInTank]	=	cache_get_field_content_int		(row,	"FuelTypeInTank",	database[0]);

	cache_delete(result, database[0]);

	return 0;
}

// TODO this works perfectly, but has to make two separate queries separately
// they need to be made in one function call, but it just don't work when done that way.
forward vehicleStat_save_sql(vehstatid);
public vehicleStat_save_sql(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return -1;	// index out of range - error code -1, fatal error.
    if(!valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id not in use

	new query[20 * 56 + 205 + 512]; // 512 EXTRA BYTES SO WE CAN'T GET ERRORS
	
	mysql_format(database[0], query, sizeof(query),
		"INSERT INTO vehicledata(Name,Plate,Model) VALUES('%e','%e','%i') ON DUPLICATE KEY UPDATE Name='%e',Plate='%e',Model='%i';", // size of about 205
		vehicleStat[vehstatid][vName], vehicleStat[vehstatid][vPlate], vehicleStat[vehstatid][vModel],
		vehicleStat[vehstatid][vName], vehicleStat[vehstatid][vPlate], vehicleStat[vehstatid][vModel]
	);

	mysql_query(database[0], query, false);
	//printf("query -%s-", query);
	query[0] = 0;

	// the first 3 columns will already have been updated by the first query
	mysql_format(database[0], query, sizeof(query),	"%s UPDATE vehicledata SET", query);
	//mysql_format(database[0], query, sizeof(query),	"%s Name='%e'",				query,	vehicleStat[vehstatid][vName]);
	//mysql_format(database[0], query, sizeof(query),	"%s,Plate='%e'",			query,	vehicleStat[vehstatid][vPlate]);
	//mysql_format(database[0], query, sizeof(query),	"%s,Model='%i'",			query,	vehicleStat[vehstatid][vModel]);
	mysql_format(database[0], query, sizeof(query),	"%s Status='%i'",			query,	vehicleStat[vehstatid][vStatus]);
	mysql_format(database[0], query, sizeof(query),	"%s Value='%i'",			query,	vehicleStat[vehstatid][vValue]);
	mysql_format(database[0], query, sizeof(query),	"%s Color1='%i'",			query,	vehicleStat[vehstatid][vColor1]);
	mysql_format(database[0], query, sizeof(query),	"%s,Color2='%i'",			query,	vehicleStat[vehstatid][vColor2]);
	mysql_format(database[0], query, sizeof(query),	"%s,PaintJob='%i'",			query,	vehicleStat[vehstatid][vPaintJob]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod00='%i'",			query,	vehicleStat[vehstatid][vMod00]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod01='%i'",			query,	vehicleStat[vehstatid][vMod01]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod02='%i'",			query,	vehicleStat[vehstatid][vMod02]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod03='%i'",			query,	vehicleStat[vehstatid][vMod03]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod04='%i'",			query,	vehicleStat[vehstatid][vMod04]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod05='%i'",			query,	vehicleStat[vehstatid][vMod05]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod06='%i'",			query,	vehicleStat[vehstatid][vMod06]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod07='%i'",			query,	vehicleStat[vehstatid][vMod07]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod08='%i'",			query,	vehicleStat[vehstatid][vMod08]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod09='%i'",			query,	vehicleStat[vehstatid][vMod09]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod10='%i'",			query,	vehicleStat[vehstatid][vMod10]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod11='%i'",			query,	vehicleStat[vehstatid][vMod11]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod12='%i'",			query,	vehicleStat[vehstatid][vMod12]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod13='%i'",			query,	vehicleStat[vehstatid][vMod13]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod14='%i'",			query,	vehicleStat[vehstatid][vMod14]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod15='%i'",			query,	vehicleStat[vehstatid][vMod15]);
	mysql_format(database[0], query, sizeof(query),	"%s,Mod16='%i'",			query,	vehicleStat[vehstatid][vMod16]);
	mysql_format(database[0], query, sizeof(query),	"%s,HealthMax='%f'",		query,	vehicleStat[vehstatid][vHealthMax]);

	mysql_format(database[0], query, sizeof(query),	"%s,WorldID='%i'",			query,	vehicleStat[vehstatid][vWorldID]);
	mysql_format(database[0], query, sizeof(query),	"%s,Spawned='%i'",			query,	vehicleStat[vehstatid][vSpawned]);
	mysql_format(database[0], query, sizeof(query),	"%s,SpawnPositionX='%f'",	query,	vehicleStat[vehstatid][vSpawnPositionX]);
	mysql_format(database[0], query, sizeof(query),	"%s,SpawnPositionY='%f'",	query,	vehicleStat[vehstatid][vSpawnPositionY]);
	mysql_format(database[0], query, sizeof(query),	"%s,SpawnPositionZ='%f'",	query,	vehicleStat[vehstatid][vSpawnPositionZ]);
	mysql_format(database[0], query, sizeof(query),	"%s,SpawnPositionR='%f'",	query,	vehicleStat[vehstatid][vSpawnPositionR]);
	mysql_format(database[0], query, sizeof(query),	"%s,ParkPositionX='%f'",	query,	vehicleStat[vehstatid][vParkPositionX]);
	mysql_format(database[0], query, sizeof(query),	"%s,ParkPositionY='%f'",	query,	vehicleStat[vehstatid][vParkPositionY]);
	mysql_format(database[0], query, sizeof(query),	"%s,ParkPositionZ='%f'",	query,	vehicleStat[vehstatid][vParkPositionZ]);
	mysql_format(database[0], query, sizeof(query),	"%s,ParkPositionR='%f'",	query,	vehicleStat[vehstatid][vParkPositionR]);

	mysql_format(database[0], query, sizeof(query),	"%s,LastDriver='%e'",		query,	vehicleStat[vehstatid][vLastDriver]);
	mysql_format(database[0], query, sizeof(query),	"%s,Owner='%e'",			query,	vehicleStat[vehstatid][vOwner]);
	mysql_format(database[0], query, sizeof(query),	"%s,Kills='%i'",			query,	vehicleStat[vehstatid][vKills]);
	mysql_format(database[0], query, sizeof(query),	"%s,Death='%i'",			query,	vehicleStat[vehstatid][vDeath]);
	
	mysql_format(database[0], query, sizeof(query),	"%s,VehicleParams='%i'",	query,	vehicleStat[vehstatid][vParams]);
	mysql_format(database[0], query, sizeof(query),	"%s,Health='%f'",			query,	vehicleStat[vehstatid][vHealth]);
	mysql_format(database[0], query, sizeof(query),	"%s,DamageStatus0='%i'",	query,	vehicleStat[vehstatid][vDamageStatus0]);
	mysql_format(database[0], query, sizeof(query),	"%s,DamageStatus1='%i'",	query,	vehicleStat[vehstatid][vDamageStatus1]);
	mysql_format(database[0], query, sizeof(query),	"%s,DamageStatus2='%i'",	query,	vehicleStat[vehstatid][vDamageStatus2]);
	mysql_format(database[0], query, sizeof(query),	"%s,DamageStatus3='%i'",	query,	vehicleStat[vehstatid][vDamageStatus3]);

	mysql_format(database[0], query, sizeof(query),	"%s,TotalDistance='%f'",	query,	vehicleStat[vehstatid][vTotalDistance]);
	mysql_format(database[0], query, sizeof(query),	"%s,Fuel='%f'",				query,	vehicleStat[vehstatid][vFuel]);
	mysql_format(database[0], query, sizeof(query),	"%s,FuelTypeInTank='%i'",	query,	vehicleStat[vehstatid][vFuelTypeInTank]);
	mysql_format(database[0], query, sizeof(query), "%s WHERE Name='%e' LIMIT 1;",	query,	vehicleStat[vehstatid][vName]);

	//mysql_pquery(database[0], query, "", "");
	mysql_query(database[0], query, false);
	//printf("query -%s-", query);

	return 0;
}



//------------------------------------------------------------------------------
//									update fuction
//------------------------------------------------------------------------------

new Float: vehpos [3];
new Float: vehpos_old [MAX_VEHICLES][3];
new Float: vehdis [3]; // displacement

forward vehicleStat_update(vehstatid);
public vehicleStat_update(vehstatid)
{
	// if vehicles engine is on, update its stats
	if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)
	{
		// only vehicles that are spawned
		if(vehicleStat[vehstatid][vSpawned])
		{
			//printf("engine on for vehicle %s", vehicleStat[vehstatid][vName]);

			// velocity
			getVehicleVelocity(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vSpeedActual], vehicleStat[vehstatid][vSpeedGround], vehicleStat[vehstatid][vHeading],
							   vehicleStat[vehstatid][vAttitude], vehicleStat[vehstatid][vAltitude], 0, 0, 1);

			// damage
			new Float:vehicleHealth;
			GetVehicleHealth(vehicleStat[vehstatid][vVehicleID], vehicleHealth); // max health is 1000, vehicle will catch fire at 250 health
			if(vehicleHealth>250){
				vehicleStat[vehstatid][vDamage] = ( (vehicleHealth-250) * 100/750 ) * -1 + 100; // calculate so damage may be measured from 0 to 100
			}
			else vehicleStat[vehstatid][vDamage] = 100;

			// set the vehicle health from the vehicle damage
			//vehicleHealth = (vehicleStat[vehstatid][vDamage] * -1 +100) * 750/100 + 250;
			//SetVehicleHealth(vehicleStat[vehstatid][vVehicleID], vehicleHealth);

			// distance
			GetVehiclePos(vehicleStat[vehstatid][vVehicleID], vehpos[0], vehpos[1], vehpos[2]); // get current position
			vehdis [0] = vehpos [0] - vehpos_old [vehstatid][0]; // calculate the displacement for each axis
			vehdis [1] = vehpos [1] - vehpos_old [vehstatid][1];
			vehdis [2] = vehpos [2] - vehpos_old [vehstatid][2];

			vehpos_old[vehstatid][0] = vehpos[0]; // record the current position as the old position, ready for next time
			vehpos_old[vehstatid][1] = vehpos[1];
			vehpos_old[vehstatid][2] = vehpos[2];

			new Float: displacement = floatsqroot(vehdis[0]*vehdis[0] + vehdis[1]*vehdis[1] + vehdis[2]*vehdis[2]); // calculate the displacement in 3d

			vehicleStat[vehstatid][vTotalDistance] += (displacement * 4/5) / 1000; // add the displacement to the vehicles total distance travelled (convert from meters to km)

			// we don't do this here anymore, it can now be done by recording the vTotalDistance when the player gets in the vehicle, and subtracting
			// that from the vTotalDistance at a later date
			//PlayerInfo[playerid][pvDistanceThisVeh] += (displacement * 4/5) / 1000; // add the displacement to the vehicles distance travelled for this player

			// fuel (dependent on distance)
			// if the vehicle has moved more than 20 units
			if(displacement < 20){
				vehicleStat[vehstatid][vFuel] -= vehicleConsumption(vehicleStat[vehstatid][vVehicleID]) / 25000 * displacement;
			}
			else{ // if it has not
				vehicleStat[vehstatid][vFuel] -= vehicleConsumption(vehicleStat[vehstatid][vVehicleID]) / 25000 * 20; // engine idling
			}
		}
	}
}



