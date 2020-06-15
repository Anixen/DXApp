#include "Timer.h"
#include "LoggerBase.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::init()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)& m_frequency);
	if (m_frequency == 0)
	{
		LogMessage(SeverityFatal, "Timer::init() : This system doesn t support high performance timers");
		throw new std::exception("This system doesn t support high performance timers");
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float) (m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)& m_startTime);
}

void Timer::reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*)& m_startTime);
}

float Timer::getElapsedTime() const
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	float timeDifference = (float)(currentTime - m_startTime);
	//LogMessage(SeverityDebug, "Timer::getElapsedTime() : timeDifference = " + std::to_string(timeDifference));

	float elapsedTime = timeDifference / m_ticksPerMs;
	//LogMessage(SeverityDebug, "Timer::getElapsedTime() : elapsedTime = " + std::to_string(elapsedTime));

	return elapsedTime;
}