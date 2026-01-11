#pragma once

// TODO: Need significant refactoring
namespace ly
{

/**
 * @brief Tracks the time between frames (frametime)
 *
 * This class helps to keep track of the delta time between frames,
 * which can be used to update game logic or animations.
 */
class Timestep
{
public:
    Timestep(float time = 0.f) : m_Time(time) {}

    operator float() const { return m_Time; }

    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000.f; }

    void SetTime(float time) { m_Time = time; }

private:
    float m_Time;
};

} // namespace ly
