#include <optional>

#include "Camera.hpp"
#include "Config.hpp"
#include "GeometricPrimitive.hpp"
#include "Octave.hpp"
#include "StepTimer.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ShaderManager.hpp"
#include "input/Keyboard.hpp"

using namespace std;
using namespace octave;
using namespace octave::graphics;
using namespace octave::input;

struct Transform {
	glm::vec3 translation = glm::vec3( 0.0f );
	glm::vec3 rotation = glm::vec3( 0.0f );
	glm::vec3 scale = glm::vec3( 1.0f );

	Transform() = default;
	explicit Transform( const glm::vec3& translation = glm::vec3( 0.0f ),
						const glm::vec3& rotation = glm::vec3( 0.0f ),
						const glm::vec3& scale = glm::vec3( 1.0f ) )
		: translation( translation ), rotation( rotation ), scale( scale ) {}
	~Transform() = default;

	[[nodiscard]] glm::mat4 GetModelMatrix() const noexcept {
		auto model = glm::identity<glm::mat4>();

		// Translation
		model = glm::translate( model, translation );

		// Rotation
		model = glm::rotate( model, glm::radians( rotation.x ),
							 glm::vec3( 1.0f, 0.0f, 0.0f ) );
		model = glm::rotate( model, glm::radians( rotation.y ),
							 glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::rotate( model, glm::radians( rotation.z ),
							 glm::vec3( 0.0f, 0.0f, 1.0f ) );

		// Scale
		model = glm::scale( model, scale );

		return model;
	}
};

struct Object {
	optional<VertexBuffer> vbo;
	optional<IndexBuffer> ibo;
	vector<shared_ptr<Texture>> textures;

	glm::vec3 position = glm::vec3( 0.0f );
	glm::vec3 scale = glm::vec3( 1.0f );

	void Draw( Shader& shader, const Renderer& renderer ) const noexcept {
		if ( !vbo ) return;

		auto model_matrix = glm::identity<glm::mat4>();
		model_matrix = glm::translate( model_matrix, position );
		model_matrix = glm::scale( model_matrix, scale );

		shader.SetMat4( "uMatModel", model_matrix );

		int n = 0;
		for ( const auto& texture : textures ) {
			shader.SetTexture( "uTextures", n++, *texture );
		}

		if ( ibo ) {
			renderer.DrawIndexedPrimitives( PrimitiveType::kTriangleList, *vbo,
											*ibo );
		} else {
			renderer.DrawPrimitives( PrimitiveType::kTriangleList, *vbo );
		}
	}
};

Mesh LoadCube() {
	auto vbo = make_shared<VertexBuffer>();
	auto ibo = make_shared<IndexBuffer>();

	GeometricPrimitive::CreateCube( *vbo, *ibo );

	auto texture = make_shared<Texture>();
	texture->LoadFromFile( "resources/textures/container.jpg" );

	vector<shared_ptr<Texture>> textures;
	textures.emplace_back( std::move( texture ) );

	return { vbo, ibo, textures };
}

inline void SetDefaultLighting( Shader& shader ) {
	const glm::vec3 direction( 0.5f, 0.0f, -0.5f );
	const glm::vec3 ambient( 0.2f );
	const glm::vec3 diffuse( 0.5f );
	const glm::vec3 specular( 0.8f );

	shader.SetVec3( "uLight.direction", direction );
	shader.SetVec3( "uLight.ambient", ambient );
	shader.SetVec3( "uLight.diffuse", diffuse );
	shader.SetVec3( "uLight.specular", specular );
}

void DebugCameraControls( const Keyboard& keyboard, Camera& camera,
						  float camera_speed, float delta ) {
	auto translation = glm::vec3( 0.0f );
	// Strafe Left
	if ( keyboard.IsKeyDown( GLFW_KEY_A ) ) {
		translation.x = -camera_speed * delta;
	}
	// Strafe right
	if ( keyboard.IsKeyDown( GLFW_KEY_D ) ) {
		translation.x = camera_speed * delta;
	}
	// Forward
	if ( keyboard.IsKeyDown( GLFW_KEY_W ) ) {
		translation.z = camera_speed * delta;
	}
	// Backward
	if ( keyboard.IsKeyDown( GLFW_KEY_S ) ) {
		translation.z = -camera_speed * delta;
	}
	// Up
	if ( keyboard.IsKeyDown( GLFW_KEY_E ) ) {
		translation.y = camera_speed * delta;
	}
	// Down
	if ( keyboard.IsKeyDown( GLFW_KEY_Q ) ) {
		translation.y = -camera_speed * delta;
	}

	auto euler_angles = glm::zero<glm::vec3>();
	// Turn left
	if ( keyboard.IsKeyDown( GLFW_KEY_LEFT ) ) {
		euler_angles.y = -camera_speed * delta;
	}
	// Turn right
	if ( keyboard.IsKeyDown( GLFW_KEY_RIGHT ) ) {
		euler_angles.y = camera_speed * delta;
	}
	// Look up
	if ( keyboard.IsKeyDown( GLFW_KEY_UP ) ) {
		euler_angles.x = camera_speed * delta;
	}
	// Look down
	if ( keyboard.IsKeyDown( GLFW_KEY_DOWN ) ) {
		euler_angles.x = -camera_speed * delta;
	}
	camera.Translate( translation );
	camera.Rotate( euler_angles );
}

int main( int argc, char* argv[] ) {
	// Initialize engine
	octave::Up();

	std::vector<Transform> object_transforms{
		Transform( glm::vec3( 0.0f ), glm::vec3( 90.0f, 0.0f, 0.0f ) ),
		Transform( glm::vec3( 1.0f, 0.25f, -1.0f ),
				   glm::vec3( 90.0f, 0.0f, 90.0f ) ) };

	try {
		Window window;

		// Renderer initialization
		Renderer renderer( window );
		cout << renderer.GetDescription() << endl;

		int width, height;
		renderer.GetFramebufferSize( width, height );

		Model model;

		if ( argc > 1 ) {
			model = Model::LoadFromFile( argv[1] );
		} else {
			model.AddMesh( LoadCube() );
		}

		Camera camera;
		camera.SetPosition( 0.0f, 0.0f, 7.0f );
		camera.SetFieldOfView( Config::Instance().GetFieldOfView() );
		camera.SetAspectRatio( static_cast<float>( width ) /
							   static_cast<float>( height ) );

		auto model_matrix = glm::identity<glm::mat4>();

		const auto shader = ShaderManager::Instance().Get( "basic" );
		if ( !shader ) {
			throw Exception( "Missing required shader " );
		}

		renderer.SetShader( *shader );
		SetDefaultLighting( *shader );

		// Update camera aspect when window size is changed
		window.AddSizeChangedCallback( [&camera]( int w, int h ) {
			const float aspect =
				static_cast<float>( w ) / static_cast<float>( h );
			camera.SetAspectRatio( aspect );
		} );

		Keyboard keyboard( window );
		StepTimer step_timer;

		Object floor;
		{
			VertexBuffer vbo;
			GeometricPrimitive::CreatePlane( vbo );
			floor.vbo = std::move( vbo );

			auto texture = make_shared<Texture>();
			texture->LoadFromFile( "./resources/textures/wood.png" );
			floor.textures.emplace_back( texture );
		}
		floor.position = glm::vec3( 0.0f, -3.0f, 0.0f );
		floor.scale = glm::vec3( 10, 0, 10 );

		// Main loop
		while ( window.IsOpen() ) {
			step_timer.Tick();
			const auto delta = step_timer.GetDeltaTime();

			if ( keyboard.IsKeyDown( GLFW_KEY_ESCAPE ) ) {
				window.Close();
			}

			DebugCameraControls( keyboard, camera, 25.0f, delta );

			shader->SetMat4( "uMatProjection", camera.GetProjectionMatrix() );
			shader->SetMat4( "uMatView", camera.GetViewMatrix() );
			shader->SetVec3( "uViewPos", camera.GetPosition() );

			// Clear the viewport
			renderer.Clear( true, true, 0.1f, 0.1f, 0.1f );

			// Draw scene
			//			model_matrix =
			//				glm::rotate( model_matrix, glm::radians( delta * 25.0f
			//), 							 glm::vec3( 0.0f, 1.0f, 0.0f ) ); 			shader->SetMat4( "uMatModel",
			//model_matrix ); 			model.Draw( *shader, renderer );

			// Draw the floor
			floor.Draw( *shader, renderer );

			// Show the result
			renderer.Present();
			window.PollEvents();
		}
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;

		octave::Down();
		return EXIT_FAILURE;
	}

	octave::Down();
	return EXIT_SUCCESS;
}
