// f_dialogs
// by Enforcer

//
// ShowPlayerDialog
// Shows the player a synchronous (only one at a time) dialog box.
//
// ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);
// playerid		The ID of the player to show the dialog to.
// dialogid		An ID to assign this dialog to, so responses can be processed. Max dialogid is 32767. Using negative values will close any open dialog.
// style		The style of the dialog.
// caption[]	The title at the top of the dialog. The length of the caption can not exceed more than 64 characters before it starts to cut off.
// info[]		The text to display in the main dialog. Use \n to start a new line and \t to tabulate.
// button1[]	The text on the left button.
// button2[]	The text on the right button. Leave it blank ( "" ) to hide it.
//
// 0 - DIALOG_STYLE_MSGBOX
// 1 - DIALOG_STYLE_INPUT
// 2 - DIALOG_STYLE_LIST
// 3 - DIALOG_STYLE_PASSWORD
//
// Use color embedding for multiple colors in the text.
// Using -1 as dialogid closes all dialogs currently shown on the client's screen.
//

//
// OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
// playerid	The ID of the player that responded to the dialog.
// dialogid	The ID of the dialog the player responded to, assigned in ShowPlayerDialog.
// response	1 for left button and 0 for right button (if only one button shown, always 1)
// listitem	The ID of the list item selected by the player (starts at 0) (only if using a list style dialog).
// inputtext[]	The text entered into the input box by the player or the selected list item text.
//
// Returning 0 in this callback will pass the dialog to another script in case no matching code were found in your gamemode's callback.
// Tip: It is appropriate to switch through different dialogids, if you have many.
//
/*
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MSGBOX){
        if(response){ // If they clicked BUTTON1 or pressed enter
            // DO SOMETHING
        }
        else{ // Pressed ESC or clicked cancel
            // DO SOMETHING
        }
        return 1; // We handled a dialog, so return 1. Just like OnPlayerCommandText.
    }

    if(dialogid == DIALOG_INPUT || dialogid == DIALOG_PASSWORD)
    {
        if(response){ // Pressed ENTER or clicked BUTTON1
            if(!isnull(inputtext)){
                // DO SOMETHING
            }
        }
        else{ // If they clicked 'Cancel' or pressed esc
            // DO SOMETHING
        }
        return 1;
    }

    if(dialogid == DIALOG_LIST)
    {
        if(response) // If they clicked BUTTON1 or a list item
        {
            switch(listitem)
            {
                case 0: // DO SOMETHING
            }
        }
        else{ // If they clicked 'Cancel' or pressed esc
            // DO SOMETHING
        }
        return 1;
    }

	return 0; // You MUST return 0 here! Just like OnPlayerCommandText.
}*/

#if defined _fdialog_included
	#endinput
#endif
#define _fdialog_included

#define DIALOG_STYLE_INVALID	-1

#if defined MAX_FUNC_NAME
	#undef MAX_FUNC_NAME
#endif
#define MAX_FUNC_NAME			(33) // includes null

#if !defined isnull
	#define isnull(%1) \
				  ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#define DIALOG:%1(%2)          \
			forward dlg_%1(%2); \
			public dlg_%1(%2)

#define showDialog(%1,%2,%3,%4) \
			dlg_%1(%2, %3, %4, -1, "", -1, -1)
// playerid, index, params[], listitem, inputtext[], dialogid, response

enum pDialog
{
	pDialogFunc[MAX_FUNC_NAME-4],	// includes null
	pDialogStyle,
	pDialogInfo[2049],				// includes null
	pDialogIndex,					// additional info
	pDialogParams[11],
	bool:pDialogColors,				// include embedded colours in returned strings
};
new playerDialog[MAX_PLAYERS][pDialog];

// copy source string into the destination string
stock dialog_strcopy(dest[], dest_size, const source[])
{
    dest_size--; // leave space for terminating null
	new idx = 0;
	while(source[idx])
	{
		if(idx >= dest_size) // if we run out of space in dest
		{
			new i = idx; // count the num of truncated chars
			while(source[i]) i++;
			dest[idx] = '\0'; // tack on a null
			return i - idx; // return num of truncated chars
		}
		dest[idx] = source[idx]; // copy from source to dest
		idx++; // move on to next char
	}
	dest[idx] = '\0'; // tack on a null
	return 0; // no truncation
}

// ShowPlayerDialogEx(playerid, dialogid, dialogfunc[], style, caption[], info[], index = -1, params[] = "", button1[] = "Accept", button2[] = "Cancel", bool:showcolors = false)
stock ShowPlayerDialogEx(playerid, dialogid, dialogfunc[], style, caption[], info[], index = -1, params[] = "", button1[] = "Accept", button2[] = "Cancel", bool:showcolors = false)
{
	dialog_strcopy(playerDialog[playerid][pDialogFunc], 33, dialogfunc);
    playerDialog[playerid][pDialogStyle] = style;
    if(showcolors) dialog_strcopy(playerDialog[playerid][pDialogInfo], 2049, info);
    playerDialog[playerid][pDialogIndex] = index;
	dialog_strcopy(playerDialog[playerid][pDialogParams], 11, params);
	playerDialog[playerid][pDialogColors] = showcolors;
	ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
    //printf("%d %d %s", playerid, dialogid, dialogfunc);
    return 0;
}

public OnGameModeInit()
{
	for(new playerid=0; playerid<MAX_PLAYERS; playerid++){
	    playerDialog[playerid][pDialogFunc][0]		= '\0';
	    playerDialog[playerid][pDialogStyle]		= DIALOG_STYLE_INVALID;
	    playerDialog[playerid][pDialogInfo][0]		= '\0';
	    playerDialog[playerid][pDialogIndex]		= -1;
	    playerDialog[playerid][pDialogParams][0]	= '\0';
	    playerDialog[playerid][pDialogColors]		= false;
	}
	// call the other callback
	#if defined fdialog_OnGameModeInit
		fdialog_OnGameModeInit();
	#endif
	return 1;
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit fdialog_OnGameModeInit
#if defined fdialog_OnGameModeInit
	forward fdialog_OnGameModeInit();
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	// only if the dialog was created by f_dialogs
	if(playerDialog[playerid][pDialogStyle] != DIALOG_STYLE_INVALID){

		//SendClientMessage(playerid, 0xFF0000FF, "This is a fucking disaster");

		// make up the name of the function to call
		new funcname[MAX_FUNC_NAME] = "dlg_";
		new idx = 0;
		for(new i=4; i<MAX_FUNC_NAME && playerDialog[playerid][pDialogFunc][idx]; i++){
		    funcname[i] = playerDialog[playerid][pDialogFunc][idx];
		    idx++;
		}

		// copy the vars we need to use
		new index = playerDialog[playerid][pDialogIndex];
		new params[11]; params = playerDialog[playerid][pDialogParams];
		dialog_strcopy(params, sizeof(params), playerDialog[playerid][pDialogParams]);
		if( !params[0] ) params[0] = 1, params[1] = 0; // don't pass a null string
		new bool:showcolors = playerDialog[playerid][pDialogColors];

		// if show embedded colours is enabled,
		if(showcolors){
			// find the string of the item in the list that they clicked on
			new listtext[128] = "";
			if(playerDialog[playerid][pDialogStyle] == DIALOG_STYLE_LIST){ // if it is a list dialog, find the string of the listitem
				new listtextIdx = 0;
				new AtListItem = 0;
				for(new i=0; playerDialog[playerid][pDialogInfo][i] && i<sizeof(listtext); i++){
					if(playerDialog[playerid][pDialogInfo][i] == '\n'){
						AtListItem++;
						continue; // don't process this char, it is a LF character
					}
					if(AtListItem == listitem){
						listtext[listtextIdx] = playerDialog[playerid][pDialogInfo][i];
						listtextIdx++;
					}
					else if(AtListItem > listitem){
						break;
					}
				}
				listtext[listtextIdx] = '\0';
			}

			// clear the globals before we call the dialogue
			playerDialog[playerid][pDialogFunc][0]		= '\0';
			playerDialog[playerid][pDialogStyle]		= DIALOG_STYLE_INVALID;
			playerDialog[playerid][pDialogInfo][0]		= '\0';
			playerDialog[playerid][pDialogIndex]		= -1;
			playerDialog[playerid][pDialogParams][0]	= '\0';
			playerDialog[playerid][pDialogColors]		= false;

			if(!listtext[0]){ // don't pass a null string
				listtext[0] = 1;
				listtext[1] = 0;
			}
			// pass listtext in place of inputtext
			CallLocalFunction(funcname, "iisisii", playerid, index, params, listitem, listtext, dialogid, response);
			return 1;
		}

		else{
			// clear the globals before we call the dialogue
			playerDialog[playerid][pDialogFunc][0]		= '\0';
			playerDialog[playerid][pDialogStyle]		= DIALOG_STYLE_INVALID;
			//playerDialog[playerid][pDialogInfo][0]	= '\0';
			playerDialog[playerid][pDialogIndex]		= -1;
			playerDialog[playerid][pDialogParams][0]	= '\0';
			playerDialog[playerid][pDialogColors]		= false;

			// if inputtext is null, pass an SOH char in its place
			if( !inputtext[0] ){
				new soh[2] = {1, '\0'};
				CallLocalFunction(funcname, "iisisii", playerid, index, params, listitem, soh, dialogid, response);
				return 1;
			}

			// inputtext is good...
			CallLocalFunction(funcname, "iisisii", playerid, index, params, listitem, inputtext, dialogid, response);
			return 1;
		}
	}

	// if the dialog was created by another script
	// call the other callback
	new returnValue = 0;
	#if defined fdialog_OnDialogResponse
		returnValue = fdialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#endif
	return returnValue;
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse fdialog_OnDialogResponse
#if defined fdialog_OnDialogResponse
	forward fdialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

#undef MAX_FUNC_NAME





// Example script using f_dialog
/*
#include <a_samp>
#include <zcmd>
#include ../include/f_dialog_1-0-0.inc

enum
{
    DIALOG_TEST
};

DIALOG:example(playerid, inputtext[], index, response, listitem, listtext[])
{
	new dialogid = DIALOG_TEST;
	new dialogfunc[] = "example";
	new style = DIALOG_STYLE_LIST;
	new caption[] = "example dialog";
	new button1[] = "Select";
	new button2[] = "Cancel";
	
	// if dialog is no open, this will open it
	if(response == -1){
        if(!isnull(inputtext)) ShowPlayerDialogEx(playerid, dialogid, dialogfunc, style, caption, inputtext, index, button1, button2);
		else{
			new info[] = ""; // default value for info
			ShowPlayerDialogEx(playerid, dialogid, dialogfunc, style, caption, info, index, button1, button2);
	    }
		return 0;
	}
	// the rest of the script is run when the dialog gets a response

	// do stuff when dialog is called up
	// just as would be done in OnDialogResponse
	// index is an extra param for added flexibility
	// listtext is a string of the listitem they clicked on (or \0 if they didn't)

	new str[256];
	format(str, sizeof(str), "player selected -%s-", inputtext);
	SendClientMessage(playerid, 0xFF0000FF, str);

	return 1;
}

CMD:dialogtest(playerid, params[])
{

	// how to call up a dialog
	// send a blank string as third param to use the default for the dialog
	// last value is only used if configured in the dialog code (index)
    showDialog(example, playerid, "list1\nlist2\nlist3", 0);
	return 1;
}
*/
