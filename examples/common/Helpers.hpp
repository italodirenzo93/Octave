#ifndef OCTAVE_SAMPLES_HELPERS_HPP
#define OCTAVE_SAMPLES_HELPERS_HPP

#include <Octave.hpp>
#include <stb_image.h>

#include <fstream>

namespace Octave::Samples {

template <class T>
std::unique_ptr<Buffer> CreateStaticBuffer( GraphicsDevice& device,
									   const std::vector<T>& data,
									   size_t stride, BufferType binding ) {
	BufferDescription desc{};
	desc.m_Size = static_cast<uint32_t>( sizeof( T ) * data.size() );
	desc.m_Stride = static_cast<uint32_t>( stride );
	desc.m_Access = ResourceAccess::WriteOnly;
	desc.m_Usage = BufferUsage::Static;
	desc.m_Type = binding;

	return device.CreateBuffer( desc, data.data() );
}

inline std::unique_ptr<Texture2D> CreateTextureFromFile(
	GraphicsDevice& device, const std::string& filename ) {
	stbi_set_flip_vertically_on_load( true );

	int n_channels, width, height;
	const stbi_uc* image = stbi_load( filename.c_str(), &width, &height,
									  &n_channels, STBI_default );

	if ( !image ) {
		throw Octave::Exception( "Could not load texture from file " +
								 filename );
	}

	TextureDescription2D desc{};
	desc.m_Width = width;
	desc.m_Height = height;
	desc.m_MipLevels = 1;

	if ( n_channels == 4 ) {
		desc.m_Format = TextureFormat::Rgba;
	} else {
		desc.m_Format = TextureFormat::Rgb;
	}

	auto texture = device.CreateTexture2D( desc );

	texture->SetData( desc.m_Format, 0, 0, 0, width, height, image );
	device.GenerateMipmap( *texture );

	return texture;
}

inline std::unique_ptr<Shader> LoadShaderFromFile( GraphicsDevice& device,
											  ShaderType type,
											  const std::string& path ) {
	std::ifstream ifs( path );
	if ( !ifs.is_open() ) {
		throw Octave::Exception( "Unable to open file " + path );
	}

	std::string code( ( std::istreambuf_iterator<char>( ifs ) ),
					  ( std::istreambuf_iterator<char>() ) );

	return device.CreateShader( type, code.c_str() );
}

}

#endif
