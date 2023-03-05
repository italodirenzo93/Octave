#ifndef OCTAVE_GRAPHICS_VERTEXARRAYLAYOUT_HPP
#define OCTAVE_GRAPHICS_VERTEXARRAYLAYOUT_HPP

#include "pch/pch.hpp"

namespace Octave {

enum class VertexAttributeName { kPosition, kColor, kTexCoord, kNormal };
enum class VertexAttributeType { kFloat, kUint, kUbyte };

struct VertexAttribute {
	VertexAttributeName name;
	uint32_t size;
	VertexAttributeType type;
	bool normalized;
};

class VertexArrayLayout {
public:
	virtual ~VertexArrayLayout() noexcept = default;

	virtual void SetAttributes( const std::vector<VertexAttribute>& attributes ) = 0;
	virtual void AddAttribute( VertexAttribute attribute ) = 0;
};

}

#endif
