// TODO retire this code
stock LoadStaticVehiclesFromFileEx(const filename[], respawn)
{
	new line[256];
	new var_from_line[64];
	new index;

	//
	// read the faction colour file, so we know what colour to make faction vehicles
	//
	new File:file_factioncols;
	file_factioncols = fopen("vehicles/factioncols.txt",filemode:io_read);
	if(!file_factioncols) return 0;

	// find out how many lines there are, so we can init the vars to the right size
	//fread(file_factioncols,line,256);
	//new lineCount = 0;
	//index = token_by_delim(line,var_from_line,';',index);
	//new lineCount = strval(var_from_line);

	// the values we will read
	new faction_id[FACTION_COLS_LINES];
	new faction_mid[FACTION_COLS_LINES];
	new faction_sid[FACTION_COLS_LINES];
	new faction_col1[FACTION_COLS_LINES];
	new faction_col2[FACTION_COLS_LINES];

	// init all the faction colours to be -1, so if a value is not defined
	// in the faction colour file, then a random colour will be picked.
	for(new lineID=0; lineID<FACTION_COLS_LINES; lineID++){
		faction_col1[lineID] = -1;
		faction_col2[lineID] = -1;
	}

	// read the bulk of the data
	for(new i=0; fread(file_factioncols,line,256)>0; i++)
	{
		index = 0;

		// Read factionid
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		faction_id[i] = strval(var_from_line);

		// Read scemeid
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		faction_sid[i] = strval(var_from_line);

		// Read modelid
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		faction_mid[i] = strval(var_from_line);

		// Read colour1
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		faction_col1[i] = strval(var_from_line);

		// Read colour2
		index = token_by_delim(line,var_from_line,';',index+1);
		faction_col2[i] = strval(var_from_line);
	}
	// lineCount should give us the size of the arrays
	fclose(file_factioncols);

	// print the file (testing purposes)
	/*for(new i=0; i<FACTION_COLS_LINES; i++){
		printf("factionID:%d,  modelID:%d, schemeID:%d, colour1:%d, colour2:%d;", faction_id[i], faction_mid[i], faction_sid[i], faction_col1[i], faction_col2[i]);
	}*/


	//
	// Read the vehicles file that tells us where to spawn vehicles
	//
	new File:file_ptr;
	new vehicletype;
	new Float:SpawnX;
	new Float:SpawnY;
	new Float:SpawnZ;
	new Float:SpawnRot;
	new Color1;
	new Color2;
	new faction;
	new scheme;

	new vehicles_loaded;

	file_ptr = fopen(filename,filemode:io_read);
	if(!file_ptr) return 0;

	vehicles_loaded = 0;

	// loop until all lines in the file have been read
	while(fread(file_ptr,line,256) > 0)
	{
		index = 0;

		// Read type
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		vehicletype = strval(var_from_line);
 		if(vehicletype < 400 || vehicletype > 611) continue;

		// Read X, Y, Z, Rotation
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnX = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnY = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnZ = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnRot = floatstr(var_from_line);

		// Read Color1, Color2
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color1 = strval(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color2 = strval(var_from_line);

		// read the vehicles faction
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		faction = strval(var_from_line);

		// Read the read the faction colour scheme
		index = token_by_delim(line,var_from_line,';',index+1);
		scheme = strval(var_from_line);


		// We use the vehicles faction to determine what colour it should be
		// If faction == 0, we just use the colour already defined
		// This is just a way to make all faction vehs look the same without
		// individually defining the colour on each one
		if(faction>0){
			//printf("faction>0"); // for testing

			for(new i=0; i<FACTION_COLS_LINES; i++){

				if(faction==faction_id[i]){
					//printf("modelid %d,  faction_mid %d", vehicletype, faction_mid[i]); // for testing

					if(vehicletype==faction_mid[i]){
						//printf("vehicletype==faction_mid"); // for testing

						if(scheme==faction_sid[i]){
							//printf("scheme==faction_sid[line %d]", i); // for testing

							Color1 = faction_col1[i];
							Color2 = faction_col2[i];
							//printf("set colour for modelid %d",vehicletype); // for testing
							break;
						}
					}
				}
			}
			//printf("checked lines"); // for testing
		}


		// spawn the vehicle
		new vehicleid = AddStaticVehicleEx(vehicletype,SpawnX,SpawnY,SpawnZ,SpawnRot,Color1,Color2,respawn);
		//new vehstatid = vehicleStat_create(vehicleid);
		new vehstatid;

		// set the stats not set by vehicleStat_set
		//vehicleStat[vehstatid][vColorScheme] = scheme;
		vehicleStat[vehstatid][vFaction] = faction;
		//vehicleStat[vehstatid][vEngine] = false; // engine off by default

		// put some fuel in the tank
		//vehicleStat[vehstatid][vFuel] = float( randnumb(1, vehicleFuelTankCapacity(vehicletype)) ); // put a random amount of fuel, from 1L to capacity in the tank
		//vehicleStat[vehstatid][vFuelTypeInTank] = getDefaultFuelType(vehicletype);

		// set the number plate
		SetVehicleNumberPlate(vehicleid, vehicleStat[vehstatid][vName]);
		SetVehicleToRespawn(vehicleid);
		ChangeVehicleColor(vehicleid, vehicleStat[vehstatid][vColor1], vehicleStat[vehstatid][vColor2]);

		// respawn to show changes
		SetVehicleToRespawn(vehicleid);

		//printf("model %d,  X %.2f,  Y %.2f,  Z %.2f,  R%.4f,  C1 %d,  C2 %d,  faction %d,  scheme %d;",vehicletype,SpawnX,SpawnY,SpawnZ,SpawnRot,Color1,Color2,faction,scheme);

		//printf("plate = %s", vehicleStat[vehstatid][vName]);

		// don't set the numplate
		/*new numplate_test[32+1];
		format(numplate_test,32,"GRLC{44AA33}%d",vid);
		SetVehicleNumberPlate(vid, numplate_test);*/

		vehicles_loaded++;
		//vehicleStat_logdump(vehstatid);
	}

	fclose(file_ptr);
	printf("Loaded %d vehicles from: %s",vehicles_loaded,filename);
	return vehicles_loaded;
}








stock Load(const filename[], series[], outputfile[])
{
	new line[256];
	new var_from_line[64];
	new index;


	//
	// Read the vehicles file that tells us where to spawn vehicles
	//
	new File:file_ptr;
	new File:output;
	new vehiclename[10]; // the numberplate
	new vehicletype;
	new Float:SpawnX;
	new Float:SpawnY;
	new Float:SpawnZ;
	new Float:SpawnRot;
	new Color1;
	new Color2;
	new faction;
	new scheme;

	new vehicles_loaded = 0;

	file_ptr = fopen(filename,filemode:io_read);
	if(!file_ptr) return 0;

	output = fopen(outputfile, io_write); // Open the file
	if(!output) return 0;

	// loop until all lines in the file have been read
	while(fread(file_ptr,line,256) > 0)
	{
		index = 0;

		// Read type
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		vehicletype = strval(var_from_line);
 		if(vehicletype < 400 || vehicletype > 611) continue;

		// Read X, Y, Z, Rotation
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnX = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnY = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnZ = floatstr(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		SpawnRot = floatstr(var_from_line);

		// Read Color1, Color2
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color1 = strval(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color2 = strval(var_from_line);

		// read the vehicles faction
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		faction = strval(var_from_line);

		// Read the read the faction colour scheme
		index = token_by_delim(line,var_from_line,';',index+1);
		scheme = strval(var_from_line);




		// now write to the output
		new output_line[256];
		format(output_line, sizeof(output_line), ""); // set to nothing

		// vehiclename[10]; // the numberplate
		if(vehicles_loaded < 10) format(vehiclename, sizeof(vehiclename), "%s_00%d", series, vehicles_loaded);
		else if(vehicles_loaded < 100) format(vehiclename, sizeof(vehiclename), "%s_0%d", series, vehicles_loaded);
		else format(vehiclename, sizeof(vehiclename), "%s_%d", series, vehicles_loaded);
		format(output_line, sizeof(output_line), "%s", vehiclename);

		// vehicletype
		format(output_line, sizeof(output_line), "%s, %d", output_line, vehicletype); // start with the vehiclemodelid

		// spawnX
		if(SpawnX >= 1000) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnX >= 100) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnX >= 10) format(output_line, sizeof(output_line), "%s,	", output_line);
		else if(SpawnX >= 0) format(output_line, sizeof(output_line), "%s,	 ", output_line);
		else if(SpawnX <= -1000) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(SpawnX <= -100) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnX <= -10) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnX <   0) format(output_line, sizeof(output_line), "%s,	", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%.4f", output_line, SpawnX);

		// spawnY
		if(SpawnY >= 1000) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnY >= 100) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnY >= 10) format(output_line, sizeof(output_line), "%s,	", output_line);
		else if(SpawnY >= 0) format(output_line, sizeof(output_line), "%s,	 ", output_line);
		else if(SpawnY <= -1000) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(SpawnY <= -100) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnY <= -10) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnY <   0) format(output_line, sizeof(output_line), "%s,	", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%.4f", output_line, SpawnY);

		// spawnZ
		if(SpawnZ >= 1000) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnZ >= 100) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnZ >= 10) format(output_line, sizeof(output_line), "%s,	", output_line);
		else if(SpawnZ >= 0) format(output_line, sizeof(output_line), "%s,	 ", output_line);
		else if(SpawnZ <= -1000) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(SpawnZ <= -100) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnZ <= -10) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(SpawnZ <   0) format(output_line, sizeof(output_line), "%s,	", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%.4f", output_line, SpawnZ);

		// SpawnRot
		if(SpawnRot >= 100) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(SpawnRot >= 10) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(SpawnRot >= 0) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%.4f", output_line, SpawnRot);

		// Color1
		if(Color1 >= 100) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(Color1 >= 10) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(Color1 >= 0) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(Color1 <   0) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%d", output_line, Color1);

		// Color2
		if(Color2 >= 100) format(output_line, sizeof(output_line), "%s, ", output_line);
		else if(Color2 >= 10) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else if(Color2 >= 0) format(output_line, sizeof(output_line), "%s,   ", output_line);
		else if(Color2 <   0) format(output_line, sizeof(output_line), "%s,  ", output_line);
		else format(output_line, sizeof(output_line), "%s, ", output_line);
		format(output_line, sizeof(output_line), "%s%d", output_line, Color2);

		// faction and color scheme
		format(output_line, sizeof(output_line), "%s, %d", output_line, faction);
		format(output_line, sizeof(output_line), "%s, %d;\r\n", output_line, scheme);

		fwrite(output, output_line); // Write to the file (\r\n is for a new line)


		vehicles_loaded++;
		//vehicleStat_logdump(vehstatid);
	}

	fclose(file_ptr);
	fclose(output); // Close the file
	printf("Loaded %d vehicles from: %s",vehicles_loaded,filename);

	new string[256];
	format(string, sizeof(string), "Loaded %d vehicles from: %s", vehicles_loaded,filename);
	log_vehicle(string);

	return vehicles_loaded;
}



// NOT USED!!!
// create a stats index for a vehicleID that doesn't have one, returns a statsID
// we don't use this anymore, TODO adapt it for the "create new vehicle" function.
stock vehicleStat_create_old(vehicleid)
{
	//if(!validVehicle(vehicleid)) return 0; // fail

	new vehstatid = vehicleStat_newid();
	//vehicleLink[vehicleid][vStatsID] = vehstatid;

	// generate a random numberplate
	new numplate[10] = "";
	for(new e=0; e<10; e++){ // loop until we get a unique numberplate that isn't already used
		new const base = 36; // working in base 36									 2147483647 max int value
		new const maxValue = base*base*base - 1;
		new const minValue = base*base;
		// this gives an output range from "100" to "ZZZ"

		// get two strings of three alpha-decimals
		new randomNumber = random(maxValue-minValue) + minValue;
		new string1[10]; string1 = generateNumberPlate(randomNumber, 3); // 3 is the number of characters to be returned
		randomNumber = random(maxValue-minValue) + minValue;
		new string2[10]; string2 = generateNumberPlate(randomNumber, 3); // 3 is the number of characters to be returned

		format(numplate, sizeof(numplate), "%s_%s", string1, string2); // string the two strings together

		new bool: numplateIsUnique = true; // the numberplate isn't the same as any other

		// check that that numberplate isn't already used
		for(new i=0; i<MAX_VEHICLES; i++){
			// if the numplate matches another numplate
			if( !strcmp(vehicleStat[i][vName], numplate, true) && !isnull(vehicleStat[i][vName]) ){
				numplateIsUnique = false; // it isn't unique any more
				//printf("vstats-%s-  numplate-%s-", vehicleStat[i][vName], numplate);
				break;
			}
		}

		// TODO add a file check for numplates that exist but aren't spawned yet

		if(numplateIsUnique) break; // if we have a unique numberplate, we can continue out of the loop
	}

	// static variables - they don't change too often
	vehicleStat[vehstatid][vVehicleID]			= vehicleid;
	vehicleStat[vehstatid][vName]				= numplate;
	vehicleStat[vehstatid][vModel]				= GetVehicleModel(vehicleid);
	//vehicleStat[vehstatid][vColorScheme]		= 0;
	GetVehicleColor(vehicleid, vehicleStat[vehstatid][vColor1], vehicleStat[vehstatid][vColor2]); // set color
	vehicleStat[vehstatid][vSpawned]			= 1; // we are setting up and existing vehid, so this will be true

	// the vehicle has just been spawned, so we can find its spawn and park position
	GetVehiclePos(vehicleid, vehicleStat[vehstatid][vSpawnPositionX], vehicleStat[vehstatid][vSpawnPositionY], vehicleStat[vehstatid][vSpawnPositionZ]);
	GetVehicleZAngle(vehicleid, vehicleStat[vehstatid][vSpawnPositionR]);
	vehicleStat[vehstatid][vParkPositionX]	= vehicleStat[vehstatid][vSpawnPositionX];
	vehicleStat[vehstatid][vParkPositionY]	= vehicleStat[vehstatid][vSpawnPositionY];
	vehicleStat[vehstatid][vParkPositionZ]	= vehicleStat[vehstatid][vSpawnPositionZ];
	vehicleStat[vehstatid][vParkPositionR]	= vehicleStat[vehstatid][vSpawnPositionR];

	//vehicleStat[vehstatid][vLastDriverID]		= INVALID_PLAYER_ID;
	vehicleStat[vehstatid][vFaction]			= 0;
	//vehicleStat[vehstatid][vFamily]			= 0;
	new owner[MAX_PLAYER_NAME+1] = " ";
	vehicleStat[vehstatid][vOwner]			= owner;
	//vehicleStat[vehstatid][vOwnerID]			= INVALID_PLAYER_ID;

	vehicleStat[vehstatid][vKills]			= 0;
	vehicleStat[vehstatid][vDeath]			= 0;

	// the realtime variables
	// get the health
	GetVehicleHealth(vehicleid, vehicleStat[vehstatid][vHealth]);
	GetVehicleDamageStatus(vehicleid, vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
						   vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);

	// vehicle params
	new objective;
	GetVehicleParamsEx(vehicleid, vehicleStat[vehstatid][vEngine], vehicleStat[vehstatid][vLights], vehicleStat[vehstatid][vAlarm],
					   vehicleStat[vehstatid][vLock], vehicleStat[vehstatid][vBonnet], vehicleStat[vehstatid][vBoot], objective);
	#pragma unused objective

	vehicleStat[vehstatid][vTotalDistance]	= 0;
	vehicleStat[vehstatid][vFuel]			= random( vehicleFuelTankCapacity(GetVehicleModel(vehicleid)) );
	vehicleStat[vehstatid][vFuelTypeInTank]	= getDefaultFuelType(vehicleStat[vehstatid][vModel]);

	//vehicleStat_logdump(vehstatid);

	return vehstatid; // success
}



// TODO retire this code
// check that this name don't already exist in stats, not case sensitive
stock vehicleStat_check(name[])
{
	for(new i=0; i<MAX_VEHICLES; i++){
		// if the numplate matches another numplate
		if( !strcmp(vehicleStat[i][vName], name, false) && !isnull(vehicleStat[i][vName]) ){
			printf("Vehicle %s already exists! %s", name, vehicleStat[i][vName]);
			return 0;
		}
	}

	return 1;
}



// NOT USED!!!
// return true if the vehicle name is already used (in memory, not in file).  Not case sensitive.
stock vehicleStat_exist_old(name[])
{
	for(new i=0; i<MAX_VEHICLES; i++){
		// if the numplate matches another numplate, not case sensitive
		if( !strcmp(vehicleStat[i][vName], name, false) && !isnull(vehicleStat[i][vName]) ){
			return 1;
		}
	}
	return 0;
}



// mess TODO of bad code
// none of this code ever runs

stock vehicleStat_loadSQL(vehstatid, name[])
{
	//printf("good1 %i", vehstatid);
	
	if(!valid_index(vehstatid, MAX_VEHICLES))			return 1;	// index out of range
	
	//print("good2");
    if(valid_modelid(vehicleStat[vehstatid][vModel]))	return 2;	// id already in use
	
	//printf("load sql");
	
	// select the vehicle from database
	new query[41 + MAX_VEHICLE_NAME];
	mysql_format(database[0], query, sizeof(query), "SELECT * FROM vehicledata WHERE Name='%e'", name);
	mysql_pquery(database[0], query, "OnVehicleDataLoad", "i", vehstatid);
	
	return 0;
}

forward OnVehicleDataLoad(vehstatid);
public OnVehicleDataLoad(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return -1;	// index out of range
	
	//new query[256];
	//new wdata[256], Locked;
	//format(query, sizeof(query), "SELECT * FROM playerdata WHERE UserName='%s'", PlayerInfo[playerid][pName]);
	//new Cache:result = mysql_query(database[0], query);
	//cache_set_active(result, database[0]);

	if(cache_get_row_count(database[0]) < 1) return 1; // vehicle not registered
	if(cache_get_row_count(database[0]) > 1) return 2; // very big fuck up

	new const row = 0;
	//cache_get_field_content(row, const field_name[], destination[], connectionHandle = 1, max_len = sizeof(destination))

	cache_get_field_content			(row,	"Name",				vehicleStat[vehstatid][vName],				database[0],	MAX_VEHICLE_NAME);
	cache_get_field_content			(row,	"Plate",			vehicleStat[vehstatid][vPlate],				database[0],	MAX_VEHICLE_PLATE);
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
	vehicleStat[vehstatid][vHealthMax]			=	cache_get_field_content_float	(row,	"HealthMax",	database[0]);

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
	
	cache_get_field_content			(row,	"LastPlayer",		vehicleStat[vehstatid][vLastDriver],		database[0],	MAX_PLAYER_NAME+1);
	cache_get_field_content			(row,	"Faction",			vehicleStat[vehstatid][vFaction],			database[0],	MAX_FACTION_NAME);
	cache_get_field_content			(row,	"Owner",			vehicleStat[vehstatid][vOwner],				database[0],	MAX_PLAYER_NAME+1);
	vehicleStat[vehstatid][vKills]			=	cache_get_field_content_int		(row,	"Kills",			database[0]);
	vehicleStat[vehstatid][vDeath]			=	cache_get_field_content_int		(row,	"Death",			database[0]);
	
	vehicleStat[vehstatid][vHealth]			=	cache_get_field_content_float	(row,	"Health",			database[0]);
	vehicleStat[vehstatid][vDamageStatus0]	=	cache_get_field_content_int		(row,	"DamageStatus0",	database[0]);
	vehicleStat[vehstatid][vDamageStatus1]	=	cache_get_field_content_int		(row,	"DamageStatus1",	database[0]);
	vehicleStat[vehstatid][vDamageStatus2]	=	cache_get_field_content_int		(row,	"DamageStatus2",	database[0]);
	vehicleStat[vehstatid][vDamageStatus3]	=	cache_get_field_content_int		(row,	"DamageStatus3",	database[0]);
	vehicleStat[vehstatid][vParams]			=	cache_get_field_content_int		(row,	"VehicleParams",	database[0]);

	vehicleStat[vehstatid][vTotalDistance]	=	cache_get_field_content_float	(row,	"TotalDistance",	database[0]);
	vehicleStat[vehstatid][vFuel]			=	cache_get_field_content_float	(row,	"Fuel",				database[0]);
	vehicleStat[vehstatid][vFuelTypeInTank]	=	cache_get_field_content_int		(row,	"FuelTypeInTank",	database[0]);

	//printf("name %s, armour %f", vehicleStat[vehstatid][vName], vehicleStat[vehstatid][vArmour]);
	vehicle_spawn(vehstatid);
	
	return 1; // delete cache
}


// TODO save not working
stock vehicleStat_saveSQL(vehstatid)
{
	if(!valid_index(vehstatid, MAX_VEHICLES))			return 1;	// index out of range
    if(!valid_modelid(vehicleStat[vehstatid][vModel]))	return 1;	// id not in use
	new vehicleid = vehicleStat[vehstatid][vVehicleID];

	// update some vars
	if(vehicleid != INVALID_VEHICLE_ID){
		// get the position and set as park position
		GetVehiclePos(vehicleid, vehicleStat[vehstatid][vParkPositionX], vehicleStat[vehstatid][vParkPositionY], vehicleStat[vehstatid][vParkPositionZ]);
		GetVehicleZAngle(vehicleid, vehicleStat[vehstatid][vParkPositionR]);

		//vehicle_virtualworld_get(vehicleid);
		vehicle_health_get(vehicleid);
		vehicle_damagestatus_get(vehicleid);
		//vehicle_params_get(vehicleid);
	}
	
	
	
	//printf("saveing");

	//new query[] = ;//"UPDATE vehicledata SET Resume='%d',Class='%d',Team='%d' WHERE UserName='%s'
	//mysql_format(database[0], query, sizeof(query), "UPDATE vehicledata SET Plate='%e' WHERE Name='%e'", vehicleStat[vehstatid][vPlate], vehicleStat[vehstatid][vName]);
	
	
	// check if vehicle exists
	/*new query[45 + MAX_VEHICLE_NAME];
	mysql_format(database[0], query, sizeof(query), "SELECT Name FROM vehicledata WHERE Name='%e'", vehicleStat[vehstatid][vName]);
	mysql_pquery(database[0], query, "vehicledata_save", "i", vehstatid);
	*/
	vehicledata_save_sql(vehstatid);

	return 1;
}



CMD:create(playerid, params[])
{
	//new string[256];
	//format(string, sizeof(string), "");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new vehstatid = vehicle_create(params, 411, x, y, z, 0.0, 1, 2, "", 0);

	if(vehstatid == INVALID_VEHSTAT_ID) print("failed");
	else print("success");

	return 1;
}

CMD:del(playerid, params[])
{
	//new string[256];
	//format(string, sizeof(string), "");

	new vehstatid = vehicle_delete(params);

	if(vehstatid == 0) print("failed");
	else print("success");

	return 1;
}

CMD:idcheck(playerid, params)
{
	new str[256];
	new vehicleid = GetPlayerVehicleID(playerid);

	format(str, sizeof(str), "player vehicle id = -%d-", vehicleid);
	SendClientMessageToAll(0xFF0000FF, str);
	format(str, sizeof(str), "vehicle vehstatid = -%d-", vehicleLink[vehicleid]);
	SendClientMessageToAll(0xFF0000FF, str);
	format(str, sizeof(str), "vehstat vehicleid = -%d-", vehicleStat[vehicleLink[vehicleid]][vVehicleID]);
	SendClientMessageToAll(0xFF0000FF, str);
	// 229
	return 1;
}



CMD:respawn(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return 1;
	
	vehicle_respawn(vehicleid);
	return 1;
}

CMD:vname(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return 1;
	new vehstatid = vehicleLink[vehicleid];
	new str[256];
	format(str, sizeof(str), "-%s-", vehicleStat[vehstatid][vName]);
	SendClientMessage(playerid, 0xFF0000FF, str);
	printf("vehicle name %s", str);
	return 1;
}

CMD:getd(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return 1;
	new vehstatid = vehicleLink[vehicleid];

	GetVehicleDamageStatus(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
						   vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);

	SendClientMessage(playerid, 0xFF0000FF, "done!");
	return 1;
}

CMD:setd(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return 1;
	new vehstatid = vehicleLink[vehicleid];

	UpdateVehicleDamageStatus(vehicleStat[vehstatid][vVehicleID], vehicleStat[vehstatid][vDamageStatus0], vehicleStat[vehstatid][vDamageStatus1],
							  vehicleStat[vehstatid][vDamageStatus2], vehicleStat[vehstatid][vDamageStatus3]);
							  
	SendClientMessage(playerid, 0xFF0000FF, "done!");
	return 1;
}

CMD:carcolorglobal(playerid, params[])
{
	new idx;
	new tmp[256];

	new modelid;
	new color1;
	new color2;

	// deal with the first param (modelid)
	tmp = strtok(params, idx);
	if(!strlen(tmp)){ // if no params has been passed

		SendClientMessage(playerid, COLOR_ERROR, "Usage: /allcarcolor {modelid} {integer} {integer}");
		return 1; // return failure

	}
	else{ // if params have been passed

		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /allcarcolor {modelid} {integer} {integer}");
			return 1; // return failure
		}

		modelid = strval(tmp); // convert the passed params to a value

		if(color1<0 || color1>255){ // if the value is out of bounds
			SendClientMessage(playerid, COLOR_ERROR, "{modelid} must be a value from '400' to '612'!");
			return 1; // return failure
		}

	}

	// deal with the second param (color1)
	tmp = strtok(params, idx);
	if(!strlen(tmp)){ // if no params has been passed

		color1 = random(256); // pick a random color

	}
	else{ // if params have been passed

		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /allcarcolor {modelid} {integer} {integer}");
			return 1; // return failure
		}

		color1 = strval(tmp); // convert the passed params to a value

		if(color1<0 || color1>255){ // if the value is out of bounds
			SendClientMessage(playerid, COLOR_ERROR, "{integer} must be a value from '0' to '255'!");
			return 1; // return failure
		}

	}

	// deal with the third param (color2)
	tmp = strtok(params, idx);
	if(!strlen(tmp)){ // if no params has been passed

		color2 = random(256); // pick a random color

	}
	else{ // if params have been passed

		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /carcolor {integer}");
			return 1; // return failure
		}

		color2 = strval(tmp); // convert the passed params to a value

		if(color1<0 || color1>255){ // if the value is out of bounds
			SendClientMessage(playerid, COLOR_ERROR, "{integer} must be a value from '0' to '255'!");
			return 1; // return failure
		}

	}

	// change color for all of this model
	for(new vehicleid=0; vehicleid<MAX_VEHICLES; vehicleid++){
		if(vehicleid != INVALID_VEHICLE_ID){
			if(GetVehicleModel(vehicleid) == modelid){
				ChangeVehicleColor(vehicleid, color1, color2);
			}
		}
	}

	return 1; // return success
}

CMD:carcolor(playerid, params[])
{
	new idx;
	new tmp[256];

	new vehicleid = GetPlayerVehicleID(playerid);
	new color1 = random(256);
	new color2 = random(256);

	// if not in a vehicle
	if(!IsPlayerInAnyVehicle(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must be in a vehicle to change its colour!");
		return 1; // return failure
	}

	// deal with the first param
	tmp = strtok(params, idx);
	if(!strlen(tmp)){ // if no params has been passed

		color1 = random(256); // pick a random color

	}
	else{ // if params have been passed

		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /carcolor {integer}");
			return 1; // return failure
		}

		color1 = strval(tmp); // convert the passed params to a value

		if(color1<0 || color1>255){ // if the value is out of bounds
			SendClientMessage(playerid, COLOR_ERROR, "{integer} must be a value from '0' to '255'!");
			return 1; // return failure
		}

	}

	// deal with the second param
	tmp = strtok(params, idx);
	if(!strlen(tmp)){ // if no params has been passed

		color2 = random(256); // pick a random color

	}
	else{ // if params have been passed

		if(!isNumeric(tmp)){ // if the passed params are invalid, (not a number)
			SendClientMessage(playerid, COLOR_ERROR, "Usage: /carcolor {integer}");
			return 1; // return failure
		}

		color2 = strval(tmp); // convert the passed params to a value

		if(color1<0 || color1>255){ // if the value is out of bounds
			SendClientMessage(playerid, COLOR_ERROR, "{integer} must be a value from '0' to '255'!");
			return 1; // return failure
		}

	}

	ChangeVehicleColor(vehicleid, color1, color2);
	return 1; // return success
}

CMD:setfuel(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return 1;
	
	vehicleStat[vehicleLink[vehicleid]][vFuel] = strval(params);
	SendClientMessage(playerid, 0xFF0000FF, "Fuel tank level set!");
	return 1;
}

CMD:vparam(playerid, params[])
{
	#pragma unused params

	new vehid = GetPlayerVehicleID(playerid);

	if(isnull(params)){
		vehicleParamsToggle(vehid, 1, 0, 0, 0, 0, 0, 0);
		printf("engine state %d", vehicleStateEngine(vehid));
		return 1;
	}

	new val = strval(params);

	vehicleParamsSet(vehid, val, -1, -1, -1, -1, -1, -1);

	printf("engine state %d", vehicleStateEngine(vehid));

	return 1;
}


// import vehicles from csv
CMD:vimport(playerid, params[])
{
	new file[256];
	format(file, sizeof(file), "%sfirstspawn/%s", VEHICLE_STATS_LOCATION, params);

	new string[256];
	format(string, sizeof(string), "Importing vehicles from csv \"%s\"", file);
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);

	// quit if the file doesn't exist
	if(!fexist(file)){
		format(string, sizeof(string), "BAD: CSV \"%s\" does not exist!", params);
		SendClientMessage(playerid, COLOR_ERROR, string);
		return 1;
	}

	new loaded = loadVehiclesFromCSV(file, false);

	format(string, sizeof(string), "%d vehicles loaded!", loaded);
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);

	return 1;
}

CMD:vimportdefault(playerid, params[])
{
	#pragma unused playerid
	#pragma unused params

	// other cars
	loadVehiclesFromCSV("vehicles/firstspawn/bone.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/flint.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/lv_airport.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/lv_gen.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/lv_law.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/red_county.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/sf_airport.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/sf_gen.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/sf_law.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/tierra.txt", false);
	loadVehiclesFromCSV("vehicles/firstspawn/whetstone.txt", false);

	// Los Santos Vehicles
	loadVehiclesFromCSV("vehicles/firstspawn/ls_vehicles.txt", false);

	// Taxi vehicles
	loadVehiclesFromCSV("vehicles/firstspawn/ls_taxis.txt", false);

	return 1;
}

CMD:obj(playerid, params[])
{
	#pragma unused params

	new vehid = GetPlayerVehicleID(playerid);
	SetVehicleParamsForPlayer(vehid, playerid, 1, 0);
	return 1;
}

CMD:vsave(playerid, params[])
{
	#pragma unused params

	new vehicleid = GetPlayerVehicleID(playerid);

	if(!IsPlayerInAnyVehicle(playerid) || !valid_vehicleid(vehicleid)){
		SendClientMessage(playerid, COLOR_ERROR, "Error: You must be in a vehicle to do this!");
		return 1;
	}

	new vehstatid = vehicleLink[vehicleid];

	if(vehstatid==INVALID_VEHSTAT_ID){
		//vehstatid = vehicleStat_create(vehicleid);
	}

	// if the vehicle does not have a stats index, make it one
	if(vehstatid==INVALID_VEHSTAT_ID){
		//vehstatid = vehicleStat_newid();
		//vehicleStat_create(vehicleid);
	}

	vehicledata_save(vehstatid);

	new string[256];
	format(string, sizeof(string), "Vehicle %s (vehstatid %d)'s stats have been saved!", vehicleStat[vehstatid][vName], vehstatid);
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);

	return 1;
}

CMD:vdisconnect(playerid, params[])
{
	#pragma unused playerid

	new vehstatid = strval(params);
	if(!vehicle_disconnect(vehstatid)){
		SendClientMessage(playerid, COLOR_ERROR, "Error: Vehicle does not exist!");
	}
	else{
		new string[256];
		format(string, sizeof(string), "Vehicle vehstatid %d has been disconnected!", vehstatid);
		SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);
	}

	return 1;
}

CMD:vsetplate(playerid, params[])
{
	// if not in a vehicle
	if(!IsPlayerInAnyVehicle(playerid)){
		SendClientMessage(playerid, COLOR_ERROR, "You must be in a vehicle to set its number plate!");
		return 1; // return failure
	}

	if(strlen(params) > 9)
	{
		SendClientMessage(playerid, COLOR_ERROR, "Your number plate may not be longer that 9 characters!");
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	new vehstatid = vehicleLink[vehicleid];
	new numplate[10];

	// deal with the params
	if(sscanf(params, "s[10]", numplate) ){ // if this fails (ie no params)
		new const base = 36; // working in base 36									 2147483647 max int value
		new const maxValue = base*base*base - 1;
		new const minValue = base*base;
		// this gives a a range from "100000" to "ZZZZZZ"

		new randomNumber = random(maxValue-minValue) + minValue;
		new string1[10]; string1 = generateNumberPlate(randomNumber, 3); // 3 is the number of characters to be returned
		randomNumber = random(maxValue-minValue) + minValue;
		new string2[10]; string2 = generateNumberPlate(randomNumber, 3); // 3 is the number of characters to be returned

		format(numplate, sizeof(numplate), "%s_%s", string1, string2); // set numplate
	}
	// if sscanf success (params have been passed)
	else{
		// code for restricting the available characters goes here!
		for(new i=0; params[i]; i++){
			if(!validFileNameCharacter(params[i])){ // if not a valid character
				new string[128];
				format(string, sizeof(string), "You have used an invalid character '%c'!", params[i]);
				SendClientMessage(playerid, COLOR_ERROR, string);

				return 1;
			}
		}

		//params[9] = '\0'; // this is crashing us

		format(numplate, sizeof(numplate), params);
	}

	// if the vehicle does not have a stats index, make it one
	if(vehstatid==INVALID_VEHSTAT_ID){
		//vehstatid = vehicleStat_newid();
		//vehicleStat_create(vehicleid);
	}

	// set the number plate, and update the stats
	vehicleStat[vehstatid][vName] = numplate;
	setVehicleNumberPlateEx(vehicleid, vehicleStat[vehstatid][vName]);

	new string[128];
	format(string, sizeof(string), "Vehicle number plate set to \"%s\"!", numplate);
	SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, string);


	// respawn to show the new plate
	// get the position
	new Float: xPos;
	new Float: yPos;
	new Float: zPos;
	new Float: zAngle;
	GetVehiclePos(vehicleid, xPos, yPos, zPos);
	GetVehicleZAngle(vehicleid, zAngle);

	// get the damage state
	new Float: health;
	new panels;
	new doors;
	new lights;
	new tires;
	GetVehicleHealth(vehicleid, health);
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	// respawn it
	SetVehicleToRespawn(vehicleid); // this will remove the player from the vehicle

	// put the vehicle back where it was, set the health, and put the player back in it
	PutPlayerInVehicle(playerid, vehicleid, 0);
	SetVehiclePos(vehicleid, xPos, yPos, zPos);
	SetVehicleZAngle(vehicleid, zAngle);
	SetVehicleHealth(vehicleid, health);
	UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	return 1; // return success
}

// TODO get this a better name and a better location in the script
stock INI_readString(filename[], const name[], value[], destsize = sizeof(value))
{
	new File: INI_File;
	INI_File = fopen(filename, io_read); // Open the file
	if(!INI_File) return 1; // fail if it doesn't exist

	new line[MAX_PLAYER_VEHICLES * MAX_VEHICLE_NAME + 16];
	while(fread(INI_File, line, sizeof(line)) > 0){
		new idx = 0; // for line

		while(line[idx]){
			if(line[idx] == ' ' || line[idx] == '='){
				line[idx] = '\0';
				break;
			}
			idx++;
		}

		// if we find the field we are looking for
		if(strmatch(name, line)){
			line[idx] = '=';
			while(line[idx] == '='  ||  line[idx] == ' ') idx++;

			// copy the value to value variable
			new i = 0; // for value
			while(line[idx]  &&  i < destsize  &&  line[idx] != '\r'  &&  line[idx] != '\n'){
				value[i] = line[idx];
				i++, idx++; // go to next index
			}
			value[i] = '\0';

			// return success
			fclose(INI_File);
			return 0;
		}
	}

	fclose(INI_File);
	return 1;
}

// return true if fail
stock string_RemoveVehicle(vehicles[], const vehiclename[])
{
	new currentVehicle[MAX_VEHICLE_NAME] = "";
	new idx = 0;
	while(vehicles[idx]){

		// get the next vehicle name 
		new i = 0;
		while(vehicles[idx]){
			if(vehicles[idx] == 1){
				idx++;
				break;
			}
			currentVehicle[i] = vehicles[idx];
			if(i + 1 < MAX_VEHICLE_NAME) i++;
			idx++;
		}
		currentVehicle[i] = '\0'; // null the end

		// if vehicle in that player has that vehicle in their list of vehicles (in their stats)
		if(strmatch(vehiclename, currentVehicle)){
			strdel(vehicles, idx-1 - strlen(currentVehicle), idx);
			return 0;
		}
	}
	return 1;
}

// return true if fail
stock PlayerStat_RemoveVehicle(playerid, const vehiclename[])
{
	new currentVehicle[MAX_VEHICLE_NAME] = "";
	new idx = 0;
	while(PlayerStat[playerid][pVehicles][idx]){

		// get the next vehicle name 
		new i = 0;
		while(PlayerStat[playerid][pVehicles][idx]){
			if(PlayerStat[playerid][pVehicles][idx] == 1){
				idx++;
				break;
			}
			currentVehicle[i] = PlayerStat[playerid][pVehicles][idx];
			if(i + 1 < MAX_VEHICLE_NAME) i++;
			idx++;
		}
		currentVehicle[i] = '\0'; // null the end

		// if vehicle in that player has that vehicle in their list of vehicles (in their stats)
		if(strmatch(vehiclename, currentVehicle)){
			strdel(PlayerStat[playerid][pVehicles], idx-1 - strlen(currentVehicle), idx);
			return 0;
		}
	}
	return 1;
}

// TODO find a better place in the script for these functions
// true if vehicle in player stats
stock PlayerStat_PlayerOwnsVehicle(playerid, const vehiclename[])
{
	new currentVehicle[MAX_VEHICLE_NAME] = "";
	new idx = 0;
	while(PlayerStat[playerid][pVehicles][idx]){

		// get the next vehicle name 
		new i = 0;
		while(PlayerStat[playerid][pVehicles][idx]){
			if(PlayerStat[playerid][pVehicles][idx] == 1){
				idx++;
				break;
			}
			currentVehicle[i] = PlayerStat[playerid][pVehicles][idx];
			if(i + 1 < MAX_VEHICLE_NAME) i++;
			idx++;
		}
		currentVehicle[i] = '\0'; // null the end

		// if vehicle in that player has that vehicle in their list of vehicles (in their stats)
		if(strmatch(vehiclename, currentVehicle)){
			return 1;
		}
	}
	return 0;
}

// true if vehicle in player stats
stock string_PlayerOwnsVehicle(vehicles[], const vehiclename[])
{
	new currentVehicle[MAX_VEHICLE_NAME] = "";
	new idx = 0;
	while(vehicles[idx]){

		// get the next vehicle name 
		new i = 0;
		while(vehicles[idx]){
			if(vehicles[idx] == 1){
				idx++;
				break;
			}
			currentVehicle[i] = vehicles[idx];
			if(i + 1 < MAX_VEHICLE_NAME) i++;
			idx++;
		}
		currentVehicle[i] = '\0'; // null the end

		// if vehicle in that player has that vehicle in their list of vehicles (in their stats)
		if(strmatch(vehiclename, currentVehicle)){
			return 1;
		}
	}
	return 0;
}

stock PlayerStat_GetVehicleInSlot(playerid, slotid)
{
	new vehicleName[MAX_VEHICLE_NAME] = "";

	// skip until we get to the slot we want to read
	new idx = 0;
	while(slotid > 0){
		while(PlayerStat[playerid][pVehicles][idx]){
			if(PlayerStat[playerid][pVehicles][idx] == 1){
				slotid--;
				idx++;
				break;
			}
			idx++;
		}
		if(!PlayerStat[playerid][pVehicles][idx]) break;
	}

	// copy the vehicle name from the slot
	new i=0;
	while(PlayerStat[playerid][pVehicles][idx]){
		if(PlayerStat[playerid][pVehicles][idx] == 1) break;
		vehicleName[i] = PlayerStat[playerid][pVehicles][idx];
		if(i + 1 < MAX_VEHICLE_NAME) i++; // only increment if the string is big enough
		idx++;
	}
	vehicleName[i] = '\0'; // null the end

	return vehicleName;
}

stock string_GetVehicleInSlot(vehicles[], slotid)
{
	new vehicleName[MAX_VEHICLE_NAME] = "";

	// skip until we get to the slot we want to read
	new idx = 0;
	while(slotid > 0){
		while(vehicles[idx]){
			if(vehicles[idx] == 1){
				slotid--;
				idx++;
				break;
			}
			idx++;
		}
		if(!vehicles[idx]) break;
	}

	// copy the vehicle name from the slot
	new i=0;
	while(vehicles[idx]){
		if(vehicles[idx] == 1) break;
		vehicleName[i] = vehicles[idx];
		if(i + 1 < MAX_VEHICLE_NAME) i++; // only increment if the string is big enough
		idx++;
	}
	vehicleName[i] = '\0'; // null the end

	return vehicleName;
}

// return true if fail
// doesn't check to see if vehicle is already in the string
stock PlayerStat_AddVehicle(playerid, const vehiclename[])
{
	// if there is not enough space for the new name
	if(strlen(PlayerStat[playerid][pVehicles]) > MAX_PLAYER_VEHICLES * MAX_VEHICLE_NAME - MAX_VEHICLE_NAME){
		return 1;
	}
	// add the name onto the end
	strcat(PlayerStat[playerid][pVehicles], "", MAX_PLAYER_VEHICLES * MAX_VEHICLE_NAME);
	strcat(PlayerStat[playerid][pVehicles], vehiclename, MAX_PLAYER_VEHICLES * MAX_VEHICLE_NAME);
	return 0;
}

// return true if fail
// doesn't check to see if vehicle is already in the string
stock string_AddVehicle(vehicles[], const vehiclename[], destsize = sizeof(vehicles))
{
	// if there is not enough space for the new name
	if(strlen(vehicles) > destsize - MAX_VEHICLE_NAME){
		return 1;
	}
	// add the name onto the end
	strcat(vehicles, "", destsize);
	strcat(vehicles, vehiclename, destsize);
	return 0;
}



// -----------------------------------------------------------------------------
/*
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
}*/

// -----------------------------------------------------------------------------

// sets a vehicles number plate, returns true if the vehicle has a number plate,
// false if it does not, or the vehicleid is invalid.  Do not set the plate to
// more than 9 characters long, or it will overflow the numberplate!
/*stock setVehicleNumberPlateEx(vehicleid, string[])
{
    new modelid = GetVehicleModel(vehicleid); // returns 0 if invlid vehicleid
    if(modelid == 0) return false;
	if(model_hasPlate(modelid)){
	    SetVehicleNumberPlate(vehicleid, string);
	    return true;
	}
	return false;
}*/



stock testfunc()
{
	new filename[256] = "test.txt";

	new File:log_file;
	log_file = fopen(filename, io_append); // Open the file
	if(!log_file) return 0;

	
	new output[2048];
	format(output, sizeof(output), "\r\n");
	for(new i=0; i<212; i++){
		if(modelidHasPlate[i]){
			format(output, sizeof(output), "%d, ", i+400);
			fwrite(log_file, output);
		}
	}

	
	fclose(log_file);

	return 1;
}



