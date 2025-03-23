#include "FrameRateCalculator.h"


void FrameRateCalculator::setFrameTimeBuffer(int frameTimeBufferSize)
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_frameTimes.rename("FrameTimes");
    m_frameTimes.resize(frameTimeBufferSize);
}

void FrameRateCalculator::addFrameTime(float frameTime)
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_frameTimes.addSample(frameTime);
}

float FrameRateCalculator::updateFrameRate()
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    return m_frameRate = 1 / m_frameTimes.getAverage();
}

float FrameRateCalculator::getFrameRate()
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    return m_frameRate;
}

void FrameRateCalculator::printFrameRate() {
    // Clear the current line
    std::cout << '\r';

    // Format frame rate with 1 decimal place
    std::cout << std::fixed << std::setprecision(1);

    // Add color based on performance thresholds
    if (m_frameRate >= 60.0f) {
        std::cout << "\033[32m"; // Green
    }
    else if (m_frameRate >= 30.0f) {
        std::cout << "\033[33m"; // Yellow
    }
    else {
        std::cout << "\033[31m"; // Red
    }

    std::cout << "FPS: " << std::setw(6) << m_frameRate;

    // Reset color
    std::cout << "\033[0m";

    // Add frame time in milliseconds
    float frameTime = 1000.0f / m_frameRate;
    std::cout << " (" << std::setw(6) << frameTime << " ms)";

    // Flush without newline to keep updating the same line
    std::cout << std::flush;
}