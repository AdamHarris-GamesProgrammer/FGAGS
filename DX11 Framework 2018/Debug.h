#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// A basic debug class for printing various types of data to the Output window in visual studio.
/// </summary>
class Debug
{
public:
	/// <summary>
	/// Prints a float to the Output window
	/// </summary>
	/// <param name="format">The string format for the output. Example "Radius: %.2f" (%.2f rounds the float to 2dp)</param>
	/// <param name="val">The floating point value which will be outputted</param>
	static void Print(std::string format, float val);

	/// <summary>
	/// Prints a double to the Output window
	/// </summary>
	/// <param name="format">The string format for the output. Example "Radius: %.2d" (%.2d rounds the double to 2dp)</param>
	/// <param name="val">The double value which will be outputted</param>
	static void Print(std::string format, double val);

	/// <summary>
	/// Prints a integer to the Output window
	/// </summary>
	/// <param name="format">The string format for the output. Example "Age: %i"</param>
	/// <param name="val"></param>
	static void Print(std::string format, int val);

	/// <summary>
	/// Prints a string to the Output window
	/// </summary>
	/// <param name="format">The message for the output</param>
	static void Print(std::string format);
};

