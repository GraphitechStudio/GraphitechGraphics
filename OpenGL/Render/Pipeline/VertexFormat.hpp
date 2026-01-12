/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** VertexFormat.hpp
*/

#ifndef VERTEXFORMAT_HPP
#define VERTEXFORMAT_HPP

namespace Graphitech {

    /**
     * @brief Enumeration of supported vertex attribute identifiers.
     *
     * @details Defines the standard vertex attributes that can be used
     * in vertex shaders. Each attribute corresponds to a specific
     * semantic meaning in the rendering pipeline.
     */
    enum class VertexAttributeId
    {
        Position,   /**< Vertex position (x, y, z) */
        Color,      /**< Vertex color (r, g, b, a) */
        TexCoord,   /**< Texture coordinates (u, v) */
        Count       /**< Total number of supported attributes */
    };

    /**
     * @brief Enumeration of vertex attribute data types.
     *
     * @details Specifies the data type for vertex attributes.
     * Currently only float types are supported.
     */
    enum class VertexAttributeType
    {
        Uninitialized,  /**< Attribute not initialized */
        Float,          /**< 32-bit floating point */
        Count           /**< Total number of supported types */
    };

    /**
     * @brief Structure describing a vertex attribute layout.
     *
     * @details Defines how a vertex attribute should be interpreted
     * by OpenGL, including the number of components, data type,
     * and normalization settings.
     */
    struct VertexLayout final
    {
        /** @brief Number of components per attribute (1-4). */
        uint8_t _attributeCount = 0u;

        /** @brief Data type of the attribute components. */
        VertexAttributeType _attributeType = VertexAttributeType::Uninitialized;

        /** @brief Whether integer values should be normalized to [0,1] or [-1,1]. */
        bool _normalized = false;
    };

    /**
     * @brief Type alias for vertex layout mapping.
     *
     * @details Maps vertex attribute IDs to their layout descriptions.
     * Used to configure vertex array objects with multiple attributes.
     */
    using VertexLayoutMap = std::unordered_map<VertexAttributeId, VertexLayout>;
}

#endif //VERTEXFORMAT_HPP