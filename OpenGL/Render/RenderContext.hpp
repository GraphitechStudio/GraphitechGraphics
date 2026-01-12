/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** RenderContext.hpp - Factory and manager for render API instances
*/

#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

#include "IRenderAPI.hpp"
#include <memory>

namespace Graphitech {

    /**
     * @brief Factory and manager for render API instances.
     *
     * @details This class provides a centralized way to create and access
     * the render API implementation. It follows the singleton pattern to
     * ensure there's only one render context per application.
     *
     * Usage:
     * @code
     * RenderContext::Create(RenderAPIType::OpenGL);
     * auto& api = RenderContext::GetAPI();
     * api.Initialize();
     * @endcode
     *
     * @ingroup Render
     */
    class RenderContext {
    public:
        /**
         * @brief Creates a render context with the specified API type.
         *
         * @param type The graphics API to use (OpenGL, Vulkan, etc.).
         * @return true if creation succeeded, false otherwise.
         */
        static bool Create(RenderAPIType type);

        /**
         * @brief Destroys the current render context.
         */
        static void Destroy();

        /**
         * @brief Gets the current render API instance.
         *
         * @return Reference to the IRenderAPI implementation.
         * @throws std::runtime_error if no context has been created.
         */
        static IRenderAPI& GetAPI();

        /**
         * @brief Checks if a render context has been created.
         *
         * @return true if a context exists, false otherwise.
         */
        static bool IsValid();

        /**
         * @brief Gets the type of the current render API.
         *
         * @return The API type, or RenderAPIType::None if no context exists.
         */
        static RenderAPIType GetAPIType();

    private:
        RenderContext() = delete;
        ~RenderContext() = delete;
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;

        /**
         * @brief The current render API instance.
         */
        static std::unique_ptr<IRenderAPI> s_renderAPI;

        /**
         * @brief The type of the current API.
         */
        static RenderAPIType s_apiType;
    };

} // namespace Graphitech

#endif // RENDERCONTEXT_HPP
