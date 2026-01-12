/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Texture.hpp
*/

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Graphitech.hpp>

namespace Graphitech {

    /**
     * @brief Configuration structure for texture creation.
     *
     * @details Defines the properties of a texture during initialization,
     * including dimensions and mipmapping settings.
     */
    struct TextureDescriptor final {
        /** @brief Width of the texture in pixels. */
        uint32_t width = 0u;

        /** @brief Height of the texture in pixels. */
        uint32_t height = 0u;

        /** @brief Whether to generate mipmaps for the texture. */
        bool generateMipMap = true;
    };

    /**
     * @brief Manages OpenGL textures and image loading.
     *
     * @details The Texture class provides functionality to load image files
     * into OpenGL textures, manage texture resources, and bind textures
     * for rendering. It supports various image formats through the stb_image
     * library and handles texture parameter setup automatically.
     *
     * @ingroup Render
     */
    class Texture final {
        public:
            /**
             * @brief Constructs a texture with the specified descriptor.
             *
             * @param desc Configuration for texture creation.
             */
            explicit Texture(const TextureDescriptor desc = {}) noexcept;

            /**
             * @brief Destructor that cleans up the OpenGL texture.
             */
            ~Texture();

            /**
             * @brief Loads an image file into the texture.
             *
             * @details Loads image data from the specified file path and
             * uploads it to the GPU. The path is relative to DEFAULT_PATH
             * if not absolute. Supports common image formats (PNG, JPEG, etc.).
             *
             * @param filePath Path to the image file to load.
             * @return true if loading succeeded, false otherwise.
             */
            bool Load(const std::string &filePath);

            /**
             * @brief Binds the texture to the specified texture unit.
             *
             * @details Makes this texture active on the specified texture unit
             * for use in shaders. Texture unit 0 is used by default.
             *
             * @param location Texture unit to bind to (0-15 typically).
             */
            void Bind(uint32_t location = 0u);

            /**
             * @brief Gets the texture configuration descriptor.
             *
             * @return Const reference to the texture descriptor.
             */
            const TextureDescriptor &GetDescriptor() const noexcept;

            /**
             * @brief Checks if the texture has been successfully loaded.
             *
             * @return true if the texture contains valid image data, false otherwise.
             */
            bool IsLoaded() const noexcept;

            /**
             * @brief Gets the internal OpenGL texture ID.
             *
             * @return The OpenGL texture object ID.
             */
            uint32_t GetInternalId() const noexcept;

            /** @brief Default path for texture files. */
            inline static const std::string DEFAULT_PATH = "clientAssets/Textures/";

        private:
            /** @brief Texture configuration and properties. */
            TextureDescriptor _desc;

            /** @brief Whether the texture has been loaded successfully. */
            bool _loaded = false;

            /** @brief OpenGL texture object ID. */
            uint32_t _internalId = 0u;
    };
}

#endif //TEXTURE_HPP
