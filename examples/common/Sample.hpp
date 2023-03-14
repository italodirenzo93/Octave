#pragma once

#include <Octave.hpp>
#include <stb_image.h>
#include <fstream>

#include "Camera.hpp"

namespace Octave::Samples {

class Sample : public Application {
protected:
	void OnInitialize() override;
};

class SampleLayer : public Layer {
public:
	explicit SampleLayer( const Sample& app );

	virtual void OnStep() = 0;
	virtual void OnRender() = 0;

protected:
	DebugCamera camera_;
	std::unique_ptr<StepTimer> step_timer_;
	std::unique_ptr<GraphicsContext> context_;
};

template <class T>
Ref<Buffer> CreateStaticBuffer( GraphicsDevice& device,
									   const std::vector<T>& data,
									   size_t stride, BufferBinding binding ) {
	BufferDescription desc{};
	desc.size = static_cast<uint32_t>( sizeof( T ) * data.size() );
	desc.stride = static_cast<uint32_t>( stride );
	desc.access_flags = ResourceAccess::Write;
	desc.bind_flags = binding;

	return device.CreateBuffer( desc, data.data() );
}

inline Ref<Texture2D> CreateTextureFromFile( GraphicsDevice& device, const std::string& filename ) {
	stbi_set_flip_vertically_on_load( true );

	int n_channels, width, height;
	const stbi_uc* image = stbi_load( filename.c_str(), &width, &height, &n_channels,
			   STBI_default );

	if ( !image ) {
		throw Octave::Exception( "Could not load texture from file " +
								 filename );
	}

	TextureDescription2D desc{};
	desc.width = width;
	desc.height = height;
	desc.access_flags = ResourceAccess::Write;
	desc.mip_levels = 1;

	if (n_channels == 4) {
		desc.format = TextureFormat::Rgba;
	} else {
		desc.format = TextureFormat::Rgb;
	}

	auto texture = device.CreateTexture2D( desc );
	texture->SetData( desc.format, 0, 0, 0, width, height, image );
	return texture;
}

inline Ref<Shader> LoadShaderFromFile( GraphicsDevice& device, ShaderType type, const std::string& path ) {
	std::ifstream ifs( path );
	if ( !ifs.is_open() ) {
		throw Octave::Exception( "Unable to open file " + path );
	}

	std::string code( (std::istreambuf_iterator<char>( ifs )),
					  (std::istreambuf_iterator<char>() ));

	return device.CreateShader( type, code.c_str() );
}

}  // namespace Octave::Samples
