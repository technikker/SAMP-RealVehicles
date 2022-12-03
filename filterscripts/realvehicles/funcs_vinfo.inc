



//------------------------------------------------------------------------------
//                              vehicle functions
//------------------------------------------------------------------------------

// vehicle related defines
#define VEHTYPE_NONE		0
#define VEHTYPE_SPORT   	1
#define VEHTYPE_COMMON  	2
#define VEHTYPE_OFFROAD 	3
#define VEHTYPE_TRUCK   	4
#define VEHTYPE_BIKE    	5
#define VEHTYPE_AIR     	6

#define VEHPROP_NONE        0
#define VEHPROP_PUSH        1
#define VEHPROP_ELECTRIC    2
#define VEHPROP_PETROL      3
#define VEHPROP_DIESEL      4
#define VEHPROP_GASTURBINE	5
#define VEHPROP_AVIATION    6

#define FUELTYPE_NONE       0
#define FUELTYPE_BATTERY    1
#define FUELTYPE_REGULAR	2
#define FUELTYPE_PREMIUM    3
#define FUELTYPE_DIESEL     4
#define FUELTYPE_AVIATION   5

#define FUELPRICE_NONE		0.0
#define FUELPRICE_BATTERY	0.5 // not yet implemented
#define FUELPRICE_REGULAR	2.5 // price (in SA dollars) per litre of fuel
#define FUELPRICE_PREMIUM	3.8
#define FUELPRICE_DIESEL	4.2
#define FUELPRICE_AVIATION	6.8

#define REFUEL_RATE			0.5 // refuel at a rate of x Litres per 0.1 seconds
#define REPAIR_RATE			0.5 // repair at a rate of x percent per 0.1 seconds
#define REFUEL_COST         1.0 // fuel price multiplyer
#define REPAIR_COST         2.0 // price (in SA dollars) per percent of repair

new const alphaDecCharacters[36] = {
'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J',
'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
};

// function converts a decimal seed into a alphadecimal number stored in a string
// plateindex is the seed, numOfChars is the number of digits to return.  Note
// that the string returned will be 1 character longer than the numOfChars,
// in order to account for a null character terminating the string
stock randomNumberPlate(plateindex, numOfChars)
{
	new const base = 36; // working in base 36
	new target[10] = ""; // target[numOfChars + 1]
	if(numOfChars>9 || numOfChars<1) return target; // we don't return alphadecimal strings with more than 9 digits, pawno can't handle it! :D

	new digits[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	new Float: decimal = float(plateindex); // convert to float

	for(new i=0; decimal>=1; i++){
		decimal = decimal / base;

		// get remainder
		new Float: remainder = decimal - floatround(decimal, floatround_floor);
		digits[i] = floatround( (remainder * base), floatround_floor);

		decimal = floatround(decimal, floatround_floor); // was ceil, but that caused it to crash
	}
	
	new digitindex = numOfChars;

	// go forward through target, and backwards through digits
	for(new i=0; i<numOfChars; i++){
		digitindex --;
		target[i] = alphaDecCharacters[digits[digitindex]];
	}

	target[numOfChars] = '\0';

	return target;
}

// -----------------------------------------------------------------------------

// a basic price for each modelid in thousands of SA dollars
new const modelidBasicPrice[212] = {
50,20,40,500,15,25,1200,900,500,45,20,76,20,34,36,30,400,1900,32,19,43,
41,23,92,10,11000,37,420,460,80,290,710,10000,820,43,50,19,800,32,31,38,1,
34,640,1000,35,65,980,5,80,50,75,65,50,80,450,90,10,30,60,500,50,5,
29,1,1,22,23,5,590,800,5,20,10,19,25,750,69,18,24,45,1,45,34,
500,40,400,750,700,45,75,20,25,75,60,60,19,900,27,19,47,1,62,61,42,
45,69,50,60,1,1,1100,260,510,440,450,31,26,19,900,40000,41,43,39,98,42,
22,23,99,36,22,22,230,37,25,38,22,5000,5000,50,37,72,23,20,100,19,43,
32,5000,18,27,29,38,950,50,46,100,100,41,39,73,69,41,6000,1,37,25,28,
9,5000,5000,5,8,45,28,21,23,50000,120,50,42,31,90,22,50,30,27,68,39,
19,50,50,50000,350,1,1000,65,65,62,73,20,100,39,46,10,10,10,10,10,32,
10,10,
};

stock getVehiclePrice(vehicleid)
{
	return getVehicleModelPrice(GetVehicleModel(vehicleid));
}

stock getVehicleModelPrice(modelid)
{
	return modelidBasicPrice[modelid-400] * VEHICLE_PRICE_MULTIPLYER;
}

// -----------------------------------------------------------------------------

// boolean value corisponds to modelid. true if that model displays a number plate that can be changed
// first value is for modelid 400, last value is for modelid 611
new const bool: modelidHasPlate[212] = {
//1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,
//1,0,1,1,0,1,0,0,1,0,0,1,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,1,
//1,0,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,
//0,1,0,1,1,1,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,1,1,
//1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,
//1,0,1,1,1,1,0,0,0,1,0,0,
true,true,true,true,true,true,false,true,true,true,true,true,true,true,true,
true,true,false,true,true,true,true,true,true,false,false,true,true,true,true,
false,true,false,true,true,false,true,true,true,true,true,false,true,true,false,
true,false,false,true,false,false,true,false,false,false,true,true,false,true,
true,false,true,true,false,false,false,true,true,false,false,true,false,false,
false,true,true,false,true,true,true,true,false,true,true,false,false,false,
false,false,true,true,true,true,false,true,true,true,false,true,true,true,false,
true,true,true,true,true,true,true,false,false,false,false,false,true,true,true,
true,true,false,false,true,false,true,true,true,true,true,false,true,false,
false,false,true,true,true,true,false,false,false,true,true,true,true,true,true,
true,true,false,true,true,true,true,false,true,true,false,false,true,true,true,
true,true,false,false,true,true,true,false,false,false,false,false,false,true,
true,true,false,true,true,true,true,true,false,false,true,true,true,true,true,
false,false,false,false,false,false,true,true,true,true,true,false,true,true,
true,true,false,false,false,true,false,false,
};



// sets a vehicles number plate, returns true if the vehicle has a number plate,
// false if it does not, or the vehicleid is invalid.  Do not set the plate to
// more than 9 characters long, or it will overflow the numberplate!
stock setVehicleNumberPlateEx(vehicleid, string[])
{
    new modelid = GetVehicleModel(vehicleid); // returns 0 if invlid vehicleid
    if(modelid == 0) return false;
	if(modelidHasPlate[modelid-400]){
	    SetVehicleNumberPlate(vehicleid, string);
	    return true;
	}
	return false;
}

// -----------------------------------------------------------------------------

stock IsTrain(vehiclemodel)
{
	switch(vehiclemodel)
	{
		case 537, 538: return 1;
	}
	return 0;
}

// -----------------------------------------------------------------------------

stock IsRestrictedVehicle(vehiclemodel)
{
	switch(vehiclemodel) {

		// no licence needed (9)
		case 457,462,471,481,509,510,568,571,572: return 1;

		// unrestricted access (54)
		case 401,404,405,410,412,418,419,421,422,424,426,436,439,445,458,463,466,467,468,474,
			 475,478,479,480,483,491,492,550,516,517,518,521,526,527,529,533,534,536,540,542,
			 543,545,547,549,551,566,567,575,576,580,585,586,589,600: return 2;

		// all seaborne (9)
		case 446,452,453,454,472,473,484,493,539: return 3;

		// high performance (38)
		case 400,402,411,415,429,434,451,461,477,489,494,495,496,500,502,503,504,505,506,507,
			 508,522,535,541,546,555,558,559,560,561,562,565,573,579,581,587,602,603: return 4;

		// all rc (5)
		case 441,464,465,501,564: return 5;

		// service vehicles (50)
		case 403,406,408,409,413,414,420,423,431,435,437,438,440,442,443,444,448,450,455,456,
			 459,482,485,486,498,499,514,515,524,525,530,531,532,552,554,556,557,574,578,582,
			 583,584,588,591,606,607,608,609,610,611: return 6;

		// all aircraft (12)
		case 417,460,469,487,488,511,512,513,519,553,577,593: return 7;

		// enforcement vehicles (14)
		case 407,416,427,428,430,490,523,528,544,596,597,598,599,601: return 8;

		// enforcement aircraft (2)
		case 497,563: return 9;

		// defence (4)
		case 432,433,470,595: return 10;

		// defence aircraft (6)
		case 425,447,476,520,548,592: return 11;

		// trains and other stuff that shouldn't be available (9)
		case 449,537,538,569,570,590,594,605,604: return 12;
	}

	return 0;
}

// -----------------------------------------------------------------------------

// returns the price per litre of fuel
stock Float: getFuelPrice(fuelType)
{
    if(fuelType == FUELTYPE_NONE) return FUELPRICE_NONE;
    if(fuelType == FUELTYPE_BATTERY) return FUELPRICE_BATTERY;
    if(fuelType == FUELTYPE_REGULAR) return FUELPRICE_REGULAR;
    if(fuelType == FUELTYPE_PREMIUM) return FUELPRICE_PREMIUM;
    if(fuelType == FUELTYPE_DIESEL) return FUELPRICE_DIESEL;
    if(fuelType == FUELTYPE_AVIATION) return FUELPRICE_AVIATION;
    return FUELPRICE_NONE;
}

// return the fuel tank capacity for a modelid
// return -1 for an unknown modelid
stock vehicleFuelTankCapacity(modelid)
{
	switch (vehicleType(modelid))
	{
        case VEHTYPE_NONE:    return 0;
		case VEHTYPE_SPORT:   return 70;
	    case VEHTYPE_COMMON:  return 60;
		case VEHTYPE_OFFROAD: return 80;
		case VEHTYPE_TRUCK:   return 130;
		case VEHTYPE_BIKE:    return 25;
		case VEHTYPE_AIR:     return 200;
	}
	return -1;
}

// return the type of propulsion for a modelid
stock vehiclePropulsionType(modelid)
{
	switch(modelid){
		case 435,450,584,591,606,607,608,610,611,590,594: return VEHPROP_NONE;

		case 481,509,510: return VEHPROP_PUSH;

		case 457,441,464,465,501,564: return VEHPROP_ELECTRIC;

		case 462,471,568,571,572,401,404,405,410,412,419,421,424,426,436,439,445,458,463,466,
		467,468,474,475,480,483,491,492,550,516,517,518,521,526,527,529,533,534,536,540,
		542,545,547,549,551,566,567,575,576,580,585,586,589,446,452,472,473,493,539,402,
		411,415,429,434,451,461,477,494,496,500,502,503,504,506,507,522,541,546,555,558,
		559,560,561,562,565,581,587,602,603,409,420,438,442,448,485,530,531,574,583,523,
		596,597,598,416,604: return VEHPROP_PETROL;

		case 418,422,478,479,543,600,453,454,484,400,489,495,505,508,535,573,579,403,406,408,
		413,414,423,431,437,440,443,444,455,456,459,482,486,498,499,514,515,524,525,532,
		552,554,556,557,578,582,588,609,407,427,428,430,490,528,544,599,601,432,433,470,
		595,605: return VEHPROP_DIESEL;

		case 417,460,469,487,488,511,512,513,519,553,577,593,497,563,425,447,476,520,548,592: return VEHPROP_AVIATION;
	}

	return VEHPROP_NONE;
}

stock getDefaultFuelType(modelid)
{
	switch(vehiclePropulsionType(modelid)){
		case VEHPROP_PUSH:     return FUELTYPE_NONE;
		case VEHPROP_ELECTRIC: return FUELTYPE_BATTERY;
		case VEHPROP_PETROL:   return FUELTYPE_REGULAR;
		case VEHPROP_DIESEL:   return FUELTYPE_DIESEL;
		case VEHPROP_AVIATION: return FUELTYPE_AVIATION;
	}
	
	return FUELTYPE_NONE;
}

stock valid_vehicleid(id)
{
	if(id<0 || id>MAX_VEHICLES || id==INVALID_VEHICLE_ID) return 0;
	return 1; // id valid
}

// check if a fuel type is compatible with a vehicle
// returns 1 for compatible and 0 for not compatible
stock checkFuelVehicle(modelid, fuelType)
{
    new propType = vehiclePropulsionType(modelid);

    // if the fueltype is safe for the engine type, return 1
    switch(propType){
        case VEHPROP_NONE: if(fuelType==FUELTYPE_NONE) return 1;

		case VEHPROP_PUSH: if(fuelType==FUELTYPE_NONE) return 1;

		case VEHPROP_ELECTRIC: if(fuelType==FUELTYPE_BATTERY) return 1;

		case VEHPROP_PETROL: if(fuelType==FUELTYPE_REGULAR || fuelType==FUELTYPE_PREMIUM) return 1;

		case VEHPROP_DIESEL: if(fuelType==FUELTYPE_DIESEL) return 1;

		case VEHPROP_GASTURBINE: if(fuelType==FUELTYPE_REGULAR || fuelType==FUELTYPE_PREMIUM || fuelType==FUELTYPE_DIESEL || fuelType==FUELTYPE_AVIATION) return 1;

		case VEHPROP_AVIATION: if(fuelType==FUELTYPE_AVIATION) return 1;
    }

    return 0; // return "not safe"
}

// return a string of the name of a fueltype_id
stock fuelTypeString(fuelType)
{
	new string[15];

	switch(fuelType){
		case FUELTYPE_NONE:     string = "No Fuel Tank";
		case FUELTYPE_BATTERY:  string = "Battery";
		case FUELTYPE_REGULAR:  string = "Regular Petrol"; // 14 chars
		case FUELTYPE_PREMIUM:  string = "Premium Petrol";
		case FUELTYPE_DIESEL:   string = "Diesel Fuel";
		case FUELTYPE_AVIATION: string = "Aviation Fuel";
	}

	return string;
}

// return the type of vehicle for a modelid
stock vehicleType(modelid)
{
	switch(modelid){
		case 402, 411, 415, 602, 429, 541, 587, 565, 494, 502, 503,
		559, 603, 506, 451, 558, 477, 560, 562: return VEHTYPE_SPORT;

		case 400, 424, 489, 495, 505, 568, 579, 599: return VEHTYPE_OFFROAD;

		case 403, 406, 407, 408, 413, 414, 416, 418, 423, 427, 428,
		431, 432, 433, 437, 440, 443, 444, 455, 456, 459, 470,
		482, 483, /*485,*/ 486, 498, 499, 508, 514, 515, 524,
		/*531, 532,*/543, 556, 557, /*572,*/573, 578, 582, 588,
		601, 609: return VEHTYPE_TRUCK;

		case 401, 404, 405, 409, 410, 412, 419, 420, 421, 422, 426,
		434, 436, 438, 439, 442, 445, 458, 466, 467, 474, 475,
		478, 479, 490, 491, 492, 496, 500, 504, 507, 516, 517,
		518, 525, 526, 527, 528, 529, 533, 534, 535, 536, 540,
		542, 545, 546, 547, 549, 551, 552, 554, 555,
		561, 566, 567, 575, 576, 580, 585, 589, 596, 597, 598,
		600, 604, 605: return VEHTYPE_COMMON;

		case 448, 457, 461, 462, 468, 471, 521, 522, 523, 530, 571,
		574, 581, 583, 586: return VEHTYPE_BIKE;

		case 592, 577, 511, 512, 593, 520, 553, 476, 519, 460, 513,
		548, 425, 417, 487, 488, 497, 563, 447, 469: return VEHTYPE_AIR;
	}

	return VEHTYPE_NONE;
}



