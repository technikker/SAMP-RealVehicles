//
// Common functions by DW
//

//------------------------------------------------------------------------------
//                              very basic operations
//------------------------------------------------------------------------------

stock ConvertToVal(stringInput[64])
{
	new endindex = 0;
	new strend[2];
	// record the end of the input
	strend = "";
    for(new index = 0; index < 64; index++) {
		if(stringInput[index] == strend[0]) {
	        endindex = index;
	        break;
		}
    }

    new bool:negativeInput = false;

	// Check if the # is negative
	strend = "-"; // reassign so we can compare it to firstChar
	if(stringInput[0] == strend[0]) {
		//printf("string val is negative");
	    negativeInput = true;
	
	    // Remove the negative sign
	    for(new index = 0; index < endindex; index++) {
	        stringInput[index] = stringInput[index + 1];
	    }
	}

    new valueOutput = 0;
    new numberPlace = 1;
    // We know how many characters there are, so convert into a value
    for(new index = endindex - 1; (index + 1)> 0; index--) {
        // Add to the final output each time the loop runs
		valueOutput = valueOutput + ((stringInput[index] - 48) * numberPlace);
		numberPlace = numberPlace * 10;

		// debugging code
		/*new executions[32];
		executions[0] = index + 48;
		printf("for loop executed");
		printf(executions);*/
    }

	if(negativeInput == true) valueOutput = valueOutput * (-1);
	return valueOutput;
}

stock ConvertToStr(valueInput)
{
    new stringOutput[64]; // The output variable

	// Find out if the value is negative
	new bool:negativeInput = false;
	if(valueInput < 0) {
		negativeInput = true;
		stringOutput = "-"; // insert the negative sign
		valueInput = valueInput * (-1); // make it positive
	}

	new numberPlace = 10;
	new numOfDigits = 0;
    
    // Find out how many digits there are...
    for(new index = 1; index < 100; index++) {
        if(valueInput < numberPlace){
			// We found the # of digits, so we quit
			numOfDigits = index;
            break;
		}
        // go up to next level
    	numberPlace = numberPlace * 10;
    }

    // This loop needs to run for every digit we want to output
    for(new outputIndex = 0; outputIndex < numOfDigits; outputIndex++) {

		// Now we work out the asc2 value of the first digit of valueInput
	    new testValue = 0;
		// work out how big the first digit is...
		for(new index = 1; index < 11; index++) {
			// alter testValue untill it is greater than valueInput,
			// to give us the size of valueInput
			//testValue = index * exponent(10,(numOfDigits - 1));
			testValue = index * exponent(10,(numOfDigits - 1 - outputIndex));
	        if(testValue > valueInput){
				// make (1st digit of) testValue hold equal place to (first digit of) valueInput
	            testValue = (index - 1) * exponent(10,(numOfDigits - 1 - outputIndex));
	            // subtract the first digit off valueInput using number calculated above
				valueInput = valueInput - testValue;

				// this is the first digit (from the left) of the valueInput
				testValue = index - 1;

	            /*/ debug code
	            printf("for loop run, index, testValue");
				new testValue3[8];
				testValue3[0] = index + 48;
				new testValue2[8];
		        testValue2[0] = testValue + 48;
		        printf(testValue3);
	        	printf(testValue2);
	        	printf("break");*/
		        break;
	        }
	    }

		// add the digit to the string
		if (negativeInput == true) stringOutput[outputIndex + 1] = testValue + 48;
		else stringOutput[outputIndex] = testValue + 48;
	}

	// return the string
    return stringOutput;
}

//------------------------------------------------------------------------------

// multiply a number using a power
stock exponent(value,power)
{
	new result = value;
	if(power == 0) return 1; // otherwise we will not return 1 when we should
	for(new index = power; index > 1; index--) {
		result = result * value;
	}
	return result;
}

//------------------------------------------------------------------------------

stock GetPlayerAccount(playerid)
{
	//
	// Code for getting a string value for the current players name and
	// the name of the file that we should read/write their data from
	//
	new playerName[64];
	GetPlayerName(playerid,playerName,64); // get the players name
	// find the end of the playername string by looking for character "null"
	new endindex = 0;
	new strend[2];
	strend = "";
	for(new index = 0; index < 64; index++) {
		if(playerName[index] == strend[0]) {
			endindex = index; // endindex now tells us where null is first encountered in the string
			break;
		}
	}
	// Now we know where the end of the string is,
	// We can write the file extension on the end
	new fileExtension[5];
	fileExtension = ".txt"; // defines what the extension is
	new playerAccount[69];
	playerAccount = playerName;
	// We have to write each index separately
	for(new index = 0; index < 4; index++) {
		playerAccount[endindex + index] = fileExtension[index]; // write the extension onto the end of the string
	}
	// strings "playername[64]" and "playerAccount[69]" now contain the current
	// player's name and their account file reference respectively.
	return playerAccount;
}

// alternative to GetPlayerAccount
//playername and playerAccount do not need to contain data when passed.
stock FindPlayerData(playerid,playername[64],playerAccount[69])
{
	//
	// Code for getting a string value for the current players name and
	// the name of the file that we should read/write their data from
	//
	//new playername[64];
	GetPlayerName(playerid,playername,64); // get the players name
	//playername = "technician";
	// find the end of the playername string by looking for character "null"
	new endindex = 0;
	new strend[2];
	strend = "";
    for(new index = 0; index < 64; index++) {
		if(playername[index] == strend[0]) {
	        endindex = index; // endindex now tells us where null is first encountered in the string
	        break;
		}
    }
    // Now we know where the end of the string is,
	// We can write the file extension on the end
	new fileExtension[5];
	fileExtension = ".txt"; // defines what the extension is
	//new playerAccount[69];
    playerAccount = playername;
	// We have to write each index separately
	for(new index = 0; index < 4; index++) {
		playerAccount[endindex + index] = fileExtension[index]; // write the extension onto the end of the string
	}
	// strings "playername[64]" and "playerAccount[69]" now contain the current
	// player's name and their account file reference respectively...

	// So we can return these values!
    return 1;
}





//------------------------------------------------------------------------------
//                              file parsing
//------------------------------------------------------------------------------

#define 					ini_SetString(%0,%1,%2)				 			if(%2[0]) fwrite(%0, %1), fputchar(%0, '=', false) && fwrite(%0, %2) && fwrite(%0, "\r\n")
#define 					ini_SetInteger(%0,%1,%2,%3)                     format(%1, sizeof(%1), "%s=%d\r\n", %2, %3) && fwrite(%0, %1)
#define 					ini_SetFloat(%0,%1,%2,%3)                     	format(%1, sizeof(%1), "%s=%.0f\r\n", %2, %3) && fwrite(%0, %1)
stock ini_GetValue(szParse[], szValueName[], szDest[], iDestLen)
{

	new
		iPos = strfind(szParse, "=", false),
		iLength = strlen(szParse);

	while(iLength-- && szParse[iLength] <= ' ')
	{
		szParse[iLength] = 0;
	}

	if(strcmp(szParse, szValueName, false, iPos) == 0)
	{
		strmid(szDest, szParse, iPos + 1, iLength + 1, iDestLen);
		return 1;
	}
	return 0;
}





//------------------------------------------------------------------------------
//                              basic operations
//------------------------------------------------------------------------------

/*Invalid characters in file and folder names

Recommendation for highest interoperability and compatibility between Windows/Linux/Unix/Mac regarding filename(spacename) conventions:

    Do not ever use the following reserved characters: <>:"\/|*?
    Do not use any of the following reserved WinNT device names (with or without trailing filename extension such as 'NUL.tx7'): 'CON' 'PRN' 'AUX' 'NUL' 'COM1' 'COM2' 'COM3' 'COM4' 'COM5' 'COM6' 'COM7' 'COM8' 'COM9' 'LPT1' 'LPT2' 'LPT3' 'LPT4' 'LPT5' 'LPT6' 'LPT7' 'LPT8' 'LPT9' 'CLOCK$' .
    Do not use non printable characters even if the underlying filesystem is capable to handle unicode filenames.
    Avoid using these charcters: []=%$+,;
    Do not start a filename with a whitespace, a period '.' (interpreted as hidden in unix like systems) or with an minus sign '-' (interpreted as a commandline argument in unix like systems).
    Do not use "~" as a file or folder name, because it is interpreted as the "home"-directory under linux systems.
    Do not end a filename with a whitespace or a period '.' (interpreted as hidden in unix like systems).
    Do not use sequences of periods '.' in the middle of filenames.
    Avoid whitespaces in the middle of filenames.
    Do not assume case-sensivity.
    Do not use a space at the end of a filename.*/

stock validFileNameCharacter(character)
{
	// no whitespace characters (use '_' instead)
    if(character==' '){
		return false;
	}

	// reserved characters
	if(character=='<' || character=='>' || character==':' || character=='\\' || character=='/' || character=='|' || character=='*' || character=='?'){
		return false;
	}
	
	// avoid these characters
	//if(character=='[' || character==']' || character=='=' || character=='%' || character=='$' || character=='+' || character==',' || character==';'){
	//	return false;
	//}
	
	// no '~' or '"'
	if(character=='~' || character=='\"'){
		return false;
	}
	
	return true;
}

// -----------------------------------------------------------------------------

// toggles a bool variable
/*stock toggle(&boolInput)
{
	if(boolInput) boolInput = false;
	else boolInput = true;
	return;
}*/

// -----------------------------------------------------------------------------

// gets a random number
stock randnumb(min, max)
{
	return random (max - min) + min;
}

// -----------------------------------------------------------------------------

// compares two strings, and returns true if they match
stock bool: stringmatch(string1[],string2[])
{
	for(new i=0; string1[i] == string2[i]; i++){
		if(!string1[i+1] && !string2[i+1]) return true;
	}
	return false;
}

// -----------------------------------------------------------------------------

// simple check to see if two strings match each other completely
stock strmatch(const string1[], const string2[])
{
	for(new i=0; string1[i] || string2[i]; i++){
		if( (string1[i] != string2[i]) ) return 0;
	}
	return 1;
}
// copy source string into the destination string
stock strcopy(dest[], dest_size, const source[])
{
	new idx;
	for(idx=0; source[idx] && idx<dest_size; idx++){
		dest[idx] = source[idx];
	}
	dest[idx] = '\0'; // tack on a null
	return 1;
}

// -----------------------------------------------------------------------------

// check to see if a player with the passed name is online
// return their playerid if so, else return invalid_player_id
stock getPlayerID(playerName[])
{
	if(isnull(playerName)) return INVALID_PLAYER_ID;
	for(new playerid; playerid<MAX_PLAYERS; playerid++){
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);
		if(strmatch(playerName, name)) return playerid;
	}
	return INVALID_PLAYER_ID;
}

// -----------------------------------------------------------------------------

// get a players name, and return it as a string
stock getPlayerName(playerid)
{
	new playerName[MAX_PLAYER_NAME];
	GetPlayerName(playerid, playerName, MAX_PLAYER_NAME);
	return playerName;
}






// no error checking, make sure id's are valid!
stock Float:playerToPlayerDistance(playerid, targetid)
{
	new Float: playerX;
	new Float: playerY;
	new Float: playerZ;
	GetPlayerPos(playerid, playerX, playerY, playerZ);
	return GetPlayerDistanceFromPoint(targetid, playerX, playerY, playerZ);
}

stock getClosestPlayer(playerid)
{
	new Float: playerX;
	new Float: playerY;
	new Float: playerZ;
	new Float: distance;
	new Float: leastDistance = 100000.00;
	GetPlayerPos(playerid, playerX, playerY, playerZ);

	new targetid = INVALID_PLAYER_ID;
	for(new i=0; i<MAX_PLAYERS; i++){
	    if(IsPlayerConnected(i) && playerid!=i){
			distance = GetPlayerDistanceFromPoint(i, playerX, playerY, playerZ);
			
			if(distance < leastDistance){
			    leastDistance = distance;
			    targetid = i;
			}
	    }
	}
	return targetid;
}

stock getClosestPlayer_cuffed(playerid)
{
	new Float: playerX;
	new Float: playerY;
	new Float: playerZ;
	new Float: distance;
	new Float: leastDistance = 100000.00;
	GetPlayerPos(playerid, playerX, playerY, playerZ);

	new targetid = INVALID_PLAYER_ID;
	for(new i=0; i<MAX_PLAYERS; i++){
	    if(IsPlayerConnected(i) && playerid!=i){
			distance = GetPlayerDistanceFromPoint(i, playerX, playerY, playerZ);
			
			if(distance < leastDistance){
			    leastDistance = distance;
			    targetid = i;
			}
	    }
	}
	return targetid;
}

// gets the 20 closest players to the playerid
stock getClosestPlayers(playerid)
{
	new closestPlayers[20] = INVALID_PLAYER_ID;

	// get the players pos
    new Float: playerX, Float: playerY, Float: playerZ, Float: distance;
	new Float: leastDistance = 100000.00;
	GetPlayerPos(playerid, playerX, playerY, playerZ);

	for(new idx=0; idx<sizeof(closestPlayers); idx++){
		new targetid = INVALID_PLAYER_ID;

		// loop to find closest player not on the list
		for(new i=0; i<MAX_PLAYERS; i++){ // check every player online
		    if(IsPlayerConnected(i) && playerid!=i && !inList20(i, closestPlayers)){ // if they are online, not the player, and not on the list
				distance = GetPlayerDistanceFromPoint(i, playerX, playerY, playerZ);
				if(distance < leastDistance){
					leastDistance = distance;
				    targetid = i;
				}
		    }
		}
	    if(targetid == INVALID_PLAYER_ID) break; // if no players were found
	    closestPlayers[idx] = targetid;
	}

	return closestPlayers;
}
// gets the 20 closest cuffed players to the playerid
stock getClosestPlayers_cuffed(playerid)
{
	new closestPlayers[10] = INVALID_PLAYER_ID;

	// get the players pos
    new Float: playerX, Float: playerY, Float: playerZ, Float: distance;
	new Float: leastDistance = 100000.00;
	GetPlayerPos(playerid, playerX, playerY, playerZ);

	for(new idx=0; idx<sizeof(closestPlayers); idx++){
		new targetid = INVALID_PLAYER_ID;

		// loop to find closest player not on the list
		for(new i=0; i<MAX_PLAYERS; i++){ // check every player online
			// if they are online, not the player, and not on the list and are cuffed
		    if(IsPlayerConnected(i) && playerid!=i && GetPlayerSpecialAction(i)==SPECIAL_ACTION_CUFFED && !inList10(i, closestPlayers)){
				distance = GetPlayerDistanceFromPoint(i, playerX, playerY, playerZ);
				if(distance < leastDistance){
					leastDistance = distance;
				    targetid = i;
				}
		    }
		}
	    if(targetid == INVALID_PLAYER_ID) break; // if no players were found
	    closestPlayers[idx] = targetid;
	}

	return closestPlayers;
}
stock inList20(id, list[20])
{
	for(new i=0; i<sizeof(list); i++){
	    if(list[i] == id) return 1;
	}
	return 0;
}
stock inList10(id, list[10])
{
	for(new i=0; i<sizeof(list); i++){
	    if(list[i] == id) return 1;
	}
	return 0;
}

stock removeBlank(string[])
{
	for(new i=0; string[i]; i++){ // traverse the array
	    if(string[i]==' ' || string[i]=='\t'){ // check for black characters
	        for(new idx=i+1; string[idx]; idx++){
	            if(string[idx]!=' ' && string[idx]!='\t'){ // look for the next non-blank
					string[i] = string[idx]; // move it to where the blank character was
					string[idx] = ' ';       //
					break; // break
	            }
	        }
	    }
	}
	//find the end, and null it
	for(new i=0; string[i]; i++){
	    if(string[i]==' ' || string[i]=='\t'){
			string[i] = '\0';
			break;
	    }
	}

	return;
}