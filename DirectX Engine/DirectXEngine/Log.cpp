
#include "Log.h"

/*******************************************************************************************************************
	Handles the text colour and colour intensity of the console window text using a Win32 handle
*******************************************************************************************************************/
void Colour(LogColour colour, bool intensity)
{
	//---------------------------------------------------------------- Checks the intensity (bool) of the colour (true is bright, false is dark)
	WORD colourIntesity = (intensity) ? FOREGROUND_INTENSITY : 0;

	//---------------------------------------------------------------- Set the text attribute for the console using the handle and pass in the colour and the intensity we want
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour | colourIntesity);
}