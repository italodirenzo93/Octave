#ifndef OCTAVE_MESH_HPP
#define OCTAVE_MESH_HPP

#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

namespace Octave {

class Mesh {
public:
	Mesh() noexcept;
	Mesh( const SharedRef<VertexBuffer>& vbo,
		  const SharedRef<IndexBuffer>& ibo,
		  const std::vector<SharedRef<Texture>>& textures ) noexcept;
	Mesh( const Mesh& other ) noexcept;
	Mesh( Mesh&& other ) noexcept;
	~Mesh() = default;

	void Draw( Shader& shader, const GraphicsContext& renderer ) const;

	Mesh& SetVertexBuffer( const SharedRef<VertexBuffer>& vbo ) noexcept;
	Mesh& SetIndexBuffer( const SharedRef<IndexBuffer>& ibo ) noexcept;
	Mesh& SetTextures(
		const std::vector<SharedRef<Texture>>& textures ) noexcept;

	[[nodiscard]] float GetShininess() const noexcept { return shininess_; }
	Mesh& SetShininess( float shininess ) noexcept;

private:
	SharedRef<VertexBuffer> vbo_;
	SharedRef<IndexBuffer> ibo_;
	std::vector<SharedRef<Texture>> textures_;

	float shininess_;

public:
	Mesh& operator=( const Mesh& other ) noexcept;
	Mesh& operator=( Mesh&& other ) noexcept;
};

}  // namespace Octave

#endif
