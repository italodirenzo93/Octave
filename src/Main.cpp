#include "CommonInclude.hpp"
#include "Config.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/VertexArrayLayout.hpp"

using namespace std;

struct VertexPositionColor {
    glm::vec3 position;
    glm::vec3 color;

    VertexPositionColor() = default;
    VertexPositionColor( const glm::vec3& pos, const glm::vec3& col )
        : position( pos ), color( col ) {}
};

int main() {
    config::Initialize();

    // Renderer initialization
    if ( !graphics::Initialize() ) {
        return EXIT_FAILURE;
    }
    cout << graphics::GetRendererInfo() << endl;

    graphics::VertexArrayLayout vao;
    vao.AddBinding( { graphics::POSITION, 3, GL_FLOAT, false } )
        .AddBinding( { graphics::COLOR, 3, GL_FLOAT, false } );

    graphics::VertexBuffer<VertexPositionColor> vbo;
    vbo.SetData( {
        VertexPositionColor( glm::vec3( 0.0f, 1.0f, 0.0f ),
                             glm::vec3( 1.0f, 0.0f, 0.0f ) ),
        VertexPositionColor( glm::vec3( -1.0f, 1.0f, 0.0f ),
                             glm::vec3( 0.0f, 1.0f, 0.0f ) ),
        VertexPositionColor( glm::vec3( 1.0f, 1.0f, 0.0f ),
                             glm::vec3( 0.0f, 0.0f, 1.0f ) ),
    } );

    vao.MapToBuffer(vbo);

    // Main loop
    while ( graphics::IsWindowOpen() ) {
        graphics::Clear( true, 0.1f, 0.1f, 0.1f );

        // TODO: Draw scene here
        auto shader = graphics::ShaderManager::Instance().Get( "basic" );

        shader->Activate();
        shader->SetMat4("uMatProjection", glm::identity<glm::mat4>());
        shader->SetMat4("uMatView", glm::identity<glm::mat4>());
        shader->SetMat4("uMatModel", glm::identity<glm::mat4>());

        vao.Bind();
        glDrawArrays( GL_TRIANGLES, 0, 3 );

        graphics::Present();
    }

    graphics::Terminate();

    return EXIT_SUCCESS;
}
