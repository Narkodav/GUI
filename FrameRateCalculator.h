#pragma once
#include <list>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <iomanip>

#include "Utilities/SampleTracker.h"


class FrameRateCalculator
{
private:
	Utils::SampleTracker<float> m_frameTimes;
	float m_totalFrameTime = 0.f;
	float m_frameRate = 0.f;
	mutable std::shared_mutex m_mutex;

public:
	void setFrameTimeBuffer(int frameTimeBufferSize);
	void addFrameTime(float frameTime);
	float updateFrameRate();
	float getFrameRate();
	void printFrameRate();
};

