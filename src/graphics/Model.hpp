#ifndef OCTAVE_MODEL_HPP
#define OCTAVE_MODEL_HPP

#include <assimp/scene.h>

#include "Mesh.hpp"
#include "pch.hpp"

namespace Octave {

struct VertexPositionNormalTexture {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;

	VertexPositionNormalTexture() = default;

	VertexPositionNormalTexture( const glm::vec3& position,
								 const glm::vec3& normal,
								 const glm::vec2& tex_coords ) noexcept
		: position( position ), normal( normal ), tex_coords( tex_coords ) {}
};

class Model {
public:
	Model() = default;
	Model( Model&& other ) noexcept;
	~Model() = default;

	void Draw( Shader& shader, const Renderer& renderer ) const;

	[[nodiscard]] const std::vector<Mesh>& GetMeshes() const noexcept {
		return meshes_;
	}

	Model& AddMesh( Mesh&& mesh ) noexcept {
		meshes_.emplace_back( mesh );
		return *this;
	}

	static Model LoadFromFile( const std::filesystem::path& path );

private:
	std::vector<Mesh> meshes_;

	explicit Model( const std::vector<Mesh>& meshes );

public:
	Model& operator=( Model&& other ) noexcept;

private:
	NON_COPYABLE_CLASS( Model )
};

}  // namespace Octave

#endif
