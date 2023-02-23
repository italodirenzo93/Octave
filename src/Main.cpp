#include "Config.hpp"
#include "GeometricPrimitive.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ShaderManager.hpp"

using namespace std;
using namespace graphics;

int main() {
    // Initialize GLFW library
    if ( !glfwInit() ) {
        cout << "Failed to initialize GLFW... exiting" << endl;
        return EXIT_FAILURE;
    }

    Window window;

	// Renderer initialization
    Renderer renderer( window );
    cout << renderer.GetDescription() << endl;

	int width, height;
    renderer.GetFramebufferSize( width, height );

    VertexBuffer vbo;
    IndexBuffer ibo;

    GeometricPrimitive::CreateCube( vbo, ibo );

    Texture texture;
    texture.LoadFromFile( "resources/textures/container.jpg" );

    auto projection = glm::perspectiveFov(
        glm::radians( 45.0f ),
        static_cast<float>(width),
        static_cast<float>(height),
        0.1f, 100.0f );

    auto view = glm::lookAt( glm::vec3( 2.0f, 0.0f, 5.0f ), glm::vec3( 0.0f ),
                             glm::vec3( 0.0f, 1.0f, 0.0f ) );

    auto model = glm::identity<glm::mat4>();

    auto last_frame_time = static_cast<float>(glfwGetTime());

    // Main loop
    while (window.IsOpen()) {
        auto now = static_cast<float>(glfwGetTime());
        float delta = now - last_frame_time;
        last_frame_time = now;

        model = glm::rotate( model, glm::radians( delta * 25 ),
                             glm::vec3( 0.0f, 1.0f, 0.0f ) );

        renderer.Clear( true, 0.1f, 0.1f, 0.1f );

        // Draw scene
        auto shader = ShaderManager::Instance().Get( "basic" );

        renderer.SetShader( *shader );
        renderer.SetTexture( texture );

        shader->SetMat4( "uMatProjection", projection );
        shader->SetMat4( "uMatView", view );
        shader->SetMat4( "uMatModel", model );

        renderer.DrawIndexedPrimitives( PrimitiveType::kTriangleList, vbo, ibo );

        renderer.Present();

        window.PollEvents();
    }

	glfwTerminate();
    return EXIT_SUCCESS;
}
