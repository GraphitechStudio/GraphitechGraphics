/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SpriteBatch.hpp - Modern sprite batching renderer with VAO/VBO RAII management
*/

#ifndef SPRITEBATCH_HPP
#define SPRITEBATCH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace Graphitech {

    /**
     * @brief Vertex structure for sprite batch rendering.
     *
     * @details Contains position (x, y), texture coordinates (u, v),
     * and color (RGBA) for each vertex in a sprite.
     * Aligned for optimal GPU memory access.
     *
     * @ingroup Render
     */
    struct SpriteVertex {
        glm::vec2 position;  /**< 2D screen position */
        glm::vec2 texCoord;  /**< Texture UV coordinates */
        glm::vec4 color;     /**< RGBA color */

        SpriteVertex() = default;
        SpriteVertex(glm::vec2 pos, glm::vec2 uv = {0.0f, 0.0f}, glm::vec4 col = {1.0f, 1.0f, 1.0f, 1.0f})
            : position(pos), texCoord(uv), color(col) {}
    };

    /**
     * @brief Entry point for rendering a single sprite in the batch.
     *
     * @details Contains all information needed to render a sprite,
     * including its quad vertices, texture binding, and sorting key.
     */
    struct SpriteDrawCall {
        std::array<SpriteVertex, 4> vertices; /**< Quad vertices (0-3 = BL, BR, TR, TL) */
        GLuint textureId = 0;                  /**< OpenGL texture handle */
        uint32_t zOrder = 0;                   /**< Depth for sorting (lower = rendered first) */
    };

    /**
     * @brief Modern OpenGL sprite batch renderer with RAII resource management.
     *
     * @details Accumulates sprite draw calls and renders them in optimized batches
     * to minimize state changes and draw calls. Implements:
     * - VAO/VBO management with automatic cleanup
     * - Dynamic vertex buffer resizing
     * - Texture-aware sorting to minimize texture binds
     * - OpenGL 3.3+ core profile support
     * - Type-safe RAII pattern for GPU resources
     *
     * Usage:
     * @code
     * SpriteBatch batch;
     * batch.Initialize(shader_program_id);
     * 
     * batch.PushSprite({...vertex data...}, textureId, zOrder);
     * batch.Flush(); // Renders all accumulated sprites
     * @endcode
     *
     * @ingroup Render
     */
    class SpriteBatch {
    public:
        /**
         * @brief Default constructor.
         *
         * @details Initializes an empty batch with no GPU resources allocated.
         * Call Initialize() to prepare for rendering.
         */
        SpriteBatch() = default;

        /**
         * @brief Destructor with RAII cleanup.
         *
         * @details Automatically releases all GPU resources (VAO, VBO, IBO)
         * when the batch goes out of scope. No manual cleanup required.
         */
        ~SpriteBatch();

        // Delete copy operations - GPU resources cannot be copied
        SpriteBatch(const SpriteBatch&) = delete;
        SpriteBatch& operator=(const SpriteBatch&) = delete;

        // Allow move operations for efficient resource transfer
        SpriteBatch(SpriteBatch&& other) noexcept;
        SpriteBatch& operator=(SpriteBatch&& other) noexcept;

        /**
         * @brief Initializes the batch with a shader program.
         *
         * @param shaderProgram OpenGL shader program ID for sprite rendering
         * @param maxSprites Maximum number of sprites in a single batch (default: 2048)
         * @return true if initialization succeeded, false on error
         *
         * @details Must be called before any rendering operations.
         * Creates VAO, VBO, and IBO with capacity for maxSprites.
         */
        bool Initialize(GLuint shaderProgram, uint32_t maxSprites = 2048);

        /**
         * @brief Releases all GPU resources.
         *
         * @details Frees VAO, VBO, and IBO. After cleanup, must call
         * Initialize() again before rendering.
         */
        void Release();

        /**
         * @brief Adds a sprite to the batch queue.
         *
         * @param sprite Sprite data including vertices, texture, and z-order
         *
         * @details Accumulates sprite for later rendering. If batch capacity
         * is exceeded, a flush occurs automatically. Sprites are sorted
         * by texture and z-order to minimize state changes.
         */
        void PushSprite(const SpriteDrawCall& sprite);

        /**
         * @brief Renders all accumulated sprites and clears the batch.
         *
         * @details Performs all necessary GPU state setup, texture binding,
         * and draw calls. Batch is empty after flush() returns.
         */
        void Flush();

        /**
         * @brief Clears accumulated sprites without rendering.
         *
         * @details Use this to discard pending sprites and start fresh.
         */
        void Clear();

        /**
         * @brief Returns the number of sprites currently in the batch.
         *
         * @return Sprite count
         */
        uint32_t GetSpriteCount() const noexcept { return _spriteCount; }

        /**
         * @brief Returns the total number of draw calls issued in last flush.
         *
         * @return Draw call count from last flush operation
         */
        uint32_t GetLastDrawCallCount() const noexcept { return _lastDrawCallCount; }

        /**
         * @brief Checks if the batch is ready for rendering.
         *
         * @return true if Initialize() was successful and no errors occurred
         */
        bool IsInitialized() const noexcept { return _vaoId != 0; }

    private:
        /**
         * @brief Sets up vertex array object attributes.
         *
         * @details Configures VAO with vertex attributes for position,
         * texture coordinates, and color.
         */
        void SetupVertexAttribs();

        /**
         * @brief Submits accumulated vertex data to GPU.
         *
         * @details Updates VBO with current vertex data buffer.
         */
        void SubmitVertexData();

        // GPU resource handles (RAII managed)
        GLuint _vaoId = 0;              /**< Vertex Array Object ID */
        GLuint _vboId = 0;              /**< Vertex Buffer Object ID */
        GLuint _iboId = 0;              /**< Index Buffer Object ID */
        GLuint _shaderProgram = 0;      /**< Linked shader program */

        // Batch state
        std::vector<SpriteVertex> _vertexBuffer;     /**< Accumulated vertex data */
        std::vector<SpriteDrawCall> _spriteQueue;    /**< Pending sprites to render */
        std::unordered_map<GLuint, std::vector<uint32_t>> _textureGroups; /**< Sprites grouped by texture */

        uint32_t _maxSprites = 2048;           /**< Maximum sprites per batch */
        uint32_t _spriteCount = 0;             /**< Current sprite count */
        uint32_t _lastDrawCallCount = 0;       /**< Draw calls from last flush */

        // Capacity management
        bool _needsResize = false;             /**< Flag for buffer resize requirement */
    };

} // namespace Graphitech

#endif // SPRITEBATCH_HPP
