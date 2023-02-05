#include "GeometricPrimitive.hpp"

namespace octave {

using namespace graphics;

GeometricPrimitive::VertexType::VertexType(
	std::initializer_list<float> initializer_list ) {
	assert( initializer_list.size() == 8 );

	auto it = initializer_list.begin();

	position.x = *( it++ );
	position.y = *( it++ );
	position.z = *( it++ );

	normal.x = *( it++ );
	normal.y = *( it++ );
	normal.z = *( it++ );

	tex_coord.x = *( it++ );
	tex_coord.y = *( it++ );
}

void GeometricPrimitive::CreateCube( VertexBuffer& vbo, IndexBuffer& ibo ) {
	const VertexBuffer::VertexLayout layout{
		{ LayoutSemantic::kPosition, 3, GL_FLOAT, false },
		{ LayoutSemantic::kNormal, 3, GL_FLOAT, false },
		{ LayoutSemantic::kTexCoord, 2, GL_FLOAT, false } };

	vbo.SetData(
		layout,
		{
			VertexType{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
			VertexType{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			VertexType{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			VertexType{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
			VertexType{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
			VertexType{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
			VertexType{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
			VertexType{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
			VertexType{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
			VertexType{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
			VertexType{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			VertexType{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			VertexType{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
			VertexType{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
			VertexType{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			VertexType{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			VertexType{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
			VertexType{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
			VertexType{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			VertexType{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			VertexType{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			VertexType{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
			VertexType{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
			VertexType{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		} );

	ibo.SetData( { 0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
				   8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
				   16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 } );
}

void GeometricPrimitive::CreateQuad( graphics::VertexBuffer& vbo ) {
	const VertexBuffer::VertexLayout layout{
		{ LayoutSemantic::kPosition, 3, GL_FLOAT, false },
		{ LayoutSemantic::kNormal, 3, GL_FLOAT, false },
		{ LayoutSemantic::kTexCoord, 2, GL_FLOAT, false } };

	// clang-format off
	vbo.SetData( layout, {
		// pos // tex
		VertexType::PositionTexture(0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		VertexType::PositionTexture(0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		VertexType::PositionTexture(0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 0.0f, 0.0f, 1.0f, 0.0f)
	});
	// clang-format on
}

void GeometricPrimitive::CreatePlane( graphics::VertexBuffer& vbo ) {
	const VertexBuffer::VertexLayout layout{
		{ LayoutSemantic::kPosition, 3, GL_FLOAT, false },
		{ LayoutSemantic::kNormal, 3, GL_FLOAT, false },
		{ LayoutSemantic::kTexCoord, 2, GL_FLOAT, false } };

	// clang-format off
	vbo.SetData(layout, {
		// pos // tex
		VertexType::PositionTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		VertexType::PositionTexture(0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		VertexType::PositionTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		VertexType::PositionTexture(1.0f, 0.0f, 0.0f, 1.0f, 0.0f)
	});
	// clang-format on
}

}  // namespace octave
