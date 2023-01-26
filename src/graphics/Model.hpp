#ifndef MODEL_HPP
#define MODEL_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include <assimp/scene.h>

//namespace graphics {
//class Model {
//public:
//    explicit Model( const char* path );
//
//    void draw( const Shader& program ) const;
//
//private:
//    std::vector<Mesh> meshes_;
//    std::string directory_;
//    std::vector<Mesh::Texture> texures_loaded_;
//
//    void LoadModel( const std::string& path );
//    void ProcessNode( aiNode* node, const aiScene* scene );
//    Mesh ProcessMesh( aiMesh* mesh, const aiScene* scene );
//    std::vector<Mesh::Texture> LoadMaterialTextures(
//        aiMaterial* material, aiTextureType texture_type,
//        const std::string& type_name );
//};
//} // namespace graphics

#endif
