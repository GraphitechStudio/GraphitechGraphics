/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SpriteRenderSystem.cpp - ECS sprite batch rendering implementation
*/

#include "SpriteRenderSystem.hpp"
#include "SpriteBatchShader.hpp"
#include <Graphitech/ECS/ECS.hpp>
#include <Graphitech/ECS/Components/PositionComponent.hpp>
#include <Graphitech/ECS/Components/ScaleComponent.hpp>
#include <Graphitech/ECS/Components/RotationComponent.hpp>
#include <Graphitech/ECS/Components/TextureComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>

namespace Graphitech {

    bool SpriteRenderSystem::Initialize() {
        // Create sprite batch shader
        _shaderProgram = CreateSpriteBatchShader();
        if (_shaderProgram == 0) {
            std::cerr << "[SpriteRenderSystem] Failed to create shader program\n";
            return false;
        }

        // Initialize sprite batch with shader
        if (!_batch.Initialize(_shaderProgram, 2048)) {
            std::cerr << "[SpriteRenderSystem] Failed to initialize SpriteBatch\n";
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
            return false;
        }

        std::cout << "[SpriteRenderSystem] Initialized successfully\n";
        return true;
    }

    void SpriteRenderSystem::Release() {
        _batch.Release();
        if (_shaderProgram != 0) {
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
        }
        std::cout << "[SpriteRenderSystem] Released successfully\n";
    }

    void SpriteRenderSystem::UpdateProjection(float logicalWidth, float logicalHeight) {
        // Create orthographic projection matrix
        glm::mat4 projection = glm::ortho(0.0f, logicalWidth, logicalHeight, 0.0f, -1.0f, 1.0f);

        // Set uniform in shader
        glUseProgram(_shaderProgram);
        GLint projLoc = glGetUniformLocation(_shaderProgram, "uProjection");
        if (projLoc >= 0) {
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }

        GLint texLoc = glGetUniformLocation(_shaderProgram, "uTexture");
        if (texLoc >= 0) {
            glUniform1i(texLoc, 0); // Texture unit 0
        }

        GLint useTexLoc = glGetUniformLocation(_shaderProgram, "uUseTexture");
        if (useTexLoc >= 0) {
            glUniform1i(useTexLoc, 1);
        }

        glUseProgram(0);
    }

    bool SpriteRenderSystem::EntityToSpriteDrawCall(ECS::Scene& scene, ECS::Entity entity, 
                                                    SpriteDrawCall& outCall) {
        auto* pos = scene.getComponent<ECS::PositionComponent>(entity);
        auto* scale = scene.getComponent<ECS::ScaleComponent>(entity);
        auto* rot = scene.getComponent<ECS::RotationComponent>(entity);
        auto* tex = scene.getComponent<ECS::TextureComponent>(entity);

        if (!pos) {
            return false;
        }

        // Get scale (default to 1.0 if not present)
        // ScaleComponent is a MULTIPLIER on the base sprite size (32x32)
        float scaleX = scale ? scale->x : 1.0f;
        float scaleY = scale ? scale->y : 1.0f;

        // Default sprite size if not specified
        float spriteWidth = 32.0f * scaleX;
        float spriteHeight = 32.0f * scaleY;

        // Build quad vertices (normalized texture coordinates 0-1)
        // Bottom-left
        outCall.vertices[0] = SpriteVertex(
            {pos->x, pos->y + spriteHeight},
            {0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        );

        // Bottom-right
        outCall.vertices[1] = SpriteVertex(
            {pos->x + spriteWidth, pos->y + spriteHeight},
            {1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        );

        // Top-right
        outCall.vertices[2] = SpriteVertex(
            {pos->x + spriteWidth, pos->y},
            {1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        );

        // Top-left
        outCall.vertices[3] = SpriteVertex(
            {pos->x, pos->y},
            {0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        );

        // Load texture if specified
        outCall.textureId = 0;
        if (tex && !tex->texturePath.empty()) {
            auto texture = TextureManager::Get().LoadTexture(tex->texturePath);
            if (texture) {
                outCall.textureId = texture->GetHandle();
            }
        }

        // Set z-order from rotation component or default
        outCall.zOrder = rot ? (uint32_t)(rot->angle * 1000) : 0;

        return true;
    }

    void SpriteRenderSystem::RenderSprites(ECS::Scene& scene, float logicalWidth, float logicalHeight) {
        if (!_batch.IsInitialized()) {
            std::cerr << "[SpriteRenderSystem] Batch not initialized\n";
            return;
        }

        // Update projection matrix
        UpdateProjection(logicalWidth, logicalHeight);

        // Use shader program
        glUseProgram(_shaderProgram);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Get all renderable entities
        // We'll use a basic approach: get entities with Position and optional Texture
        auto entities = scene.getEntitiesWith<ECS::PositionComponent>();

        _lastSpriteCount = 0;
        for (auto entity : entities) {
            SpriteDrawCall call;
            if (EntityToSpriteDrawCall(scene, entity, call)) {
                _batch.PushSprite(call);
                _lastSpriteCount++;
            }
        }

        // Issue batch render calls
        _batch.Flush();

        // Restore state
        glDisable(GL_BLEND);
        glUseProgram(0);

        // Log statistics (every 60 frames)
        static int frameCount = 0;
        if (++frameCount % 60 == 0) {
            std::cout << "[SpriteRenderSystem] Sprites: " << _lastSpriteCount 
                      << ", DrawCalls: " << _batch.GetLastDrawCallCount() << "\n";
        }
    }

} // namespace Graphitech
