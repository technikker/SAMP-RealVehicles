


#define DIALOG_VEHICLES					5000

// vehicle menu
#define DIALOG_VEHICLE					5001
#define DIALOG_VEHICLE_EJECT			5002
#define DIALOG_VEHICLE_REMOTE			5003
#define DIALOG_VEHICLE_PURCHASE			5004
#define DIALOG_VEHICLE_OPTIONS			5005

// options sub dialogs
#define DIALOG_VEHICLE_PARK				5006
#define DIALOG_VEHICLE_SPAWN			5007
#define DIALOG_VEHICLE_DESPAWN			5008
#define DIALOG_VEHICLE_CONNECT			5009
#define DIALOG_VEHICLE_DISCONNECT		5010
#define DIALOG_VEHICLE_PLATE			5011
#define DIALOG_VEHICLE_COLORS			5012
#define DIALOG_VEHICLE_SELL				5013
#define DIALOG_VEHICLE_GUEST			5014



#define COLOR_EMBED_BRIGHTPINK			"{ff80ff}"
#define COLOR_EMBED_BRIGHTGREEN			"{80FF80}"




// bring up the current vehicle menu
CMD:vehicle(playerid, params[])
{
	//SendClientMessage(playerid, COLOR_SERVER_HELP_MSG, "good 1");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid == 0) return 1;

	showDialog(vehicle, playerid, vehicleLink[vehicleid], "");

	return 1;
}


// bring up a list of vehicles the player owns
CMD:vehicles(playerid, params[])
{
	showDialog(vehicles, playerid, -1, "");
	return 1;
}


//playerid, dialogid, response, listitem, inputtext[], index, params[]
//ShowPlayerDialogEx(playerid, dialogid, dialogfunc[], style, caption[], info[], index = -1, params[] = "", button1[] = "Accept", button2[] = "Cancel", bool:showcolors = false)



// TODO this makes the server crash
DIALOG:vehicles(playerid, index, params[], listitem, inputtext[], dialogid, response)
{
	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLES;
		new dialogfunc[]	= "vehicles";
		new caption[]		= "Vehicles Owned";

		//new string[] = "LSOUT_094:LSOUT_349:LSOUT_002:";
		new info[300] = "";

		for(new idx = 0; idx < playerStat[playerid][pVehiclesLen]; idx++)
		{
			strcat(info, player_vehicle_get(playerid, idx));
			strcat(info, "\n");
		}
		
		// collect cash from sold vehicles
		if(playerStat[playerid][pMoneyOwed] > 0)
		{
			strcat(info, "Collect Payments ");
			new temp[20];
			format(temp, sizeof(temp), "($%d)\n", playerStat[playerid][pMoneyOwed]);
			strcat(info, temp);
		}
		
		// if they have no vehicles
		if(!info[0]) strcat(info, "No vehicles to display!");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_LIST, caption, info, -1, "", "Select", "Close");
		return 0;
	}

	// do stuff on Dialog Response
	if(response){
		// they selected a vehicle
		if(listitem < playerStat[playerid][pVehiclesLen])
		{
			new vehstatid = player_vehstatid_get(playerid, listitem);
			if(vehstatid == INVALID_VEHSTAT_ID)
			{
				vehstatid = vehicle_connect(inputtext);
			}

			showDialog(vehicle, playerid, vehstatid, "");
		}
		
		// they selected one of the other two options
		else
		{
			// collect cash owed
			if(strmatch2(inputtext, "Co", 2))
			{
				GivePlayerMoney(playerid, playerStat[playerid][pMoneyOwed]);
				playerStat[playerid][pMoneyOwed] = 0;
				playerStat_save(playerStat[playerid]); // make sure they don't collect twice if server crashes
			}

			showDialog(vehicles, playerid, index, "");
		}

		
	}

	return 1;
}





//-----------------------------------------------------------------------------

// index will be passed a vehstatid
DIALOG:vehicle(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE;
		new dialogfunc[]	= "vehicle";
		new caption[MAX_VEHICLE_NAME];
		strcopy(caption, sizeof(caption), vehicleStat[vehstatid][vName]);
		new info[300];
		
		// Make up the dialog list
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Engine\t\t(ON)\t\t\n"); 
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Engine\t\t(OFF)\t\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Lights\t\t(ON)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Lights\t\t(OFF)\t\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Doors\t\t(LOCKED)\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Doors\t\t(UNLOCKED)\t\n");
		//strcat(info, "Eject\n"); // TODO eject players from vehicle
		strcat(info, "Remote\n");
		strcat(info, "Park\n");
		if(vehicleStat[vehstatid][vStatus] & VEHSTATUS_ONSALE) // if vehicle for sale
		{
			new sale_price[40];
			format(sale_price, sizeof(sale_price), "{00ff00}For Sale ($%d)\n", vehicleStat[vehstatid][vValue]);
			strcat(info, sale_price);
		}
		else strcat(info, "{808080}Not For Sale\n");
		
		// only if player is owner
		new playername[MAX_PLAYER_NAME];
		GetPlayerName(playerid, playername, sizeof(playername));
		if(vehicleStat[vehstatid][vOwner][0] && !strcmp(vehicleStat[vehstatid][vOwner], playername)){
			strcat(info, "Options\n");
		}
		
		// if vehicle is guest and player is admin
		if(vehicleStat[vehstatid][vStatus] & VEHSTATUS_GUEST && IsPlayerAdmin(playerid))
		{
			strcat(info, "Upgrade from Guest\n");
		}

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_LIST, caption, info, vehstatid, params, "Select", "Close");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		if	   (strmatch2(inputtext, "En", 2)) vehicle_params_toggle(vehstatid, 1, 0, 0, 0, 0, 0, 0);
		else if(strmatch2(inputtext, "Li", 2)) vehicle_params_toggle(vehstatid, 0, 1, 0, 0, 0, 0, 0);
		else if(strmatch2(inputtext, "Do", 2)) vehicle_params_toggle(vehstatid, 0, 0, 0, 1, 0, 0, 0);
		//else if(strmatch2(inputtext, "Ej", 2)) SendClientMessage(playerid, 0xFF0000FF, "Eject");
		else if(strmatch2(inputtext, "Re", 2)) showDialog(vehicle_remote, playerid, vehstatid, "");
		else if(strmatch2(inputtext, "Pa", 2))
		{
			SendClientMessage(playerid, 0xFF0000FF, "Vehicle parked in current location");
			vehicle_park(vehstatid);
		}
		else if(strmatch2(inputtext, "Fo", 2)) showDialog(vehicle_purchase, playerid, vehstatid, "");
		else if(strmatch2(inputtext, "No", 2)) showDialog(vehicle, playerid, vehstatid, "");
		else if(strmatch2(inputtext, "Op", 2)) showDialog(vehicle_options, playerid, vehstatid, "");
		else if(strmatch2(inputtext, "Up", 2)) showDialog(vehicle_guest, playerid, vehstatid, "");
	}

	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_remote(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_REMOTE;
		new dialogfunc[]	= "vehicle_remote";
		new caption[25]		= "Toggle Systems ";
		strcat(caption, vehicleStat[vehstatid][vName]);

		new info[180];
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ENGINE)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Engine\t\t(ON)\t\t\n"); 
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Engine\t\t(OFF)\t\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LIGHTS)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Lights\t\t(ON)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Lights\t\t(OFF)\t\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_ALARM)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Alarm\t\t(ON)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Alarm\t\t(OFF)\t\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_LOCK)		strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Doors\t\t(LOCKED)\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Doors\t\t(UNLOCKED)\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BONNET)	strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Bonnet\t\t(OPEN)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Bonnet\t\t(CLOSED)\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_BOOT)		strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Boot\t\t(OPEN)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Boot\t\t(CLOSED)\t\n");
		if(vehicleStat[vehstatid][vParams] & VEHPARAM_OBJ)		strcat(info, COLOR_EMBED_BRIGHTGREEN),	strcat(info, "Objective\t(ON)\t\t\n");
		else													strcat(info, COLOR_EMBED_BRIGHTPINK),	strcat(info, "Objective\t(OFF)\t\t\n");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_LIST, caption, info, vehstatid, params, "Toggle", "Back");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		switch(listitem)
		{
			case 0: vehicle_params_toggle(vehstatid, 1, 0, 0, 0, 0, 0, 0);
			case 1: vehicle_params_toggle(vehstatid, 0, 1, 0, 0, 0, 0, 0);
			case 2: vehicle_params_toggle(vehstatid, 0, 0, 1, 0, 0, 0, 0);
			case 3: vehicle_params_toggle(vehstatid, 0, 0, 0, 1, 0, 0, 0);
			case 4: vehicle_params_toggle(vehstatid, 0, 0, 0, 0, 1, 0, 0);
			case 5: vehicle_params_toggle(vehstatid, 0, 0, 0, 0, 0, 1, 0);
			case 6: vehicle_params_toggle(vehstatid, 0, 0, 0, 0, 0, 0, 1);
		}
		// bring the dialog back up
		showDialog(vehicle_remote, playerid, vehstatid, "");
	}
	else{
		// return them to the vehicle menu
		showDialog(vehicle, playerid, vehstatid, "");
	}

	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_options(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_OPTIONS;
		new dialogfunc[]	= "vehicle_options";
		new caption[]		= "Vehicle Options";

		new info[180];
		if(vehicleStat[vehstatid][vSpawned]) strcat(info, "Despawn\n");
		else strcat(info, "Spawn\n");
		strcat(info, "Disconnect\n");
		strcat(info, "Change Name\n");
		new color1[9]; strcopy(color1, sizeof(color1), GetEmbeddedColor(GetVehicleColorRGB(vehicleStat[vehstatid][vColor1])));
		new color2[9]; strcopy(color2, sizeof(color2), GetEmbeddedColor(GetVehicleColorRGB(vehicleStat[vehstatid][vColor2])));
		strcat(info, "Change Colour ("), strcat(info, color1), strcat(info, "col1, "), strcat(info, color2), strcat(info, "col2{ffffff})\n");
		strcat(info, "Sell\n");
		if(vehicleStat[vehstatid][vStatus] & VEHSTATUS_ONSALE) strcat(info, "Don't Sell\n");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_LIST, caption, info, vehstatid, params, "Select", "Back");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		switch(listitem)
		{
			case 0:
			{
				if(vehicleStat[vehstatid][vSpawned]) showDialog(vehicle_despawn, playerid, vehstatid, "");
				else
				{
					vehicle_spawn(vehstatid);
					SendClientMessage(playerid, 0xFF0000FF, "Vehicle spawned.");
					showDialog(vehicle_options, playerid, vehstatid, "");
				}
			}
			case 1: showDialog(vehicle_disconnect, playerid, vehstatid, "");
			case 2: showDialog(vehicle_plate, playerid, vehstatid, "");
			case 3: showDialog(vehicle_color, playerid, vehstatid, "01"); // page 0, colour 1
			case 4: showDialog(vehicle_sell, playerid, vehstatid, ""); // Sell option
			case 5:
			{
				vehicleStat[vehstatid][vStatus] &= ~VEHSTATUS_ONSALE;
				SendClientMessage(playerid, 0xFF0000FF, "Vehicle is no longer for sale.");
				showDialog(vehicle_options, playerid, vehstatid, "");
			}
			case 6: showDialog(vehicle_guest, playerid, vehstatid, "");
		}
		// bring the dialog back up
	}
	else{
		// return them to the vehicle menu
		showDialog(vehicle, playerid, vehstatid, "");
	}

	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_despawn(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_DESPAWN;
		new dialogfunc[]	= "vehicle_despawn";
		new caption[]		= "Despawn Vehicle";
		new info[400];
		strcat(info, "WARNING: Are you sure you want to despawn ");
		strcat(info, vehicleStat[vehstatid][vPlate]);
		strcat(info, "?");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_MSGBOX, caption, info, vehstatid, params, "Yes", "No");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		// despawn the vehicle
		if(vehicle_despawn(vehstatid))
			SendClientMessage(playerid, 0xFF0000FF, "Vehicle has successfully despawned.  Access your /vehicles menu to spawn it again."); // TODO set a better colour
		else SendClientMessage(playerid, 0xFF0000FF, "Failed to despawn!"); // TODO set a better colour
	}

	// return them to the vehicle options menu
	showDialog(vehicle_options, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_disconnect(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_DISCONNECT;
		new dialogfunc[]	= "vehicle_disconnect";
		new caption[]		= "Disconnect Vehicle";
		new info[400];
		strcat(info, "WARNING: Are you sure you want to disconnect ");
		strcat(info, vehicleStat[vehstatid][vPlate]);
		strcat(info, "{ffffff}?");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_MSGBOX, caption, info, vehstatid, params, "Yes", "No");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		// Try to disconnect the vehicle
		if(!vehicle_disconnect(vehstatid)){ // if successful
			SendClientMessage(playerid, 0xFF0000FF, "Vehicle has successfully disconnected from the server."); // TODO set a better colour
		}
		else{
			SendClientMessage(playerid, 0xFF0000FF, "BAD: Failed to disconnect!"); // TODO set a better colour
		}
	}

	// return them to the vehicle options menu
	showDialog(vehicle_options, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_plate(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_PLATE;
		new dialogfunc[]	= "vehicle_plate";
		new caption[]		= "Enter new number plate...";
		new info[400];
		strcat(info, "{ffffff}Do not enter more than 32 characters!\n\n");
		strcat(info, "You can specify RGB colours by using {} characters.\n");
		strcat(info, "The actual text to display as the number plate must\n");
		strcat(info, "not exceed nine characters.  Avoid using any\n");
		//strcat(info, "< > : \\ / | * ? [ ] = % $ + , ; ~ \" characters.\n\n"); // banned under windows \ / : * ? " < > |
		strcat(info, "< > : \\ / | * ? ~ \" characters.\n\n"); // slightly less strict
		strcat(info, "Example:  \"{ ff0000 }ADMN{ 0000ff }FNTRY\" Will\n");
		strcat(info, "read as   \"{ff0000}ADMN{0000ff}FNTRY{ffffff}\"   on the vehicle.");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_INPUT, caption, info, vehstatid, params, "Set", "Back");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		new string[120];
		// Try to set the new number plate
		if(!vehicle_name_set(vehstatid, inputtext)){ // if successful
			format(string, sizeof(string), "Number plate changed to \"%s\"! To see the new plate you will need to re-spawn the vehicle.", vehicleStat[vehstatid][vName]);
			SendClientMessage(playerid, 0xFF0000FF, string); // TODO set a better colour
		}
		else{
			format(string, sizeof(string), "BAD: Failed to set number plate! Please make sure you enter a valid number plate.", inputtext);
			SendClientMessage(playerid, 0xFF0000FF, string); // TODO set a better colour
		}
	}

	// return them to the vehicle options menu
	showDialog(vehicle_options, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_sell(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_SELL;
		new dialogfunc[]	= "vehicle_sell";
		new caption[]		= "Enter your sale price...";

		new info[400];
		strcat(info, "{ffffff}Please enter your price for your vehicle.\n\n");
		strcat(info, "You will receive payment once another player\n");
		strcat(info, "chooses to purchase it.  You may continue using\n");
		strcat(info, "your vehicle in the meantime\n\n");
		strcat(info, "The system may reclaim vehicles from players if\n");
		strcat(info, "the asking price is low enough.");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_INPUT, caption, info, vehstatid, params, "Sell", "Back");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){

		new price = strval(inputtext); // string to int
		if(price == 0) price = vehicleStat[vehstatid][vValue];
		if(price > 0)
		{
			vehicleStat[vehstatid][vStatus] |= VEHSTATUS_ONSALE;
			vehicleStat[vehstatid][vValue] = price;
			SendClientMessage(playerid, 0xFF0000FF, "Vehicle is for sale!"); // TODO set a better colour
		}
		else
		{
			SendClientMessage(playerid, 0xFF0000FF, "You can't sell your vehicle for a negative ammount.  That's stupid."); // TODO set a better colour
		}

	}

	// return them to the vehicle options menu
	showDialog(vehicle_options, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_purchase(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_PURCHASE;
		new dialogfunc[]	= "vehicle_purchase";
		new caption[]		= "Purchase Vehicle?";

		new info[400];
		strcat(info, "{ffffff}Do you want to purchase this ");
		strcat(info, vehicleNames[ vehicleStat[vehstatid][vModel] - 400 ]);
		new vValue_str[15];
		format(vValue_str, sizeof(vValue_str), " for $%d?", vehicleStat[vehstatid][vValue]);
		strcat(info, vValue_str);

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_MSGBOX, caption, info, vehstatid, params, "Yes", "No");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		
		// if insufficient funds
		if(GetPlayerMoney(playerid) < vehicleStat[vehstatid][vValue])
		{
			SendClientMessage(playerid, 0xFF0000FF, "You have insufficient funds to do this."); // TODO set a better colour
		}
		
		// make the trade
		else
		{
			// take their money
			new saleprice = vehicleStat[vehstatid][vValue];
			GivePlayerMoney(playerid, -saleprice);
			// give funds to previous owner and cancel their ownership
			offline_player_vehicle_sold(vehicleStat[vehstatid][vOwner], vehicleStat[vehstatid][vName], saleprice);
			// change the owner
			vehicle_owner_set(vehstatid, getPlayerName(playerid));
			printf("pname %s", getPlayerName(playerid));
			printf("pname %s", playerStat[playerid][pName]);
			// not for sale anymore
			vehicleStat[vehstatid][vStatus] &= ~VEHSTATUS_ONSALE;

			SendClientMessage(playerid, 0xFF0000FF, "Vehicle purchased!"); // TODO set a better colour
		}

	}

	// return them to the vehicle menu
	showDialog(vehicle, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

DIALOG:vehicle_guest(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_GUEST;
		new dialogfunc[]	= "vehicle_guest";
		new caption[]		= "Upgrate From Guest?";

		new info[400];
		strcat(info, "{ffffff}Do you want to make this ");
		strcat(info, vehicleNames[ vehicleStat[vehstatid][vModel] - 400 ]);
		strcat(info, "\n a permanent vehicle on the server?");

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_MSGBOX, caption, info, vehstatid, params, "Yes", "No");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		
		vehicleStat[vehstatid][vStatus] &= ~VEHSTATUS_GUEST; // unset guest marker
		SendClientMessage(playerid, 0xFF0000FF, "Vehicle status upgraded from guest to resident."); // TODO set a better colour
	}

	// return them to the vehicle menu
	showDialog(vehicle, playerid, vehstatid, "");
	return 1;
}

//-----------------------------------------------------------------------------

// params[0] == page number, params[1] == colour number
DIALOG:vehicle_color(playerid, vehstatid, params[], listitem, inputtext[], dialogid, response)
{
	if(!valid_index(vehstatid, MAX_VEHICLES)) return 0;				// index out of range
	if(!valid_modelid(vehicleStat[vehstatid][vModel])) return 0;	// id not in use

	new page  = 0; // 0 to 3
	new color = 1; // 1 or 2 (which colour to set)
	if(params[0]){
		if(params[0] > '0' && params[0] < '4') page = params[0] - 48; // char play
		if(params[1] == '2') color = 2;
	}

	// create the dialog
	if(response == -1){
		dialogid			= DIALOG_VEHICLE_COLORS;
		new dialogfunc[]	= "vehicle_color";
		new caption[25]		= "";
		format(caption, sizeof(caption), "Select Colour %d (page %d)", color, page + 1);

		// now for the gigantosaurus
		new info[2048] = "";
		switch(page)
		{
			// The existing colours from San Andreas
			case 0:{ // page 0, first 64 colours
				strcat(info, "---- Next Page ----\n");
				strcat(info, getVehicleColorEmbedded(  0)), strcat(info, "black\n");								//   0		0,0,0			black
				strcat(info, getVehicleColorEmbedded(  1)), strcat(info, "white\n");								//   1		245,245,245		white
				strcat(info, getVehicleColorEmbedded(  2)), strcat(info, "police car blue\n");						//   2		42,119,161		blue
				strcat(info, getVehicleColorEmbedded(  3)), strcat(info, "cherry red\n");							//   3		132,4,16		red
				strcat(info, getVehicleColorEmbedded(  4)), strcat(info, "midnight blue\n");						//   4		38,55,57		dark blue
				strcat(info, getVehicleColorEmbedded(  5)), strcat(info, "temple curtain purple\n");				//   5		134,68,110		purple
				strcat(info, getVehicleColorEmbedded(  6)), strcat(info, "taxi yellow\n");							//   6		215,142,16		yellow
				strcat(info, getVehicleColorEmbedded(  7)), strcat(info, "striking blue\n");						//   7		76,117,183		bright blue
				strcat(info, getVehicleColorEmbedded(  8)), strcat(info, "light blue grey\n");						//   8		189,190,198		light blue grey
				strcat(info, getVehicleColorEmbedded(  9)), strcat(info, "hoods\n");								//   9		94,112,114		light
				strcat(info, getVehicleColorEmbedded( 10)), strcat(info, "saxony blue poly\n");						//  10		70,89,122		blue
				strcat(info, getVehicleColorEmbedded( 11)), strcat(info, "concord blue poly\n");					//  11		101,106,121		grey
				strcat(info, getVehicleColorEmbedded( 12)), strcat(info, "jasper green poly\n");					//  12		93,126,141		blue
				strcat(info, getVehicleColorEmbedded( 13)), strcat(info, "pewter gray poly\n");						//  13		88,89,90		grey
				strcat(info, getVehicleColorEmbedded( 14)), strcat(info, "frost white\n");							//  14		214,218,214		light
				strcat(info, getVehicleColorEmbedded( 15)), strcat(info, "silver stone poly\n");					//  15		156,161,163		grey
				strcat(info, getVehicleColorEmbedded( 16)), strcat(info, "rio red\n");								//  16		51,95,63		green
				strcat(info, getVehicleColorEmbedded( 17)), strcat(info, "torino red pearl\n");						//  17		115,14,26		red
				strcat(info, getVehicleColorEmbedded( 18)), strcat(info, "formula red\n");							//  18		123,10,42		red
				strcat(info, getVehicleColorEmbedded( 19)), strcat(info, "honey beige poly\n");						//  19		159,157,148		light
				strcat(info, getVehicleColorEmbedded( 20)), strcat(info, "mariner blue\n");							//  20		59,78,120		blue
				strcat(info, getVehicleColorEmbedded( 21)), strcat(info, "blaze red\n");							//  21		115,46,62		red
				strcat(info, getVehicleColorEmbedded( 22)), strcat(info, "classic red\n");							//  22		105,30,59		red
				strcat(info, getVehicleColorEmbedded( 23)), strcat(info, "winning silver poly\n");					//  23		150,145,140		grey
				strcat(info, getVehicleColorEmbedded( 24)), strcat(info, "steel gray poly\n");						//  24		81,84,89		grey
				strcat(info, getVehicleColorEmbedded( 25)), strcat(info, "shadow silver poly\n");					//  25		63,62,69		dark
				strcat(info, getVehicleColorEmbedded( 26)), strcat(info, "silver stone poly\n");					//  26		165,169,167		grey
				strcat(info, getVehicleColorEmbedded( 27)), strcat(info, "warm grey mica\n");						//  27		99,92,90		grey
				strcat(info, getVehicleColorEmbedded( 28)), strcat(info, "harbor blue poly\n");						//  28		61,74,104		blue
				strcat(info, getVehicleColorEmbedded( 29)), strcat(info, "porcelain silver poly\n");				//  29		151,149,146		grey
				strcat(info, getVehicleColorEmbedded( 30)), strcat(info, "mellow burgundy\n");						//  30		66,31,33		dark
				strcat(info, getVehicleColorEmbedded( 31)), strcat(info, "graceful red mica\n");					//  31		95,39,43		red
				strcat(info, getVehicleColorEmbedded( 32)), strcat(info, "currant blue poly\n");					//  32		132,148,171		blue
				strcat(info, getVehicleColorEmbedded( 33)), strcat(info, "gray poly\n");							//  33		118,123,124		grey
				strcat(info, getVehicleColorEmbedded( 34)), strcat(info, "arctic white\n");							//  34		100,100,100		light
				strcat(info, getVehicleColorEmbedded( 35)), strcat(info, "anthracite gray poly\n");					//  35		90,87,82		grey
				strcat(info, getVehicleColorEmbedded( 36)), strcat(info, "black poly\n");							//  36		37,37,39		dark
				strcat(info, getVehicleColorEmbedded( 37)), strcat(info, "dark green poly\n");						//  37		45,58,53		green
				strcat(info, getVehicleColorEmbedded( 38)), strcat(info, "seafoam poly\n");							//  38		147,163,150		light
				strcat(info, getVehicleColorEmbedded( 39)), strcat(info, "diamond blue poly\n");					//  39		109,122,136		blue
				strcat(info, getVehicleColorEmbedded( 40)), strcat(info, "biston brown poly\n");					//  40		34,25,24		dark
				strcat(info, getVehicleColorEmbedded( 41)), strcat(info, "desert taupe poly\n");					//  41		111,103,95		dark
				strcat(info, getVehicleColorEmbedded( 42)), strcat(info, "garnet red poly\n");						//  42		124,28,42		red
				strcat(info, getVehicleColorEmbedded( 43)), strcat(info, "desert red\n");							//  43		95,10,21		red
				strcat(info, getVehicleColorEmbedded( 44)), strcat(info, "green\n");								//  44		25,56,38		green
				strcat(info, getVehicleColorEmbedded( 45)), strcat(info, "cabernet red poly\n");					//  45		93,27,32		red
				strcat(info, getVehicleColorEmbedded( 46)), strcat(info, "light ivory\n");							//  46		157,152,114		light
				strcat(info, getVehicleColorEmbedded( 47)), strcat(info, "pueblo beige\n");							//  47		122,117,96		green
				strcat(info, getVehicleColorEmbedded( 48)), strcat(info, "smoke silver poly\n");					//  48		152,149,134		lightV
				strcat(info, getVehicleColorEmbedded( 49)), strcat(info, "astra silver poly\n");					//  49		173,176,176		greyV
				strcat(info, getVehicleColorEmbedded( 50)), strcat(info, "ascot gray\n");							//  50		132,137,136		greyV
				strcat(info, getVehicleColorEmbedded( 51)), strcat(info, "agate green\n");							//  51		48,79,69		green
				strcat(info, getVehicleColorEmbedded( 52)), strcat(info, "petrol blue green poly\n");				//  52		77,98,104		greenV
				strcat(info, getVehicleColorEmbedded( 53)), strcat(info, "surf blue\n");							//  53		22,34,72		blue
				strcat(info, getVehicleColorEmbedded( 54)), strcat(info, "nautical blue poly\n");					//  54		39,47,75		blue
				strcat(info, getVehicleColorEmbedded( 55)), strcat(info, "woodrose poly\n");						//  55		125,98,86		dark
				strcat(info, getVehicleColorEmbedded( 56)), strcat(info, "crystal blue poly\n");					//  56		158,164,171		blue
				strcat(info, getVehicleColorEmbedded( 57)), strcat(info, "bisque frost poly\n");					//  57		156,141,113		light
				strcat(info, getVehicleColorEmbedded( 58)), strcat(info, "currant red solid\n");					//  58		109,24,34		red
				strcat(info, getVehicleColorEmbedded( 59)), strcat(info, "lt.crystal blue poly\n");					//  59		78,104,129		blue
				strcat(info, getVehicleColorEmbedded( 60)), strcat(info, "lt.titanium poly\n");						//  60		156,156,152		light
				strcat(info, getVehicleColorEmbedded( 61)), strcat(info, "race yellow solid\n");					//  61		145,115,71		yellow
				strcat(info, getVehicleColorEmbedded( 62)), strcat(info, "brt.currant red poly\n");					//  62		102,28,38		red
				strcat(info, getVehicleColorEmbedded( 63)), strcat(info, "clear crystal blue frost poly\n");		//  63		148,157,159		grey
				strcat(info, "---- Next Page ----\n");
			}
			case 1:{ // page 1, 64 colours
				strcat(info, "---- Next Page ----\n");
				strcat(info, getVehicleColorEmbedded( 64)), strcat(info, "silver poly\n");							//  64		164,167,165		grey
				strcat(info, getVehicleColorEmbedded( 65)), strcat(info, "pastel alabaster\n");						//  65		142,140,70		yellow
				strcat(info, getVehicleColorEmbedded( 66)), strcat(info, "mid currant red poly\n");					//  66		52,26,30		dark
				strcat(info, getVehicleColorEmbedded( 67)), strcat(info, "med regatta blue poly\n");				//  67		106,122,140		blue
				strcat(info, getVehicleColorEmbedded( 68)), strcat(info, "oxford white solid\n");					//  68		170,173,142		yellow
				strcat(info, getVehicleColorEmbedded( 69)), strcat(info, "alabaster solid\n");						//  69		171,152,143		light
				strcat(info, getVehicleColorEmbedded( 70)), strcat(info, "elec.currant red poly\n");				//  70		133,31,46		red
				strcat(info, getVehicleColorEmbedded( 71)), strcat(info, "spinnaker blue solid\n");					//  71		111,130,151		blue
				strcat(info, getVehicleColorEmbedded( 72)), strcat(info, "dk.titanium poly\n");						//  72		88,88,83		grey
				strcat(info, getVehicleColorEmbedded( 73)), strcat(info, "pastel alabaster solid\n");				//  73		154,167,144		light
				strcat(info, getVehicleColorEmbedded( 74)), strcat(info, "med.cabernet solid\n");					//  74		96,26,35		red
				strcat(info, getVehicleColorEmbedded( 75)), strcat(info, "twilight blue poly\n");					//  75		32,32,44		dark
				strcat(info, getVehicleColorEmbedded( 76)), strcat(info, "titanium frost poly\n");					//  76		164,160,150		grey
				strcat(info, getVehicleColorEmbedded( 77)), strcat(info, "sandalwood frost poly\n");				//  77		170,157,132		light
				strcat(info, getVehicleColorEmbedded( 78)), strcat(info, "wild strawberry poly\n");					//  78		120,34,43		red
				strcat(info, getVehicleColorEmbedded( 79)), strcat(info, "ultra blue poly\n");						//  79		14,49,109		blue
				strcat(info, getVehicleColorEmbedded( 80)), strcat(info, "vermilion solid\n");						//  80		114,42,63		red
				strcat(info, getVehicleColorEmbedded( 81)), strcat(info, "med.sandalwood poly\n");					//  81		123,113,94		light
				strcat(info, getVehicleColorEmbedded( 82)), strcat(info, "med.red solid\n");						//  82		116,29,40		red
				strcat(info, getVehicleColorEmbedded( 83)), strcat(info, "deep jewel green\n");						//  83		30,46,50		green
				strcat(info, getVehicleColorEmbedded( 84)), strcat(info, "med.woodrose poly\n");					//  84		77,50,47		dark
				strcat(info, getVehicleColorEmbedded( 85)), strcat(info, "vermillion solid\n");						//  85		124,27,68		red
				strcat(info, getVehicleColorEmbedded( 86)), strcat(info, "green\n");								//  86		46,91,32		green
				strcat(info, getVehicleColorEmbedded( 87)), strcat(info, "bright blue poly\n");						//  87		57,90,131		blue
				strcat(info, getVehicleColorEmbedded( 88)), strcat(info, "bright red\n");							//  88		109,40,55		red
				strcat(info, getVehicleColorEmbedded( 89)), strcat(info, "lt.champagne poly\n");					//  89		167,162,143		light
				strcat(info, getVehicleColorEmbedded( 90)), strcat(info, "silver poly\n");							//  90		175,177,177		silver
				strcat(info, getVehicleColorEmbedded( 91)), strcat(info, "steel blue poly\n");						//  91		54,65,85		blue
				strcat(info, getVehicleColorEmbedded( 92)), strcat(info, "medium gray poly\n");						//  92		109,108,110		grey
				strcat(info, getVehicleColorEmbedded( 93)), strcat(info, "arctic pearl\n");							//  93		15,106,137		light
				strcat(info, getVehicleColorEmbedded( 94)), strcat(info, "nassau blue poly\n");						//  94		32,75,107		blue
				strcat(info, getVehicleColorEmbedded( 95)), strcat(info, "med.sapphire blue poly\n");				//  95		43,62,87		blue
				strcat(info, getVehicleColorEmbedded( 96)), strcat(info, "silver poly\n");							//  96		155,159,157		silver
				strcat(info, getVehicleColorEmbedded( 97)), strcat(info, "lt.sapphire blue poly\n");				//  97		108,132,149		blue
				strcat(info, getVehicleColorEmbedded( 98)), strcat(info, "malachite poly\n");						//  98		77.93,96		blue
				strcat(info, getVehicleColorEmbedded( 99)), strcat(info, "flax\n");									//  99		174,155,127		yellow
				strcat(info, getVehicleColorEmbedded(100)), strcat(info, "med.maui blue poly\n");					// 100		64,108,143		blue
				strcat(info, getVehicleColorEmbedded(101)), strcat(info, "dk.sapphire blue poly\n");				// 101		31,37,59		dark
				strcat(info, getVehicleColorEmbedded(102)), strcat(info, "copper beige\n");							// 102		171,146,118		light
				strcat(info, getVehicleColorEmbedded(103)), strcat(info, "bright blue poly\n");						// 103		19,69,115		blue
				strcat(info, getVehicleColorEmbedded(104)), strcat(info, "med.flax\n");								// 104		150,129,108		yellow
				strcat(info, getVehicleColorEmbedded(105)), strcat(info, "med.gray poly\n");						// 105		100,104,106		grey
				strcat(info, getVehicleColorEmbedded(106)), strcat(info, "bright blue poly\n");						// 106		16,80,130		blue
				strcat(info, getVehicleColorEmbedded(107)), strcat(info, "lt.driftwood poly\n");					// 107		161,153,131		light
				strcat(info, getVehicleColorEmbedded(108)), strcat(info, "blue\n");									// 108		56,86,148		blue
				strcat(info, getVehicleColorEmbedded(109)), strcat(info, "steel gray poly\n");						// 109		82,86,97		grey
				strcat(info, getVehicleColorEmbedded(110)), strcat(info, "lt.beechwood poly\n");					// 110		127,105,86		dark
				strcat(info, getVehicleColorEmbedded(111)), strcat(info, "slate gray\n");							// 111		140,146,154		grey
				strcat(info, getVehicleColorEmbedded(112)), strcat(info, "lt.sapphire blue poly\n");				// 112		89,110,135		blue
				strcat(info, getVehicleColorEmbedded(113)), strcat(info, "dk.beechwood poly\n");					// 113		71,53,50		dark
				strcat(info, getVehicleColorEmbedded(114)), strcat(info, "torch red\n");							// 114		68,98,79		green
				strcat(info, getVehicleColorEmbedded(115)), strcat(info, "bright red\n");							// 115		115,10,39		red
				strcat(info, getVehicleColorEmbedded(116)), strcat(info, "med.sapphire blue firemist\n");			// 116		34,52,87		blue
				strcat(info, getVehicleColorEmbedded(117)), strcat(info, "med.garnet red poly\n");					// 117		100,13,27		red
				strcat(info, getVehicleColorEmbedded(118)), strcat(info, "white diamond pearl\n");					// 118		163,173,198		light
				strcat(info, getVehicleColorEmbedded(119)), strcat(info, "dk.sable poly\n");						// 119		105,88,83		dark
				strcat(info, getVehicleColorEmbedded(120)), strcat(info, "antelope beige\n");						// 120		155,139,128		light
				strcat(info, getVehicleColorEmbedded(121)), strcat(info, "brilliant red poly\n");					// 121		98,11,28		red
				strcat(info, getVehicleColorEmbedded(122)), strcat(info, "gun metal poly\n");						// 122		91,93,94		grey
				strcat(info, getVehicleColorEmbedded(123)), strcat(info, "med.beechwood poly\n");					// 123		98,68,40		dark
				strcat(info, getVehicleColorEmbedded(124)), strcat(info, "brilliant red poly\n");					// 124		115,24,39		red
				strcat(info, getVehicleColorEmbedded(125)), strcat(info, "bright blue poly\n");						// 125		27,55,109		blue
				strcat(info, getVehicleColorEmbedded(126)), strcat(info, "pink\n");									// 126		236,106,174		light
				strcat(info, getVehicleColorEmbedded(127)), strcat(info, "black\n");								// 127		0,0,0			black
				strcat(info, "---- Next Page ----\n");
			}
			// SA-MP extended colours (0.3x)
			case 2:{ // page 3, 64 colours (id 128 to 191)
				strcat(info, "---- Next Page ----\n");
				new colorid = 128;
				new colorid_str[4];
				while(colorid < 192){
					format(colorid_str, sizeof(colorid_str), "%d", colorid);
					strcat(info, getVehicleColorEmbedded(colorid)), strcat(info, "SAMP Colour "), strcat(info, colorid_str), strcat(info, "\n");
					colorid++;
				}
				strcat(info, "---- Next Page ----\n");
			}
			case 3:{ // page 4, 64 colours (id 192 to 255)
				strcat(info, "---- Next Page ----\n");
				new colorid = 192;
				new colorid_str[4];
				while(colorid < 256){
					format(colorid_str, sizeof(colorid_str), "%d", colorid);
					strcat(info, getVehicleColorEmbedded(colorid)), strcat(info, "SAMP Colour "), strcat(info, colorid_str), strcat(info, "\n");
					colorid++;
				}
				strcat(info, "---- Next Page ----\n");
			}
			default: strcat(info, "---- Next Page ----\n");
		}

		ShowPlayerDialogEx(playerid, dialogid, dialogfunc, DIALOG_STYLE_LIST, caption, info, vehstatid, params, "Select", "Back");
		return 1;
	}

	// do stuff on Dialog Response
	if(response){
		if(listitem == 0 || listitem == 65){
			// go to next page
			if(page > 2) page = 0;
			else page++;
			// make up the params string for next dialogue
			new string[3] = {1, 1, 0};
			string[0] = page  + 48; // char play
			string[1] = color + 48;
			showDialog(vehicle_color, playerid, vehstatid, string);
		}
		else{
			// set the colour
			listitem -= 1;
			if(page >= 1) listitem += 64;
			if(page >= 2) listitem += 64;
			if(page >= 3) listitem += 64;
			if(color == 1)		vehicle_color_set(vehstatid, listitem, -2);
			else if(color == 2)	vehicle_color_set(vehstatid, -2, listitem);

			// call next dialogue
			if(color == 1){
				new string[3] = {'0', '2', 0}; // go to page 0, colour 2.
				showDialog(vehicle_color, playerid, vehstatid, string);
			}
			else{
				// show the vehicle options dialogue
				showDialog(vehicle_options, playerid, vehstatid, "");
			}
		}
	}
	else{
		// show the vehicle options dialogue
		showDialog(vehicle_options, playerid, vehstatid, "");
	}

	return 1;
}

