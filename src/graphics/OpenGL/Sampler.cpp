#include "pch/pch.hpp"
#include "Sampler.hpp"

namespace Octave {

static GLuint WrapToGLType( TextureWrap wrap ) noexcept {
	switch (wrap) {
		default:
		case TextureWrap::Repeat:
			return GL_REPEAT;
		case TextureWrap::Mirror:
			return GL_MIRRORED_REPEAT;
		case TextureWrap::ClampEdge:
			return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampBorder:
			return GL_CLAMP_TO_BORDER;
	}
}

static GLuint FilterToGLType( TextureFilter filter ) noexcept {
	switch ( filter ) {
		default:
		case TextureFilter::Linear:
			return GL_LINEAR;
		case TextureFilter::Nearest:
			return GL_NEAREST;
		case TextureFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
	}
}

Sampler::Sampler( const SamplerDescription& desc ) : id_( 0 ), desc_( desc ) {
	glCreateSamplers( 1, &id_ );

	// Set sampler params
	glSamplerParameterf( id_, GL_TEXTURE_LOD_BIAS, desc.mip_lod_bias );

	glSamplerParameterf( id_, GL_TEXTURE_MAX_LOD, desc.max_lod );
	glSamplerParameterf( id_, GL_TEXTURE_MIN_LOD, desc.min_lod );

	glSamplerParameteri( id_, GL_TEXTURE_MAG_FILTER, FilterToGLType( desc.filter ) );
	glSamplerParameteri( id_, GL_TEXTURE_MIN_FILTER, FilterToGLType( desc.filter ) );

	glSamplerParameteri( id_, GL_TEXTURE_WRAP_S, WrapToGLType( desc.wrap_s ) );
	glSamplerParameteri( id_, GL_TEXTURE_WRAP_T, WrapToGLType( desc.wrap_t ) );
}

Sampler::~Sampler() noexcept {
	glDeleteSamplers( 1, &id_ );
}

}
