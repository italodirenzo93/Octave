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
	Mesh( const std::shared_ptr<VertexBuffer>& vbo,
		  const std::shared_ptr<IndexBuffer>& ibo,
		  const std::vector<std::shared_ptr<Texture>>& textures ) noexcept;
	Mesh( const Mesh& other ) noexcept;
	Mesh( Mesh&& other ) noexcept;
	~Mesh() = default;

	void Draw( Shader& shader, const Renderer& renderer ) const;

	Mesh& SetVertexBuffer( const std::shared_ptr<VertexBuffer>& vbo ) noexcept;
	Mesh& SetIndexBuffer( const std::shared_ptr<IndexBuffer>& ibo ) noexcept;
	Mesh& SetTextures(
		const std::vector<std::shared_ptr<Texture>>& textures ) noexcept;

	[[nodiscard]] float GetShininess() const noexcept { return shininess_; }
	Mesh& SetShininess( float shininess ) noexcept;

private:
	std::shared_ptr<VertexBuffer> vbo_;
	std::shared_ptr<IndexBuffer> ibo_;
	std::vector<std::shared_ptr<Texture>> textures_;

	float shininess_;

public:
	Mesh& operator=( const Mesh& other ) noexcept;
	Mesh& operator=( Mesh&& other ) noexcept;
};

}  // namespace Octave

#endif
