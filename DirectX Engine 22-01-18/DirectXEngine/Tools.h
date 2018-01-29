#pragma once

/*******************************************************************************************************************
	Tools.h, Tools.cpp
	Created by Karsten Vermeulen

	Modified by Kim Kane
	Last updated: 06/01/2018

	*Still under construction.
	Handles all of our typical miscellaneous functions used, e.g. converting a string to an integer.

*******************************************************************************************************************/
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <d3d11.h>
#include <xnamath.h>

//*************************************
//string case functions
//*************************************
void ToUpper(std::string& str);
void ToLower(std::string& str);

//*************************************
//string manipulation functions
//*************************************
void ListString(const std::string& str);
void RemoveCharacter(std::string& str, char character);
void CutString(std::string& str, int startPos, int length);
void AddString(std::string& finalString, const char* str);
void AddString(std::string& finalString, std::string& str);
void ParseString(std::string& str, std::vector<std::string>& subStrings, char token);
void ReplaceCharacter(std::string& str, const char current, const char replaceWith);

//*************************************
//string/type conversion functions
//*************************************
int StringToInteger(const std::string& str);
float StringToFloat(const std::string& str);
bool StringToBool(std::string const& s);

template <typename T> std::string NumberToString(T number) {
	std::stringstream str;
	str << number;
	return str.str();
}

//*************************************
//output message to console window
//*************************************
template <typename T> std::ostream& Debug(T str) {
	return std::cout << str << std::endl;;
}

//temp function being used in pipeline manager - DELETE!
std::string AddString(const std::string& a, const std::string& b);

XMFLOAT3 VectorToFloat(const XMVECTOR& vectorToConvert);
XMVECTOR FloatToVector(const XMFLOAT3& floatToConvert);