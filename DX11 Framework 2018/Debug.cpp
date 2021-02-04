#include "Debug.h"

//Prints a integer value
void Debug::Print(std::string format, int val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}

//Prints a string
void Debug::Print(std::string format)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str());
	OutputDebugStringA(msg);
}

//Prints a float value
void Debug::Print(std::string format, float val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}

//Prints a double value
void Debug::Print(std::string format, double val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}
