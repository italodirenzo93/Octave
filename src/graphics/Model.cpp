//#include "Model.hpp"
//
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//
//namespace octave::graphics {
//using namespace std;
//
//uint32_t TextureFromFile( const char* file_name, const string& directory ) {
//    uint32_t id = 0;
//
//    const auto full_path = directory + "/" + file_name;
//
//    int w, h, n_channels;
//    auto image =
//        stbi_load( full_path.c_str(), &w, &h, &n_channels, STBI_default );
//    if (image) {
//        GLint format = GL_RGB;
//        if (n_channels == 1)
//            format = GL_RED;
//        else if (n_channels == 3)
//            format = GL_RGB;
//        else if (n_channels == 4)
//            format = GL_RGBA;
//
//        glGenTextures( 1, &id );
//        glBindTexture( GL_TEXTURE_2D, id );
//
//        glTexImage2D( GL_TEXTURE_2D, 0, format, w, h, 0, format,
//                      GL_UNSIGNED_BYTE, image );
//        glGenerateMipmap( GL_TEXTURE_2D );
//
//        glBindTexture( GL_TEXTURE_2D, 0 );
//        stbi_image_free( image );
//    } else {
//        cout << "ERROR: Could not find texture at " << full_path << "." << endl;
//    }
//
//    return id;
//}
//
//Model::Model( const char* path ) { LoadModel( path ); }
//
//void Model::draw( const Shader& program ) const {
//    for (auto& mesh : meshes_) {
//        mesh.draw( program );
//    }
//}
//
//void Model::LoadModel( const std::string& path ) {
//    Assimp::Importer importer;
//    const aiScene* scene =
//        importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
//        !scene->mRootNode) {
//        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
//        return;
//    }
//
//    directory_ = path.substr( 0, path.find_last_of( '/' ) );
//
//    ProcessNode( scene->mRootNode, scene );
//}
//
//void Model::ProcessNode( aiNode* node, const aiScene* scene ) {
//    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
//        auto mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes_.push_back( ProcessMesh( mesh, scene ) );
//    }
//
//    for (uint32_t i = 0; i < node->mNumChildren; i++) {
//        ProcessNode( node->mChildren[i], scene );
//    }
//}
//
//Mesh Model::ProcessMesh( aiMesh* mesh, const aiScene* scene ) {
//    vector<Mesh::Vertex> vertices;
//    vector<uint32_t> indices;
//    vector<Mesh::Texture> textures;
//
//    // Process vertices
//    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
//        Mesh::Vertex v = {};
//
//        v.position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y,
//                                mesh->mVertices[i].z );
//
//        if (mesh->HasNormals()) {
//            v.normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y,
//                                  mesh->mNormals[i].z );
//        }
//
//        if (mesh->mTextureCoords[0]) {
//            v.tex_coords = glm::vec2( mesh->mTextureCoords[0][i].x,
//                                     mesh->mTextureCoords[0][i].y );
//        } else {
//            v.tex_coords= glm::vec2( 0.0f );
//        }
//
//        vertices.push_back( v );
//    }
//
//    // Process indices
//    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
//        auto face = mesh->mFaces[i];
//        for (uint32_t j = 0; j < face.mNumIndices; j++) {
//            indices.push_back( face.mIndices[j] );
//        }
//    }
//
//    // Process material
//    if (mesh->mMaterialIndex >= 0) {
//        auto material = scene->mMaterials[mesh->mMaterialIndex];
//
//        vector<Mesh::Texture> diffuse_maps = LoadMaterialTextures(
//            material, aiTextureType_DIFFUSE, "texture_diffuse" );
//        textures.insert( textures.end(), diffuse_maps.begin(),
//                         diffuse_maps.end() );
//
//        vector<Mesh::Texture> specular_maps = LoadMaterialTextures(
//            material, aiTextureType_SPECULAR, "texture_specular" );
//        textures.insert( textures.end(), specular_maps.begin(),
//                         specular_maps.end() );
//    }
//
//    return Mesh( vertices, indices, textures );
//}
//
//vector<Mesh::Texture> Model::LoadMaterialTextures( aiMaterial* material,
//                                                   aiTextureType texture_type,
//                                                   const string& type_name ) {
//    vector<Mesh::Texture> textures;
//    for (uint32_t i = 0; i < material->GetTextureCount( texture_type ); i++) {
//        aiString str;
//        material->GetTexture( texture_type, i, &str );
//        bool skip = false;
//
//        for (auto& tex : texures_loaded_) {
//            if (std::strcmp( tex.path.c_str(), str.C_Str() ) == 0) {
//                textures.push_back( tex );
//                skip = true;
//                break;
//            }
//        }
//
//        if (!skip) {
//            Mesh::Texture texture;
//            texture.id = TextureFromFile( str.C_Str(), directory_ );
//            texture.type = type_name;
//            texture.path = str.C_Str();
//            textures.push_back( texture );
//            texures_loaded_.push_back( texture );
//        }
//    }
//    return textures;
//}
//} // namespace octave::graphics
