//
// =============================================================================
//                                   globals.inc
// =============================================================================
//

// -----------------------------------------------------------------------------
//                                    defines
// -----------------------------------------------------------------------------

#if defined INVALID_VEHICLE_ID
#else
#define INVALID_VEHICLE_ID				65535
#endif

// works on ints also
#define toggle(%0)          if(%0) %0 = false; else %0 = true

#define MAX_FILE_NAME					256

#define MAX_PLAYER_VEHICLES				10

#define MAX_VEHICLE_NAME				10 // 9 + null
#define MAX_VEHICLE_PLATE				33 // 32 + null

// dcmd
//#define dcmd(%1,%2,%3) if (!strcmp((%3)[1], #%1, true, (%2)) && ((((%3)[(%2) + 1] == '\0') && (dcmd_%1(playerid, ""))) || (((%3)[(%2) + 1] == ' ') && (dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1


#define DATABASE_ENABLE_INI
//#define DATABASE_ENABLE_SQL

#define DATABASE_INI 		0
#define DATABASE_SQL		1
new primary_database = 		0;


#define NO_SIREN						1

#define VEHICLE_PRICE_MULTIPLYER		20000

#define PLAYER_VEHICLES_NAMES_DELIM		':'


#define VEHPARAM_ENGINE					1
#define VEHPARAM_LIGHTS					2
#define VEHPARAM_ALARM					4
#define VEHPARAM_LOCK					8
#define VEHPARAM_BONNET					16
#define VEHPARAM_BOOT					32
#define VEHPARAM_OBJ					64

#define VEHSTATUS_SIREN					1
#define VEHSTATUS_1						2 // UNUSED
#define VEHSTATUS_2						4 // UNUSED
#define VEHSTATUS_3						8 // UNUSED
#define VEHSTATUS_GUEST					16
#define VEHSTATUS_ONSALE				32


#define MAX_FACTION_NAME				25

#define PLAYER_STATS_LOCATION			"players/database/"

#define VEHICLE_STATS_LOCATION			"vehicles/database/"
#define VEHICLE_LOGS_LOCATION			"vehicles/logs/"
#define VEHICLE_LOG_NAME				"vehiclelog.txt"
#define INVALID_VEHSTAT_ID				-1

#define VEHICLE_RESPAWN_DELAY			-1			// don't ever respawn unless we tell you to
#define VEHICLE_RESPAWN					-1
#define VEHICLE_STARTUP_DELAY 			0			// wait 10 secs after init to load the vehicles

#define MAX_FILENAME					256

#define COLOR_DEAD						0xAFAFAFAA
#define COLOR_GREEN						0x33AA33AA
#define COLOR_LIGHTGREEN				0x9ACD32AA
#define COLOR_DEATH						0xAA3333AA
#define COLOR_LIGHTBLUE					0x33CCFFAA
#define COLOR_YELLOW					0xFFFF00AA
#define COLOR_WHITE						0xFFFFFFAA
#define COLOR_ERROR						0xFF0000FF
#define COLOR_PURPLE					0xC2A2DAAA
#define COLOR_SERVER_MAIN_MSG			0xFFFFFFAA
#define COLOR_SERVER_HELP_MSG			0x33CCFFAA
#define COLOR_PRIVATE_MSG				0xFFFF00AA
#define COLOR_GROUP						0xDAA520AA
#define COLOR_ADMIN						0xFF40FFFF
#define COLOR_PINK						0xFF40FFFF

#define COLOR_DISPATCH					0x456EAF67
#define COLOR_TEAM_LAW					0x000047B2AA
#define COLOR_TEAM_DRIVER				0x00B800AA



//------------------------------------------------------------------------------
//							 global variables
//------------------------------------------------------------------------------



// list of all the vehicles spawned, and their vehstatid
new vehicleLink[MAX_VEHICLES];

stock vehicleLink_clear(vehicleid)
{
	vehicleLink[vehicleid] = INVALID_VEHICLE_ID;
}

stock vehicleLink_init()
{
	for(new vehicleid=0; vehicleid<MAX_VEHICLES; vehicleid++){
		vehicleLink_clear(vehicleid);
	}
	return;
}

//#define get_vehicleid(%1) 
//#define get_vehstatid(%1)






enum coordinate
{
	Float: X,
	Float: Y,
	Float: Z,
}

new const fuelStations[][coordinate] =
{
	{ 1595.5406, 2198.0520, 10.3863 },
	{ 2202.0649, 2472.6697, 10.5677 },
	{ 2115.1929, 919.9908, 10.5266 },
	{ 2640.7209, 1105.9565, 10.5274 },
	{ 608.5971, 1699.6238, 6.9922 },
	{ 618.4878, 1684.5792, 6.9922 },
	{ 2146.3467, 2748.2893, 10.5245 },
	{ -1679.4595, 412.5129, 6.9973 },
	{ -1327.5607, 2677.4316, 49.8093 },
	{ -1470.0050, 1863.2375, 32.3521 },
	{ -2409.2200, 976.2798, 45.2969 },
	{ -2244.1396, -2560.5833, 31.9219 },
	{ -1606.0544, -2714.3083, 48.5335 },
	{ 1937.4293, -1773.1865, 13.3828 },
	{ -91.3854, -1169.9175, 2.4213 },
	{ 1383.4221, 462.5385, 20.1506 },
	{ 660.4590, -565.0394, 16.3359 },
	{ 1381.7206, 459.1907, 20.3452 },
	{ -1605.7156, -2714.4573, 48.5335 },
	{ 1008.1107, -937.3372, 42.6493 },
	{ -2029.4047, 157.2388, 28.5722 }
};

new Float: distanceAtStart[MAX_PLAYERS];



// -----------------------------------------------------------------------------
//                                vehicle stats
// -----------------------------------------------------------------------------

// TODO
// any function that will set any of the following vars needs to check the new value is in range!
// vColor1,
// vColor2,
// vPaintJob,
// vMod00 - vMod16
// vWorldID, (0 to 2,147,483,647)
// vHealth, 0==explode, 250==catch fire, >=750==fine
// vFuel
// vFuelTypeInTank

enum vStats
{
	vVehicleID, // the samp vehicleid, if the vehicle is spawned

	// vehicle stats (are only changed when certain events happen)
	// information about the vehicle, name, colour, and mods

	// info
	vName[MAX_VEHICLE_NAME],
	vPlate[MAX_VEHICLE_PLATE], // includes colour codes (for multi-colour numberplate)
	vStatus,	// guest (stats aren't saved to file on disconnect), for sale,
	vValue,		// price of vehicle if for sale
	vModel,
	vColor1,
	vColor2,
	vPaintJob,
	vMod00,
	vMod01,
	vMod02,
	vMod03,
	vMod04,
	vMod05,
	vMod06,
	vMod07,
	vMod08,
	vMod09,
	vMod10,
	vMod11,
	vMod12,
	vMod13,
	vMod14,
	vMod15,
	vMod16,
	Float: vHealthMax,						// TODO max vehicle health

	// position
	vWorldID,
	vSpawned,
	Float: vSpawnPositionX,
	Float: vSpawnPositionY,
	Float: vSpawnPositionZ,
	Float: vSpawnPositionR,
	Float: vParkPositionX,
	Float: vParkPositionY,
	Float: vParkPositionZ,
	Float: vParkPositionR,

	// statistics
	vLastDriver[MAX_PLAYER_NAME+1],
	vOwner[MAX_PLAYER_NAME+1],				// name of player that owns (set to "" for no owner)
	vKills,									// times a player in this vehicle kills a player
	vDeath,									// times this vehicle has been killed

	// realtime [stats] (kept up to date when the vehicle is engineon)
	// params
	vParams,								// Engine, Lights, Alarm, Lock, Bonnet, Boot, Obj
	Float: vHealth,							// vDamage should be set from vHealth, not the other way around
	vDamageStatus0,							// panels (only used on spawn/despawn)
	vDamageStatus1,							// doors (only used on spawn/despawn)
	vDamageStatus2,							// lights (only used on spawn/despawn)
	vDamageStatus3,							// tyres (only used on spawn/despawn)

	// extended [stats]
	Float: vTotalDistance,
	Float: vFuel,			// in litres
	vFuelTypeInTank, 		// the fuel type currently in the tank, eg aviation fuel

	// this stuff don't get saved to file
	// temp
	Float: vDamage,		 // same as vHealth, except gives the ammount of damage the vehicle has recieved as a percentage.
	Float: vSpeedActual,
	Float: vSpeedGround,
	Float: vHeading,
	Float: vAttitude,
	Float: vAltitude,
};

new vehicleStat[MAX_VEHICLES][vStats];



// -----------------------------------------------------------------------------
//                                  car mods API
// -----------------------------------------------------------------------------

// vehicle components divided into groups
// 0 - CARMODTYPE_SPOILER
new const vcomp_spoiler[20]		=  {1000, 1001, 1002, 1003, 1014, 1015, 1016, 1023, 1058, 1060, 1049, 1050, 1138, 1139, 1146, 1147, 1158, 1162, 1163, 1164};
// 1 - CARMODTYPE_HOOD
new const vcomp_bscoop[4]		=  {1004, 1005, 1011, 1012};
// 2 - CARMODTYPE_ROOF
new const vcomp_rscoop[17]		=  {1006, 1032, 1033, 1035, 1038, 1053, 1054, 1055, 1061, 1067, 1068, 1088, 1091, 1103, 1128, 1130, 1131};
// 3 - CARMODTYPE_SIDESKIRT
new const vcomp_lskirt[21]		=  {1017, 1027, 1030, 1040, 1041, 1051, 1052, 1062, 1063, 1071, 1072, 1094, 1095, 1099, 1101, 1102, 1107, 1120, 1121, 1124, 1137};
new const vcomp_rskirt[21]		=  {1007, 1026, 1031, 1036, 1039, 1042, 1047, 1048, 1056, 1057, 1069, 1070, 1090, 1093, 1106, 1108, 1118, 1119, 1133, 1122, 1134};
// 4 - CARMODTYPE_LAMPS
new const vcomp_lights[2]		=  {1013, 1024};
// 5 - CARMODTYPE_NITRO
new const vcomp_nitro[3]		=  {1008, 1009, 1010};
// 6 - CARMODTYPE_EXHAUST
new const vcomp_exhaust[28]		=  {1018, 1019, 1020, 1021, 1022, 1028, 1029, 1037, 1043, 1044, 1045, 1046, 1059, 1064, 1065, 1066, 1089, 1092, 1104, 1105, 1113, 1114, 1126, 1127, 1129, 1132, 1135, 1136};
// 7 - CARMODTYPE_WHEELS
new const vcomp_wheels[17]		=  {1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1096, 1097, 1098};
// 8 - CARMODTYPE_STEREO
new const vcomp_base[1]			=  {1086};
// 9 - CARMODTYPE_HYDRAULICS
new const vcomp_hydraulics[1]	=  {1087};
// 10 - CARMODTYPE_FRONT_BUMPER
new const vcomp_fbumper[23]		=  {1117, 1152, 1153, 1155, 1157, 1160, 1165, 1167, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1179, 1181, 1182, 1185, 1188, 1189, 1192, 1193};
new const vcomp_fbbars[2]		=  {1115, 1116};
// 11 - CARMODTYPE_REAR_BUMPER
new const vcomp_rbumper[22]		=  {1140, 1141, 1148, 1149, 1150, 1151, 1154, 1156, 1159, 1161, 1166, 1168, 1176, 1177, 1178, 1180, 1183, 1184, 1186, 1187, 1190, 1191};
new const vcomp_rbbars[4]		=  {1109, 1110, 1123, 1125};
// 12 - CARMODTYPE_VENT_RIGHT
new const vcomp_bventr[2]		=  {1143, 1145};
// 13 - CARMODTYPE_VENT_LEFT
new const vcomp_bventl[2]		=  {1142, 1144};



new const VehicleColorTableRGB[256] = {
	// The existing colours from San Andreas
	0x000000, 0xF5F5F5, 0x2A77A1, 0x840410, 0x263739, 0x86446E, 0xD78E10, 0x4C75B7, 0xBDBEC6, 0x5E7072,
	0x46597A, 0x656A79, 0x5D7E8D, 0x58595A, 0xD6DAD6, 0x9CA1A3, 0x335F3F, 0x730E1A, 0x7B0A2A, 0x9F9D94,
	0x3B4E78, 0x732E3E, 0x691E3B, 0x96918C, 0x515459, 0x3F3E45, 0xA5A9A7, 0x635C5A, 0x3D4A68, 0x979592,
	0x421F21, 0x5F272B, 0x8494AB, 0x767B7C, 0x646464, 0x5A5752, 0x252527, 0x2D3A35, 0x93A396, 0x6D7A88,
	0x221918, 0x6F675F, 0x7C1C2A, 0x5F0A15, 0x193826, 0x5D1B20, 0x9D9872, 0x7A7560, 0x989586, 0xADB0B0,
	0x848988, 0x304F45, 0x4D6268, 0x162248, 0x272F4B, 0x7D6256, 0x9EA4AB, 0x9C8D71, 0x6D1822, 0x4E6881,
	0x9C9C98, 0x917347, 0x661C26, 0x949D9F, 0xA4A7A5, 0x8E8C46, 0x341A1E, 0x6A7A8C, 0xAAAD8E, 0xAB988F,
	0x851F2E, 0x6F8297, 0x585853, 0x9AA790, 0x601A23, 0x20202C, 0xA4A096, 0xAA9D84, 0x78222B, 0x0E316D,
	0x722A3F, 0x7B715E, 0x741D28, 0x1E2E32, 0x4D322F, 0x7C1B44, 0x2E5B20, 0x395A83, 0x6D2837, 0xA7A28F,
	0xAFB1B1, 0x364155, 0x6D6C6E, 0x0F6A89, 0x204B6B, 0x2B3E57, 0x9B9F9D, 0x6C8495, 0x4D8495, 0xAE9B7F,
	0x406C8F, 0x1F253B, 0xAB9276, 0x134573, 0x96816C, 0x64686A, 0x105082, 0xA19983, 0x385694, 0x525661,
	0x7F6956, 0x8C929A, 0x596E87, 0x473532, 0x44624F, 0x730A27, 0x223457, 0x640D1B, 0xA3ADC6, 0x695853,
	0x9B8B80, 0x620B1C, 0x5B5D5E, 0x624428, 0x731827, 0x1B376D, 0xEC6AAE, 0x000000,
	// SA-MP extended colours (0.3x)
	0x177517, 0x210606, 0x125478, 0x452A0D, 0x571E1E, 0x010701, 0x25225A, 0x2C89AA, 0x8A4DBD, 0x35963A,
	0xB7B7B7, 0x464C8D, 0x84888C, 0x817867, 0x817A26, 0x6A506F, 0x583E6F, 0x8CB972, 0x824F78, 0x6D276A,
	0x1E1D13, 0x1E1306, 0x1F2518, 0x2C4531, 0x1E4C99, 0x2E5F43, 0x1E9948, 0x1E9999, 0x999976, 0x7C8499,
	0x992E1E, 0x2C1E08, 0x142407, 0x993E4D, 0x1E4C99, 0x198181, 0x1A292A, 0x16616F, 0x1B6687, 0x6C3F99,
	0x481A0E, 0x7A7399, 0x746D99, 0x53387E, 0x222407, 0x3E190C, 0x46210E, 0x991E1E, 0x8D4C8D, 0x805B80,
	0x7B3E7E, 0x3C1737, 0x733517, 0x781818, 0x83341A, 0x8E2F1C, 0x7E3E53, 0x7C6D7C, 0x020C02, 0x072407,
	0x163012, 0x16301B, 0x642B4F, 0x368452, 0x999590, 0x818D96, 0x99991E, 0x7F994C, 0x839292, 0x788222,
	0x2B3C99, 0x3A3A0B, 0x8A794E, 0x0E1F49, 0x15371C, 0x15273A, 0x375775, 0x060820, 0x071326, 0x20394B,
	0x2C5089, 0x15426C, 0x103250, 0x241663, 0x692015, 0x8C8D94, 0x516013, 0x090F02, 0x8C573A, 0x52888E,
	0x995C52, 0x99581E, 0x993A63, 0x998F4E, 0x99311E, 0x0D1842, 0x521E1E, 0x42420D, 0x4C991E, 0x082A1D,
	0x96821D, 0x197F19, 0x3B141F, 0x745217, 0x893F8D, 0x7E1A6C, 0x0B370B, 0x27450D, 0x071F24, 0x784573,
	0x8A653A, 0x732617, 0x319490, 0x56941D, 0x59163D, 0x1B8A2F, 0x38160B, 0x041804, 0x355D8E, 0x2E3F5B,
	0x561A28, 0x4E0E27, 0x706C67, 0x3B3E42, 0x2E2D33, 0x7B7E7D, 0x4A4442, 0x28344E
};

stock GetVehicleColorRGB(colorid)
{
	if(colorid < 0 || colorid >= sizeof(VehicleColorTableRGB)) return 0;
	return VehicleColorTableRGB[colorid];
}










