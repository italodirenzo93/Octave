#include "CommonInclude.hpp"
#include "graphics/Renderer.hpp"

#include "Config.hpp"

using namespace std;

int main() {
    config::Initialize();

    // Renderer initialization
    if (!graphics::Initialize()) {
        return EXIT_FAILURE;
    }
    cout << graphics::GetRendererInfo() << endl;

    // Main loop
    while (graphics::IsWindowOpen()) {
        graphics::Clear( true, 0.1f, 0.1f, 0.1f );

		// TODO: Draw scene here

        graphics::Present();
    }

    graphics::Terminate();

    return EXIT_SUCCESS;
}
