//==============================================================================
//
// RealVehicle 2.0.0, Release Date 11-September-2014
//
//
//
//
//			 RealVehicle include by Don_Walt "Enforcer"
//
//
//
//
// This filterscript aims to give sa-mp vehicles
// an enhanced feeling of realism, by implementing
// an advanced fuel system and by providing
// informational in game textdraws.
// v2.0.0 adds the dynamic vehicle saving system
// and numerous other functions.
//
// update v1.1.0 - added...
//
// Credits go to:
// Don_Walt "Enforcer", for most of the code
// Srdjan, for sections of code taken from "Realistic fuel system 1.0"
// Credits also go to the respective owners of the include files we use.
//
// Over 5000 lines of code :D
//
//==============================================================================



// TODO
// make a "surfing disable option" by using GetPlayerSurfingVehicleID(vehicleid); (INVALID_VEHICLE_ID if not surfing)
// make a "max surf speed" so they fall off if they go too fast


#if defined _included_realvehicles
#endinput
#endif
#define _included_realvehicles



//------------------------------------------------------------------------------
//							  system includes
//------------------------------------------------------------------------------

//#define FILTERSCRIPT
//#include <a_samp>
//#include <sscanf2>
//#include <GetVehicleColor>
//#include <YSI\y_ini>
//#include <zcmd>
#include <strlib>

#include "../include/gl_common.inc"

#include ../include/f_dialog.inc
#include ../include/functions.inc
#include ../include/gen/database.inc
#include ../include/vnames.inc
#include ../include/quatmath.inc



//------------------------------------------------------------------------------
//							 custom includes
//------------------------------------------------------------------------------

#include realvehicles/r_banker.inc
#include realvehicles/r_realvehicles.inc

#include realvehicles/generic.inc
#include realvehicles/globals.inc
#include realvehicles/funcs_vinfo.inc
#include realvehicles/funcs_vcfg.inc
#include realvehicles/funcs_sampex.inc
#include realvehicles/funcs_rv.inc
#include realvehicles/vehicleStat.inc
#include realvehicles/playerStat.inc
#include realvehicles/logging.inc
#include realvehicles/textdraws.inc
#include realvehicles/csv_loader.inc

#include realvehicles/callbacks.inc
#include realvehicles/api.inc
#include realvehicles/api_modding.inc
#include realvehicles/api_legacy.inc
#include realvehicles/dialogs.inc
#include realvehicles/commands.inc



// -----------------------------------------------------------------------------
//                       include api function forwards
// -----------------------------------------------------------------------------

forward vehicle_create(plate[], modelid, Float:park_x, Float:park_y, Float:park_z, Float:park_r, color1, color2, owner[], status);
forward vehicle_delete(name[]);
forward vehicle_params_get(vehicleid);
forward vehicle_params_set(vehstatid, engine, lights, alarm, doors, bonnet, boot, objective);
forward vehicle_params_toggle(vehstatid, engine, lights, alarm, doors, bonnet, boot, objective);
forward vehicle_params_update(vehicleid);
forward vehicle_params_set_forplayer(vehicleid, playerid, doors, objective);
forward vehicle_respawn(vehicleid);
forward vehicle_color_get(vehicleid, &color1, &color2);
forward vehicle_color_set(vehstatid, color1, color2);
forward vehicle_color_update(vehicleid);
forward vehicle_paintjob_set(vehstatid, paintjobid);
forward vehicle_paintjob_update(vehicleid);
forward vehicle_health_get(vehicleid);
forward vehicle_health_set(vehstatid, Float:health);
forward vehicle_health_update(vehicleid);
forward vehicle_trailer_get(vehicleid);
forward vehicle_trailer_attach(vehicleid);
forward vehicle_trailer_detach(vehstatid);
forward vehicle_name_set(vehstatid, newplate[]);
forward vehicle_model_get(vehicleid);
forward vehicle_model_set(vehstatid, modelid);
forward vehicle_model_update(vehicleid);
forward vehicle_repair(vehstatid);
forward vehicle_damagestatus_get(vehicleid);
forward vehicle_damagestatus_set(vehstatid, panels, doors, lights, tires);
forward vehicle_damagestatus_update(vehicleid);
forward vehicle_virtualworld_get(vehicleid);
forward vehicle_virtualworld_set(vehstatid, worldid);
forward vehicle_virtualworld_update(vehicleid);
forward vehicle_connect(name[]);
forward vehicle_disconnect(vehstatid);
forward vehicle_spawn(vehstatid);
forward vehicle_spawnEx(vehstatid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:spawn_r);
forward vehicle_despawn(vehstatid);
forward vehicle_park(vehstatid);
forward vehicle_parkEx(vehstatid, Float:park_x, Float:park_y, Float:park_z, Float:park_r);
forward vehicle_kill(vehicleid);
forward vehicle_reposition(vehicleid, Float:x, Float:y, Float:z, Float:r);
forward vehicle_fuel_set(vehstatid, Float:fuel);
forward vehicle_fueltype_set(vehstatid, fuelTypeID);
forward vehicle_maxhealth_set(vehstatid, Float:maxhealth);
forward vehicle_owner_set(vehstatid, playername[]);
forward vehicle_owner_unset(vehstatid);
forward vehicle_exists(name[]);
forward vehicle_cheatcheck(vehicleid);
forward vehicle_quarantine(vehicleid);
forward vehicleid_get(vehstatid);
forward vehstatid_get(vehicleid);
forward vehicle_component_added(vehstatid, componentid);
forward vehicle_component_add(vehstatid, componentid);
forward vehicle_component_get(vehicleid);
forward vehicle_component_update(vehicleid);
forward vehicle_component_remove(vehstatid, componentid);
forward vehicle_component_removeall(vehstatid);

// legacy support functions
forward rvCreateVehicle(modelid, Float:x, Float:y, Float:z, Float:r, color1, color2, respawn_delay);



//------------------------------------------------------------------------------
//							Don Walt "Enforcer"
//------------------------------------------------------------------------------



