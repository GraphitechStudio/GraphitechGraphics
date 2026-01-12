/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SpriteBatch.cpp - Modern sprite batching implementation
*/

#include "SpriteBatch.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>

namespace Graphitech {

    SpriteBatch::~SpriteBatch() {
        Release();
    }

    SpriteBatch::SpriteBatch(SpriteBatch&& other) noexcept
        : _vaoId(other._vaoId), _vboId(other._vboId), _iboId(other._iboId),
          _shaderProgram(other._shaderProgram), _vertexBuffer(std::move(other._vertexBuffer)),
          _spriteQueue(std::move(other._spriteQueue)), _textureGroups(std::move(other._textureGroups)),
          _maxSprites(other._maxSprites), _spriteCount(other._spriteCount),
          _lastDrawCallCount(other._lastDrawCallCount), _needsResize(other._needsResize) {
        other._vaoId = 0;
        other._vboId = 0;
        other._iboId = 0;
        other._shaderProgram = 0;
        other._spriteCount = 0;
    }

    SpriteBatch& SpriteBatch::operator=(SpriteBatch&& other) noexcept {
        if (this != &other) {
            Release();
            _vaoId = other._vaoId;
            _vboId = other._vboId;
            _iboId = other._iboId;
            _shaderProgram = other._shaderProgram;
            _vertexBuffer = std::move(other._vertexBuffer);
            _spriteQueue = std::move(other._spriteQueue);
            _textureGroups = std::move(other._textureGroups);
            _maxSprites = other._maxSprites;
            _spriteCount = other._spriteCount;
            _lastDrawCallCount = other._lastDrawCallCount;
            _needsResize = other._needsResize;

            other._vaoId = 0;
            other._vboId = 0;
            other._iboId = 0;
            other._shaderProgram = 0;
            other._spriteCount = 0;
        }
        return *this;
    }

    bool SpriteBatch::Initialize(GLuint shaderProgram, uint32_t maxSprites) {
        if (_vaoId != 0) {
            std::cerr << "[SpriteBatch] Already initialized. Call Release() first.\n";
            return false;
        }

        _shaderProgram = shaderProgram;
        _maxSprites = maxSprites;

        // Reserve space for vertex and index data
        // Each sprite = 4 vertices + 6 indices (2 triangles)
        _vertexBuffer.reserve(maxSprites * 4);
        _spriteQueue.reserve(maxSprites);

        // Create Vertex Array Object
        glGenVertexArrays(1, &_vaoId);
        if (_vaoId == 0) {
            std::cerr << "[SpriteBatch] Failed to create VAO\n";
            return false;
        }

        glBindVertexArray(_vaoId);

        // Create Vertex Buffer Object
        glGenBuffers(1, &_vboId);
        if (_vboId == 0) {
            std::cerr << "[SpriteBatch] Failed to create VBO\n";
            glDeleteVertexArrays(1, &_vaoId);
            _vaoId = 0;
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        // Allocate with initial capacity
        glBufferData(GL_ARRAY_BUFFER, maxSprites * 4 * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);

        // Create Index Buffer Object
        glGenBuffers(1, &_iboId);
        if (_iboId == 0) {
            std::cerr << "[SpriteBatch] Failed to create IBO\n";
            glDeleteBuffers(1, &_vboId);
            glDeleteVertexArrays(1, &_vaoId);
            _vboId = 0;
            _vaoId = 0;
            return false;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
        // Generate and upload quad indices
        std::vector<uint32_t> indices;
        indices.reserve(maxSprites * 6);
        for (uint32_t i = 0; i < maxSprites; ++i) {
            uint32_t baseIdx = i * 4;
            indices.push_back(baseIdx);
            indices.push_back(baseIdx + 1);
            indices.push_back(baseIdx + 2);
            indices.push_back(baseIdx);
            indices.push_back(baseIdx + 2);
            indices.push_back(baseIdx + 3);
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        // Setup vertex attributes
        SetupVertexAttribs();

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "[SpriteBatch] OpenGL error during initialization: " << err << "\n";
            Release();
            return false;
        }

        std::cout << "[SpriteBatch] Initialized successfully (VAO=" << _vaoId 
                  << ", VBO=" << _vboId << ", IBO=" << _iboId 
                  << ", capacity=" << maxSprites << " sprites)\n";
        return true;
    }

    void SpriteBatch::SetupVertexAttribs() {
        // Position attribute (vec2)
        GLint posLoc = glGetAttribLocation(_shaderProgram, "aPosition");
        if (posLoc >= 0) {
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
                                  (void*)offsetof(SpriteVertex, position));
        }

        // TexCoord attribute (vec2)
        GLint texLoc = glGetAttribLocation(_shaderProgram, "aTexCoord");
        if (texLoc >= 0) {
            glEnableVertexAttribArray(texLoc);
            glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
                                  (void*)offsetof(SpriteVertex, texCoord));
        }

        // Color attribute (vec4)
        GLint colLoc = glGetAttribLocation(_shaderProgram, "aColor");
        if (colLoc >= 0) {
            glEnableVertexAttribArray(colLoc);
            glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
                                  (void*)offsetof(SpriteVertex, color));
        }
    }

    void SpriteBatch::Release() {
        if (_iboId != 0) {
            glDeleteBuffers(1, &_iboId);
            _iboId = 0;
        }
        if (_vboId != 0) {
            glDeleteBuffers(1, &_vboId);
            _vboId = 0;
        }
        if (_vaoId != 0) {
            glDeleteVertexArrays(1, &_vaoId);
            _vaoId = 0;
        }

        _vertexBuffer.clear();
        _spriteQueue.clear();
        _textureGroups.clear();
        _spriteCount = 0;
        _shaderProgram = 0;

        GLenum err = glGetError();
        if (err == GL_NO_ERROR) {
            std::cout << "[SpriteBatch] Released successfully\n";
        }
    }

    void SpriteBatch::PushSprite(const SpriteDrawCall& sprite) {
        if (_vaoId == 0) {
            std::cerr << "[SpriteBatch] Cannot push sprite - batch not initialized\n";
            return;
        }

        // Check capacity
        if (_spriteCount >= _maxSprites) {
            std::cerr << "[SpriteBatch] Batch capacity exceeded (" << _maxSprites 
                      << "). Flushing early.\n";
            Flush();
        }

        // Add to queue
        _spriteQueue.push_back(sprite);
        _spriteCount++;

        // Track texture groups for optimization
        if (sprite.textureId != 0) {
            _textureGroups[sprite.textureId].push_back(_spriteCount - 1);
        }
    }

    void SpriteBatch::SubmitVertexData() {
        if (_vertexBuffer.empty()) {
            return;
        }

        glBindBuffer(GL_COPY_WRITE_BUFFER, _vboId);
        size_t dataSize = _vertexBuffer.size() * sizeof(SpriteVertex);
        
        // Check if we need to resize
        if (_needsResize) {
            glBufferData(GL_COPY_WRITE_BUFFER, dataSize * 2, nullptr, GL_DYNAMIC_DRAW);
            _needsResize = false;
        }

        glBufferSubData(GL_COPY_WRITE_BUFFER, 0, dataSize, _vertexBuffer.data());
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    void SpriteBatch::Flush() {
        if (_spriteCount == 0) {
            return;
        }

        // Convert sprite queue to vertex data
        _vertexBuffer.clear();
        for (const auto& sprite : _spriteQueue) {
            for (const auto& vertex : sprite.vertices) {
                _vertexBuffer.push_back(vertex);
            }
        }

        // Submit vertex data to GPU
        glBindVertexArray(_vaoId);
        glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        SubmitVertexData();

        // Sort and render by texture to minimize state changes
        _lastDrawCallCount = 0;

        // Simple approach: render all with no texture first, then by texture
        std::unordered_map<GLuint, std::vector<uint32_t>> drawGroups;
        for (uint32_t i = 0; i < _spriteQueue.size(); ++i) {
            drawGroups[_spriteQueue[i].textureId].push_back(i);
        }

        // Render each texture group
        for (const auto& [textureId, spriteIndices] : drawGroups) {
            if (textureId != 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureId);
            }

            // Issue draw calls for this texture group
            for (uint32_t spriteIdx : spriteIndices) {
                uint32_t baseIdx = spriteIdx * 6; // 6 indices per sprite (2 triangles)
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 
                             (void*)(baseIdx * sizeof(uint32_t)));
                _lastDrawCallCount++;
            }
        }

        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Clear();
    }

    void SpriteBatch::Clear() {
        _spriteQueue.clear();
        _textureGroups.clear();
        _spriteCount = 0;
    }

} // namespace Graphitech
