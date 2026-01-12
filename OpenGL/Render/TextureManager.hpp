/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** TextureManager.hpp - RAII texture resource management
*/

#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace Graphitech {

    /**
     * @brief RAII wrapper for OpenGL texture resources.
     *
     * @details Guarantees automatic cleanup of GPU texture memory
     * when the object is destroyed. No manual glDeleteTextures() needed.
     */
    class Texture {
    public:
        /**
         * @brief Creates a texture from raw pixel data.
         *
         * @param width Texture width in pixels
         * @param height Texture height in pixels
         * @param data Pixel data (RGBA format, 8-bit per channel)
         * @param generateMipmaps Whether to generate mipmaps
         * @return true if creation succeeded
         */
        bool Create(int width, int height, const void* data, bool generateMipmaps = true);

        /**
         * @brief Gets the OpenGL texture handle.
         *
         * @return Texture ID for glBindTexture()
         */
        GLuint GetHandle() const noexcept { return _handle; }

        /**
         * @brief Gets texture width in pixels.
         */
        int GetWidth() const noexcept { return _width; }

        /**
         * @brief Gets texture height in pixels.
         */
        int GetHeight() const noexcept { return _height; }

        /**
         * @brief Binds texture to a specific texture unit.
         *
         * @param unit Texture unit (0-31, typically)
         */
        void Bind(uint32_t unit = 0) const noexcept;

        /**
         * @brief Checks if texture is valid.
         */
        bool IsValid() const noexcept { return _handle != 0; }

        // Constructor/Destructor with RAII cleanup
        Texture() = default;
        ~Texture();

        // Delete copy - GPU resources cannot be duplicated
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        // Allow move semantics for efficient transfer
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

    private:
        GLuint _handle = 0;
        int _width = 0;
        int _height = 0;

        void Release();
    };

    /**
     * @brief Singleton texture manager with caching and reference counting.
     *
     * @details Manages texture loading and caching to avoid duplicate GPU uploads.
     * Provides RAII texture objects through std::shared_ptr for automatic cleanup.
     *
     * Usage:
     * @code
     * auto texture = TextureManager::Get().LoadTexture("assets/sprite.png");
     * if (texture) {
     *     texture->Bind(0);
     *     // render...
     * }
     * // Automatically cleaned up when shared_ptr goes out of scope
     * @endcode
     *
     * @ingroup Render
     */
    class TextureManager {
    public:
        using TexturePtr = std::shared_ptr<Texture>;

        /**
         * @brief Gets the singleton instance.
         */
        static TextureManager& Get();

        /**
         * @brief Loads texture from file with caching.
         *
         * @param path File path to texture (absolute or relative to assets/)
         * @return Shared pointer to texture, or nullptr on failure
         *
         * @details If texture is already loaded, returns cached copy.
         * Reference counted - GPU memory freed when all references are released.
         */
        TexturePtr LoadTexture(const std::string& path);

        /**
         * @brief Creates a texture from raw pixel data.
         *
         * @param width Texture width
         * @param height Texture height
         * @param data Pixel data (RGBA format)
         * @param key Cache key for future lookups (optional)
         * @return Shared pointer to texture
         */
        TexturePtr CreateTexture(int width, int height, const void* data, 
                                const std::string& key = "");

        /**
         * @brief Clears all cached textures.
         *
         * @details Frees GPU memory for all textures not referenced elsewhere.
         */
        void ClearCache();

        /**
         * @brief Gets cache size (number of cached textures).
         */
        size_t GetCacheSize() const noexcept { return _cache.size(); }

        /**
         * @brief Gets total GPU memory used (estimate in bytes).
         */
        size_t GetMemoryUsage() const noexcept { return _memoryUsage; }

    private:
        TextureManager() = default;
        ~TextureManager() { ClearCache(); }

        // Delete copy - singleton
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        std::unordered_map<std::string, TexturePtr> _cache;
        size_t _memoryUsage = 0;

        std::string ResolveTexturePath(const std::string& path);
    };

} // namespace Graphitech

#endif // TEXTUREMANAGER_HPP
