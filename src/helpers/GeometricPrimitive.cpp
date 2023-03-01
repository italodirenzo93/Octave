#include "GeometricPrimitive.hpp"

namespace Octave {

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
		{ VertexAttributeName::kPosition, 3, VertexAttributeType::kFloat,
		  false },
		{ VertexAttributeName::kNormal, 3, VertexAttributeType::kFloat, false },
		{ VertexAttributeName::kTexCoord, 2, VertexAttributeType::kFloat,
		  false } };

	std::vector<VertexType> vertices{
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
	};

	vbo.SetData( layout, vertices );

	ibo.SetData( { 0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
				   8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
				   16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 } );
}

void GeometricPrimitive::CreateQuad( VertexBuffer& vbo ) {
	const VertexBuffer::VertexLayout layout{
		{ VertexAttributeName::kPosition, 3, VertexAttributeType::kFloat,
		  false },
		{ VertexAttributeName::kNormal, 3, VertexAttributeType::kFloat, false },
		{ VertexAttributeName::kTexCoord, 2, VertexAttributeType::kFloat,
		  false } };

	std::vector<VertexType> vertices{
		// pos // tex
		VertexType::PositionTexture( 0.0f, 1.0f, 0.0f, 0.0f, 1.0f ),
		VertexType::PositionTexture( 1.0f, 0.0f, 0.0f, 1.0f, 0.0f ),
		VertexType::PositionTexture( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f ),
		VertexType::PositionTexture( 0.0f, 1.0f, 0.0f, 0.0f, 1.0f ),
		VertexType::PositionTexture( 1.0f, 1.0f, 0.0f, 1.0f, 1.0f ),
		VertexType::PositionTexture( 1.0f, 0.0f, 0.0f, 1.0f, 0.0f ) };

	vbo.SetData( layout, vertices );
}

static constexpr float kPlaneExtent = 10.0f;

void GeometricPrimitive::CreatePlane( VertexBuffer& vbo ) {
	const VertexBuffer::VertexLayout layout{
		{ VertexAttributeName::kPosition, 3, VertexAttributeType::kFloat,
		  false },
		{ VertexAttributeName::kNormal, 3, VertexAttributeType::kFloat, false },
		{ VertexAttributeName::kTexCoord, 2, VertexAttributeType::kFloat,
		  false } };

	constexpr glm::vec3 normal_vector( 0.0f, 1.0f, 0.0f );

	std::vector<VertexType> vertices{
		// pos // tex
		VertexType::PositionNormalTexture(
			glm::vec3( -kPlaneExtent, 0.0f, -kPlaneExtent ), normal_vector,
			glm::vec2( 0.0f, 0.0f ) ),  // Back left
		VertexType::PositionNormalTexture(
			glm::vec3( kPlaneExtent, 0.0f, -kPlaneExtent ), normal_vector,
			glm::vec2( kPlaneExtent, 0.0f ) ),  // Back right
		VertexType::PositionNormalTexture(
			glm::vec3( -kPlaneExtent, 0.0f, kPlaneExtent ), normal_vector,
			glm::vec2( 0.0f, kPlaneExtent ) ),  // Front left

		VertexType::PositionNormalTexture(
			glm::vec3( kPlaneExtent, 0.0f, -kPlaneExtent ), normal_vector,
			glm::vec2( kPlaneExtent, 0.0f ) ),
		VertexType::PositionNormalTexture(
			glm::vec3( kPlaneExtent, 0.0f, kPlaneExtent ), normal_vector,
			glm::vec2( kPlaneExtent, kPlaneExtent ) ),
		VertexType::PositionNormalTexture(
			glm::vec3( -kPlaneExtent, 0.0f, kPlaneExtent ), normal_vector,
			glm::vec2( 0.0f, kPlaneExtent ) ),
	};

	vbo.SetData( layout, vertices );
}

}  // namespace Octave
