#pragma once
#include <string>
#include <Windows.h>

class Debug
{
public:
	static void Print(std::string format, float val);
	static void Print(std::string format, double val);
	static void Print(std::string format, int val);
	static void Print(std::string format);
};

