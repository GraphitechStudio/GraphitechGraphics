/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** VertexArray.hpp
*/

#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include "Graphitech.hpp"
#include "VertexFormat.hpp"

namespace Graphitech {

    /**
     * @brief Manages OpenGL vertex array objects and associated buffers.
     *
     * @details VertexArray encapsulates the creation and management of OpenGL
     * vertex array objects, vertex buffers, and index buffers. It provides
     * methods to set vertex and index data, bind the array for rendering,
     * and perform draw operations.
     *
     * The class handles the complete lifecycle of vertex data from setup
     * to rendering, including proper resource cleanup.
     *
     * @ingroup Render
     */
    class VertexArray final
    {
        public:
            /**
             * @brief Submits the vertex array for rendering.
             *
             * @details Prepares the vertex array for drawing by ensuring
             * all buffers are properly set up and ready for rendering.
             */
            void Submit();

            /**
             * @brief Binds the vertex array object for rendering.
             *
             * @details Makes this vertex array the current active VAO
             * in the OpenGL context.
             */
            void Bind();

            /**
             * @brief Performs the actual draw operation.
             *
             * @details Executes the OpenGL draw call using the configured
             * vertex and index data. Uses indexed drawing if an index
             * buffer is present, otherwise uses direct vertex drawing.
             */
            void Draw();

            /**
             * @brief Releases all OpenGL resources.
             *
             * @details Cleans up the VAO, VBO, and IBO, freeing GPU memory.
             * Should be called when the vertex array is no longer needed.
             */
            void Release();

            /**
             * @brief Sets vertex buffer data with specified layout.
             *
             * @param pData Pointer to the vertex data.
             * @param size Size of the vertex data in bytes.
             * @param vertexLayoutMap Layout description for vertex attributes.
             */
            void SetVertexBufferData(void* pData, uint32_t size, const VertexLayoutMap& vertexLayoutMap);

            /**
             * @brief Sets index buffer data for indexed drawing.
             *
             * @param pData Pointer to the index data.
             * @param size Size of the index data in bytes.
             */
            void SetIndexBufferData(void* pData, uint32_t size);

            /**
             * @brief Checks if an index buffer is configured.
             *
             * @return true if index buffer data has been set, false otherwise.
             */
            bool HasIndexBuffer() const noexcept;

        private:
            /** @brief OpenGL vertex array object ID. */
            uint32_t _vaoId = 0u;

            /** @brief OpenGL vertex buffer object ID. */
            uint32_t _vboId = 0u;

            /** @brief OpenGL index buffer object ID. */
            uint32_t _iboId = 0u;

            /** @brief Size of vertex data in bytes. */
            uint32_t _vertexSize = 0u;

            /** @brief Size of index data in bytes. */
            uint32_t _indexSize = 0u;

            /** @brief Pointer to vertex data (managed externally). */
            void* _vertexData = nullptr;

            /** @brief Pointer to index data (managed externally). */
            void* _indexData = nullptr;

            /** @brief Layout mapping for vertex attributes. */
            VertexLayoutMap _vertexLayoutMap = {};
    };
}

#endif //VERTEXARRAY_HPP