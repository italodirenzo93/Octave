#ifndef OCTAVE_GRAPHICS_OPENGL_OPENGLGRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICS_OPENGL_OPENGLGRAPHICSDEVICE_HPP

#include "core/Window.hpp"
#include "graphics/GraphicsDevice.hpp"

#include <SDL.h>

namespace Octave {

class OpenGLGraphicsDevice final : public GraphicsDevice {
public:
	explicit OpenGLGraphicsDevice( const Window& window );

	[[nodiscard]] std::string TryDequeueError() override;

	[[nodiscard]] std::unique_ptr<GraphicsContext> CreateContext() override;
	void DestroyContext( std::unique_ptr<GraphicsContext> context ) override;

	[[nodiscard]] std::unique_ptr<Buffer> CreateBuffer(
		const BufferDescription& desc, const void* data ) override;
	void DestroyBuffer( std::unique_ptr<Buffer> buffer ) override;

	[[nodiscard]] std::unique_ptr<Program> CreateProgram(
		const Shader& vs, const Shader& fs ) override;
	void DestroyProgram( std::unique_ptr<Program> program ) override;

	[[nodiscard]] std::unique_ptr<Sampler> CreateSampler(
		const SamplerDescription& desc ) override;
	void DestroySampler( std::unique_ptr<Sampler> sampler ) override;

	[[nodiscard]] std::unique_ptr<Shader> CreateShader(
		ShaderType type, const char* source ) override;
	void DestroyShader( std::unique_ptr<Shader> shader ) override;

	[[nodiscard]] std::unique_ptr<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc ) override;
	void DestroyTexture2D( std::unique_ptr<Texture2D> texture ) override;

	void GenerateMipmap( const Texture2D& texture ) override;
	void SwapBuffers() override;

private:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;
};

}  // namespace Octave

#endif
