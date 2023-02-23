#ifndef _MODEL_HPP
#define MODEL_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

#include <assimp/scene.h>

namespace graphics {
class Model {
public:
    Model( const char* path );

    void draw( const Shader& program ) const;

private:
    std::vector<Mesh> _meshes;
    std::string _directory;
    std::vector<Mesh::Texture> _texuresLoaded;

    void loadModel( const std::string& path );
    void processNode( aiNode* node, const aiScene* scene );
    Mesh processMesh( aiMesh* mesh, const aiScene* scene );
    std::vector<Mesh::Texture> loadMaterialTextures(
        aiMaterial* material, aiTextureType textureType,
        const std::string& typeName );
};
} // namespace graphics

#endif
