#ifndef OCTAVE_GRAPHICS_STRUCTS_HPP
#define OCTAVE_GRAPHICS_STRUCTS_HPP

#include <cstdint>

namespace Octave {

//////////////////////////////
// Buffers
//////////////////////////////

enum class BufferBinding {
	VertexBuffer,
	IndexBuffer,
	UniformBuffer
};

enum class ResourceAccess {
	Write,
	Read,
	Copy
};

struct BufferDescription {
	uint32_t size;
	uint32_t stride;
	BufferBinding bind_flags;
	ResourceAccess access_flags;
};

//////////////////////////////
// Vertex Array
//////////////////////////////

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

//////////////////////////////
// Shaders
//////////////////////////////

enum class ShaderType {
	VertexShader,
	FragmentShader
};

//////////////////////////////
// Textures
//////////////////////////////

enum class TextureFormat { Rgba, Rgb }; 

struct TextureDescription2D {
	uint32_t width;
	uint32_t height;
	TextureFormat format;
	uint32_t mip_levels;
	ResourceAccess access_flags;
};

//////////////////////////////
// Rasterizer State
//////////////////////////////

enum class VertexWindingOrder { CounterClockwise, Clockwise };
enum class CullFace { None, Back, Front, FrontAndBack };

struct RasterizerState {
	VertexWindingOrder winding_order;
	CullFace cull_face;
};

}

#endif
