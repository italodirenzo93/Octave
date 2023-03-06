#ifndef OCTAVE_GRAPHICS_STRUCTS_HPP
#define OCTAVE_GRAPHICS_STRUCTS_HPP

#include <cstdint>
#include <vector>

namespace Octave {

enum class BufferBinding {
	VertexBuffer,
	IndexBuffer,
	UniformBuffer
};

enum class BufferAccess {
	Write,
	Read,
	Copy
};

struct BufferDescription {
	uint32_t size;
	uint32_t stride;
	BufferBinding bind_flags;
	BufferAccess access_flags;
};

enum class VertexAttributeName { kPosition, kColor, kTexCoord, kNormal };
enum class VertexAttributeType { kFloat, kUint, kUbyte };

struct VertexAttribute {
	VertexAttributeName name;
	uint32_t size;
	VertexAttributeType type;
	bool normalized;
};

struct VertexArrayDescription {
	VertexAttribute* attributes;
	size_t count;
};

enum class ShaderType {
	VertexShader,
	FragmentShader
};

}

#endif
