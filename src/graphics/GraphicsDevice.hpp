#ifndef OCTAVE_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICSDEVICE_HPP

#include <string>

#include "core/Types.hpp"
#include "core/Window.hpp"
#include "GraphicsContext.hpp"
#include "Resources.hpp"
#include "Structs.hpp"

namespace Octave {

class GraphicsDevice {
public:
	virtual ~GraphicsDevice() noexcept = default;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual Ref<GraphicsContext> CreateContext() noexcept = 0;
	[[nodiscard]] virtual Ref<Buffer> CreateBuffer( const BufferDescription& desc, const void* initial_data = nullptr ) = 0;
	[[nodiscard]] virtual Ref<VertexArray> CreateVertexArray( const VertexArrayDescription& desc ) = 0;
	[[nodiscard]] virtual Ref<Pipeline> CreatePipeline() = 0;
	[[nodiscard]] virtual Ref<Sampler> CreateSampler(
		const SamplerDescription& desc ) = 0;
	[[nodiscard]] virtual Ref<Shader> CreateShader( ShaderType type, const char* source ) = 0;
	[[nodiscard]] virtual Ref<Texture2D> CreateTexture2D(
		const TextureDescription2D& desc ) = 0;

    static Ref<GraphicsDevice> Create( const Window& window );
};

}

#endif
