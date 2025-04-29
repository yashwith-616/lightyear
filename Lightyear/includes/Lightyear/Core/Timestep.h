#pragma once

namespace ly {

/**
 * @brief Tracks timesteps in the game
 *
 * // TODO: Update timestep to use chrono, make default unit as ms
 *
 */
class Timestep {
public:
    Timestep(float time = 0.f) : m_Time(time) {}

    operator float() const { return m_Time; }

    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000.f; }

private:
    float m_Time;
};

}  // namespace ly