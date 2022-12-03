// load vehicles from file

//----------------------------------------------------------
// Tokenise by a delimiter
// Return string and index of the end determined by the
// provided delimiter in delim

// already in gl_common.inc
/*stock token_by_delim(const string[], return_str[], delim, start_index)
{
	new x=0;
	while(string[start_index] != EOS && string[start_index] != delim) {
		return_str[x] = string[start_index];
		x++;
		start_index++;
	}
	return_str[x] = EOS;
	if(string[start_index] == EOS) start_index = (-1);
	return start_index;
}*/

stock remove_blank(string[])
{
	for(new i=0; string[i]; i++){ // traverse the array
		if(string[i]==' ' || string[i]=='\t'){ // check for black characters
			for(new idx=i+1; string[idx]; idx++){
				if(string[idx]!=' ' && string[idx]!='\t'){ // look for the next non-blank
					string[i] = string[idx]; // move it to where the blank character was
					string[idx] = ' ';	   //
					break; // break
				}
			}
		}
	}
	//find the end, and null it
	for(new i=0; string[i]; i++){
		if(string[i]==' ' || string[i]=='\t'){
			string[i] = '\0';
			break;
		}
	}

	return;
}



// check all numberplates and unload them if they match a numberplate in the file
// TODO
stock unloadVehiclesFromCSV(const filename[])
{
	return;
}



// guest == true if the vehicle is not going to have its stats saved
// returns the number of vehicles loaded
stock loadVehiclesFromCSV(const filename[], bool:guest = false)
{
	new line[256];
	new var_from_line[64];
	new index;

	//
	// Read the vehicles file that tells us where to spawn vehicles
	//
	new File:file_ptr;
	new vehiclename[10]; // the numberplate
	new vehicletype;
	new Float:SpawnX;
	new Float:SpawnY;
	new Float:SpawnZ;
	new Float:SpawnR;
	new Color1;
	new Color2;
	new status;
	new owner[MAX_PLAYER_NAME];

	new vehicles_loaded = 0;

	file_ptr = fopen(filename, filemode:io_read);
	if(!file_ptr){
		//new str[256];
		//format(str, sizeof(str), "file %s not found!",filename);
		//SendClientMessage(0, COLOR_SERVER_HELP_MSG, str);
		printf("vehicle csv file \"%s\" not found!", filename);
		return 0;
	}

	// loop until all lines in the file have been read
	while(fread(file_ptr, line, 256) > 0)
	{
		index = 0;

		// Read numplate
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		format(vehiclename, sizeof(vehiclename), var_from_line);

		// Read type
		index = token_by_delim(line,var_from_line,',',index+1);
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
		SpawnR = floatstr(var_from_line);

		// Read Color1, Color2
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color1 = strval(var_from_line);

		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		Color2 = strval(var_from_line);

		// read the vehicles status
		index = token_by_delim(line,var_from_line,',',index+1);
		if(index == (-1)) continue;
		status = strval(var_from_line);

		// Read the vehicle owner
		index = token_by_delim(line,var_from_line,';',index+1);
		format(owner, sizeof(owner), var_from_line);
		remove_char(owner, ' ');

		// if loading vehicles as guest
		if(guest) status |= VEHSTATUS_GUEST;

		// attempt to connect the vehicle
		new vehstatid = vehicle_connect(vehiclename);
		

		// if the vehicle doesn't connect (prob because it doesn't exist)
		if(vehstatid == INVALID_VEHSTAT_ID){
			// create it
			vehstatid = vehicle_create(vehiclename, vehicletype, SpawnX, SpawnY, SpawnZ, SpawnR, Color1, Color2, owner, status); // TODO modify this function to  load status from file
		}

		// spawn it
		new vehid;
		if(vehstatid != INVALID_VEHSTAT_ID){
			vehid = vehicle_spawn(vehstatid);
		}

		if(vehid != INVALID_VEHICLE_ID){
			vehicles_loaded++;
		}
	}

	fclose(file_ptr);

	new string[256];
	format(string, sizeof(string), "Loaded %d vehicles from: %s", vehicles_loaded, filename);
	print(string);
	log_vehicle(string);

	return vehicles_loaded;
}






/*
// SPECIAL
//LoadStaticVehiclesFromFile("vehicles/trains.txt");
//LoadStaticVehiclesFromFile("vehicles/pilots.txt");

// LAS VENTURAS
LoadStaticVehiclesFromFileEx("vehicles/lv_law.txt",respawn);
LoadStaticVehiclesFromFile("vehicles/lv_airport.txt");
LoadStaticVehiclesFromFile("vehicles/lv_gen.txt");

// SAN FIERRO
LoadStaticVehiclesFromFile("vehicles/sf_law.txt");
LoadStaticVehiclesFromFile("vehicles/sf_airport.txt");
LoadStaticVehiclesFromFile("vehicles/sf_gen.txt");

// LOS SANTOS
//LoadStaticVehiclesFromFile("vehicles/ls_law.txt");
//LoadStaticVehiclesFromFile("vehicles/ls_airport.txt");
//LoadStaticVehiclesFromFile("vehicles/ls_gen_inner.txt");
//LoadStaticVehiclesFromFile("vehicles/ls_gen_outer.txt");

// OTHER AREAS
LoadStaticVehiclesFromFile("vehicles/whetstone.txt");
LoadStaticVehiclesFromFile("vehicles/bone.txt");
LoadStaticVehiclesFromFile("vehicles/flint.txt");
LoadStaticVehiclesFromFile("vehicles/tierra.txt");
LoadStaticVehiclesFromFile("vehicles/red_county.txt");
*/

