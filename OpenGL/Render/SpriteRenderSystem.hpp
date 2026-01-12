/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SpriteRenderSystem.hpp - ECS system for rendering sprites with batch optimization
*/

#ifndef SPRITERENDERSYSTEM_HPP
#define SPRITERENDERSYSTEM_HPP

#include "SpriteBatch.hpp"
#include "TextureManager.hpp"
#include <glm/glm.hpp>
#include <cstdint>

namespace ECS { class Scene; class Entity; }

namespace Graphitech {

    /**
     * @brief ECS system for batch-optimized sprite rendering.
     *
     * @details Integrates the modern SpriteBatch with the ECS system.
     * Automatically batches sprites by texture to minimize draw calls.
     * 
     * Features:
     * - Automatic sorting by texture and z-order
     * - Per-sprite color tinting
     * - Efficient VAO/VBO management
     * - Frustum culling ready (stub implemented)
     *
     * @ingroup Render
     */
    class SpriteRenderSystem {
    public:
        /**
         * @brief Initializes the sprite render system.
         *
         * @return true if initialization succeeded
         */
        bool Initialize();

        /**
         * @brief Releases all GPU resources.
         */
        void Release();

        /**
         * @brief Renders all sprites in the scene using batch rendering.
         *
         * @param scene ECS scene containing sprite entities
         * @param logicalWidth Logical game world width
         * @param logicalHeight Logical game world height
         *
         * @details Accumulates all sprite entities, sorts by texture,
         * and issues optimized draw calls.
         */
        void RenderSprites(ECS::Scene& scene, float logicalWidth, float logicalHeight);

        /**
         * @brief Gets the sprite batch for manual control (advanced).
         */
        SpriteBatch& GetBatch() noexcept { return _batch; }

        /**
         * @brief Gets the texture manager.
         */
        TextureManager& GetTextureManager() noexcept { 
            return TextureManager::Get(); 
        }

        /**
         * @brief Gets statistics from last render.
         */
        uint32_t GetLastSpriteCount() const noexcept { return _lastSpriteCount; }
        uint32_t GetLastDrawCallCount() const noexcept { return _batch.GetLastDrawCallCount(); }

    private:
        SpriteBatch _batch;
        GLuint _shaderProgram = 0;
        uint32_t _lastSpriteCount = 0;

        /**
         * @brief Converts entity data to sprite draw call.
         *
         * @return true if conversion succeeded
         */
        bool EntityToSpriteDrawCall(ECS::Scene& scene, ECS::Entity entity, 
                                   SpriteDrawCall& outCall);

        /**
         * @brief Updates projection matrix for current viewport.
         */
        void UpdateProjection(float logicalWidth, float logicalHeight);
    };

} // namespace Graphitech

#endif // SPRITERENDERSYSTEM_HPP
