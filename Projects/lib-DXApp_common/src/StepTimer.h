/**
 * A simple timer that provides elapsed time information.
 *
 * @class StepTimer
 *
 * @file src/StepTimer.h
 * @author Chuck Walbourn
 * @date 20170809 - Initial Release
 * 
 * @author Olivier Falconner
 * @date 20200615 - Added log messages
 * @date 20211015 - Included to project lib-DXApp_common
 */

#ifndef DX_STEPTIMER
#define DX_STEPTIMER


#include "Logger.h"

namespace DX
{
    // Helper class for animation and simulation timing.
    class StepTimer
    {
    public:
        StepTimer() noexcept(false) :
            m_elapsedTicks(0),
            m_totalTicks(0),
            m_leftOverTicks(0),
            m_frameCount(0),
            m_framesPerSecond(0),
            m_framesThisSecond(0),
            m_qpcSecondCounter(0),
            m_isFixedTimeStep(false),
            m_targetElapsedTicks(TicksPerSecond / 60)
        {
            if (!QueryPerformanceFrequency(&m_qpcFrequency))
            {
                WriteLogMessage(nxn::SeverityFatal, "StepTimer::ctor() : QueryPerformanceFrequency");
                throw std::exception("QueryPerformanceFrequency");
            }

            if (!QueryPerformanceCounter(&m_qpcLastTime))
            {
                WriteLogMessage(nxn::SeverityFatal, "StepTimer::ctor() : QueryPerformanceCounter");
                throw std::exception("QueryPerformanceCounter");
            }

            // Initialize max delta to 1/10 of a second.
            m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10);
        }

        // Get elapsed time since the previous Update call.
        uint64_t GetElapsedTicks() const noexcept { return m_elapsedTicks; }
        double GetElapsedSeconds() const noexcept { return TicksToSeconds(m_elapsedTicks); }

        // Get total time since the start of the program.
        uint64_t GetTotalTicks() const noexcept { return m_totalTicks; }
        double GetTotalSeconds() const noexcept { return TicksToSeconds(m_totalTicks); }

        // Get total number of updates since start of the program.
        uint32_t GetFrameCount() const noexcept { return m_frameCount; }

        // Get the current framerate.
        uint32_t GetFramesPerSecond() const noexcept { return m_framesPerSecond; }

        // Set whether to use fixed or variable timestep mode.
        void SetFixedTimeStep(bool isFixedTimestep) noexcept { m_isFixedTimeStep = isFixedTimestep; }

        // Set how often to call Update when in fixed timestep mode.
        void SetTargetElapsedTicks(uint64_t targetElapsed) noexcept { m_targetElapsedTicks = targetElapsed; }
        void SetTargetElapsedSeconds(double targetElapsed) noexcept { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

        // Integer format represents time using 10,000,000 ticks per second.
        static const uint64_t TicksPerSecond = 10000000;

        static constexpr double TicksToSeconds(uint64_t ticks) noexcept { return static_cast<double>(ticks) / TicksPerSecond; }
        static constexpr uint64_t SecondsToTicks(double seconds) noexcept { return static_cast<uint64_t>(seconds * TicksPerSecond); }

        // After an intentional timing discontinuity (for instance a blocking IO operation)
        // call this to avoid having the fixed timestep logic attempt a set of catch-up
        // Update calls.

        void ResetElapsedTime()
        {
            if (!QueryPerformanceCounter(&m_qpcLastTime))
            {
//              WriteLogMessage(nxn::SeverityFatal, "StepTimer::ResetElapsedTime() : QueryPerformanceCounter");
                throw std::exception("QueryPerformanceCounter");
            }

            m_leftOverTicks = 0;
            m_framesPerSecond = 0;
            m_framesThisSecond = 0;
            m_qpcSecondCounter = 0;
        }

        // Update timer state, calling the specified Update function the appropriate number of times.
        template<typename TUpdate>
        void Tick(const TUpdate& update)
        {
            // Query the current time.
            LARGE_INTEGER currentTime;

            if (!QueryPerformanceCounter(&currentTime))
            {
                WriteLogMessage(nxn::SeverityFatal, "StepTimer::Tick() : QueryPerformanceCounter");
                throw std::exception("QueryPerformanceCounter");
            }

            uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - m_qpcLastTime.QuadPart);
            /*
            GetLogStream(SeverityInfo)
                << "StepTimer::Tick() : timeDelta = " << timeDelta << std::endl;
            //*/


            m_qpcLastTime = currentTime;
            m_qpcSecondCounter += timeDelta;

            // Clamp excessively large time deltas (e.g. after paused in the debugger).
            if (timeDelta > m_qpcMaxDelta)
            {
                timeDelta = m_qpcMaxDelta;
            }

            // Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
            timeDelta *= TicksPerSecond;
            timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart);

            uint32_t lastFrameCount = m_frameCount;

            if (m_isFixedTimeStep)
            {
                // Fixed timestep update logic

                // If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
                // the clock to exactly match the target value. This prevents tiny and irrelevant errors
                // from accumulating over time. Without this clamping, a game that requested a 60 fps
                // fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
                // accumulate enough tiny errors that it would drop a frame. It is better to just round
                // small deviations down to zero to leave things running smoothly.

                if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - m_targetElapsedTicks))) < TicksPerSecond / 4000)
                {
                    timeDelta = m_targetElapsedTicks;
                }

                m_leftOverTicks += timeDelta;
                /*
                GetLogStream(SeverityInfo)
                    << "StepTimer::Tick() : m_leftOverTicks = " << m_leftOverTicks << std::endl;
                //*/

                while (m_leftOverTicks >= m_targetElapsedTicks)
                {
                    m_elapsedTicks = m_targetElapsedTicks;
                    m_totalTicks += m_targetElapsedTicks;
                    m_leftOverTicks -= m_targetElapsedTicks;
                    m_frameCount++;

                    update();
                }
            }
            else
            {
                // Variable timestep update logic.
                m_elapsedTicks = timeDelta;
                m_totalTicks += timeDelta;
                m_leftOverTicks = 0;
                m_frameCount++;

                update();
            }

            /*
            GetLogStream(SeverityInfo)
                << "StepTimer::Tick() : m_leftOverTicks = " << m_leftOverTicks << std::endl;
            //*/

            // Track the current framerate.
            if (m_frameCount != lastFrameCount)
            {
                m_framesThisSecond++;
            }

            if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart))
            {
                m_framesPerSecond = m_framesThisSecond;
                m_framesThisSecond = 0;
                m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart);
            }
        }

    private:
        // Source timing data uses QPC units.
        LARGE_INTEGER m_qpcFrequency;
        LARGE_INTEGER m_qpcLastTime;
        uint64_t m_qpcMaxDelta;

        // Derived timing data uses a canonical tick format.
        uint64_t m_elapsedTicks;
        uint64_t m_totalTicks;
        uint64_t m_leftOverTicks;

        // Members for tracking the framerate.
        uint32_t m_frameCount;
        uint32_t m_framesPerSecond;
        uint32_t m_framesThisSecond;
        uint64_t m_qpcSecondCounter;

        // Members for configuring fixed timestep mode.
        bool m_isFixedTimeStep;
        uint64_t m_targetElapsedTicks;
    };
}


#endif // !DX_STEPTIMER

/*
 * The MIT License(MIT)
 *
 * Copyright(c) 2017 - 2021 Microsoft Corp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files(the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions :
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */