#ifndef OCTAVE_GRAPHICS_STRUCTS_HPP
#define OCTAVE_GRAPHICS_STRUCTS_HPP

#include <cstdint>
#include <vector>

namespace Octave {

enum class ResourceAccess {
	WriteOnly,
	ReadOnly,
	ReadWrite
};

//////////////////////////////
// Buffers
//////////////////////////////

enum class BufferType {
	VertexBuffer,
	IndexBuffer,
	UniformBuffer
};

enum class BufferUsage {
	Static,
	Dynamic
};

struct BufferDescription {
	const char* m_Name = "";
	uint32_t m_Size = 0;
	uint32_t m_Stride = 0;
	ResourceAccess m_Access = ResourceAccess::WriteOnly;
	BufferType m_Type = BufferType::VertexBuffer;
	BufferUsage m_Usage = BufferUsage::Static;
};

//////////////////////////////
// Vertex Array
//////////////////////////////

enum class VertexAttributeName { kPosition, kColor, kTexCoord, kNormal };
enum class VertexAttributeType { kFloat, kUint, kUbyte };

struct VertexAttribute {
	VertexAttributeName m_Name = VertexAttributeName::kPosition;
	uint32_t m_Size = 0;
	VertexAttributeType m_Type = VertexAttributeType::kFloat;
	bool m_IsNormalized = false;
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
	const char* m_Name = "";
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	TextureFormat m_Format = TextureFormat::Rgba;
	uint32_t m_MipLevels = 1;
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
	const char* m_Name = "";
	TextureWrap m_WrapS = TextureWrap::Repeat;
	TextureWrap m_WrapT = TextureWrap::Repeat;
	TextureFilter m_Filter = TextureFilter::Linear;
	uint32_t m_MaxAnsiotropy = 0;
	float m_MaxLod = 0.0f;
	float m_MinLod = 0.0f;
	float m_MipLodBias = 0.0f;
};

//////////////////////////////
// Rasterizer State
//////////////////////////////

enum class VertexWindingOrder { CounterClockwise, Clockwise };
enum class CullFace { None, Back, Front, FrontAndBack };

struct RasterizerStateDescription{
	VertexWindingOrder m_WindingOrder = VertexWindingOrder::Clockwise;
	CullFace m_CullFace = CullFace::Back;
};

//////////////////////////////
// Depth / Stencil
//////////////////////////////

struct DepthStencilStateDescription {
	bool m_DepthEnabled = true;
	float m_DepthValue = 1.0f;
	bool m_StencilEnabled = false;
	int m_StencilValue = 0;
};

}

#endif
