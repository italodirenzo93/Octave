#ifndef OCTAVE_GEOMETRICPRIMITIVE_HPP
#define OCTAVE_GEOMETRICPRIMITIVE_HPP

#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBuffer.hpp"

namespace octave {

class GeometricPrimitive {
public:
	struct VertexType {
		glm::vec3 position = glm::zero<glm::vec3>();
		glm::vec3 normal = glm::zero<glm::vec3>();
		glm::vec2 tex_coord = glm::zero<glm::vec3>();

		VertexType() = default;
		VertexType( std::initializer_list<float> initializer_list );

		static VertexType PositionTexture( const glm::vec3& position,
										   const glm::vec2& tex_coord ) {
			VertexType vert;
			vert.position = position;
			vert.tex_coord = tex_coord;
			return vert;
		}

		static VertexType PositionTexture( float pos_x, float pos_y,
										   float pos_z, float tex_x,
										   float tex_y ) {
			return PositionTexture( glm::vec3( pos_x, pos_y, pos_z ),
									glm::vec2( tex_x, tex_y ) );
		}
	};

	static void CreateCube( graphics::VertexBuffer& vbo,
							graphics::IndexBuffer& ibo );
	static void CreateQuad( graphics::VertexBuffer& vbo );
	static void CreatePlane( graphics::VertexBuffer& vbo );
};

}  // namespace octave

#endif
