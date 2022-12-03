//
// banker.inc remote include file
// include in remote script for access to banker functions
// (banker must be loaded as filterscript or gamemode
//

#if defined BANKER_INC
#endinput
#endif

#if defined R_BANKER_INC
#endinput
#else
#define R_BANKER_INC
#endif

#define GivePlayerMoney(%1,%2)	CallRemoteFunction("banker_GivePlayerMoney",	"ii", %1, %2)
#define ResetPlayerMoney(%1)	CallRemoteFunction("banker_ResetPlayerMoney",	"i",  %1)
#define GetPlayerMoney(%1)		CallRemoteFunction("banker_GetPlayerMoney",		"i",  %1)
#define GivePlayerBank(%1,%2)	CallRemoteFunction("banker_GivePlayerBank",		"ii", %1, %2)
#define ResetPlayerBank(%1)		CallRemoteFunction("banker_ResetPlayerBank",	"i",  %1)
#define GetPlayerBank(%1)		CallRemoteFunction("banker_GetPlayerBank",		"i",  %1)

