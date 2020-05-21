/**
 * @file High precision timer.
 *
 * @author Olivier Falconnet
 */

#pragma once

#include <windows.h>

class Timer {

public:
	/**
	 * Timer constructor
	 */
	Timer();

	/**
	 * Timer de-constructor
	 */
	~Timer();

	/**
	 * Initialize the timer
	 */
	void init();

	/**
	 * Resets the timer
	 */
	void reset();

	/**
	 * @returns {float} The duration in milliseconds since the timer has been initialized/reset
	 */
	float getElapsedTime() const;

private:
	INT64 mFrequency;
	float mTicksPerMs;

	INT64 mStartTime;

	/**
	 * Timer copy constructor
	 * is private because we do not allow copying items of this class
	 */
	Timer(const Timer&);  // Intentionally undefined

	/**
	 * Timer assignment operator
	 * is private because we do not allow copying items of this class
	 */
	Timer& operator=(const Timer&); // Intentionally undefined

}; // class Timer