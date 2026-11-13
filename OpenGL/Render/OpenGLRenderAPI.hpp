/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** OpenGLRenderAPI.hpp - OpenGL implementation of the render API
*/

#ifndef OPENGLRENDERAPI_HPP
#define OPENGLRENDERAPI_HPP

#include "IRenderAPI.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>

// macOS only offers OpenGL core profiles (3.2+) without the fixed-function
// pipeline, so immediate-mode drawing is emulated with a small shader there.
#if defined(__APPLE__)
#define GRAPHITECH_GL_CORE_PROFILE 1
#endif

namespace Graphitech {

    /**
     * @brief OpenGL implementation of the IRenderAPI interface.
     *
     * @details This class implements the render API abstraction layer using
     * OpenGL 3.3 (compatibility profile). It provides all the rendering
     * capabilities through native OpenGL calls.
     *
     * @ingroup Render
     */
    class OpenGLRenderAPI : public IRenderAPI {
    public:
        OpenGLRenderAPI() = default;
        ~OpenGLRenderAPI() override = default;

        // ============ Base Interface ============
        RenderAPIType GetType() const override { return RenderAPIType::OpenGL; }

        // ============ Initialization & Context ============
        bool Initialize() override;
        void Shutdown() override;
        void MakeContextCurrent(void* contextHandle) override;

        // ============ State Management ============
        void SetViewport(int x, int y, int width, int height) override;
        void SetOrtho2D(float left, float right, float bottom, float top, float near, float far) override;
        void SetClearColor(const Color& color) override;
        void Clear(bool colorBuffer, bool depthBuffer) override;
        void SetDrawColor(const Color& color) override;
        void SetLineWidth(float width) override;
        void SetBlendMode(BlendMode mode) override;

        // ============ Drawing Operations ============
        void BeginDraw(PrimitiveType type) override;
        void EndDraw() override;
        void Vertex(float x, float y, float z) override;
        void TexCoord(float u, float v) override;
        void DrawElements(PrimitiveType type, int count, uint32_t indexType, const void* indices) override;
        void DrawArrays(PrimitiveType type, int first, int count) override;

        // ============ Texture Operations ============
        uint32_t CreateTexture(int width, int height, const void* data, bool generateMipmaps) override;
        void DeleteTexture(uint32_t textureId) override;
        void BindTexture(uint32_t textureId, uint32_t unit) override;

        // ============ Shader Operations ============
        uint32_t CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) override;
        void DeleteShaderProgram(uint32_t programId) override;
        void UseShaderProgram(uint32_t programId) override;
        void SetUniformInt(uint32_t programId, const std::string& name, int value) override;
        void SetUniformFloat(uint32_t programId, const std::string& name, float value) override;
        void SetUniformVec2(uint32_t programId, const std::string& name, float x, float y) override;
        void SetUniformVec3(uint32_t programId, const std::string& name, float x, float y, float z) override;
        void SetUniformVec4(uint32_t programId, const std::string& name, float x, float y, float z, float w) override;
        void SetUniformMat4(uint32_t programId, const std::string& name, const float* matrix) override;

        // ============ Error Handling ============
        std::string GetLastError() const override;

    private:
        /**
         * @brief Converts PrimitiveType to OpenGL primitive type.
         */
        GLenum ToGLPrimitive(PrimitiveType type) const;

        /**
         * @brief Gets the uniform location for a shader program.
         */
        GLint GetUniformLocation(uint32_t programId, const std::string& name) const;

        /**
         * @brief Compiles a shader from source.
         */
        GLuint CompileShader(GLenum type, const std::string& source);

        /**
         * @brief Cache for uniform locations to avoid repeated lookups.
         */
        mutable std::unordered_map<uint32_t, std::unordered_map<std::string, GLint>> uniformCache_;

        /**
         * @brief Last error message.
         */
        mutable std::string lastError_;

        /**
         * @brief Current draw color.
         */
        Color currentColor_{1.0f, 1.0f, 1.0f, 1.0f};

        /**
         * @brief Whether the API has been initialized.
         */
        bool initialized_ = false;

#if defined(GRAPHITECH_GL_CORE_PROFILE)
        // Immediate-mode emulation (core profile)
        bool InitImmediateEmulation();
        float projection_[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
        std::vector<float> immVertices_;   // x, y, z, u, v, r, g, b, a
        GLenum immPrimitive_ = GL_TRIANGLES;
        bool immHasTexCoord_ = false;
        float immU_ = 0.0f, immV_ = 0.0f;
        GLuint immProgram_ = 0, immVao_ = 0, immVbo_ = 0;
#endif
    };

} // namespace Graphitech

#endif // OPENGLRENDERAPI_HPP
