#pragma once
#include <string>
#include <Windows.h>
#include "Precision.h"

class Debug
{
public:
	static void Print(std::string message, float val);
	static void Print(std::string message, double val);
	static void Print(std::string message, int val);
	static void Print(std::string message);
};

