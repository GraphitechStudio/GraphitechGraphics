/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SpriteBatchShader.hpp - Shader sources for sprite batch rendering
*/

#ifndef SPRITEBATCHSHADER_HPP
#define SPRITEBATCHSHADER_HPP

#include <string>
#include <iostream>
#include <glad/glad.h>

namespace Graphitech {

    /**
     * @brief Sprite batch vertex shader source code (GLSL 3.30+)
     *
     * @details Transforms 2D sprite vertices from world space to clip space.
     * Supports per-vertex color tinting and texture coordinate passthrough.
     */
    static constexpr const char* SPRITE_BATCH_VERTEX_SHADER = R"glsl(
        #version 330 core

        // Input vertex attributes
        layout (location = 0) in vec2 aPosition;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec4 aColor;

        // Output to fragment shader
        out VS_OUT {
            vec2 texCoord;
            vec4 color;
        } vs_out;

        // Uniforms
        uniform mat4 uProjection;

        void main()
        {
            // Transform position to clip space
            gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
            
            // Pass data to fragment shader
            vs_out.texCoord = aTexCoord;
            vs_out.color = aColor;
        }
    )glsl";

    /**
     * @brief Sprite batch fragment shader source code (GLSL 3.30+)
     *
     * @details Samples from texture and applies per-vertex color.
     * Supports both textured and solid color rendering.
     */
    static constexpr const char* SPRITE_BATCH_FRAGMENT_SHADER = R"glsl(
        #version 330 core

        // Input from vertex shader
        in VS_OUT {
            vec2 texCoord;
            vec4 color;
        } fs_in;

        // Output
        out vec4 FragColor;

        // Uniforms
        uniform sampler2D uTexture;
        uniform bool uUseTexture;

        void main()
        {
            if (uUseTexture) {
                // Sample texture and multiply by vertex color
                vec4 texColor = texture(uTexture, fs_in.texCoord);
                FragColor = texColor * fs_in.color;
            } else {
                // Use vertex color directly (fallback)
                FragColor = fs_in.color;
            }
        }
    )glsl";

    /**
     * @brief Helper function to compile shader source.
     *
     * @param source GLSL source code
     * @param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @return OpenGL shader object ID, or 0 on failure
     */
    inline GLuint CompileShader(const char* source, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        // Check compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            const char* typeStr = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
            std::cerr << "[" << typeStr << " Shader] Compilation failed:\n" 
                      << infoLog << "\n";
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    /**
     * @brief Helper function to link shader program.
     *
     * @param vertexShader Compiled vertex shader ID
     * @param fragmentShader Compiled fragment shader ID
     * @return OpenGL program object ID, or 0 on failure
     */
    inline GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Check linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "[Shader Program] Linking failed:\n" << infoLog << "\n";
            glDeleteProgram(program);
            return 0;
        }

        // Clean up shaders (they're now linked into the program)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    /**
     * @brief Creates the sprite batch shader program.
     *
     * @return OpenGL program object ID, or 0 on failure
     *
     * @details Compiles and links the vertex and fragment shaders
     * for sprite batch rendering.
     */
    inline GLuint CreateSpriteBatchShader() {
        #include <glad/glad.h>

        GLuint vs = CompileShader(SPRITE_BATCH_VERTEX_SHADER, GL_VERTEX_SHADER);
        if (vs == 0) return 0;

        GLuint fs = CompileShader(SPRITE_BATCH_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        if (fs == 0) {
            glDeleteShader(vs);
            return 0;
        }

        GLuint program = LinkShaderProgram(vs, fs);
        if (program != 0) {
            std::cout << "[SpriteBatchShader] Program created successfully (ID=" 
                      << program << ")\n";
        }
        return program;
    }

} // namespace Graphitech

#endif // SPRITEBATCHSHADER_HPP
