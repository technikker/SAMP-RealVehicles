// =============================================================================
// Connect to a database specified in a text file
//
// requires y_ini, mysql
// =============================================================================



// -----------------------------------------------------------------------------
//                                    globals
// -----------------------------------------------------------------------------
#if !defined MAX_DATABASES
	#define MAX_DATABASES				1
#endif

#if !defined SQL_QUERY_SIZE
	#define SQL_QUERY_SIZE				256
#endif

#if !defined DBINFO_FILE
	#define DBINFO_FILE					"dbinfo.ini"
#endif

new SQL_HOST[MAX_DATABASES][32] =		{"localhost"			};
new SQL_USER[MAX_DATABASES][32] =		{"your user name"		};
new SQL_PASS[MAX_DATABASES][128] =		{"your password"		};
new SQL_DTBS[MAX_DATABASES][32] =		{"the db to connect to"	};
new database[MAX_DATABASES] =			{0};

new SQL_DEBUG							= 1;



// -----------------------------------------------------------------------------
//                                    dbinfo.ini
// -----------------------------------------------------------------------------

forward dbinfo_load_connection(databaseid, name[], value[]);
public dbinfo_load_connection(databaseid, name[], value[])
{
	new str[16];

	format(str, sizeof(str), "SQL_HOST_%d", databaseid);
	INI_String	(str,	SQL_HOST[databaseid],	sizeof(SQL_HOST[]));

	format(str, sizeof(str), "SQL_USER_%d", databaseid);
	INI_String	(str,	SQL_USER[databaseid],	sizeof(SQL_USER[]));

	format(str, sizeof(str), "SQL_PASS_%d", databaseid);
	INI_String	(str,	SQL_PASS[databaseid],	sizeof(SQL_PASS[]));

	format(str, sizeof(str), "SQL_DTBS_%d", databaseid);
	INI_String	(str,	SQL_DTBS[databaseid],	sizeof(SQL_DTBS[]));
	return 0;
}

stock dbinfo_load()
{
	if(!fexist(DBINFO_FILE)){
		dbinfo_save();
		return 1;
	}
	for(new databaseid=0; databaseid<MAX_DATABASES; databaseid++)
	{
		INI_ParseFile(DBINFO_FILE, "dbinfo_load_%s", .bExtra = true, .extra = databaseid);
	}
	return 0;
}

stock dbinfo_save()
{
	// will create file if it doesn't exist
	// "INI_Open" returns a variable with tag "INI".
	new INI:iniFile = INI_Open(DBINFO_FILE);

	// 
	// y_ini supports tags, that is:
	// 
	//  [tag]
	//  key = value
	// 

	// [connection]
	INI_SetTag		(iniFile, "connection");

	for(new databaseid=0; databaseid<MAX_DATABASES; databaseid++)
	{
		if(isnull(SQL_HOST[databaseid])) continue;

		new str[16];
		format(str, sizeof(str), "SQL_HOST_%d", databaseid);
		INI_WriteString	(iniFile, str, SQL_HOST[databaseid]);
		
		format(str, sizeof(str), "SQL_USER_%d", databaseid);
		INI_WriteString	(iniFile, str, SQL_USER[databaseid]);
		
		format(str, sizeof(str), "SQL_PASS_%d", databaseid);
		INI_WriteString	(iniFile, str, SQL_PASS[databaseid]);
		
		format(str, sizeof(str), "SQL_DTBS_%d", databaseid);
		INI_WriteString	(iniFile, str, SQL_DTBS[databaseid]);
	}

	// Now close the current file:
	INI_Close(iniFile);
	return 0;
}



// -----------------------------------------------------------------------------
//                                 mysql functions
// -----------------------------------------------------------------------------

stock database_connect(databaseid)
{
	print("db connect");
	if(databaseid >= MAX_DATABASES) return 1;

	database[databaseid] = mysql_connect(SQL_HOST[databaseid], SQL_USER[databaseid], SQL_DTBS[databaseid], SQL_PASS[databaseid]);
	//TODO fix
	if(database[databaseid]){
		if(SQL_DEBUG)	mysql_log(LOG_ERROR | LOG_WARNING | LOG_DEBUG, LOG_TYPE_TEXT);
		else			mysql_log(LOG_ERROR, LOG_TYPE_TEXT);
		printf("[MYSQL]: Connection To `%s` Successful!", SQL_DTBS[databaseid]);
	}
	else{
		printf("[MYSQL]: [ERROR]: Connection To `%s` Failed!", SQL_DTBS[databaseid]);
		SendRconCommand("exit");
	}

	return 0;
}

stock database_disconnect(databaseid)
{
	if(databaseid >= MAX_DATABASES) return 1;
	mysql_close(database[databaseid]);
	return 0;
}





//==============================================================================
//======================START OF MYSQL FUNCTIONS================================
//==============================================================================

stock MySQLConnect(sqlhost[], sqluser[], sqlpass[], sqldb[])
{
	database[0] = mysql_connect(sqlhost, sqluser, sqldb, sqlpass);
	//TODO fix
	if(database[0]){
		if(SQL_DEBUG)	mysql_log(LOG_ERROR | LOG_WARNING | LOG_DEBUG, LOG_TYPE_TEXT);
		else			mysql_log(LOG_ERROR, LOG_TYPE_TEXT);
		printf("[MYSQL]: Connection To `%s` Successful!", sqldb);
	}
	else{
		printf("[MYSQL]: [ERROR]: Connection To `%s` Failed!", sqldb);
		SendRconCommand("exit");
	}
	return 1;
}

stock MySQLDisconnect() // by Luk0r
{
	mysql_close();
	return 1;
}

// unused
stock MySQLCheckConnection() // by Luk0r
{
	if(mysql_ping()==0){
		return 1;
	}
	else{
		print("MYSQL: Connection Seems Dead, Retrying...");
		MySQLDisconnect();

		MySQLConnect(MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DTBS);

		if(mysql_ping()==0){
			print("[MYSQL]: Reconnection Successful. We Can Continue As Normal.");
			return 1;
		}
		else{
			print("[MYSQL]: [ERROR]: Could Not Reconnect To Server, Terminating Server...");
            SendRconCommand("exit");
			return 0;
		}
	}
}

// unused
stock MySQLUpdateBuild(query[], sqlplayerid) // by Luk0r
{
	new querylen = strlen(query);
	new querymax = MAX_STRING;
	if(querylen < 1){
		format(query, querymax, "UPDATE players SET ");
	}
	else if (querymax-querylen < 50){ // make sure we're being safe here
		// query is too large, send this one and reset
		new whereclause[32];
		format(whereclause, sizeof(whereclause), " WHERE id=%d", sqlplayerid);
		strcat(query, whereclause, querymax);
		new Cache:result = mysql_query(database[0], query);
		format(query, querymax, "UPDATE playerinfo SET ");
	}
	else if(strfind(query, "=", true) != -1){
		strcat(query, ",", MAX_STRING);
	}
	return 1;
}

// unused
stock MySQLUpdateFinish(query[], sqlplayerid) // by Luk0r
{
	if (strcmp(query, "WHERE id=", false) == 0) new Cache:result = mysql_query(database[0], query);
	else{
		new whereclause[32];
		format(whereclause, sizeof(whereclause), " WHERE id=%d", sqlplayerid);
		strcat(query, whereclause, MAX_STRING);
		new Cache:result = mysql_query(database[0], query);
		format(query, MAX_STRING, "UPDATE playerinfo SET ");
	}
	return 1;
}

// unused
stock MySQLUpdatePlayerInt(query[], sqlplayerid, sqlvalname[], sqlupdateint) // by Luk0r
{
	MySQLUpdateBuild(query, sqlplayerid);
	new updval[64];
	format(updval, sizeof(updval), "%s=%d", sqlvalname, sqlupdateint);
	strcat(query, updval, MAX_STRING);
	return 1;
}

// unused
stock MySQLUpdatePlayerFlo(query[], sqlplayerid, sqlvalname[], Float:sqlupdateflo) // by Luk0r
{
	new flotostr[32];
	format(flotostr, sizeof(flotostr), "%f", sqlupdateflo);
	MySQLUpdatePlayerStr(query, sqlplayerid, sqlvalname, flotostr);
	return 1;
}

// unused
stock MySQLUpdatePlayerStr(query[], sqlplayerid, sqlvalname[], sqlupdatestr[]) // by Luk0r
{
	MySQLUpdateBuild(query, sqlplayerid);
	new escstr[128];
	new updval[128];
	mysql_real_escape_string(sqlupdatestr, escstr);
	format(updval, sizeof(updval), "%s='%s'", sqlvalname, escstr);
	strcat(query, updval, MAX_STRING);
	return 1;
}

// unused
stock MySQLUpdatePlayerIntSingle(sqlplayerid, sqlvalname[], sqlupdateint) // by Luk0r
{
	new query[128];
	format(query, sizeof(query), "UPDATE playerinfo SET %s=%d WHERE id=%d", sqlvalname, sqlupdateint, sqlplayerid);
	new Cache:result = mysql_query(database[0], query);
	return 1;
}

// unused
stock MySQLCheckAccount(sqlplayersname[]) // by Luk0r
{
	new query[128];
	new escstr[MAX_PLAYER_NAME];
	mysql_real_escape_string(sqlplayersname, escstr);
	format(query, sizeof(query), "SELECT id FROM playerinfo WHERE user = '%s' LIMIT 1", escstr);
	new Cache:result = mysql_query(database[0], query);
	mysql_store_result();
	if (mysql_num_rows()==0)
	{
		return 0;
	}
	else
	{
		new strid[32];
		new intid;
		mysql_fetch_row(strid);
		intid = strval(strid);
		return intid;
	}
}

// unused
stock MySQLCheckIPBanned(ip[])
{
	new query[64];
	format(query, sizeof(query), "SELECT type FROM bans WHERE ip = '%s' AND inactive = 0 ORDER BY id DESC LIMIT 1", ip);
	new Cache:result = mysql_query(database[0], query);
	mysql_store_result();
	if (mysql_num_rows() != 0)
	{
		new bantypestr[4];
		new bantypeint;
		mysql_fetch_row(bantypestr);
		bantypeint = strval(bantypestr);
		cache_delete(result, database[0]);
		return bantypeint;
	}
	return 0;
}

// unused
stock MySQLFetchAcctSingle(sqlplayername[], sqlvalname[], sqlresult[])
{
	new query[128];
	format(query, sizeof(query), "SELECT %s FROM playerinfo WHERE user = %s LIMIT 1", sqlvalname, sqlplayername);
	new Cache:result = mysql_query(database[0], query);
	mysql_store_result();
	if(mysql_fetch_row(sqlresult)==1)
	{
		return 1;
	}
	return 0;
}

// unused
stock MySQLFetchAcctRecord(sqlplayername[], sqlresult[]) // by Luk0r
{
	new query[64];
	format(query, sizeof(query), "SELECT * FROM playerinfo WHERE user = %s LIMIT 1", sqlplayername);
	new Cache:result = mysql_query(database[0], query);
	mysql_store_result();
	if(mysql_fetch_row(sqlresult)==1)
	{
		return 1;
	}
	return 0;
}

// unused
stock MySQLCreateAccount(newplayersname[], newpassword[]) // by Luk0r
{
	new query[128];
	new sqlplyname[64];
	new sqlpassword[64];
	mysql_real_escape_string(newplayersname, sqlplyname);
	mysql_real_escape_string(newpassword, sqlpassword);
	format(query, sizeof(query), "INSERT INTO playerinfo (user, password) VALUES ('%s', '%s')", sqlplyname, sqlpassword);
	new Cache:result = mysql_query(database[0], query);
	new newplayersid = MySQLCheckAccount(newplayersname);
	if (newplayersid != 0)
	{
		return newplayersid;
	}
	return 0;
}
//==============================================================================
//======================END OF MYSQL FUNCTIONS==================================
//==============================================================================
//==============================================================================