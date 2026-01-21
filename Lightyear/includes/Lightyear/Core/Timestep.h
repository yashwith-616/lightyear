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
    Timestep(float time = 0.f) : m_time(time) {}

    operator float() const { return m_time; }

    float getSeconds() const { return m_time; }
    float getMilliseconds() const { return m_time * 1000.f; }

    void setTime(float time) { m_time = time; }

private:
    float m_time;
};

} // namespace ly
