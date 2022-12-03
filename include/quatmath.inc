/*
 *
 * Quaternion Converter
 * Allows simple conversion from quaternion angles (supplied by new SA-MP 0.3b functions) to yaw/pitch/roll
 *
 * NAME         Quaternion Converter
 * AUTHOR		ev0lution
 * FILE         quaternion.inc
 * VERSION      1.0.0
 * LICENSE      GNU General Public License (see below)
 * COPYRIGHT    Copyright © 2010 ev0lution
 *
 *	This file is part of Quaternion Converter.
 *
 * 	Quaternion Converter is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * 	Quaternion Converter is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Quaternion Converter.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

stock QuaternionToYawPitchRoll(Float:quat_w,Float:quat_x,Float:quat_y,Float:quat_z,&Float:x,&Float:y,&Float:z) {
	x = atan2(2*((quat_x*quat_y)+(quat_w+quat_z)),(quat_w*quat_w)+(quat_x*quat_x)-(quat_y*quat_y)-(quat_z*quat_z));
	y = atan2(2*((quat_y*quat_z)+(quat_w*quat_x)),(quat_w*quat_w)-(quat_x*quat_x)-(quat_y*quat_y)+(quat_z*quat_z));
	z = asin(-2*((quat_x*quat_z)+(quat_w*quat_y)));
	return 1;
}

stock QuaternionGetRoll(Float:quat_w,Float:quat_x,Float:quat_y,Float:quat_z,&Float:roll) {
	roll = atan2(2*((quat_x*quat_y)+(quat_w+quat_z)),(quat_w*quat_w)+(quat_x*quat_x)-(quat_y*quat_y)-(quat_z*quat_z));
	return 1;
}

stock QuaternionGetPitch(Float:quat_w,Float:quat_x,Float:quat_y,Float:quat_z,&Float:pitch) {
	pitch = atan2(2*((quat_y*quat_z)+(quat_w*quat_x)),(quat_w*quat_w)-(quat_x*quat_x)-(quat_y*quat_y)+(quat_z*quat_z));
	return 1;
}

stock QuaternionGetYaw(Float:quat_w,Float:quat_x,Float:quat_y,Float:quat_z,&Float:yaw) {
	yaw = asin(-2*((quat_x*quat_z)+(quat_w*quat_y)));
	return 1;
}



// =============================================================================
// =============================================================================
// =============================================================================
// code by DANGER1979, edited by Don Walt
stock ConvertNonNormaQuatToEuler(Float: qw, Float: qx, Float:qy, Float:qz, &Float:heading, &Float:attitude, &Float:bank)
{
    new Float: sqw = qw*qw;
    new Float: sqx = qx*qx;
    new Float: sqy = qy*qy;
    new Float: sqz = qz*qz;
    new Float: unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    //åñëè normalised, - îäèí, â ïðîòèâíîì ñëó÷àå - ïîêàçàòåëü êîððåêöèè
    new Float: test = qx*qy + qz*qw;
    if (test > 0.499*unit)
    { // singularity at north pole - îñîáåííîñòü íà ñåâåðíîì ïîëþñå
        heading = 2*atan2(qx,qw);
        bank = 3.141592653/2;
        attitude = 0;
        return 1;
    }
    if (test < -0.499*unit)
    { // singularity at south pole - îñîáåííîñòü íà þæíîì ïîëþñå
        heading = -2*atan2(qx,qw);
        bank = -3.141592653/2;
        attitude = 0;
        return 1;
    }
    heading = atan2(2*qy*qw - 2*qx*qz, sqx - sqy - sqz + sqw);
    bank = asin(2*test/unit);
    attitude = atan2(2*qx*qw - 2*qy*qz, -sqx + sqy - sqz + sqw);
    return 1;
}

stock GetVehicleRotation(vehicleid, &Float:heading, &Float:attitude, &Float:bank)
{
    new Float:quat_w, Float:quat_x, Float:quat_y, Float:quat_z;
    GetVehicleRotationQuat(vehicleid, quat_w, quat_x, quat_y, quat_z);
    ConvertNonNormaQuatToEuler(quat_w,quat_x, quat_z, quat_y, heading, attitude, bank);

    if(heading<0) heading += 360; // heading be from 0 to 360degrees clockwise
	attitude *= -1; // attitude be + for climb, - for descend, 0 for straight and level
	bank *= -1; // bank be + for roll to right, - for roll to left
    return 1;
}

// code by Don Walt
// get the velocity and altitude for a vehicle
stock getVehicleVelocity(vehicleid, &Float:actual, &Float:ground, &Float:heading, &Float:attitude, &Float:altitude, unitSpeed, unitAngle, unitAltitude)
{
	// units;  speed: kmh/mph,  angle: mils/degrees,  altitude: meters/feet (0/1 respectively)
	
	// get velocity in KM/H
	// ground/actual speed
	new Float:speed_x;
	new Float:speed_y;
	new Float:speed_z;
	GetVehicleVelocity(vehicleid, speed_x, speed_y, speed_z); // get the velocity for x y and z
	actual = floatsqroot( (speed_x*speed_x) + (speed_y*speed_y) + (speed_z*speed_z) ) * 180.00; // get the actual speed
	ground = floatsqroot( (speed_x*speed_x) + (speed_y*speed_y) ) * 180.00; // get the ground speed
	// 175 KM/H In LV Police Car at max speed
	
	
	// get the angles in degrees
	//new Float:heading;
	//new Float:attitude;
	new Float:bank;
	new Float:quat_w, Float:quat_x, Float:quat_y, Float:quat_z;
	
    GetVehicleRotationQuat(vehicleid, quat_w, quat_x, quat_y, quat_z);
    ConvertNonNormaQuatToEuler(quat_w,quat_x, quat_z, quat_y, heading, attitude, bank);

    if(heading<0) heading += 360; // heading be from 0 to 360degrees clockwise
	attitude *= -1; // attitude be + for climb, - for descend, 0 for straight and level
	bank *= -1; // bank be + for roll to right, - for roll to left
	
	
	// get the altitude in meters (samp uses meters as default unit)
	new Float:pos_x;
	new Float:pos_y;
	//new Float:pos_z;
	GetVehiclePos(vehicleid, pos_x, pos_y, altitude);
	
	
	// convert to the desired units as specified in "unitSpeed, unitAngle, unitAltitude"
	if(unitSpeed == 1){
		actual *= 0.621371; // km/h to mph scale factor
		ground *= 0.621371;
	}
	if(unitAngle == 0){
		heading *= 6400/360; // degrees to mils scale factor
		attitude *= 6400/360;
	}
	if(unitAltitude == 1){
		altitude *= 3.28084; // m to ft scale factor
	}
}



// =============================================================================
// =============================================================================
// =============================================================================
// code by JernejL
// transforms a vector using a 4x4 matrix (translation and scaling is ignored, only 3x3 part is used, we only need rotation!)
stock MatrixTransformVector(Float:vector[3], Float:m[4][4], &Float:resx, &Float:resy, &Float:resz) {
	resz = vector[2] * m[0][0] + vector[1] * m[0][1] + vector[0] * m[0][2] + m[0][3];
	resy = vector[2] * m[1][0] + vector[1] * m[1][1] + vector[0] * m[1][2] + m[1][3];
	resx = -(vector[2] * m[2][0] + vector[1] * m[2][1] + vector[0] * m[2][2] + m[2][3]); // don't ask why -x was needed, i don't know either.
}

// something for 0.3b
stock RotatePointVehicleRotation(vehid, Float:Invector[3], &Float:resx, &Float:resy, &Float:resz, worldspace=0) {

	new Float:Quaternion[4];
	new Float:transformationmatrix[4][4];

	GetVehicleRotationQuat(vehid, Quaternion[0], Quaternion[1], Quaternion[2], Quaternion[3]);

	// build a transformation matrix out of the quaternion
	new Float:xx = Quaternion[0] * Quaternion[0];
	new Float:xy = Quaternion[0] * Quaternion[1];
	new Float:xz = Quaternion[0] * Quaternion[2];
	new Float:xw = Quaternion[0] * Quaternion[3];
	new Float:yy = Quaternion[1] * Quaternion[1];
	new Float:yz = Quaternion[1] * Quaternion[2];
	new Float:yw = Quaternion[1] * Quaternion[3];
	new Float:zz = Quaternion[2] * Quaternion[2];
	new Float:zw = Quaternion[2] * Quaternion[3];

	transformationmatrix[0][0] = 1 - 2 * ( yy + zz );
	transformationmatrix[0][1] =     2 * ( xy - zw );
	transformationmatrix[0][2] =     2 * ( xz + yw );
	transformationmatrix[0][3] = 0.0;

	transformationmatrix[1][0] =     2 * ( xy + zw );
	transformationmatrix[1][1] = 1 - 2 * ( xx + zz );
	transformationmatrix[1][2] =     2 * ( yz - xw );
	transformationmatrix[1][3] = 0.0;

	transformationmatrix[2][0] =     2 * ( xz - yw );
	transformationmatrix[2][1] =     2 * ( yz + xw );
	transformationmatrix[2][2] = 1 - 2 * ( xx + yy );
	transformationmatrix[2][3] = 0;

	transformationmatrix[3][0] = 0;
	transformationmatrix[3][1] = 0;
	transformationmatrix[3][2] = 0;
	transformationmatrix[3][3] = 1;

	// transform the point thru car's rotation
	MatrixTransformVector(Invector, transformationmatrix, resx, resy, resz);

	// if worldspace is set it'll return the coords in global space - useful to check tire coords against tire spike proximity directly, etc..

	if (worldspace == 1) {
		new Float:fX, Float:fY, Float:fZ;
		GetVehiclePos(vehid, fX, fY, fZ);
		resx += fX;
		resy += fY;
		resz += fZ;
	}
}
