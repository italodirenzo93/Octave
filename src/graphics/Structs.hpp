#ifndef OCTAVE_GRAPHICS_STRUCTS_HPP
#define OCTAVE_GRAPHICS_STRUCTS_HPP

#include <vector>

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

using VertexLayout = std::vector<VertexAttribute>;

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

enum class TextureWrap { Repeat, Mirror, ClampEdge, ClampBorder };

enum class TextureFilter {
	Nearest,
	Linear,
	NearestMipmapNearest,
	NearestMipmapLinear,
	LinearMipmapNearest,
	LinearMipmapLinear
};

struct SamplerDescription {
	TextureWrap wrap_s;
	TextureWrap wrap_t;
	TextureFilter filter;
	uint32_t max_ansiotropy;
	float max_lod;
	float min_lod;
	float mip_lod_bias;
};

//////////////////////////////
// Rasterizer State
//////////////////////////////

enum class VertexWindingOrder { CounterClockwise, Clockwise };
enum class CullFace { None, Back, Front, FrontAndBack };

struct RasterizerStateDescription{
	VertexWindingOrder winding_order;
	CullFace cull_face;
};

//////////////////////////////
// Depth / Stencil
//////////////////////////////

struct DepthStencilStateDescription {
	bool depth_enabled;
	float depth_value;

	bool stencil_enabled;
	int stencil_value;
};

}

#endif
