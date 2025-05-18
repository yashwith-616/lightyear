#pragma once

#include <entt/entt.hpp>
#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API SceneManager {
public:
    static SceneManager& Get();
    static entt::entity CreateEntity(const std::string& name);

    void Init();
    void DestoryEntity(entt::entity& entity);

    entt::registry& GetRegistry() { return *m_Registry; }

    uint32_t GetFrameIndex() const { return m_FrameIndex; }
    void IncrementFrame() { ++m_FrameIndex; }

private:
    SceneManager() = default;
    Scope<entt::registry> m_Registry;
    uint32_t m_FrameIndex;
};

}  // namespace ly