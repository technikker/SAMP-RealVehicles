//
// simplified td display
//

new Text: vehStatsDisplayProf_txt[5];
new PlayerText: vehStatsDisplayProf_ptxt[MAX_PLAYERS][11];

// update the player textdraws with the information for the passed vehicleid
stock vehStatsDisplayProf_update()
{
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++){
		if(IsPlayerConnected(playerid) && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
			new vehicleid = GetPlayerVehicleID(playerid);
			new vehstatid = vehicleLink[vehicleid];//vehicleLink[vehicleid];

			new string[32]; // [32] something small will sufice, but for testing use 256

			//							  vehicle model				   actual	  ground		heading	pitch	  altitude		 damage		  fuel			consumption 	type		total distance (for vehid)
			//format(string,sizeof(string),"Police Cruiser LVPD  Velocity gnd 500km/h act 600km/h head 6400mils ptc 6400mils Alt 50000ft   Damage 000%%   Fuel 500.0L 000.0%% 50.0L/100km Aviation Fuel   Distance 5000000km"); // we measure heading in mils
			//format(string,sizeof(string),"XXXXXXXXX Police Cruiser LVPD	 Velocity 600km/h 6400mils	 Alt 50000ft	 Damage 000%%	 Fuel 500.0L 000.0%% 500km Aviation Fuel					  Distance 5000000km");
			//format(string,sizeof(string),"XXXXXXXXX Police Cruiser LVPD	 Speed 600km/h	 Alt 50000ft	 Damage 000%%	 Fuel 500.0L Aviation Fuel												  Distance 5000000km");

			// number plate
			format(string,sizeof(string), "%s", vehicleStat[vehstatid][vName]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][0], string);

			// model name
			format(string,sizeof(string), "%s", vehicleNames[GetVehicleModel(vehicleid) - 400]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][1], string);

			// actual speed
			format(string,sizeof(string), "%.0fkm/h", vehicleStat[vehstatid][vSpeedActual]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][2], string);//

			// heading
			format(string,sizeof(string), "%.0fmils", vehicleStat[vehstatid][vHeading]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][3], string);//

			// altitude
			format(string,sizeof(string), "%.0fft", vehicleStat[vehstatid][vAltitude]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][4], string);//

			// damage
			format(string,sizeof(string), "%.0f%%", vehicleStat[vehstatid][vDamage]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][5], string);//

			// fuel litres
			format(string,sizeof(string), "%.1fL", vehicleStat[vehstatid][vFuel]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][6], string);//

			// fuel percent
			new Float: fuelPercent = vehicleStat[vehstatid][vFuel] / vehicleFuelTankCapacity(GetVehicleModel(vehicleid)) * 100;
			format(string,sizeof(string), "%.1f%%", fuelPercent);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][7], string);//

			// distance remaining
			format(string,sizeof(string), "%.0fkm", playerStat[playerid][pDistanceRemaining]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][8], string);

			// fuel type
			format(string,sizeof(string), "%s", fuelTypeString(vehicleStat[vehstatid][vFuelTypeInTank]) );
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][9], string);

			// distance
			format(string,sizeof(string), "%.0fkm", vehicleStat[vehstatid][vTotalDistance]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayProf_ptxt[playerid][10], string);  //
		}
	}

	return;
}

stock vehStatsDisplayProf_show(playerid)
{
	for(new i=0; i<5; i++){
		TextDrawShowForPlayer(playerid, vehStatsDisplayProf_txt[i]);
	}

	for(new i=0; i<11; i++){
		PlayerTextDrawShow(playerid, vehStatsDisplayProf_ptxt[playerid][i]);
	}

	return;
}

stock vehStatsDisplayProf_hide(playerid)
{
	for(new i=0; i<5; i++){
		TextDrawHideForPlayer(playerid, vehStatsDisplayProf_txt[i]);
	}

	for(new i=0; i<11; i++){
		PlayerTextDrawHide(playerid, vehStatsDisplayProf_ptxt[playerid][i]);
	}

	return;
}

stock vehStatsDisplayProf_init()
{
	// static textdraws
	// velocity
	vehStatsDisplayProf_txt[0] = TextDrawCreate(124.000000, 439.000000, "Velocity");
	TextDrawAlignment(vehStatsDisplayProf_txt[0], 1);
	TextDrawColor(vehStatsDisplayProf_txt[0], 0xFFFE80FF);

	// altitude
	vehStatsDisplayProf_txt[1] = TextDrawCreate(231.000000, 439.000000, "Alt");
	TextDrawAlignment(vehStatsDisplayProf_txt[1], 1);
	TextDrawColor(vehStatsDisplayProf_txt[1], 0xFFFE80FF);

	// damage
	vehStatsDisplayProf_txt[2] = TextDrawCreate(286.000000, 439.000000, "Damage");
	TextDrawAlignment(vehStatsDisplayProf_txt[2], 1);
	TextDrawColor(vehStatsDisplayProf_txt[2], 0xFFFE80FF);

	// fuel
	vehStatsDisplayProf_txt[3] = TextDrawCreate(351.000000, 439.000000, "Fuel");
	TextDrawAlignment(vehStatsDisplayProf_txt[3], 1);
	TextDrawColor(vehStatsDisplayProf_txt[3], 0xFFFE80FF);

	// distance
	vehStatsDisplayProf_txt[4] = TextDrawCreate(557.000000, 439.000000, "Distance");
	TextDrawAlignment(vehStatsDisplayProf_txt[4], 1);
	TextDrawColor(vehStatsDisplayProf_txt[4], 0xFFFE80FF);

	// static common
	for(new i=0; i<5; i++){
		TextDrawBackgroundColor(vehStatsDisplayProf_txt[i], 0x00000077);
		TextDrawFont(vehStatsDisplayProf_txt[i], 1);
		TextDrawLetterSize(vehStatsDisplayProf_txt[i], 0.200000, 0.700000);
		TextDrawSetOutline(vehStatsDisplayProf_txt[i], 0);
		TextDrawSetProportional(vehStatsDisplayProf_txt[i], 1);
		TextDrawSetShadow(vehStatsDisplayProf_txt[i], 1);
	}

	return;
}

// create the textdraws
stock vehStatsDisplayProf_init_player(playerid)
{
	// player textdraws
	// number plate
	vehStatsDisplayProf_ptxt[playerid][0] = CreatePlayerTextDraw(playerid, 2.000000, 439.000000, "XXXXXXXXX"); // 2,470	 Police Cruiser LVPD	 Velocity 500km/h 6400mils	 Damage 000%	 Fuel 000.0%
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][0], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][0], 0x88ED77FF); // 0x7BF0F0FF
	// display a box for the vehicle info
	PlayerTextDrawUseBox(playerid, vehStatsDisplayProf_ptxt[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, vehStatsDisplayProf_ptxt[playerid][0], 0x40404080);
	PlayerTextDrawTextSize(playerid, vehStatsDisplayProf_ptxt[playerid][0], 638.00, 7.00);

	// model name
	vehStatsDisplayProf_ptxt[playerid][1] = CreatePlayerTextDraw(playerid, 46.000000, 439.000000, "Cruiser"); // 2,470
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][1], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][1], 0x88ED77FF);

	// velocity
	vehStatsDisplayProf_ptxt[playerid][2] = CreatePlayerTextDraw(playerid, 182.000000, 439.000000, "600km/h");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][2], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][2], -1);

	// heading
	vehStatsDisplayProf_ptxt[playerid][3] = CreatePlayerTextDraw(playerid, 215.000000, 439.000000, "6400mils");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][3], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][3], -1);

	// altitude
	vehStatsDisplayProf_ptxt[playerid][4] = CreatePlayerTextDraw(playerid, 269.000000, 439.000000, "50000ft");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][4], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][4], -1);

	// damage
	vehStatsDisplayProf_ptxt[playerid][5] = CreatePlayerTextDraw(playerid, 333.000000, 439.000000, "100%");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][5], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][5], -1);

	// litres
	vehStatsDisplayProf_ptxt[playerid][6] = CreatePlayerTextDraw(playerid, 388.500000, 439.000000, "500.0L");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][6], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][6], -1);

	// percent
	vehStatsDisplayProf_ptxt[playerid][7] = CreatePlayerTextDraw(playerid, 418.000000, 439.000000, "100.0%");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][7], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][7], -1);

	// distance
	vehStatsDisplayProf_ptxt[playerid][8] = CreatePlayerTextDraw(playerid, 446.000000, 439.000000, "1000km");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][8], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][8], -1);

	// fuel type
	vehStatsDisplayProf_ptxt[playerid][9] = CreatePlayerTextDraw(playerid, 450.000000, 439.000000, "Aviation Fuel");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][9], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][9], -1);

	// distance
	vehStatsDisplayProf_ptxt[playerid][10] = CreatePlayerTextDraw(playerid, 587.000000, 439.000000, "5000000km");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayProf_ptxt[playerid][10], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayProf_ptxt[playerid][10], -1);

	// player common
	for(new i=0; i<11; i++){
		PlayerTextDrawBackgroundColor(playerid, vehStatsDisplayProf_ptxt[playerid][i], 0x00000077);
		PlayerTextDrawFont(playerid, vehStatsDisplayProf_ptxt[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, vehStatsDisplayProf_ptxt[playerid][i], 0.200000, 0.700000);
		PlayerTextDrawSetOutline(playerid, vehStatsDisplayProf_ptxt[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, vehStatsDisplayProf_ptxt[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, vehStatsDisplayProf_ptxt[playerid][i], 1);
	}

	return;
}

stock vehStatsDisplayProf_delete()
{
	for(new i=0; i<5; i++){
		TextDrawDestroy(vehStatsDisplayProf_txt[i]);
	}

	return;
}





//
// advanced text draw display
//

new Text:vehStatsDisplayAdvanced_txt[8]; // 8 static td's
new PlayerText:vehStatsDisplayAdvanced_ptxt[MAX_PLAYERS][12]; // 12 player td's

// update the player textdraws with the information for the passed vehicleid
stock vehStatsDisplayAdvanced_update()
{
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++){
		if(IsPlayerConnected(playerid) && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
			new vehicleid = GetPlayerVehicleID(playerid);
			new vehstatid = vehicleLink[vehicleid][vStatsID];//vehicleLink[vehicleid][vStatsID];

			new string[32]; // [32] something small will sufice, but for testing use 256

			//							  vehicle model				   actual	  ground		heading	pitch	  altitude		 damage		  fuel			consumption 	type		total distance (for vehid)
			//format(string,sizeof(string),"Police Cruiser LVPD  Velocity gnd 500km/h act 600km/h head 6400mils ptc 6400mils Alt 50000ft   Damage 000%%   Fuel 500.0L 000.0%% 50.0L/100km Aviation Fuel   Distance 5000000km"); // we measure heading in mils
			//format(string,sizeof(string),"XXXXXXXXX Police Cruiser LVPD	 Velocity 600km/h 6400mils	 Alt 50000ft	 Damage 000%%	 Fuel 500.0L 000.0%% 500km Aviation Fuel					  Distance 5000000km");
			//format(string,sizeof(string),"XXXXXXXXX Police Cruiser LVPD	 Speed 600km/h	 Alt 50000ft	 Damage 000%%	 Fuel 500.0L Aviation Fuel												  Distance 5000000km");

			// model name
			format(string,sizeof(string), "%s", vehicleNames[GetVehicleModel(vehicleid) - 400]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], string);

			// ground
			format(string,sizeof(string), "%.0fkm/h", vehicleStat[vehstatid][vSpeedGround]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][1], string);

			// actual speed
			format(string,sizeof(string), "%.0fkm/h", vehicleStat[vehstatid][vSpeedActual]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][2], string);

			// heading
			format(string,sizeof(string), "%.0fmils", vehicleStat[vehstatid][vHeading]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][3], string);

			// elevation
			format(string,sizeof(string), "%.0fmils", vehicleStat[vehstatid][vAttitude]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][4], string);

			// altitude
			format(string,sizeof(string), "%.0fft", vehicleStat[vehstatid][vAltitude]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][5], string);

			// damage
			format(string,sizeof(string), "%.0f%%", vehicleStat[vehstatid][vDamage]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][6], string);

			// fuel litres
			format(string,sizeof(string), "%.1fL", vehicleStat[vehstatid][vFuel]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][7], string);

			// fuel percent
			new Float: fuelPercent = vehicleStat[vehstatid][vFuel] / vehicleFuelTankCapacity(GetVehicleModel(vehicleid)) * 100;
			format(string,sizeof(string), "%.1f%%", fuelPercent);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][8], string);

			// fuel consumption
			format(string,sizeof(string),"%.1fL/100km",vehicleConsumption(vehicleid));
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][9], string);

			// fuel type
			format(string,sizeof(string), "%s", fuelTypeString(vehicleStat[vehstatid][vFuelTypeInTank]) );
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][10], string);

			// distance
			format(string,sizeof(string), "%.0fkm", vehicleStat[vehstatid][vTotalDistance]);
			PlayerTextDrawSetString(playerid, vehStatsDisplayAdvanced_ptxt[playerid][11], string);
		}
	}

	return;
}

stock vehStatsDisplayAdvanced_show(playerid)
{
	for(new i=0; i<8; i++){
		TextDrawShowForPlayer(playerid, vehStatsDisplayAdvanced_txt[i]);
	}

	for(new i=0; i<12; i++){
		PlayerTextDrawShow(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i]);
	}

	return;
}

stock vehStatsDisplayAdvanced_hide(playerid)
{
	for(new i=0; i<8; i++){
		TextDrawHideForPlayer(playerid, vehStatsDisplayAdvanced_txt[i]);
	}

	for(new i=0; i<12; i++){
		PlayerTextDrawHide(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i]);
	}

	return;
}

stock vehStatsDisplayAdvanced_init()
{
	// static
	// velocity ground
	vehStatsDisplayAdvanced_txt[0] = TextDrawCreate(70.000000, 439.000000, "Velocity gnd"); // 2,470
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[0], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[0], 0xFFFE80FF);

	// velocity actual
	vehStatsDisplayAdvanced_txt[1] = TextDrawCreate(146.000000, 439.000000, "act");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[1], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[1], 0xFFFE80FF);

	// heading
	vehStatsDisplayAdvanced_txt[2] = TextDrawCreate(192.000000, 439.000000, "head");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[2], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[2], 0xFFFE80FF);

	// elevation
	vehStatsDisplayAdvanced_txt[3] = TextDrawCreate(244.000000, 439.000000, "ptc");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[3], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[3], 0xFFFE80FF);

	// altitude
	vehStatsDisplayAdvanced_txt[4] = TextDrawCreate(289.000000, 439.000000, "Alt");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[4], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[4], 0xFFFE80FF);

	// damage
	vehStatsDisplayAdvanced_txt[5] = TextDrawCreate(338.000000, 439.000000, "Damage");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[5], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[5], 0xFFFE80FF);

	// fuel
	vehStatsDisplayAdvanced_txt[6] = TextDrawCreate(397.000000, 439.000000, "Fuel");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[6], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[6], 0xFFFE80FF);

	// total vehicle distance
	vehStatsDisplayAdvanced_txt[7] = TextDrawCreate(567.000000, 439.000000, "Distance");
	TextDrawAlignment(vehStatsDisplayAdvanced_txt[7], 1);
	TextDrawColor(vehStatsDisplayAdvanced_txt[7], 0xFFFE80FF);

	// static common
	for(new i=0; i<8; i++){
		TextDrawBackgroundColor(vehStatsDisplayAdvanced_txt[i], 0x00000077);
		TextDrawFont(vehStatsDisplayAdvanced_txt[i], 1);
		TextDrawLetterSize(vehStatsDisplayAdvanced_txt[i], 0.200000, 0.700000);
		TextDrawSetOutline(vehStatsDisplayAdvanced_txt[i], 0);
		TextDrawSetProportional(vehStatsDisplayAdvanced_txt[i], 1);
		TextDrawSetShadow(vehStatsDisplayAdvanced_txt[i], 1);
	}

	return;
}

// create the textdraws
stock vehStatsDisplayAdvanced_create(playerid)
{
	// player
	// model name
	vehStatsDisplayAdvanced_ptxt[playerid][0] = CreatePlayerTextDraw(playerid, 2.000000, 439.000000, "Cruiser"); // 2,470	 Police Cruiser LVPD	 Velocity 500km/h 6400mils	 Damage 000%	 Fuel 000.0%
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], 0x88ED77FF);
	// display a box for the vehicle info
	PlayerTextDrawUseBox(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], 0x40404080);
	PlayerTextDrawTextSize(playerid, vehStatsDisplayAdvanced_ptxt[playerid][0], 638.00, 7.00);

	// velocity ground
	vehStatsDisplayAdvanced_ptxt[playerid][1] = CreatePlayerTextDraw(playerid, 142.000000, 439.000000, "500km/h");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][1], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][1], -1);

	// velocity actual
	vehStatsDisplayAdvanced_ptxt[playerid][2] = CreatePlayerTextDraw(playerid, 188.000000, 439.000000, "600km/h");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][2], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][2], -1);

	// heading
	vehStatsDisplayAdvanced_ptxt[playerid][3] = CreatePlayerTextDraw(playerid, 239.000000, 439.000000, "6400mils");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][3], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][3], -1);

	// elevation
	vehStatsDisplayAdvanced_ptxt[playerid][4] = CreatePlayerTextDraw(playerid, 285.000000, 439.000000, "6400mils");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][4], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][4], -1);

	// altitude
	vehStatsDisplayAdvanced_ptxt[playerid][5] = CreatePlayerTextDraw(playerid, 328.000000, 439.000000, "50000ft");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][5], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][5], -1);

	// damage
	vehStatsDisplayAdvanced_ptxt[playerid][6] = CreatePlayerTextDraw(playerid, 386.000000, 439.000000, "100%");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][6], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][6], -1);

	// fuel
	vehStatsDisplayAdvanced_ptxt[playerid][7] = CreatePlayerTextDraw(playerid, 435.000000, 439.000000, "500.0L");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][7], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][7], -1);

	vehStatsDisplayAdvanced_ptxt[playerid][8] = CreatePlayerTextDraw(playerid, 465.000000, 439.000000, "100.0%");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][8], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][8], -1);

	vehStatsDisplayAdvanced_ptxt[playerid][9] = CreatePlayerTextDraw(playerid, 512.000000, 439.000000, "50.0L/100km");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][9], 3);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][9], -1);

	// fuel type
	vehStatsDisplayAdvanced_ptxt[playerid][10] = CreatePlayerTextDraw(playerid, 516.000000, 439.000000, "Petrol");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][10], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][10], -1);

	// total distance
	vehStatsDisplayAdvanced_ptxt[playerid][11] = CreatePlayerTextDraw(playerid, 597.000000, 439.000000, "5000000km");
	PlayerTextDrawAlignment(playerid, vehStatsDisplayAdvanced_ptxt[playerid][11], 1);
	PlayerTextDrawColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][11], -1);

	// player common
	for(new i=0; i<12; i++){
		PlayerTextDrawBackgroundColor(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 0x00000077);
		PlayerTextDrawFont(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 0.200000, 0.700000);
		PlayerTextDrawSetOutline(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, vehStatsDisplayAdvanced_ptxt[playerid][i], 1);
	}

	return;
}

stock vehStatsDisplayAdvanced_delete()
{
	for(new i=0; i<5; i++){
		TextDrawDestroy(vehStatsDisplayAdvanced_txt[i]);
	}

	return;
}





