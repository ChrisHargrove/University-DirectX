#ifndef LOG_H_
#define LOG_H_

/*******************************************************************************************************************
	Log.h, Log.cpp
	Created by Kim Kane
	Last updated: 04/01/2018
	
	A simple logging system that displays a message to the console window in a number of different colours.
	The colour intesity can be changed by passing true or false as an argument to the Colour function.
	
	More log types can be added to the LogType enum if necessary.
	This must also be updated within the switch statement in the Debug function.
	
	Supports variables of other data types (int, float, etc.) - when not passing a variable use EMPTY.
	A macro can also be used for simplicity: #define Log(message) std::cout << message << std::endl;

	Also supports OpenGL error messages (see defined macros below) (been removed in DirectX)

*******************************************************************************************************************/
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>

#define EMPTY -1

/*******************************************************************************************************************
	Macros have been defined for differentiating between Debug and Release modes.
	The Debug and OpenGLDebug functions will only be called during Debug mode.

	You could also use ASSERT(function) if(!(function)) __debugbreak(); to generate a breakpoint.
*******************************************************************************************************************/
#if DEBUG_MODE == 1
	#define Log(message, variable, type) Debug(message, variable, type)
#elif defined(RELEASE_MODE)
	#define Log(message, variable, type)
#endif

/*******************************************************************************************************************
	Enums that define the log type (error message, etc.) and the log colour (colour of text in console)
*******************************************************************************************************************/
enum LogType	{ LOG_MESSAGE, LOG_WARN, LOG_ERROR, LOG_SUCCESS, LOG_MEMORY, LOG_COPY, LOG_RESOURCE };
enum LogColour	{ GREY, BLUE, GREEN, CYAN, RED, PINK, YELLOW, WHITE };

void Colour(LogColour colour, bool intensity = true);

/*******************************************************************************************************************
	Debug function that outputs text to the console window. Replaces std::cout << for faster debugging
*******************************************************************************************************************/
template <typename T> inline void Debug(const std::string& message, T variable, LogType type)
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

	//---------------------------------------------------------------- Switch to check the passed in LogType and output the passed in message and/or variable (complete with pretty colours), to make debugging easy and fast
	switch (type)  {

		case LOG_MESSAGE:	{ Colour(GREY); std::cout << "[MESSAGE] " << outputString << std::endl;	break; }
		case LOG_WARN:		{ Colour(YELLOW); std::cout << "[WARNING] " << outputString << std::endl; break; }
		case LOG_ERROR:		{ Colour(RED); std::cout << "[ERROR] " << outputString << std::endl; break; }
		case LOG_SUCCESS:	{ Colour(GREEN); std::cout << "[SUCCESS] " << outputString << std::endl; break; }
		case LOG_MEMORY:	{ Colour(PINK); std::cout << "[MEMORY] " << outputString << std::endl; break; }
		case LOG_COPY:		{ Colour(CYAN); std::cout << "[COPY CONSTRUCTOR] " << outputString << std::endl; break; }
		case LOG_RESOURCE:	{ Colour(BLUE); std::cout << "[RESOURCE] " << outputString << std::endl; break; }
	}
}

#endif