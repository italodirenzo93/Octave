#include "pch/pch.hpp"
#include "GraphicsDevice.hpp"

#include "core/glfw/WindowGLFW.hpp"
#include "graphics/OpenGL/GraphicsDeviceGL.hpp"

namespace Octave {

std::unique_ptr<GraphicsDevice> GraphicsDevice::Create( const Window& window ) {
    // TODO: Do this per platform
    const auto& glfw_window = dynamic_cast<const Impl::WindowGLFW&>( window );
    return std::make_unique<Impl::GraphicsDeviceGL>( glfw_window.GetPointer() );
}

}
