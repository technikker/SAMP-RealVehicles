// hardness, engine type, tanksize, FUEL CONSUMPTION, price, 


//
// vehicle hardness - the max health a vehicle may have
//

// default hardness
new Float:vehicleHardness[212] = {
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0, 1000.0,
	1000.0, 1000.0
};

stock Float:VehicleHardness(modelid)
{
	return vehicleHardness[modelid-400];
}

stock loadHardnessCSV()
{
	new line[256];
	new var_from_line[64];
	new index;

	new File:file_ptr;
	new filename[256];
	new model;
	new Float:hardness;

	format(filename, sizeof(filename), "vehicles/modelinfo/hardness.txt");
	file_ptr = fopen(filename, filemode:io_read);
	if(!file_ptr){
		printf("file \"%s\" not found!", filename);
		return 0;
	}

	// loop until all lines in the file have been read
	while(fread(file_ptr, line, 256) > 0)
	{
		index = 0;

		// modelid
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		model = strval(var_from_line);
 		if(model < 400 || model > 611) continue;

		// model hardness
		index = token_by_delim(line,var_from_line,';',index+1);
		hardness = floatstr(var_from_line);
		
		vehicleHardness[model-400] = hardness;
	}

	fclose(file_ptr);

	return 1;
}

stock vehicleHardness_logdump()
{
	print(" ------------------------------------------------------------------------------");
	print("Only showing models not using the default hardness!");
	for(new i=0; i<212; i++){
		if(vehicleHardness[i] != 1000.0){
			printf("Model %d\tHardness %.0f\t    Name %s", i+400, vehicleHardness[i], vehicleNames[i]);
		}
	}
	print(" ------------------------------------------------------------------------------\n");
	return;
}











// 0  == undefined
// -1 == no fuel tank/push powered
// >0 == this is the tank capacity
/*new Float:vehicleTankCapacity[212] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0
};*/

// TODO I hope this never gets called
stock loadFuelCapacityCSV_fuck()
{
	new line[256];
	new var_from_line[64];
	new index;

	new File:file_ptr;
	new filename[256];
	new model;
	new Float:capacity;

	format(filename, sizeof(filename), "vehicles/fuel_capacity.txt");
	file_ptr = fopen(filename, filemode:io_read);
	if(!file_ptr){
		printf("file \"%s\" not found!", filename);
		return 0;
	}

	// loop until all lines in the file have been read
	while(fread(file_ptr, line, 256) > 0)
	{
		index = 0;

		// modelid
		index = token_by_delim(line,var_from_line,',',index);
		if(index == (-1)) continue;
		model = strval(var_from_line);
 		if(model < 400 || model > 611) continue;

		// model capacity
		index = token_by_delim(line,var_from_line,';',index+1);
		capacity = floatstr(var_from_line);
		
		vehicleHardness[model-400] = capacity;
	}

	fclose(file_ptr);

	return 1;
}

stock vehicleTankSize_logdump()
{
	print(" ------------------------------------------------------------------------------");
	print("Please set a fuel tank capacity for the following vehicles!");
	for(new i=0; i<212; i++){
		if(vehicleHardness[i] != 1000.0){
			printf("Model %d\tHardness %.0f\t    Name %s", i+400, vehicleHardness[i], vehicleNames[i]);
		}
	}
	print(" ------------------------------------------------------------------------------\n");
	return;
}
