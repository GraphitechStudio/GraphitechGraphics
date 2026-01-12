/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Shader.hpp
*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include "Graphitech.hpp"

namespace Graphitech {

    /**
     * @brief Manages OpenGL shader programs and uniform variables.
     *
     * @details The Shader class encapsulates the creation, compilation,
     * and management of OpenGL shader programs. It provides methods
     * to load vertex and fragment shaders from files, activate the
     * shader program, and set various types of uniform variables.
     *
     * The class includes comprehensive error checking for compilation
     * and linking phases to help with shader debugging.
     *
     * @ingroup Render
     */
    class Shader final {
        public:
            /**
             * @brief Destructor that cleans up the shader program.
             */
            ~Shader();

            /**
             * @brief Loads and compiles vertex and fragment shaders from files.
             *
             * @details Reads shader source code from the specified files,
             * compiles both shaders, and links them into a complete program.
             * Paths are relative to the DEFAULT_PATH if not absolute.
             *
             * @param vertexPath Path to the vertex shader file.
             * @param fragmentPath Path to the fragment shader file.
             */
            void Load(const std::string &vertexPath, const std::string &fragmentPath);

            /**
             * @brief Activates this shader program for rendering.
             *
             * @details Makes this shader program the current active program
             * in the OpenGL context. All subsequent rendering will use
             * this shader until another shader is activated.
             */
            void Use();

            /**
             * @brief Sets a boolean uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param value Boolean value to set.
             */
            void SetUniform(const std::string& name, bool value) const;

            /**
             * @brief Sets an integer uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param value Integer value to set.
             */
            void SetUniform(const std::string& name, int32_t value) const;

            /**
             * @brief Sets a float uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param value Float value to set.
             */
            void SetUniform(const std::string& name, float value) const;

            /**
             * @brief Sets a 2D vector uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param x X component of the vector.
             * @param y Y component of the vector.
             */
            void SetUniform(const std::string& name, float x, float y) const;

            /**
             * @brief Sets a 2D vector uniform variable using glm::vec2.
             *
             * @param name Name of the uniform variable in the shader.
             * @param v Vector value to set.
             */
            void SetUniform(const std::string& name, const glm::vec2& v) const;

            /**
             * @brief Sets a 3D vector uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param x X component of the vector.
             * @param y Y component of the vector.
             * @param z Z component of the vector.
             */
            void SetUniform(const std::string& name, float x, float y, float z) const;

            /**
             * @brief Sets a 3D vector uniform variable using glm::vec3.
             *
             * @param name Name of the uniform variable in the shader.
             * @param v Vector value to set.
             */
            void SetUniform(const std::string& name, const glm::vec3& v) const;

            /**
             * @brief Sets a 4D vector uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param x X component of the vector.
             * @param y Y component of the vector.
             * @param z Z component of the vector.
             * @param w W component of the vector.
             */
            void SetUniform(const std::string& name, float x, float y, float z, float w) const;

            /**
             * @brief Sets a 4D vector uniform variable using glm::vec4.
             *
             * @param name Name of the uniform variable in the shader.
             * @param v Vector value to set.
             */
            void SetUniform(const std::string& name, const glm::vec4& v) const;

            /**
             * @brief Sets a 2x2 matrix uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param m Matrix value to set.
             */
            void SetUniform(const std::string& name, const glm::mat2& m) const;

            /**
             * @brief Sets a 3x3 matrix uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param m Matrix value to set.
             */
            void SetUniform(const std::string& name, const glm::mat3& m) const;

            /**
             * @brief Sets a 4x4 matrix uniform variable.
             *
             * @param name Name of the uniform variable in the shader.
             * @param m Matrix value to set.
             */
            void SetUniform(const std::string& name, const glm::mat4& m) const;

            /** @brief Default path for shader files. */
            inline static const std::string DEFAULT_PATH = "clientAssets/Shaders/";

        private:
            /**
             * @brief Checks for compilation errors in a shader.
             *
             * @param shaderId OpenGL shader object ID.
             * @param type Description of the shader type for error messages.
             */
            void CheckCompileErrors(uint32_t shaderId, const std::string &type) const;

            /**
             * @brief Checks for linking errors in the shader program.
             */
            void CheckLinkErrors() const;

            /** @brief OpenGL shader program ID. */
            uint32_t _programId = 0u;
    };
}


#endif //SHADER_HPP
