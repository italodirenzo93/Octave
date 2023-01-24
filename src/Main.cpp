#include "CommonInclude.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ShaderManager.hpp"

using namespace std;

struct VertexPositionColor {
    glm::vec3 position;
    glm::vec3 color;

    VertexPositionColor() = default;
    VertexPositionColor( const glm::vec3& pos, const glm::vec3& col )
        : position( pos ), color( col ) {}
};

int main() {
    // Renderer initialization
    graphics::Renderer renderer;
    cout << renderer.GetDescription() << endl;

    graphics::VertexArrayLayout vao;
    vao.AddBindings( { { graphics::POSITION, 3, GL_FLOAT, false },
                       { graphics::COLOR, 3, GL_FLOAT, false } } );

    graphics::VertexBuffer vbo;
    vbo.SetData( vao, {
                          VertexPositionColor( glm::vec3( 0.0f, 1.0f, 0.0f ),
                                               glm::vec3( 1.0f, 0.0f, 0.0f ) ),
                          VertexPositionColor( glm::vec3( -1.0f, -1.0f, 0.0f ),
                                               glm::vec3( 0.0f, 1.0f, 0.0f ) ),
                          VertexPositionColor( glm::vec3( 1.0f, -1.0f, 0.0f ),
                                               glm::vec3( 0.0f, 0.0f, 1.0f ) ),
                      } );

    // Main loop
    while ( renderer.IsWindowOpen() ) {
        renderer.Clear( true, 0.1f, 0.1f, 0.1f );

        // TODO: Draw scene here
        auto shader = graphics::ShaderManager::Instance().Get( "basic" );

        shader->Activate();
        shader->SetMat4( "uMatProjection", glm::identity<glm::mat4>() );
        shader->SetMat4( "uMatView", glm::identity<glm::mat4>() );
        shader->SetMat4( "uMatModel", glm::identity<glm::mat4>() );

        renderer.DrawPrimitives( vao, vbo );

        renderer.Present();
    }

    return EXIT_SUCCESS;
}
