#pragma once


class Time
{
public:
	Time();
	/// <summary>
	/// Gets the total time the game has ran for
	/// </summary>
	float GameTime()const;

	/// <summary>
	/// Gets the value of Delta time
	/// </summary>
	float DeltaTime() const;

	/// <summary>
	/// Resets the variables associated with the game timer class
	/// </summary>
	void Reset();


	/// <summary>
	/// Allows the programmer to set a time scale variable that affects the delta time variable. Defaults at 1.0f
	/// </summary>
	void SetTimeScale(float value);

	float GetTimeScale();

	void Start();


	void Stop();

	/// <summary>
	/// This function calculates delta time each frame 
	/// </summary>
	void Tick();

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPreviousTime;
	__int64 mCurrentTime;

	bool isStopped;

	float timeScale = 1.0f;

};

