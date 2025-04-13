#include "Lightyear/Utils/TextureLoader.h"
#include "Lightyear/Utils/Log.h"
#include <glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Lightyear::utils {

TextureLoader::TextureLoader(const std::filesystem::path& filepath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* rawData = stbi_load(filepath.string().c_str(), &width_, &height_, &nrChannels_, 0);
    if (rawData) {
        size_t imageSize = width_ * height_ * nrChannels_;
        textureData_.resize(imageSize);
        std::memcpy(textureData_.data(), rawData, imageSize);
        stbi_image_free(rawData);
        bIsInitialized_ = true;
    } else {
        LY_CLIENT_ERROR("ERROR::TEXTURE_FAILES_TO_LOAD");
    }
}

}
