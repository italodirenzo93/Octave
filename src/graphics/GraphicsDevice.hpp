#ifndef OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP

#include <string>

#include "core/Types.hpp"
#include "core/Window.hpp"
#include "GraphicsContext.hpp"
#include "Resources.hpp"
#include "Structs.hpp"

namespace Octave {

class GraphicsDevice {
public:
	explicit GraphicsDevice( const Window& window );
	~GraphicsDevice() noexcept;

	GraphicsDevice() = delete;
	GraphicsDevice( const GraphicsDevice& ) = delete;
	GraphicsDevice& operator=( const GraphicsDevice& ) = delete;

	[[nodiscard]] std::string TryDequeueError() noexcept;

	[[nodiscard]] Ref<GraphicsContext> CreateContext() noexcept;
	[[nodiscard]] Ref<Buffer> CreateBuffer( const BufferDescription& desc, const void* initial_data = nullptr );
	[[nodiscard]] Ref<Fence> CreateFence() noexcept;
	[[nodiscard]] Ref<VertexArray> CreateVertexArray( const VertexLayout& desc );
	[[nodiscard]] Ref<Pipeline> CreatePipeline();
	[[nodiscard]] Ref<Sampler> CreateSampler(
		const SamplerDescription& desc );
	[[nodiscard]] Ref<Shader> CreateShader( ShaderType type, const char* source );
	[[nodiscard]] Ref<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc );
};

}

#endif
