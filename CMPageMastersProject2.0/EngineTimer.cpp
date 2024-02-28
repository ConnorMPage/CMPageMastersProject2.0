#include "EngineTimer.h"

EngineTimer::EngineTimer()
{
	ResetTimer();
	mIsTimerOn = true;
}

void EngineTimer::StartTimer()
{
	if (!mIsTimerOn)
	{
		
		auto now = mClock.now();
		mStart += now - mStop;
		mLap += now - mStop;
		mIsTimerOn = true;
	}
}

void EngineTimer::StopTimer()
{
	if (mIsTimerOn)
	{
		mStop = mClock.now();
		mIsTimerOn = false;
	}
}

void EngineTimer::ResetTimer()
{
	mStart = mLap = mStop = mClock.now();
}

float EngineTimer::GetTime()
{
	Clock::time_point lastRunningTime = mIsTimerOn ? mClock.now() : mStop;
	std::chrono::duration<float> timePassed = lastRunningTime - mStart;
	return timePassed.count();
}

float EngineTimer::GetTimeLap()
{
	Clock::time_point lastRunningTime = mIsTimerOn ? mClock.now() : mStop;
	std::chrono::duration<float> timePassed = lastRunningTime - mLap;
	mLap = lastRunningTime;
	return timePassed.count();
}
