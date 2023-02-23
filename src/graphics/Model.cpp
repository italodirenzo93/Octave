#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace graphics {
using namespace std;

uint32_t textureFromFile( const char* fileName, const string& directory ) {
    uint32_t ID = 0;

    auto fullPath = directory + "/" + fileName;

    int w, h, nChannels;
    auto image =
        stbi_load( fullPath.c_str(), &w, &h, &nChannels, STBI_default );
    if (image) {
        GLint format = GL_RGB;
        if (nChannels == 1)
            format = GL_RED;
        else if (nChannels == 3)
            format = GL_RGB;
        else if (nChannels == 4)
            format = GL_RGBA;

        glGenTextures( 1, &ID );
        glBindTexture( GL_TEXTURE_2D, ID );

        glTexImage2D( GL_TEXTURE_2D, 0, format, w, h, 0, format,
                      GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );

        glBindTexture( GL_TEXTURE_2D, 0 );
        stbi_image_free( image );
    } else {
        cout << "ERROR: Could not find texture at " << fullPath << "." << endl;
    }

    return ID;
}

Model::Model( const char* path ) { loadModel( path ); }

void Model::draw( const Shader& program ) const {
    for (auto& mesh : _meshes) {
        mesh.draw( program );
    }
}

void Model::loadModel( const std::string& path ) {
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }

    _directory = path.substr( 0, path.find_last_of( '/' ) );

    processNode( scene->mRootNode, scene );
}

void Model::processNode( aiNode* node, const aiScene* scene ) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back( processMesh( mesh, scene ) );
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode( node->mChildren[i], scene );
    }
}

Mesh Model::processMesh( aiMesh* mesh, const aiScene* scene ) {
    vector<Mesh::Vertex> vertices;
    vector<uint32_t> indices;
    vector<Mesh::Texture> textures;

    // Process vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Mesh::Vertex v = {};

        v.position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z );

        if (mesh->HasNormals()) {
            v.normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z );
        }

        if (mesh->mTextureCoords[0]) {
            v.texCoords = glm::vec2( mesh->mTextureCoords[0][i].x,
                                     mesh->mTextureCoords[0][i].y );
        } else {
            v.texCoords = glm::vec2( 0.0f );
        }

        vertices.push_back( v );
    }

    // Process indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back( face.mIndices[j] );
        }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0) {
        auto material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse" );
        textures.insert( textures.end(), diffuseMaps.begin(),
                         diffuseMaps.end() );

        vector<Mesh::Texture> specularMaps = loadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular" );
        textures.insert( textures.end(), specularMaps.begin(),
                         specularMaps.end() );
    }

    return Mesh( vertices, indices, textures );
}

vector<Mesh::Texture> Model::loadMaterialTextures( aiMaterial* material,
                                                   aiTextureType textureType,
                                                   const string& typeName ) {
    vector<Mesh::Texture> textures;
    for (uint32_t i = 0; i < material->GetTextureCount( textureType ); i++) {
        aiString str;
        material->GetTexture( textureType, i, &str );
        bool skip = false;

        for (auto& tex : _texuresLoaded) {
            if (std::strcmp( tex.path.c_str(), str.C_Str() ) == 0) {
                textures.push_back( tex );
                skip = true;
                break;
            }
        }

        if (!skip) {
            Mesh::Texture texture;
            texture.id = textureFromFile( str.C_Str(), _directory );
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back( texture );
            _texuresLoaded.push_back( texture );
        }
    }
    return textures;
}
} // namespace graphics
