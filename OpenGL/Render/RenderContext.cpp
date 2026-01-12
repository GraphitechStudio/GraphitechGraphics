/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** RenderContext.cpp - Factory and manager for render API instances
*/

#include "RenderContext.hpp"
#include "OpenGLRenderAPI.hpp"
#include <stdexcept>
#include <iostream>

namespace Graphitech {

    std::unique_ptr<IRenderAPI> RenderContext::s_renderAPI = nullptr;
    RenderAPIType RenderContext::s_apiType = RenderAPIType::None;

    bool RenderContext::Create(RenderAPIType type) {
        if (s_renderAPI) {
            std::cerr << "RenderContext: Warning - Context already exists, destroying previous one\n";
            Destroy();
        }

        s_apiType = type;

        switch (type) {
            case RenderAPIType::OpenGL:
                s_renderAPI = std::make_unique<OpenGLRenderAPI>();
                break;

            case RenderAPIType::Vulkan:
                std::cerr << "RenderContext: Vulkan API not yet implemented\n";
                return false;

            case RenderAPIType::DirectX11:
                std::cerr << "RenderContext: DirectX11 API not yet implemented\n";
                return false;

            case RenderAPIType::DirectX12:
                std::cerr << "RenderContext: DirectX12 API not yet implemented\n";
                return false;

            case RenderAPIType::Metal:
                std::cerr << "RenderContext: Metal API not yet implemented\n";
                return false;

            case RenderAPIType::None:
            default:
                std::cerr << "RenderContext: Invalid API type specified\n";
                return false;
        }

        if (!s_renderAPI) {
            std::cerr << "RenderContext: Failed to create render API\n";
            s_apiType = RenderAPIType::None;
            return false;
        }

        std::cout << "RenderContext: Created context for API type " << static_cast<int>(type) << "\n";
        return true;
    }

    void RenderContext::Destroy() {
        if (s_renderAPI) {
            s_renderAPI->Shutdown();
            s_renderAPI.reset();
            s_apiType = RenderAPIType::None;
            std::cout << "RenderContext: Destroyed render context\n";
        }
    }

    IRenderAPI& RenderContext::GetAPI() {
        if (!s_renderAPI) {
            throw std::runtime_error("RenderContext: No render API has been created. Call Create() first.");
        }
        return *s_renderAPI;
    }

    bool RenderContext::IsValid() {
        return s_renderAPI != nullptr;
    }

    RenderAPIType RenderContext::GetAPIType() {
        return s_apiType;
    }

} // namespace Graphitech
