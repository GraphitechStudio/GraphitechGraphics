/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** TextureManager.cpp - RAII texture resource management implementation
*/

#include "TextureManager.hpp"
#include <iostream>
#include <filesystem>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

namespace Graphitech {

    // ============ Texture Class ============

    bool Texture::Create(int width, int height, const void* data, bool generateMipmaps) {
        if (_handle != 0) {
            std::cerr << "[Texture] Already created. Use Release() first.\n";
            return false;
        }

        if (width <= 0 || height <= 0) {
            std::cerr << "[Texture] Invalid dimensions: " << width << "x" << height << "\n";
            return false;
        }

        _width = width;
        _height = height;

        glGenTextures(1, &_handle);
        if (_handle == 0) {
            std::cerr << "[Texture] Failed to create OpenGL texture\n";
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, _handle);

        // Set texture wrapping and filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixel data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        if (generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "[Texture] OpenGL error: " << err << "\n";
            Release();
            return false;
        }

        std::cout << "[Texture] Created successfully (ID=" << _handle << ", " 
                  << width << "x" << height << ")\n";
        return true;
    }

    void Texture::Bind(uint32_t unit) const noexcept {
        if (_handle == 0) {
            std::cerr << "[Texture] Cannot bind - texture not created\n";
            return;
        }
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, _handle);
    }

    Texture::~Texture() {
        Release();
    }

    Texture::Texture(Texture&& other) noexcept
        : _handle(other._handle), _width(other._width), _height(other._height) {
        other._handle = 0;
        other._width = 0;
        other._height = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            Release();
            _handle = other._handle;
            _width = other._width;
            _height = other._height;
            other._handle = 0;
        }
        return *this;
    }

    void Texture::Release() {
        if (_handle != 0) {
            glDeleteTextures(1, &_handle);
            std::cout << "[Texture] Released (ID=" << _handle << ")\n";
            _handle = 0;
        }
    }

    // ============ TextureManager Singleton ============

    TextureManager& TextureManager::Get() {
        static TextureManager instance;
        return instance;
    }

    std::string TextureManager::ResolveTexturePath(const std::string& path) {
        // Check if path is absolute and exists
        if (std::filesystem::exists(path)) {
            return path;
        }

        // Try Assets/Textures/ relative path
        std::string relPath = "Assets/Textures/" + path;
        if (std::filesystem::exists(relPath)) {
            return relPath;
        }

        // Try just filename in Assets/Textures/
        std::filesystem::path p(path);
        relPath = "Assets/Textures/" + p.filename().string();
        if (std::filesystem::exists(relPath)) {
            return relPath;
        }

        return path; // Return original if not found - stbi_load will fail gracefully
    }

    TextureManager::TexturePtr TextureManager::LoadTexture(const std::string& path) {
        // Check cache first
        auto it = _cache.find(path);
        if (it != _cache.end()) {
            std::cout << "[TextureManager] Cache hit: " << path << "\n";
            return it->second;
        }

        // Resolve path
        std::string resolvedPath = ResolveTexturePath(path);

        // Load image file
        int width, height, channels;
        unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data) {
            std::cerr << "[TextureManager] Failed to load image: " << path 
                      << " (resolved: " << resolvedPath << ")\n";
            return nullptr;
        }

        // Create texture
        auto texture = std::make_shared<Texture>();
        if (!texture->Create(width, height, data, true)) {
            stbi_image_free(data);
            return nullptr;
        }

        stbi_image_free(data);

        // Cache and track memory
        _cache[path] = texture;
        _memoryUsage += width * height * 4; // Estimate RGBA storage

        std::cout << "[TextureManager] Loaded and cached: " << path 
                  << " (" << width << "x" << height << ")\n";
        return texture;
    }

    TextureManager::TexturePtr TextureManager::CreateTexture(int width, int height, const void* data, 
                                                             const std::string& key) {
        auto texture = std::make_shared<Texture>();
        if (!texture->Create(width, height, data, true)) {
            return nullptr;
        }

        if (!key.empty()) {
            _cache[key] = texture;
            _memoryUsage += width * height * 4;
        }

        return texture;
    }

    void TextureManager::ClearCache() {
        _cache.clear();
        _memoryUsage = 0;
        std::cout << "[TextureManager] Cache cleared\n";
    }

} // namespace Graphitech
