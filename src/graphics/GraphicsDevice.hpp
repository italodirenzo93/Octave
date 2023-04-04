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

	[[nodiscard]] Ref<Buffer> CreateBuffer( const BufferDescription& desc,
											const void* data );
	void DestroyBuffer( Ref<Buffer> buffer );

	[[nodiscard]] Ref<VertexArray> CreateVertexArray(
		const VertexLayout& desc );
	void DestroyVertexArray( Ref<VertexArray> vertex_array );

	[[nodiscard]] Ref<Program> CreateProgram( const Shader& vs,
											  const Shader& fs );
	void DestroyProgram( Ref<Program> program );

	[[nodiscard]] Ref<Sampler> CreateSampler( const SamplerDescription& desc );
	void DestroySampler( Ref<Sampler> sampler );

	[[nodiscard]] Ref<Shader> CreateShader( ShaderType type,
											const char* source );
	void DestroyShader( Ref<Shader> shader );

	[[nodiscard]] Ref<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc );
	void DestroyTexture2D( Ref<Texture2D> texture );

	void GenerateMipmap( const Texture2D& texture );
};

}  // namespace Octave

#endif
