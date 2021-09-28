/**
 * High precision timer.
 *
 * @class Timer
 *
 *
 * @file src/Timer.h
 * @author Olivier Falconnet
 * @date 20200521 - File creation
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <windows.h>


namespace nxn {


class Timer {

public:
	        Timer           ();
	        ~Timer          ();

	void    init            ();
	void    reset           ();

	float   getElapsedTime  () const;

private:
            Timer           (const Timer&); // Intentionally undefined. Is private because we do not allow copying items of this class.
    Timer&  operator=       (const Timer&); // Intentionally undefined. Is private because we do not allow copying items of this class.

	INT64   m_frequency;
	float   m_ticksPerMs;

	INT64   m_startTime;

}; // class Timer

} // namespace nxn
