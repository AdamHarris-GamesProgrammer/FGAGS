#include "Debug.h"

void Debug::Print(std::string format, int val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}

void Debug::Print(std::string format)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str());
	OutputDebugStringA(msg);
}

void Debug::Print(std::string format, float val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}

void Debug::Print(std::string format, double val)
{
	char msg[1024] = { 0 };
	sprintf_s(msg, format.c_str(), val);
	OutputDebugStringA(msg);
}
