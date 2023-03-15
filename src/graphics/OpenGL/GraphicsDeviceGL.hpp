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
	[[nodiscard]] std::string TryDequeueError() noexcept override;

	[[nodiscard]] Ref<GraphicsContext> CreateContext() noexcept override;
	[[nodiscard]] Ref<Buffer> CreateBuffer( const BufferDescription& desc, const void* initial_data = nullptr ) override;
	[[nodiscard]] Ref<VertexArray> CreateVertexArray( const VertexArrayDescription& desc ) override;
	[[nodiscard]] Ref<Pipeline> CreatePipeline() override;
	[[nodiscard]] Ref<Sampler> CreateSampler(const SamplerDescription& desc) override;
	[[nodiscard]] Ref<Shader> CreateShader( ShaderType type, const char* source ) override;
	[[nodiscard]] Ref<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc ) override;
};

}

#endif

