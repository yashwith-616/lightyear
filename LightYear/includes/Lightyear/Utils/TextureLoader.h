#pragma once

#include "Lightyear/LightyearAPI.h"
#include <lypch.h>

namespace ly {

class LIGHTYEAR_API TextureLoader {
public:
    explicit TextureLoader(const std::filesystem::path& filepath);

    [[nodiscard]] bool IsLoaded() const noexcept { return bIsInitialized_; }
    [[nodiscard]] int GetWidth() const noexcept { return width_; }
    [[nodiscard]] int GetHeight() const noexcept { return height_; }
    [[nodiscard]] int GetChannels() const noexcept { return nrChannels_; }
    [[nodiscard]] const unsigned char* GetData() const noexcept { return textureData_.data(); }

private:
    std::vector<unsigned char> textureData_;
    int width_ {}, height_ {}, nrChannels_ {};
    bool bIsInitialized_ { false };
};
}
