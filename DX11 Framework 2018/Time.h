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


	void Start();


	void Stop();

	/// <summary>
	/// This function calculates delta time each frame 
	/// </summary>
	void Tick();

private:
	double _secondsPerCount;
	double _deltaTime;

	__int64 _baseTime;
	__int64 _pausedTime;
	__int64 _stopTime;
	__int64 _previousTime;
	__int64 _currentTime;

	bool _isStopped;

};

