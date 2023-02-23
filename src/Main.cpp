#include "Config.hpp"
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
    graphics::Window window;

    // Renderer initialization
    graphics::Renderer renderer( window );
    cout << renderer.GetDescription() << endl;

    graphics::VertexArrayLayout vao{ { graphics::POSITION, 3, GL_FLOAT, false },
                                     { graphics::COLOR, 3, GL_FLOAT, false } };

    graphics::VertexBuffer vbo;
    vbo.SetData( vao, {
                          VertexPositionColor( glm::vec3( 0.0f, 1.0f, 0.0f ),
                                               glm::vec3( 1.0f, 0.0f, 0.0f ) ),
                          VertexPositionColor( glm::vec3( -1.0f, -1.0f, 0.0f ),
                                               glm::vec3( 0.0f, 1.0f, 0.0f ) ),
                          VertexPositionColor( glm::vec3( 1.0f, -1.0f, 0.0f ),
                                               glm::vec3( 0.0f, 0.0f, 1.0f ) ),
                      } );

    auto projection = glm::perspective(
        glm::radians( 45.0f ),
        static_cast<float>( Config::Instance().GetFramebufferWidth() ) /
            static_cast<float>( Config::Instance().GetFramebufferHeight() ),
        0.1f, 100.0f );

    auto view = glm::lookAt( glm::vec3( 2.0f, 0.0f, 5.0f ), glm::vec3( 0.0f ),
                             glm::vec3( 0.0f, 1.0f, 0.0f ) );

    // Main loop
    while ( window.IsOpen() ) {
        renderer.Clear( true, 0.1f, 0.1f, 0.1f );

        // TODO: Draw scene here
        auto shader = graphics::ShaderManager::Instance().Get( "basic" );

        shader->Activate();
        shader->SetMat4( "uMatProjection", projection );
        shader->SetMat4( "uMatView", view );
        shader->SetMat4( "uMatModel", glm::identity<glm::mat4>() );

        renderer.DrawPrimitives( vao, vbo );

        renderer.Present();

        window.PollEvents();
    }

    return EXIT_SUCCESS;
}
