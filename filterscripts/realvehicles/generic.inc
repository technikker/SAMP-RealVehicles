// generic functions that could be used in other scripts

//------------------------------------------------------------------------------

// remove all '~' [Tilde] characters from a string
stock removeTilde(string[])
{
	for(new i=0; string[i]; i++){ // traverse the array
		if(string[i]=='~'){ // check for black characters
			for(new idx=i+1; string[idx]; idx++){
				if(string[idx]!='~'){ // look for the next non-blank
					string[i] = string[idx]; // move it to where the blank character was
					string[idx] = '~';	   //
					break; // break
				}
			}
		}
	}
	//find the end, and null it
	for(new i=0; string[i]; i++){
		if(string[i]=='~'){
			string[i] = '\0';
			break;
		}
	}
	return;
}

stock remove_char(string[], char_to_remove)
{
	for(new i=0; string[i]; i++){ // traverse the array
		if(string[i]==char_to_remove){ // check for black characters
			for(new idx=i+1; string[idx]; idx++){
				if(string[idx]!=char_to_remove){ // look for the next non-blank
					string[i] = string[idx]; // move it to where the blank character was
					string[idx] = char_to_remove;	   //
					break; // break
				}
			}
		}
	}
	//find the end, and null it
	for(new i=0; string[i]; i++){
		if(string[i]==char_to_remove){
			string[i] = '\0';
			break;
		}
	}
	return string;
}

stock removeEmbeddedColors(string[])
{
	new bool:color = false;
	for(new index=0; string[index]; index++){
		if(string[index] == '~'){
			if(color) color = false;
			else color = true;
		}
		else if(string[index] == '{'){
			color = true;
			string[index] = '~';
		}
		else if(string[index] == '}'){
			color = false;
			string[index] = '~';
		}
		else if(color){
			string[index] = '~';
		}
	}
	removeTilde(string);
	return;
}

//------------------------------------------------------------------------------

// variable converted into a function
stock model_hasPlate(modelid)
{
	switch(modelid)
	{
		case 400, 401, 402, 403, 404, 405, 407, 408, 409, 410, 411, 412, 413,
		414, 415, 416, 418, 419, 420, 421, 422, 423, 426, 427, 428, 429, 431,
		433, 434, 436, 437, 438, 439, 440, 442, 443, 445, 448, 451, 455, 456,
		458, 459, 461, 462, 466, 467, 470, 474, 475, 477, 478, 479, 480, 482,
		483, 489, 490, 491, 492, 494, 495, 496, 498, 499, 500, 502, 503, 504,
		505, 506, 507, 508, 514, 515, 516, 517, 518, 521, 523, 524, 525, 526,
		527, 529, 533, 534, 535, 536, 540, 541, 542, 543, 544, 545, 546, 547,
		549, 550, 551, 552, 554, 555, 558, 559, 560, 561, 562, 565, 566, 567,
		574, 575, 576, 578, 579, 580, 581, 582, 585, 586, 587, 588, 589, 596,
		597, 598, 599, 600, 602, 603, 604, 605, 609: return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------

stock valid_index(index, size)
{
	if(index>=0 && index<size) return 1;
	return 0;
}

// vehicle checks
stock valid_modelid(modelid)
{
	if(modelid>=400 && modelid<612) return 1;
	return 0;
}

stock valid_vColor(color)
{
	if(color>=-1 && color<256) return 1;
	return 0;
}

stock valid_paintjob(paintjobid)
{
	if(paintjobid>=-1 && paintjobid<3) return 1;
	return 0;
}

stock valid_worldid(worldid)
{
	if(worldid>=0 && worldid<=2147483647) return 1;
	return 0;
}
	
//------------------------------------------------------------------------------

// return the id for the driver of a vehicle
stock GetVehicleDriverID(vehicleid)
{
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++){
		if(GetPlayerVehicleID(playerid)==vehicleid && GetPlayerState(playerid)==PLAYER_STATE_DRIVER){
			return playerid;
		}
	}
	return INVALID_PLAYER_ID;
}

//------------------------------------------------------------------------------

stock hexToString(value)
{
	new const base = 0x10;
	new digits[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	new Float: decimal = float(value); // convert to float

	for(new i=0; decimal>=1; i++){
		decimal = decimal / base;
		// get remainder
		new Float: remainder = decimal - floatround(decimal, floatround_floor);
		digits[i] = floatround( (remainder * base), floatround_floor);
		decimal = floatround(decimal, floatround_floor); // was ceil, but that caused it to crash
	}

	new digitindex = sizeof(digits) - 1;
	new target[sizeof(digits) + 3] = "0x"; // 1 extra for the null
	new targetIdx = 2;
	new const hexCharsLower[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	// go forward through target, and backwards through digits

	while(targetIdx < sizeof(target)-1  &&  digitindex >= 0){
		if(digits[digitindex] == 0  &&  digitindex != 0){
			digitindex--;
			continue;
		}
		target[targetIdx] = hexCharsLower[digits[digitindex]];
		digitindex--;
		targetIdx++;
	}
	target[targetIdx] = '\0';

	return target;
}

stock GetEmbeddedColor(rgbvalue)
{
	new const base = 0x10;
	new digits[6] = {0, 0, 0, 0, 0, 0};
	new Float: decimal = float(rgbvalue); // convert to float
	new target[sizeof(digits) + 3] = ""; // 2 for {}, 1 for \0
	if(rgbvalue > 0xffffff) return target;
	target[0] = '{';
	for(new i=0; decimal>=1; i++){
		decimal = decimal / base;
		// get remainder
		new Float: remainder = decimal - floatround(decimal, floatround_floor);
		digits[i] = floatround( (remainder * base), floatround_floor);
		decimal = floatround(decimal, floatround_floor); // was ceil, but that caused it to crash
	}
	new digitindex = sizeof(digits) - 1;
	new targetIdx = 1;
	new const hexCharsLower[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	// go forward through target, and backwards through digits
	while(targetIdx < sizeof(target)-2  &&  digitindex >= 0){
		target[targetIdx] = hexCharsLower[digits[digitindex]];
		digitindex--;
		targetIdx++;
	}
	target[targetIdx] = '}';
	target[targetIdx+1] = '\0';
	return target;
}

//------------------------------------------------------------------------------

// simple check to see if two strings match each other completely
stock strmatch2(const string1[], const string2[], length = -1)
{
	for(new i=0; string1[i] || string2[i]; i++){
		if( i == length ) break;
		if( (string1[i] != string2[i]) ) return 0;
	}
	return 1;
}

//------------------------------------------------------------------------------

stock getVehicleColorEmbedded(colorid)
{
	new string[9] = "";
	switch(colorid)
	{
		// San Andreas Default
		case   0:	string = "{000000}";
		case   1:	string = "{f5f5f5}";
		case   2:	string = "{2a77a1}";
		case   3:	string = "{840410}";
		case   4:	string = "{263739}";
		case   5:	string = "{86446e}";
		case   6:	string = "{d78e10}";
		case   7:	string = "{4c75b7}";
		case   8:	string = "{bdbec6}";
		case   9:	string = "{5e7072}";
		case  10:	string = "{46597a}";
		case  11:	string = "{656a79}";
		case  12:	string = "{5d7e8d}";
		case  13:	string = "{58595a}";
		case  14:	string = "{d6dad6}";
		case  15:	string = "{9ca1a3}";
		case  16:	string = "{335f3f}";
		case  17:	string = "{730e1a}";
		case  18:	string = "{7b0a2a}";
		case  19:	string = "{9f9d94}";
		case  20:	string = "{3b4e78}";
		case  21:	string = "{732e3e}";
		case  22:	string = "{691e3b}";
		case  23:	string = "{96918c}";
		case  24:	string = "{515459}";
		case  25:	string = "{3f3e45}";
		case  26:	string = "{a5a9a7}";
		case  27:	string = "{635c5a}";
		case  28:	string = "{3d4a68}";
		case  29:	string = "{979592}";
		case  30:	string = "{421f21}";
		case  31:	string = "{5f272b}";
		case  32:	string = "{8494ab}";
		case  33:	string = "{767b7c}";
		case  34:	string = "{646464}";
		case  35:	string = "{5a5752}";
		case  36:	string = "{252527}";
		case  37:	string = "{2d3a35}";
		case  38:	string = "{93a396}";
		case  39:	string = "{6d7a88}";
		case  40:	string = "{221918}";
		case  41:	string = "{6f675f}";
		case  42:	string = "{7c1c2a}";
		case  43:	string = "{5f0a15}";
		case  44:	string = "{193826}";
		case  45:	string = "{5d1b20}";
		case  46:	string = "{9d9872}";
		case  47:	string = "{7a7560}";
		case  48:	string = "{989586}";
		case  49:	string = "{adb0b0}";
		case  50:	string = "{848988}";
		case  51:	string = "{304f45}";
		case  52:	string = "{4d6268}";
		case  53:	string = "{162248}";
		case  54:	string = "{272f4b}";
		case  55:	string = "{7d6256}";
		case  56:	string = "{9ea4ab}";
		case  57:	string = "{9c8d71}";
		case  58:	string = "{6d1822}";
		case  59:	string = "{4e6881}";
		case  60:	string = "{9c9c98}";
		case  61:	string = "{917347}";
		case  62:	string = "{661c26}";
		case  63:	string = "{949d9f}";
		case  64:	string = "{a4a7a5}";
		case  65:	string = "{8e8c46}";
		case  66:	string = "{341a1e}";
		case  67:	string = "{6a7a8c}";
		case  68:	string = "{aaad8e}";
		case  69:	string = "{ab988f}";
		case  70:	string = "{851f2e}";
		case  71:	string = "{6f8297}";
		case  72:	string = "{585853}";
		case  73:	string = "{9aa790}";
		case  74:	string = "{601a23}";
		case  75:	string = "{20202c}";
		case  76:	string = "{a4a096}";
		case  77:	string = "{aa9d84}";
		case  78:	string = "{78222b}";
		case  79:	string = "{0e316d}";
		case  80:	string = "{722a3f}";
		case  81:	string = "{7b715e}";
		case  82:	string = "{741d28}";
		case  83:	string = "{1e2e32}";
		case  84:	string = "{4d322f}";
		case  85:	string = "{7c1b44}";
		case  86:	string = "{2e5b20}";
		case  87:	string = "{395a83}";
		case  88:	string = "{6d2837}";
		case  89:	string = "{a7a28f}";
		case  90:	string = "{afb1b1}";
		case  91:	string = "{364155}";
		case  92:	string = "{6d6c6e}";
		case  93:	string = "{0f6a89}";
		case  94:	string = "{204b6b}";
		case  95:	string = "{2b3e57}";
		case  96:	string = "{9b9f9d}";
		case  97:	string = "{6c8495}";
		case  98:	string = "{4d8495}";
		case  99:	string = "{ae9b7f}";
		case 100:	string = "{406c8f}";
		case 101:	string = "{1f253b}";
		case 102:	string = "{ab9276}";
		case 103:	string = "{134573}";
		case 104:	string = "{96816c}";
		case 105:	string = "{64686a}";
		case 106:	string = "{105082}";
		case 107:	string = "{a19983}";
		case 108:	string = "{385694}";
		case 109:	string = "{525661}";
		case 110:	string = "{7f6956}";
		case 111:	string = "{8c929a}";
		case 112:	string = "{596e87}";
		case 113:	string = "{473532}";
		case 114:	string = "{44624f}";
		case 115:	string = "{730a27}";
		case 116:	string = "{223457}";
		case 117:	string = "{640d1b}";
		case 118:	string = "{a3adc6}";
		case 119:	string = "{695853}";
		case 120:	string = "{9b8b80}";
		case 121:	string = "{620b1c}";
		case 122:	string = "{5b5d5e}";
		case 123:	string = "{624428}";
		case 124:	string = "{731827}";
		case 125:	string = "{1b376d}";
		case 126:	string = "{ec6aae}";
		case 127:	string = "{000000}";

		// SA-MP Extended
		case 128:	string = "{177517}";
		case 129:	string = "{210606}";
		case 130:	string = "{125478}";
		case 131:	string = "{452a0d}";
		case 132:	string = "{571e1e}";
		case 133:	string = "{010701}";
		case 134:	string = "{25225a}";
		case 135:	string = "{2c89aa}";
		case 136:	string = "{8a4dbd}";
		case 137:	string = "{35963a}";
		case 138:	string = "{b7b7b7}";
		case 139:	string = "{464c8d}";
		case 140:	string = "{84888c}";
		case 141:	string = "{817867}";
		case 142:	string = "{817a26}";
		case 143:	string = "{6a506f}";
		case 144:	string = "{583e6f}";
		case 145:	string = "{8cb972}";
		case 146:	string = "{824f78}";
		case 147:	string = "{6d276a}";
		case 148:	string = "{1e1d13}";
		case 149:	string = "{1e1306}";
		case 150:	string = "{1f2518}";
		case 151:	string = "{2c4531}";
		case 152:	string = "{1e4c99}";
		case 153:	string = "{2e5f43}";
		case 154:	string = "{1e9948}";
		case 155:	string = "{1e9999}";
		case 156:	string = "{999976}";
		case 157:	string = "{7c8499}";
		case 158:	string = "{992e1e}";
		case 159:	string = "{2c1e08}";
		case 160:	string = "{142407}";
		case 161:	string = "{993e4d}";
		case 162:	string = "{1e4c99}";
		case 163:	string = "{198181}";
		case 164:	string = "{1a292a}";
		case 165:	string = "{16616f}";
		case 166:	string = "{1b6687}";
		case 167:	string = "{6c3f99}";
		case 168:	string = "{481a0e}";
		case 169:	string = "{7a7399}";
		case 170:	string = "{746d99}";
		case 171:	string = "{53387e}";
		case 172:	string = "{222407}";
		case 173:	string = "{3e190c}";
		case 174:	string = "{46210e}";
		case 175:	string = "{991e1e}";
		case 176:	string = "{8d4c8d}";
		case 177:	string = "{805b80}";
		case 178:	string = "{7b3e7e}";
		case 179:	string = "{3c1737}";
		case 180:	string = "{733517}";
		case 181:	string = "{781818}";
		case 182:	string = "{83341a}";
		case 183:	string = "{8e2f1c}";
		case 184:	string = "{7e3e53}";
		case 185:	string = "{7c6d7c}";
		case 186:	string = "{020c02}";
		case 187:	string = "{072407}";
		case 188:	string = "{163012}";
		case 189:	string = "{16301b}";
		case 190:	string = "{642b4f}";
		case 191:	string = "{368452}";
		case 192:	string = "{999590}";
		case 193:	string = "{818d96}";
		case 194:	string = "{99991e}";
		case 195:	string = "{7f994c}";
		case 196:	string = "{839292}";
		case 197:	string = "{788222}";
		case 198:	string = "{2b3c99}";
		case 199:	string = "{3a3a0b}";
		case 200:	string = "{8a794e}";
		case 201:	string = "{0e1f49}";
		case 202:	string = "{15371c}";
		case 203:	string = "{15273a}";
		case 204:	string = "{375775}";
		case 205:	string = "{060820}";
		case 206:	string = "{071326}";
		case 207:	string = "{20394b}";
		case 208:	string = "{2c5089}";
		case 209:	string = "{15426c}";
		case 210:	string = "{103250}";
		case 211:	string = "{241663}";
		case 212:	string = "{692015}";
		case 213:	string = "{8c8d94}";
		case 214:	string = "{516013}";
		case 215:	string = "{090f02}";
		case 216:	string = "{8c573a}";
		case 217:	string = "{52888e}";
		case 218:	string = "{995c52}";
		case 219:	string = "{99581e}";
		case 220:	string = "{993a63}";
		case 221:	string = "{998f4e}";
		case 222:	string = "{99311e}";
		case 223:	string = "{0d1842}";
		case 224:	string = "{521e1e}";
		case 225:	string = "{42420d}";
		case 226:	string = "{4c991e}";
		case 227:	string = "{082a1d}";
		case 228:	string = "{96821d}";
		case 229:	string = "{197f19}";
		case 230:	string = "{3b141f}";
		case 231:	string = "{745217}";
		case 232:	string = "{893f8d}";
		case 233:	string = "{7e1a6c}";
		case 234:	string = "{0b370b}";
		case 235:	string = "{27450d}";
		case 236:	string = "{071f24}";
		case 237:	string = "{784573}";
		case 238:	string = "{8a653a}";
		case 239:	string = "{732617}";
		case 240:	string = "{319490}";
		case 241:	string = "{56941d}";
		case 242:	string = "{59163d}";
		case 243:	string = "{1b8a2f}";
		case 244:	string = "{38160b}";
		case 245:	string = "{041804}";
		case 246:	string = "{355d8e}";
		case 247:	string = "{2e3f5b}";
		case 248:	string = "{561a28}";
		case 249:	string = "{4e0e27}";
		case 250:	string = "{706c67}";
		case 251:	string = "{3b3e42}";
		case 252:	string = "{2e2d33}";
		case 253:	string = "{7b7e7d}";
		case 254:	string = "{4a4442}";
		case 255:	string = "{28344e}";
	}
	return string;
}

//------------------------------------------------------------------------------

