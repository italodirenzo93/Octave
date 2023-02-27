#include "pch/pch.hpp"
#include "GraphicsSystem.hpp"

#include "core/glfw/WindowGLFW.hpp"
#include "graphics/OpenGL/GraphicsSystemGL.hpp"

namespace Octave {

std::unique_ptr<GraphicsSystem> GraphicsSystem::Create( const Window& window ) {
    // TODO: Do this per platform
    const auto& glfw_window = dynamic_cast<const Impl::WindowGLFW&>( window );
    return std::make_unique<Impl::GraphicsSystemGL>( glfw_window.GetPointer() );
}

}
