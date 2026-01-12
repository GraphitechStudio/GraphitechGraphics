/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Texture.cpp
*/
#include <Render/Texture.hpp>
#include <Debugger/Debug.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

MOD("Core.Render.Texture");

Graphitech::Texture::Texture(const TextureDescriptor desc) noexcept : _desc(desc) {}

Graphitech::Texture::~Texture()
{
    glDeleteTextures(1, &this->_internalId);
}

bool Graphitech::Texture::Load(const std::string &filePath)
{
    glGenTextures(1, &this->_internalId);
    glBindTexture(GL_TEXTURE_2D, this->_internalId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int32_t width, height, channelCount;
    uint8_t *data = stbi_load((DEFAULT_PATH + filePath).c_str(), &width, &height, &channelCount, 0);

    if (!data) {
        DEBUG_WARN("Texture file doesn't exist!");
        return false;
    } else {
        if (channelCount == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if (channelCount == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else {
            DEBUG_WARN("Channel count " + std::to_string(channelCount) + " is not supported!");
            return false;
        }
    }
    this->_desc.width = static_cast<uint32_t>(width);
    this->_desc.height = static_cast<uint32_t>(height);
    this->_loaded = true;
    return true;
}

void Graphitech::Texture::Bind(uint32_t location)
{
    if (this->_loaded) {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, this->_internalId);
    }
}
const Graphitech::TextureDescriptor &Graphitech::Texture::GetDescriptor() const noexcept { return this->_desc; }
bool Graphitech::Texture::IsLoaded() const noexcept { return this->_loaded; }
uint32_t Graphitech::Texture::GetInternalId() const noexcept { return this->_internalId; }
