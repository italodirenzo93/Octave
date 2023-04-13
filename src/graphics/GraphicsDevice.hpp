#ifndef OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICS_GRAPHICSDEVICE_HPP

#include <string>

#include "GraphicsContext.hpp"
#include "Resources.hpp"
#include "Structs.hpp"

namespace Octave {

class GraphicsDevice {
public:
	GraphicsDevice() = default;
	virtual ~GraphicsDevice() noexcept = default;

	GraphicsDevice( const GraphicsDevice& ) = delete;
	GraphicsDevice& operator=( const GraphicsDevice& ) = delete;

	[[nodiscard]] virtual std::string TryDequeueError() = 0;

	[[nodiscard]] virtual std::unique_ptr<GraphicsContext> CreateContext() = 0;
	virtual void DestroyContext( std::unique_ptr<GraphicsContext> context ) = 0;

	[[nodiscard]] virtual std::unique_ptr<Buffer> CreateBuffer(
		const BufferDescription& desc, const void* data ) = 0;
	virtual void DestroyBuffer( std::unique_ptr<Buffer> buffer ) = 0;

	[[nodiscard]] virtual std::unique_ptr<Program> CreateProgram(
		const Shader& vs, const Shader& fs ) = 0;
	virtual void DestroyProgram( std::unique_ptr<Program> program ) = 0;

	[[nodiscard]] virtual std::unique_ptr<Sampler> CreateSampler(
		const SamplerDescription& desc ) = 0;
	virtual void DestroySampler( std::unique_ptr<Sampler> sampler ) = 0;

	[[nodiscard]] virtual std::unique_ptr<Shader> CreateShader(
		ShaderType type, const char* source ) = 0;
	virtual void DestroyShader( std::unique_ptr<Shader> shader ) = 0;

	[[nodiscard]] virtual std::unique_ptr<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc ) = 0;
	virtual void DestroyTexture2D( std::unique_ptr<Texture2D> texture ) = 0;

	virtual void GenerateMipmap( const Texture2D& texture ) = 0;
	virtual void SwapBuffers() = 0;
};

}  // namespace Octave

#endif
