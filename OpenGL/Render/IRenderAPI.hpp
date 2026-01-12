/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** IRenderAPI.hpp - Hardware Abstraction Layer for graphics APIs
*/

#ifndef IRENDERAPI_HPP
#define IRENDERAPI_HPP

#include <Graphitech.hpp>
#include <cstdint>
#include <string>

namespace Graphitech {

    /**
     * @brief Enumeration of supported graphics APIs.
     */
    enum class RenderAPIType {
        None = 0,
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
        Metal
    };

    /**
     * @brief Blend modes for rendering operations.
     */
    enum class BlendMode {
        None,
        Alpha,
        Additive,
        Multiply
    };

    /**
     * @brief Primitive types for drawing operations.
     */
    enum class PrimitiveType {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };

    /**
     * @brief Color representation (RGBA).
     */
    struct Color {
        float r, g, b, a;
        Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}
    };

    /**
     * @brief Abstract interface for graphics API abstraction layer.
     *
     * @details This interface provides a hardware-agnostic way to interact
     * with different graphics APIs (OpenGL, Vulkan, DirectX, etc.).
     * All rendering operations should go through this interface to maintain
     * portability and flexibility.
     *
     * @ingroup Render
     */
    class IRenderAPI {
    public:
        virtual ~IRenderAPI() = default;

        /**
         * @brief Gets the type of this render API.
         * @return The API type (OpenGL, Vulkan, etc.).
         */
        virtual RenderAPIType GetType() const = 0;

        // ============ Initialization & Context ============

        /**
         * @brief Initializes the render API.
         * @return true if initialization succeeded, false otherwise.
         */
        virtual bool Initialize() = 0;

        /**
         * @brief Shuts down the render API and releases resources.
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Sets the current rendering context (if applicable).
         * @param contextHandle Platform-specific context handle.
         */
        virtual void MakeContextCurrent(void* contextHandle) = 0;

        // ============ State Management ============

        /**
         * @brief Sets the viewport dimensions.
         * @param x X coordinate of the lower-left corner.
         * @param y Y coordinate of the lower-left corner.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         */
        virtual void SetViewport(int x, int y, int width, int height) = 0;

        /**
         * @brief Sets up orthographic 2D projection.
         * @param left Left edge.
         * @param right Right edge.
         * @param bottom Bottom edge.
         * @param top Top edge.
         * @param near Near clipping plane.
         * @param far Far clipping plane.
         */
        virtual void SetOrtho2D(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f) = 0;

        /**
         * @brief Sets the clear color.
         * @param color Color to use when clearing.
         */
        virtual void SetClearColor(const Color& color) = 0;

        /**
         * @brief Clears the specified buffers.
         * @param colorBuffer Clear color buffer.
         * @param depthBuffer Clear depth buffer.
         */
        virtual void Clear(bool colorBuffer = true, bool depthBuffer = true) = 0;

        /**
         * @brief Sets the current draw color.
         * @param color Color to use for drawing.
         */
        virtual void SetDrawColor(const Color& color) = 0;

        /**
         * @brief Sets the line width for line drawing.
         * @param width Width in pixels.
         */
        virtual void SetLineWidth(float width) = 0;

        /**
         * @brief Sets the blend mode.
         * @param mode Blend mode to use.
         */
        virtual void SetBlendMode(BlendMode mode) = 0;

        // ============ Drawing Operations ============

        /**
         * @brief Begins immediate mode drawing (legacy-style).
         * @param type Primitive type to draw.
         */
        virtual void BeginDraw(PrimitiveType type) = 0;

        /**
         * @brief Ends immediate mode drawing.
         */
        virtual void EndDraw() = 0;

        /**
         * @brief Specifies a vertex position (immediate mode).
         * @param x X coordinate.
         * @param y Y coordinate.
         * @param z Z coordinate (default 0.0).
         */
        virtual void Vertex(float x, float y, float z = 0.0f) = 0;

        /**
         * @brief Specifies a texture coordinate (immediate mode).
         * @param u U coordinate.
         * @param v V coordinate.
         */
        virtual void TexCoord(float u, float v) = 0;

        /**
         * @brief Draws indexed geometry.
         * @param type Primitive type.
         * @param count Number of indices.
         * @param indexType Data type of indices (e.g., GL_UNSIGNED_INT).
         * @param indices Pointer to index data.
         */
        virtual void DrawElements(PrimitiveType type, int count, uint32_t indexType, const void* indices) = 0;

        /**
         * @brief Draws non-indexed geometry.
         * @param type Primitive type.
         * @param first Starting vertex.
         * @param count Number of vertices.
         */
        virtual void DrawArrays(PrimitiveType type, int first, int count) = 0;

        // ============ Texture Operations ============

        /**
         * @brief Creates a texture.
         * @param width Texture width.
         * @param height Texture height.
         * @param data Pixel data (RGBA).
         * @param generateMipmaps Whether to generate mipmaps.
         * @return Texture handle/ID.
         */
        virtual uint32_t CreateTexture(int width, int height, const void* data, bool generateMipmaps = true) = 0;

        /**
         * @brief Deletes a texture.
         * @param textureId Texture handle to delete.
         */
        virtual void DeleteTexture(uint32_t textureId) = 0;

        /**
         * @brief Binds a texture to the specified unit.
         * @param textureId Texture handle.
         * @param unit Texture unit (0-15 typically).
         */
        virtual void BindTexture(uint32_t textureId, uint32_t unit = 0) = 0;

        // ============ Shader Operations ============

        /**
         * @brief Creates a shader program.
         * @param vertexSource Vertex shader source code.
         * @param fragmentSource Fragment shader source code.
         * @return Shader program handle/ID.
         */
        virtual uint32_t CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) = 0;

        /**
         * @brief Deletes a shader program.
         * @param programId Shader program handle.
         */
        virtual void DeleteShaderProgram(uint32_t programId) = 0;

        /**
         * @brief Uses (activates) a shader program.
         * @param programId Shader program handle.
         */
        virtual void UseShaderProgram(uint32_t programId) = 0;

        /**
         * @brief Sets a uniform integer value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param value Value to set.
         */
        virtual void SetUniformInt(uint32_t programId, const std::string& name, int value) = 0;

        /**
         * @brief Sets a uniform float value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param value Value to set.
         */
        virtual void SetUniformFloat(uint32_t programId, const std::string& name, float value) = 0;

        /**
         * @brief Sets a uniform vec2 value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param x X component.
         * @param y Y component.
         */
        virtual void SetUniformVec2(uint32_t programId, const std::string& name, float x, float y) = 0;

        /**
         * @brief Sets a uniform vec3 value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param x X component.
         * @param y Y component.
         * @param z Z component.
         */
        virtual void SetUniformVec3(uint32_t programId, const std::string& name, float x, float y, float z) = 0;

        /**
         * @brief Sets a uniform vec4 value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param x X component.
         * @param y Y component.
         * @param z Z component.
         * @param w W component.
         */
        virtual void SetUniformVec4(uint32_t programId, const std::string& name, float x, float y, float z, float w) = 0;

        /**
         * @brief Sets a uniform mat4 value.
         * @param programId Shader program handle.
         * @param name Uniform name.
         * @param matrix Pointer to 16 floats representing the matrix.
         */
        virtual void SetUniformMat4(uint32_t programId, const std::string& name, const float* matrix) = 0;

        // ============ Error Handling ============

        /**
         * @brief Gets the last error message from the API.
         * @return Error message string, or empty if no error.
         */
        virtual std::string GetLastError() const = 0;
    };

} // namespace Graphitech

#endif // IRENDERAPI_HPP
