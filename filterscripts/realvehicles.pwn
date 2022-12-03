// This is a comment
// uncomment the line below if you want to write a filterscript
//#define FILTERSCRIPT

#include <a_samp>
#include <a_mysql>
#include <GetVehicleColor>
#include <YSI/y_ini>
#include <zcmd>
#include <sscanf2>

#include realvehicles/include.inc



public OnFilterScriptInit()
{
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

public OnGameModeInit()
{
	print(" - Loading Vehicles...");
	LoadDefaultVehicles(); // TODO fix this up

    //player_vehicle_add(0, "LSOUT_046");
    //player_vehicle_add(0, "LSOUT_047");
    //player_vehicle_remove(0, "LSOUT_046");

    //playerStat_load_ini("technician", 0);
	//printf("%d", playerStat[0][pMoneyOwed]);

    //offline_player_vehicle_sold("technician1", "LSOUT_290", 20000);
    
    //vehicle_owner_set(500, "technician");
    
    //new vehstatid = vehicleStat_getid("asdf");
    
    //printf("name %s, id %d", vehicleStat[vehstatid][vName], vehstatid);
    
    //printf("name %s", player_vehicle_get(0, 0));
    
    
    return 1;
}

public OnGameModeExit()
{
	return 1;
}



stock readcsv()
{
	print("reading");


	new filename[] = "vehicles/identities/ls_airport.txt";
	new File:file_csv = fopen(filename, filemode:io_read);
	if(!file_csv){
		printf("csv file \"%s\" not found!", filename);
		return 0;
	}



	// loop until all lines in the file have been read
	new line[256], data[64], idx;
	while(fread(file_csv, line, sizeof(line)) > 0)
	{
		idx = csv_content_get(data, sizeof(data), line, 0); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx); if(idx < 0) continue;
		printf("result -%s-", data);
		idx = csv_content_get(data, sizeof(data), line, idx);
		printf("result -%s-", data);


	}



	return 0;
}


stock csv_content_get(dest[], dest_size = sizeof(dest), const input[], index)
{
	if(dest_size < 1) return -1;
	dest[0] = '\0';
	if(index < 0) return index;

	new dest_idx = 0;
	new is_string = false;
	while(input[index])
	{
		// toggle string mode when we see '"' chars
		if(input[index] == '\"')
	    {
			toggle(is_string);
	        index++;
	        continue; // don't process this char
	    }

		// we can't see csv chars when in string mode
		if(!is_string)
		{
			// skip blank chars
			if(input[index] == ' ' || input[index] == '\t')
			{
			    index++;
			    continue;
			}

			// end field
		    if(input[index] == ',')
		    {
				if(input[index+1]) index++;
				else index = -1;
				break;
		    }

	        // end line
		    if(input[index] == ';' || input[index] == '\r' || input[index] == '\n')
		    {
				index = -1;
				break;
		    }
	    }

	    // skip char if dest is not big enough
	    if(dest_idx >= dest_size - 1){
			index++;
			continue;
		}

	    dest[dest_idx] = input[index]; // dest = source
	    index++, dest_idx++; // next index
	}
	dest[dest_idx] = '\0'; // end with null

	return index;
}


