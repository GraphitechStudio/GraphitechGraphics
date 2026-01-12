/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** VertexArray.cpp
*/
#include <Render/Pipeline/VertexArray.hpp>

void Graphitech::VertexArray::Submit()
{
    glGenVertexArrays(1, &this->_vaoId);
    glGenBuffers(1, &this->_vboId);

    if (this->_indexSize)
        glGenBuffers(1, &this->_iboId);
    glBindVertexArray(this->_vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vboId);
    glBufferData(GL_ARRAY_BUFFER, this->_vertexSize, this->_vertexData, GL_STATIC_DRAW);

    if (this->_indexSize) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indexSize, this->_indexData, GL_STATIC_DRAW);
    }

    uint8_t totalAttributesCount = 0u;
    for (size_t i = 0; i < this->_vertexLayoutMap.size(); ++i)
        totalAttributesCount += this->_vertexLayoutMap[static_cast<VertexAttributeId>(i)]._attributeCount;
    uint32_t count = 0u;
    for (size_t i = 0; i < this->_vertexLayoutMap.size(); ++i) {
        auto key = static_cast<VertexAttributeId>(i);
        GLenum type = 0u;
        uint64_t size = 0ull;

        switch (this->_vertexLayoutMap[key]._attributeType) {
            case VertexAttributeType::Float:
                type = GL_FLOAT;
                size = sizeof(float);
                break;
            default:
                break;
        }
        glVertexAttribPointer(i, this->_vertexLayoutMap[key]._attributeCount, type, this->_vertexLayoutMap[key]._normalized, static_cast<GLsizei>(totalAttributesCount * size), reinterpret_cast<void *>(count * size));

        glEnableVertexAttribArray(i);
        count += this->_vertexLayoutMap[key]._attributeCount;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Graphitech::VertexArray::Bind() { glBindVertexArray(this->_vaoId); }
void Graphitech::VertexArray::Draw()
{
    if (this->_indexSize)
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->_indexSize), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->_vertexSize));
}
void Graphitech::VertexArray::Release()
{
    if (this->_vaoId)
        glDeleteVertexArrays(1, &this->_vaoId);
    if (this->_vboId)
        glDeleteBuffers(1, &this->_vboId);
    if (this->_iboId)
        glDeleteBuffers(1, &this->_iboId);
    this->_vertexData = nullptr;
    this->_indexData = nullptr;
}
void Graphitech::VertexArray::SetVertexBufferData(void *data, uint32_t size, const VertexLayoutMap &vertexLayoutMap)
{
    if (data != nullptr && size > 0u && !vertexLayoutMap.empty()) {
        this->_vertexData = data;
        this->_vertexSize = size;
        this->_vertexLayoutMap = vertexLayoutMap;
    }
}
void Graphitech::VertexArray::SetIndexBufferData(void *data, uint32_t size)
{
    if (data != nullptr && size > 0u) {
        this->_indexData = data;
        this->_indexSize = size;
    }
}

bool Graphitech::VertexArray::HasIndexBuffer() const noexcept
{
    return this->_indexSize;
}
