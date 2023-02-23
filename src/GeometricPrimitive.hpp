#ifndef GEOMETRICPRIMITIVE_HPP
#define GEOMETRICPRIMITIVE_HPP

#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBuffer.hpp"

namespace octave {

class GeometricPrimitive {
public:
	struct VertexType {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coord;

		VertexType() = default;
		VertexType( std::initializer_list<float> initializer_list );
	};

	static void CreateCube( graphics::VertexBuffer& vbo,
							graphics::IndexBuffer& ibo );
};

}  // namespace octave

#endif
