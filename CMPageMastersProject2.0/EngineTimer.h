#pragma once
#include <chrono>
using Clock = std::chrono::steady_clock;


class EngineTimer
{

public:
	EngineTimer();


	void StartTimer();

	void StopTimer();

	void ResetTimer();

	float GetTime();

	float GetTimeLap();
private:

	Clock mClock;

	Clock::time_point mStart;

	Clock::time_point mLap;

	Clock::time_point mStop;

	bool mIsTimerOn;

};

