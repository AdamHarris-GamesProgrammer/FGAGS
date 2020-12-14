#include "Time.h"
#include <Windows.h>

Time::Time() 
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), 
	mPausedTime(0), mPreviousTime(0.0), mCurrentTime(0), mIsStopped(false)
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	mSecondsPerCount = 1.0 / (double)countsPerSecond;
}


float Time::GameTime() const
{
	if (mIsStopped) {
		//Gets the total time from when we paused
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		//Gets the total time minus the amount of time we have spent paused
		return (float)(((mCurrentTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}

}

float Time::DeltaTime() const
{
	return (float)mDeltaTime;
}

void Time::Reset()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	mBaseTime = currentTime;
	mPreviousTime = currentTime;
	mStopTime = 0.0;
	mIsStopped = false;
}

void Time::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mIsStopped) {
		//Accumulate the time elapsed between stop and start function calls
		mPausedTime += (startTime - mStopTime);

		//Sets the previous time to the new time 
		mPreviousTime = startTime;

		//resets stop time
		mStopTime = 0;
		mIsStopped = false;
	}
}

void Time::Stop()
{
	//if the timer is stopped then dont do anything
	if (mIsStopped) return;

	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//Save the time we stopped at
	mStopTime = currentTime;

	mIsStopped = true;
}


void Time::Tick()
{
	//if the timer is stopped then dont calculate delta time
	if (mIsStopped) {
		mDeltaTime = 0.0f;
		return;
	}

	//Get the time of this frame
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	mCurrentTime = currentTime;

	//Calculate time difference between this frame and the previous one
	mDeltaTime = (mCurrentTime - mPreviousTime) * mSecondsPerCount;

	//Prepare values for next frame
	mPreviousTime = mCurrentTime;

	//Nonnegative check, if the processor goes into power saver mode or
	//the application is changed the DX timer can sometimes return a negative value
	if (mDeltaTime < 0.0) {
		mDeltaTime = 0.0;
	}
}
