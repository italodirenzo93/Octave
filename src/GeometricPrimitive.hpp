#ifndef GEOMETRICPRIMITIVE_HPP
#define GEOMETRICPRIMITIVE_HPP

#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexArrayLayout.hpp"
#include "graphics/VertexBuffer.hpp"

class GeometricPrimitive {
public:
    struct VertexType {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coord;

        VertexType() = default;
        explicit VertexType( std::initializer_list<float> initializer_list );
    };


    static void CreateCube( graphics::VertexArrayLayout& vao,
                            graphics::VertexBuffer& vbo,
                            graphics::IndexBuffer& ibo );
};

#endif
