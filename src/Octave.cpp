#include "Octave.hpp"

#include "CommonInclude.hpp"

using namespace std;

namespace octave {

static bool b_is_initialized = false;

bool Up() {
	if ( b_is_initialized ) return false;

	// Initialize GLFW library
	if ( !glfwInit() ) {
		cout << "Failed to initialize GLFW" << endl;
		return false;
	}

	b_is_initialized = true;
	return true;
}

void Down() {
	if ( !b_is_initialized ) return;

	glfwTerminate();

	b_is_initialized = false;
}

}  // namespace octave
