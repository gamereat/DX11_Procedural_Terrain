#include "timer.h"

Timer::Timer()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		MessageBox(NULL, L"No support for high performance timer", L"ERROR", MB_OK);
	}

	// Find out how many times the frequency counter ticks every second.
	m_ticksPerS = (float)(m_frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	m_totalTime = 0.0f;
}


Timer::~Timer()
{
}

void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	timeDifference = (float)(currentTime - m_startTime);

	// Calculate the frame time by the time difference over the timer speed resolution.
	m_frameTime = timeDifference / m_ticksPerS;

	// Restart the timer.
	m_startTime = currentTime;

	// Increase the total time that has passed
	m_totalTime += m_frameTime;
	return;
}


float Timer::GetTime()
{
	return m_frameTime;
}

float Timer::GetTotalTimePast()
{
	return m_totalTime;
}
