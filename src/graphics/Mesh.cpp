#include "Mesh.hpp"

namespace Octave {
using namespace std;

static constexpr float kDefaultShininess = 32.0f;

Mesh::Mesh() noexcept {
	shininess_ = kDefaultShininess;
}

Mesh::Mesh( const std::shared_ptr<VertexBuffer>& vbo,
			const std::shared_ptr<IndexBuffer>& ibo,
			const std::vector<std::shared_ptr<Texture>>& textures ) noexcept {
	vbo_ = vbo;
	ibo_ = ibo;
	textures_ = textures;
	shininess_ = kDefaultShininess;
}

Mesh::Mesh( const Mesh& other ) noexcept {
	vbo_ = other.vbo_;
	ibo_ = other.ibo_;
	textures_ = other.textures_;
	shininess_ = other.shininess_;
}

Mesh::Mesh( Mesh&& other ) noexcept {
	vbo_ = std::move( other.vbo_ );
	ibo_ = std::move( other.ibo_ );
	textures_ = std::move( other.textures_ );

	shininess_ = other.shininess_;
	other.shininess_ = kDefaultShininess;
}

void Mesh::Draw( Shader& shader, const Renderer& renderer ) const {
	if ( vbo_ == nullptr ) {
		throw Exception( "Vertex buffer not set" );
	}

	if ( ibo_ == nullptr ) {
		throw Exception( "Index buffer not set" );
	}

	// Set textures
	int texture_index = 0;
	for ( const auto& texture : textures_ ) {
		if ( texture != nullptr ) {
			shader.SetTexture( "uTextures", texture_index, *texture );
		}

		++texture_index;
	}

	// Set shininess
	shader.SetFloat( "uShininess", shininess_ );

	renderer.DrawIndexed( *vbo_, *ibo_ );
}

Mesh& Mesh::SetVertexBuffer(
	const std::shared_ptr<VertexBuffer>& vbo ) noexcept {
	vbo_ = vbo;
	return *this;
}

Mesh& Mesh::SetIndexBuffer( const std::shared_ptr<IndexBuffer>& ibo ) noexcept {
	ibo_ = ibo;
	return *this;
}

Mesh& Mesh::SetTextures(
	const std::vector<std::shared_ptr<Texture>>& textures ) noexcept {
	textures_ = textures;
	return *this;
}

Mesh& Mesh::SetShininess( float shininess ) noexcept {
	shininess_ = glm::clamp( shininess, 0.0f, 32.0f );
	return *this;
}

Mesh& Mesh::operator=( const Mesh& other ) noexcept {
	if ( &other == this ) {
		return *this;
	}

	vbo_ = other.vbo_;
	ibo_ = other.ibo_;
	textures_ = other.textures_;
	shininess_ = other.shininess_;

	return *this;
}

Mesh& Mesh::operator=( Mesh&& other ) noexcept {
	vbo_ = std::move( other.vbo_ );
	ibo_ = std::move( other.ibo_ );
	textures_ = std::move( other.textures_ );

	shininess_ = other.shininess_;
	other.shininess_ = kDefaultShininess;

	return *this;
}

}  // namespace Octave
