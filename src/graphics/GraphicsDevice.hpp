#ifndef OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP

#include <string>

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

	[[nodiscard]] std::unique_ptr<GraphicsContext> CreateContext() noexcept;
	void DestroyContext( std::unique_ptr<GraphicsContext> context );

	[[nodiscard]] std::unique_ptr<Buffer> CreateBuffer( const BufferDescription& desc,
											const void* data );
	void DestroyBuffer( std::unique_ptr<Buffer> buffer );

	[[nodiscard]] std::unique_ptr<Program> CreateProgram( const Shader& vs,
											  const Shader& fs );
	void DestroyProgram( std::unique_ptr<Program> program );

	[[nodiscard]] std::unique_ptr<Sampler> CreateSampler( const SamplerDescription& desc );
	void DestroySampler( std::unique_ptr<Sampler> sampler );

	[[nodiscard]] std::unique_ptr<Shader> CreateShader( ShaderType type,
											const char* source );
	void DestroyShader( std::unique_ptr<Shader> shader );

	[[nodiscard]] std::unique_ptr<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc );
	void DestroyTexture2D( std::unique_ptr<Texture2D> texture );

	void GenerateMipmap( const Texture2D& texture );
};

}  // namespace Octave

#endif
