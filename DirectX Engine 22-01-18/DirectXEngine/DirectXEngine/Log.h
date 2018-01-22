#ifndef LOG_H_
#define LOG_H_

/*******************************************************************************************************************
	Displays a message to the console window in a number of different colours
	Colour intesity can be changed by passing true or false as an argument to the Colour function
	More log types can be added to the LogType enum if necessary e.g. kNullPtrException
	This must also be updated within the switch statement in the Log function
	Supports variables of other data types too (int, float, etc.), when not passing a variable use EMPTY
	See comments below, which contain important notes on enums and inline functions
	A macro can also be used for simplicity: #define Log(message) std::cout << message << std::endl;
*******************************************************************************************************************/

#include "Windows.h"
#include <iostream>
#include <string>
#include <sstream>

#define EMPTY -1

/*******************************************************************************************************************
	Enums that define the log type (error message, etc.) and the log colour (colour of text in console)
*******************************************************************************************************************/
enum LogType	{ LOG_MSG, LOG_WARN, LOG_ERROR, LOG_SUCCESS };
enum LogColour	{ GREY, BLUE, GREEN, CYAN, RED, PINK, YELLOW, WHITE };

/*******************************************************************************************************************
	Handles the text colour and colour intensity of the console window text using a Win32 handle
*******************************************************************************************************************/
inline void Colour(LogColour colour, bool intensity = true)
{
	//---------------------------------------------------------------- Get the handle to our console window
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//---------------------------------------------------------------- Checks the intensity (bool) of the colour (true is bright, false is dark)
	WORD colourIntesity = (intensity) ? FOREGROUND_INTENSITY : 0;

	//---------------------------------------------------------------- Set the text attribute for the console using the handle and pass in the colour and the intensity we want
	SetConsoleTextAttribute(hStdOut, colour | colourIntesity);
}

/*******************************************************************************************************************
	Log function that outputs text to the console window. Replaces std::cout << for faster debugging
*******************************************************************************************************************/
template <typename T> void Log(const std::string& message, T variable, LogType type = LOG_MSG)
{
	//---------------------------------------------------------------- Create a local string to hold final debug message
	std::string outputString; 

	//---------------------------------------------------------------- Create a local sstream that holds the T variable data (int, float, char, etc.)
	std::stringstream tempVariable;

	//---------------------------------------------------------------- Store this data in our sstream so we can return a string copy of it using .str()
	tempVariable << variable; 
	
	//---------------------------------------------------------------- If variable is EMPTY, just return the message. Otherwise, concatenate the strings
	if (variable != (T)EMPTY)	{ outputString = message + tempVariable.str(); } 
	else						{ outputString = message; }

	//---------------------------------------------------------------- Switch to check the passed in LogType and output the passed in message and/or variable, complete with pretty colours to make debugging easy and fast!
	switch (type)  {

		case LOG_MSG:		{ Colour(GREY); std::cout << "[MESSAGE] " << outputString << std::endl;	break; }
		case LOG_WARN:		{ Colour(YELLOW); std::cout << "[WARNING] " << outputString << std::endl; break; }
		case LOG_ERROR:		{ Colour(RED); std::cout << "[ERROR] " << outputString << std::endl; break; }
		case LOG_SUCCESS:	{ Colour(GREEN); std::cout << "[SUCCESS] " << outputString << std::endl; break; }
	}

	//---------------------------------------------------------------- Reset text colour in console to grey, otherwise the colour will remain the last colour we used
	Colour(GREY);
}

#endif

/*COG***************************************************************************************************************
	NOTE ON ENUM NAMING CONVENTIONS:
	https://google.github.io/styleguide/cppguide.html#Enumerator_Names
	Until January 2009, the style was to name enum values like macros (TYPE_NAME).
	This caused problems with name collisions between enum values and macros.
	(e.g. in this case, the Windows.h file has already defined 'ERROR')
	Hence, the change to prefer constant-style naming was put in place.
	New code should prefer constant-style naming if possible (kTypeName).
	However, there is no reason to change old code to use constant-style names,
	unless the old names are actually causing a compile-time problem.

	NOTE ON INLINE FUNCTIONS:
	https://google.github.io/styleguide/cppguide.html#Inline_Functions
	A decent rule of thumb is to not inline a function if it is more than 10 lines long.
	Beware of destructors, which are often longer than they appear because of implicit member
	- and base - destructor calls!
	Another useful rule of thumb: it's typically not cost effective to inline functions with
	loops or switch statements (unless, in the common case, the loop or switch statement is never executed).
*******************************************************************************************************************/

