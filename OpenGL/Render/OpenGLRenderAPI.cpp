/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** OpenGLRenderAPI.cpp - OpenGL implementation of the render API
*/

#include "OpenGLRenderAPI.hpp"
#include <iostream>
#include <sstream>

namespace Graphitech {

    bool OpenGLRenderAPI::Initialize() {
        if (initialized_) {
            return true;
        }

        // GLAD should already be initialized by the window/context creation
        // This is just a verification step
        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            lastError_ = "Failed to initialize GLAD";
            return false;
        }

        // Enable some default states
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initialized_ = true;
        return true;
    }

    void OpenGLRenderAPI::Shutdown() {
        uniformCache_.clear();
        initialized_ = false;
    }

    void OpenGLRenderAPI::MakeContextCurrent(void* contextHandle) {
        if (contextHandle) {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(contextHandle));
        }
    }

    // ============ State Management ============

    void OpenGLRenderAPI::SetViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderAPI::SetOrtho2D(float left, float right, float bottom, float top, float near, float far) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(left, right, bottom, top, near, far);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void OpenGLRenderAPI::SetClearColor(const Color& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderAPI::Clear(bool colorBuffer, bool depthBuffer) {
        GLbitfield mask = 0;
        if (colorBuffer) mask |= GL_COLOR_BUFFER_BIT;
        if (depthBuffer) mask |= GL_DEPTH_BUFFER_BIT;
        if (mask) glClear(mask);
    }

    void OpenGLRenderAPI::SetDrawColor(const Color& color) {
        currentColor_ = color;
        glColor4f(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderAPI::SetLineWidth(float width) {
        glLineWidth(width);
    }

    void OpenGLRenderAPI::SetBlendMode(BlendMode mode) {
        switch (mode) {
            case BlendMode::None:
                glDisable(GL_BLEND);
                break;
            case BlendMode::Alpha:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case BlendMode::Additive:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendMode::Multiply:
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                break;
        }
    }

    // ============ Drawing Operations ============

    void OpenGLRenderAPI::BeginDraw(PrimitiveType type) {
        glBegin(ToGLPrimitive(type));
    }

    void OpenGLRenderAPI::EndDraw() {
        glEnd();
    }

    void OpenGLRenderAPI::Vertex(float x, float y, float z) {
        glVertex3f(x, y, z);
    }

    void OpenGLRenderAPI::TexCoord(float u, float v) {
        glTexCoord2f(u, v);
    }

    void OpenGLRenderAPI::DrawElements(PrimitiveType type, int count, uint32_t indexType, const void* indices) {
        glDrawElements(ToGLPrimitive(type), count, indexType, indices);
    }

    void OpenGLRenderAPI::DrawArrays(PrimitiveType type, int first, int count) {
        glDrawArrays(ToGLPrimitive(type), first, count);
    }

    // ============ Texture Operations ============

    uint32_t OpenGLRenderAPI::CreateTexture(int width, int height, const void* data, bool generateMipmaps) {
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        if (generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        return textureId;
    }

    void OpenGLRenderAPI::DeleteTexture(uint32_t textureId) {
        GLuint id = textureId;
        glDeleteTextures(1, &id);
    }

    void OpenGLRenderAPI::BindTexture(uint32_t textureId, uint32_t unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    // ============ Shader Operations ============

    GLuint OpenGLRenderAPI::CompileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            lastError_ = std::string("Shader compilation failed: ") + infoLog;
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    uint32_t OpenGLRenderAPI::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader) return 0;

        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!fragmentShader) {
            glDeleteShader(vertexShader);
            return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            lastError_ = std::string("Shader linking failed: ") + infoLog;
            glDeleteProgram(program);
            program = 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void OpenGLRenderAPI::DeleteShaderProgram(uint32_t programId) {
        glDeleteProgram(programId);
        uniformCache_.erase(programId);
    }

    void OpenGLRenderAPI::UseShaderProgram(uint32_t programId) {
        glUseProgram(programId);
    }

    GLint OpenGLRenderAPI::GetUniformLocation(uint32_t programId, const std::string& name) const {
        auto& cache = uniformCache_[programId];
        auto it = cache.find(name);
        if (it != cache.end()) {
            return it->second;
        }

        GLint location = glGetUniformLocation(programId, name.c_str());
        cache[name] = location;
        return location;
    }

    void OpenGLRenderAPI::SetUniformInt(uint32_t programId, const std::string& name, int value) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniform1i(loc, value);
    }

    void OpenGLRenderAPI::SetUniformFloat(uint32_t programId, const std::string& name, float value) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniform1f(loc, value);
    }

    void OpenGLRenderAPI::SetUniformVec2(uint32_t programId, const std::string& name, float x, float y) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniform2f(loc, x, y);
    }

    void OpenGLRenderAPI::SetUniformVec3(uint32_t programId, const std::string& name, float x, float y, float z) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniform3f(loc, x, y, z);
    }

    void OpenGLRenderAPI::SetUniformVec4(uint32_t programId, const std::string& name, float x, float y, float z, float w) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniform4f(loc, x, y, z, w);
    }

    void OpenGLRenderAPI::SetUniformMat4(uint32_t programId, const std::string& name, const float* matrix) {
        GLint loc = GetUniformLocation(programId, name);
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
    }

    // ============ Error Handling ============

    std::string OpenGLRenderAPI::GetLastError() const {
        if (!lastError_.empty()) {
            return lastError_;
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::ostringstream oss;
            oss << "OpenGL Error: 0x" << std::hex << err;
            return oss.str();
        }

        return "";
    }

    // ============ Helper Methods ============

    GLenum OpenGLRenderAPI::ToGLPrimitive(PrimitiveType type) const {
        switch (type) {
            case PrimitiveType::Points: return GL_POINTS;
            case PrimitiveType::Lines: return GL_LINES;
            case PrimitiveType::LineStrip: return GL_LINE_STRIP;
            case PrimitiveType::Triangles: return GL_TRIANGLES;
            case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
            case PrimitiveType::TriangleFan: return GL_TRIANGLE_FAN;
            default: return GL_TRIANGLES;
        }
    }

} // namespace Graphitech
