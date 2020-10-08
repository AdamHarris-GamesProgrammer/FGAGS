#include "Time.h"
#include <Windows.h>

Time::Time() 
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), 
	mPausedTime(0), mPreviousTime(0.0), mCurrentTime(0), isStopped(false)
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	mSecondsPerCount = 1.0 / (double)countsPerSecond;
}

float Time::GameTime() const
{
	return 0.0f;
}

float Time::DeltaTime() const
{
	return (float)mDeltaTime;
}

void Time::Reset()
{
	return;
}

void Time::Start()
{
	return;
}

void Time::Stop()
{
	return;
}

void Time::Tick()
{
	//if the timer is stopped then dont calculate delta time
	if (isStopped) {
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
