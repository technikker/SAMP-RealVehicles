//
// realvehicles.inc remote include file
// include in remote script for access to banker functions
// (realvehicles must be loaded as filterscript or gamemode
//

#if defined _included_realvehicles
#endinput
#endif

#if defined R_REALVEHICLES_INC
#endinput
#else
#define R_REALVEHICLES_INC
#endif

#define CreateVehicle(%1,%2,%3,%4,%5,%6,%7,%8,%9)		CallRemoteFunction("rvCreateVehicle",		"iffffiiii", %1, %2, %3, %4, %5, %6, %7, %8, %9)

