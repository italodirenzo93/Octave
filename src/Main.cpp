#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Config.hpp"
#include "GeometricPrimitive.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ShaderManager.hpp"

using namespace std;
using namespace graphics;

struct VertexPositionNormalTexture {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;

    VertexPositionNormalTexture() = default;

    VertexPositionNormalTexture( const glm::vec3& position,
                                 const glm::vec3& normal,
                                 const glm::vec2& tex_coords ) noexcept
        : position( position ), normal( normal ), tex_coords( tex_coords ) {
    }
};

Mesh LoadMesh( const filesystem::path& path ) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(
        path.string(), aiProcess_Triangulate | aiProcess_FlipWindingOrder );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw Exception( importer.GetErrorString() );
    }

    if (scene->mNumMeshes == 0) {
        throw Exception( "Scene has no meshes" );
    }

    const aiMesh* mesh = scene->mMeshes[0];

    VertexBuffer vbo;
    IndexBuffer ibo;
    vector<Texture> textures( 3 );

    // Parse vertex data
    {
        vector<VertexPositionNormalTexture> vertices;
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            const aiVector3D pos = mesh->mVertices[i];

            VertexPositionNormalTexture vertex = {};
            vertex.position.x = pos.x;
            vertex.position.y = pos.y;
            vertex.position.z = pos.z;

            if (mesh->HasNormals()) {
                const aiVector3D normal = mesh->mNormals[i];
                vertex.normal.x = normal.x;
                vertex.normal.y = normal.y;
                vertex.normal.z = normal.z;
            }

            if (mesh->HasTextureCoords( 0 )) {
                const aiVector3D tex_coords = mesh->mTextureCoords[0][i];
                vertex.tex_coords.x = tex_coords.x;
                vertex.tex_coords.y = tex_coords.y;
            }

            vertices.emplace_back( vertex );
        }

        vbo.SetData( {{POSITION, 3, GL_FLOAT, false},
                      {NORMAL, 3, GL_FLOAT, false},
                      {TEXCOORD, 2, GL_FLOAT, false}},
                     vertices );
    }

    // Parse index data
    {
        vector<uint32_t> indices;
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            const aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back( face.mIndices[j] );
            }
        }

        ibo.SetData( indices );
    }

    // Parse textures
    auto material = scene->mMaterials[mesh->mMaterialIndex];
    if (material != nullptr) {
        auto load_texture = [&]( aiTextureType type ) {
            aiString str;
            material->GetTexture( type, 0, &str );

            Texture texture;
            try {
                texture.LoadFromFile(
                    ( path.parent_path() / str.C_Str() ).string() );
            } catch (const Exception& e) {
                cerr << "Error loading model texture : " << e.what() << endl;
            }
            return texture;
        };

        if (material->GetTextureCount( aiTextureType_DIFFUSE ) > 0) {
            textures[0] = load_texture( aiTextureType_DIFFUSE );
        } else {
            cout << "No diffuse texture to load" << endl;
        }

        if (material->GetTextureCount( aiTextureType_SPECULAR ) > 0) {
            textures[1] = load_texture( aiTextureType_SPECULAR );
        } else {
            cout << "No specular texture to load" << endl;
        }

        if (material->GetTextureCount( aiTextureType_NORMALS ) > 0) {
            textures[2] = load_texture( aiTextureType_NORMALS );
        } else {
            cout << "No height texture to load" << endl;
        }
    }

    return {std::move( vbo ), std::move( ibo ), std::move( textures )};
}

Mesh LoadCube() {
    VertexBuffer vbo;
    IndexBuffer ibo;

    GeometricPrimitive::CreateCube( vbo, ibo );

    Texture texture;
    texture.LoadFromFile( "resources/textures/container.jpg" );

    vector<Texture> textures;
    textures.emplace_back( std::move( texture ) );

    return {std::move( vbo ), std::move( ibo ), std::move( textures )};
}

inline void SetDefaultLighting( const Shader& shader ) {
    const glm::vec3 direction( 0.5f, 0.0f, -0.5f );
    const glm::vec3 ambient( 0.2f );
    const glm::vec3 diffuse( 0.5f );
    const glm::vec3 specular( 0.8f );

    shader.SetVec3( "uLight.direction", direction );
    shader.SetVec3( "uLight.ambient", ambient );
    shader.SetVec3( "uLight.diffuse", diffuse );
    shader.SetVec3( "uLight.specular", specular );
}

int main( int argc, char* argv[] ) {
    // Initialize GLFW library
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW... exiting" << endl;
        return EXIT_FAILURE;
    }

    try {
        Window window;

        // Renderer initialization
        Renderer renderer( window );
        cout << renderer.GetDescription() << endl;

        int width, height;
        renderer.GetFramebufferSize( width, height );

        Mesh mesh;

        if (argc > 1) {
            mesh = LoadMesh( argv[1] );
        } else {
            mesh = LoadCube();
        }

        const float fov = Config::Instance().GetFieldOfView();

        auto projection = glm::perspectiveFov(
            glm::radians( fov ), static_cast<float>(width),
            static_cast<float>(height), 0.1f, 100.0f );

        glm::vec3 position( 0.0f, 0.0f, 5.0f );

        auto view = glm::lookAt( position, glm::vec3( 0.0f ),
                                 glm::vec3( 0.0f, 1.0f, 0.0f ) );

        auto model = glm::identity<glm::mat4>();
        model = glm::rotate( model, glm::radians( 90.0f ),
                             glm::vec3( 1.0f, 0.0f, 0.0f ) );

        auto last_frame_time = static_cast<float>(glfwGetTime());

        auto shader = ShaderManager::Instance().Get( "basic" );
        if (!shader) {
            throw Exception( "Missing required shader " );
        }

        // Update camera aspect when window size is changed
        window.SetSizeChangedCallback( [&projection]( int w, int h ) {
            projection = glm::perspectiveFov(
                glm::radians( 45.0f ), static_cast<float>(w),
                static_cast<float>(h), 0.1f, 100.0f );
        } );

        // Main loop
        while (window.IsOpen()) {
            auto now = static_cast<float>(glfwGetTime());
            float delta = now - last_frame_time;
            last_frame_time = now;

            model = glm::rotate( model, glm::radians( delta * 25 ),
                                 glm::vec3( 0.0f, 0.0f, 1.0f ) );

            renderer.Clear( true, 0.1f, 0.1f, 0.1f );

            // Draw scene
            renderer.SetShader( *shader );

            shader->SetMat4( "uMatProjection", projection );
            shader->SetMat4( "uMatView", view );
            shader->SetMat4( "uMatModel", model );

            shader->SetVec3( "uViewPos", position );
            SetDefaultLighting( *shader );

            mesh.Draw( *shader, renderer );

            renderer.Present();

            window.PollEvents();
        }
    } catch (const std::exception& e) {
        cerr << "Critical Error: " << e.what() << " ...exiting" << endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
