#include "Model.hpp"

// #include <assimp/postprocess.h>

// #include <assimp/Importer.hpp>

using namespace std;

namespace Octave {

static unordered_map<string, SharedRef<Texture>> texture_cache;

// static Mesh ProcessMesh( const string& directory, aiMesh* mesh,
// 						 const aiScene* scene ) {
// 	Mesh ret_mesh;

// 	vector<VertexPositionNormalTexture> vertices;
// 	vector<uint32_t> indices;

// 	auto vbo = MakeShared<VertexBuffer>();
// 	auto ibo = MakeShared<IndexBuffer>();
// 	vector<SharedRef<Texture>> textures;

// 	// Process vertices
// 	for ( uint32_t i = 0; i < mesh->mNumVertices; i++ ) {
// 		VertexPositionNormalTexture v = {};

// 		v.position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y,
// 								mesh->mVertices[i].z );

// 		if ( mesh->HasNormals() ) {
// 			v.normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y,
// 								  mesh->mNormals[i].z );
// 		}

// 		if ( mesh->mTextureCoords[0] ) {
// 			v.tex_coords = glm::vec2( mesh->mTextureCoords[0][i].x,
// 									  mesh->mTextureCoords[0][i].y );
// 		} else {
// 			v.tex_coords = glm::vec2( 0.0f );
// 		}

// 		vertices.push_back( v );
// 	}

// 	// Process indices
// 	for ( uint32_t i = 0; i < mesh->mNumFaces; i++ ) {
// 		auto face = mesh->mFaces[i];
// 		for ( uint32_t j = 0; j < face.mNumIndices; j++ ) {
// 			indices.push_back( face.mIndices[j] );
// 		}
// 	}

// 	// Process material
// 	if ( mesh->mMaterialIndex >= 0 ) {
// 		const auto material = scene->mMaterials[mesh->mMaterialIndex];

// 		auto load_texture = [&]( aiTextureType type ) {
// 			aiString str;
// 			material->GetTexture( type, 0, &str );

// 			filesystem::path texture_path = str.C_Str();
// 			const auto full_path = directory / texture_path;

// 			const auto iter = texture_cache.find( full_path.string() );
// 			if ( iter != texture_cache.end() ) {
// 				return iter->second;
// 			}

// 			auto texture = MakeShared<Texture>();
// 			try {
// 				texture->LoadFromFile( full_path.string() );
// 				texture_cache[full_path.string()] = texture;
// 			} catch ( const Exception& e ) {
// 				cerr << "Error loading model texture : " << e.what() << endl;
// 			}
// 			return texture;
// 		};

// 		// Load diffuse maps
// 		if ( material->GetTextureCount( aiTextureType_DIFFUSE ) > 0 ) {
// 			textures.emplace_back( load_texture( aiTextureType_DIFFUSE ) );
// 		}

// 		// Load specular maps
// 		if ( material->GetTextureCount( aiTextureType_SPECULAR ) > 0 ) {
// 			textures.emplace_back( load_texture( aiTextureType_SPECULAR ) );
// 		}

// 		// Other material properties
// 		float shininess;
// 		if ( material->Get( AI_MATKEY_SHININESS, shininess ) ==
// 			 aiReturn_SUCCESS ) {
// 			ret_mesh.SetShininess( shininess );
// 		}
// 	}

// 	// Populate the buffers
// 	vbo->SetData( { { VertexAttributeName::kPosition, 3, GL_FLOAT, false },
// 					{ VertexAttributeName::kNormal, 3, GL_FLOAT, false },
// 					{ VertexAttributeName::kTexCoord, 2, GL_FLOAT, false } },
// 				  vertices );

// 	ibo->SetData( indices );

// 	ret_mesh.SetVertexBuffer( vbo );
// 	ret_mesh.SetIndexBuffer( ibo );
// 	ret_mesh.SetTextures( textures );

// 	return ret_mesh;
// }

// static void ProcessNode( const string& directory, vector<Mesh>& meshes,
// 						 aiNode* node, const aiScene* scene ) {
// 	for ( uint32_t i = 0; i < node->mNumMeshes; i++ ) {
// 		auto mesh = scene->mMeshes[node->mMeshes[i]];
// 		meshes.emplace_back( ProcessMesh( directory, mesh, scene ) );
// 	}

// 	for ( uint32_t i = 0; i < node->mNumChildren; i++ ) {
// 		ProcessNode( directory, meshes, node->mChildren[i], scene );
// 	}
// }

Model::Model( const std::vector<Mesh>& meshes ) {
	meshes_ = meshes;
}

Model::Model( Model&& other ) noexcept {
	meshes_ = std::move( other.meshes_ );
}

void Model::Draw( Shader& shader, const GraphicsContext& renderer ) const {
	for ( const auto& mesh : meshes_ ) {
		mesh.Draw( shader, renderer );
	}
}

Model Model::LoadFromFile( const std::filesystem::path& path ) {
	// Assimp::Importer importer;
	// const auto scene = importer.ReadFile(
	// 	path.string(), aiProcess_Triangulate | aiProcess_FlipWindingOrder );

	// if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
	// 	 !scene->mRootNode ) {
	// 	throw Exception( "Unable to open file " + path.string() );
	// }

	// vector<Mesh> meshes;

	// // Recursively read mesh data
	// ProcessNode( path.parent_path().string(), meshes, scene->mRootNode, scene );

	// return Model( meshes );
	return {};
}

Model& Model::operator=( Model&& other ) noexcept {
	meshes_ = std::move( other.meshes_ );
	return *this;
}

}  // namespace Octave
