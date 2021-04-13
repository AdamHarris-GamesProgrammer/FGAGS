#include "Time.h"
#include <Windows.h>

Time::Time() 
	: _secondsPerCount(0.0), _deltaTime(-1.0), _baseTime(0), 
	_pausedTime(0), _previousTime(0.0), _currentTime(0), _isStopped(false)
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	_secondsPerCount = 1.0 / (double)countsPerSecond;
}


float Time::GameTime() const
{
	if (_isStopped) {
		//Gets the total time from when we paused
		return (float)(((_stopTime - _pausedTime) - _baseTime) * _secondsPerCount);
	}
	else
	{
		//Gets the total time minus the amount of time we have spent paused
		return (float)(((_currentTime - _pausedTime) - _baseTime) * _secondsPerCount);
	}

}

float Time::DeltaTime() const
{
	return (float)_deltaTime;
}

void Time::Reset()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	_baseTime = currentTime;
	_previousTime = currentTime;
	_stopTime = 0.0;
	_isStopped = false;
}

void Time::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (_isStopped) {
		//Accumulate the time elapsed between stop and start function calls
		_pausedTime += (startTime - _stopTime);

		//Sets the previous time to the new time 
		_previousTime = startTime;

		//resets stop time
		_stopTime = 0;
		_isStopped = false;
	}
}

void Time::Stop()
{
	//if the timer is stopped then dont do anything
	if (_isStopped) return;

	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//Save the time we stopped at
	_stopTime = currentTime;

	_isStopped = true;
}


void Time::Tick()
{
	//if the timer is stopped then dont calculate delta time
	if (_isStopped) {
		_deltaTime = 0.0f;
		return;
	}

	//Get the time of this frame
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	_currentTime = currentTime;

	//Calculate time difference between this frame and the previous one
	_deltaTime = (_currentTime - _previousTime) * _secondsPerCount;

	//Prepare values for next frame
	_previousTime = _currentTime;

	//Nonnegative check, if the processor goes into power saver mode or
	//the application is changed the DX timer can sometimes return a negative value
	if (_deltaTime < 0.0) {
		_deltaTime = 0.0;
	}
}
