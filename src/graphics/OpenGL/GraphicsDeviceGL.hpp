#ifndef OCTAVE_GRAPHICSDEVICEGL_HPP
#define OCTAVE_GRAPHICSDEVICEGL_HPP

#include "graphics/GraphicsDevice.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace Octave::Impl {

class GraphicsDeviceGL : public GraphicsDevice {
public:
	// Constructors
	explicit GraphicsDeviceGL( GLFWwindow* window );
	GraphicsDeviceGL( const GraphicsDeviceGL& other ) = delete;
	GraphicsDeviceGL( GraphicsDeviceGL&& other ) noexcept = delete;
	~GraphicsDeviceGL() noexcept override;

	// Operators
	GraphicsDeviceGL& operator=( const GraphicsDeviceGL& other ) = delete;
	GraphicsDeviceGL& operator=( GraphicsDeviceGL&& other ) noexcept = delete;

	// Methods
    void SwapBuffers() override;

	[[nodiscard]] std::string TryDequeueError() noexcept override;

	[[nodiscard]] Ref<GraphicsContext> CreateContext() noexcept override;
	[[nodiscard]] Ref<Buffer> CreateBuffer( const BufferDescription& desc, const void* initial_data = nullptr ) noexcept override;
	[[nodiscard]] Ref<VertexArray> CreateVertexArray( const VertexArrayDescription& desc ) noexcept override;
	[[nodiscard]] Ref<Pipeline> CreatePipeline() noexcept override;
	[[nodiscard]] Ref<Shader> CreateShader( ShaderType type, const char* source ) noexcept override;
	[[nodiscard]] Ref<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc ) override;

private:
    GLFWwindow* window_;
};

}

#endif

