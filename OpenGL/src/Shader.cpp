/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Shader.cpp
*/
#include <Render/Shader.hpp>
#include <Debugger/Debug.hpp>

MOD("Core.Render.Shader");


Graphitech::Shader::~Shader()
{
    glDeleteProgram((this->_programId));
}

void Graphitech::Shader::Load(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexShaderFile.open(DEFAULT_PATH + vertexPath);
        fragmentShaderFile.open(DEFAULT_PATH + fragmentPath);

        std::stringstream vertexShaderStream;
        std::stringstream fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    } catch (std::ifstream::failure &e) { DEBUG_WARN("Shader file couldn't be read succesfully!"); }

    const char *vertexShaderCode = vertexCode.c_str();
    const char *fragmentShaderCode = fragmentCode.c_str();

    uint32_t vertexId = 0u;
    uint32_t fragmentId = 0u;

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexId);
    CheckCompileErrors(vertexId, "Vertex");

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentId);
    CheckCompileErrors(fragmentId, "Fragment");

    this->_programId = glCreateProgram();
    glAttachShader(this->_programId, vertexId);
    glAttachShader(this->_programId, fragmentId);
    glLinkProgram(this->_programId);
    CheckLinkErrors();

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Graphitech::Shader::Use()
{
    glUseProgram(this->_programId);
}

void Graphitech::Shader::SetUniform(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(_programId, name.c_str()), static_cast<int32_t>(value));
}

void Graphitech::Shader::SetUniform(const std::string& name, int32_t value) const
{
    glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
}

void Graphitech::Shader::SetUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
}

void Graphitech::Shader::SetUniform(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(_programId, name.c_str()), x, y);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::vec2& v) const
{
    glUniform2fv(glGetUniformLocation(_programId, name.c_str()), GL_TRUE, &v[0]);
}

void Graphitech::Shader::SetUniform(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_programId, name.c_str()), x, y, z);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::vec3& v) const
{
    glUniform3fv(glGetUniformLocation(_programId, name.c_str()), GL_TRUE, &v[0]);
}

void Graphitech::Shader::SetUniform(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(_programId, name.c_str()), x, y, z, w);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::vec4& v) const
{
    glUniform4fv(glGetUniformLocation(_programId, name.c_str()), GL_TRUE, &v[0]);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::mat2& m) const
{
    glUniformMatrix2fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::mat3& m) const
{
    glUniformMatrix3fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Graphitech::Shader::SetUniform(const std::string& name, const glm::mat4& m) const
{
    glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Graphitech::Shader::CheckCompileErrors(uint32_t shaderId, const std::string &type) const
{
    int32_t success = 0;
    char infoLog[1024];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
        DEBUG_WARN(type + " Shader compilation failed. InfoLog: \n" + std::string(infoLog));
    }
}

void Graphitech::Shader::CheckLinkErrors() const
{
    int32_t success = 0;
    char infoLog[1024];

    glGetProgramiv(this->_programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->_programId, 1024, nullptr, infoLog);
        DEBUG_WARN("Linking failed. InfoLog:\n" + std::string(infoLog));
    }
}
