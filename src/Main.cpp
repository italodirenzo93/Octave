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
		// rotate
		model_matrix = glm::scale( model_matrix, scale );

		shader.SetMat4( "uMatModel", model_matrix );
		{
			int n = 0;
			for ( const auto& texture : textures ) {
				shader.SetTexture( "uTextures", n++, *texture );
			}
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

	shader.SetVec3( "uDirectionalLight.direction", direction );
	shader.SetVec3( "uDirectionalLight.ambient", ambient );
	shader.SetVec3( "uDirectionalLight.diffuse", diffuse );
	shader.SetVec3( "uDirectionalLight.specular", specular );

	shader.SetBool( "uPointLights[0].enabled", true );
	shader.SetVec3( "uPointLights[0].position", glm::vec3( 2, 3, -2 ) );
	shader.SetVec3( "uPointLights[0].ambient", ambient );
	shader.SetVec3( "uPointLights[0].diffuse", diffuse );
	shader.SetVec3( "uPointLights[0].specular", specular );
	shader.SetFloat( "uPointLights[0].constant", 1.0f );
	shader.SetFloat( "uPointLights[0].linear", 0.09f );
	shader.SetFloat( "uPointLights[0].quadratic", 0.032f );
}

void DebugCameraControls( const Keyboard& keyboard, DebugCamera& camera,
						  float camera_speed, float delta ) {
	// Strafe Left
	if ( keyboard.IsKeyDown( GLFW_KEY_A ) ) {
		camera.position_ -=
			glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
			camera_speed * delta;
	}
	// Strafe right
	if ( keyboard.IsKeyDown( GLFW_KEY_D ) ) {
		camera.position_ +=
			glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
			camera_speed * delta;
	}
	// Forward
	if ( keyboard.IsKeyDown( GLFW_KEY_W ) ) {
		camera.position_ += camera_speed * delta * camera.front_;
	}
	// Backward
	if ( keyboard.IsKeyDown( GLFW_KEY_S ) ) {
		camera.position_ -= camera_speed * delta * camera.front_;
	}
	// Up
	if ( keyboard.IsKeyDown( GLFW_KEY_E ) ) {
		camera.position_ += camera_speed * delta * camera.up_;
	}
	// Down
	if ( keyboard.IsKeyDown( GLFW_KEY_Q ) ) {
		camera.position_ -= camera_speed * delta * camera.up_;
	}

	// Turn left
	const float turn_speed = camera_speed * 3.0f;
	if ( keyboard.IsKeyDown( GLFW_KEY_LEFT ) ) {
		camera.yaw_ -= turn_speed * delta;
	}
	// Turn right
	if ( keyboard.IsKeyDown( GLFW_KEY_RIGHT ) ) {
		camera.yaw_ += turn_speed * delta;
	}
	// Look up
	if ( keyboard.IsKeyDown( GLFW_KEY_UP ) ) {
		camera.pitch_ += turn_speed * delta;
	}
	// Look down
	if ( keyboard.IsKeyDown( GLFW_KEY_DOWN ) ) {
		camera.pitch_ -= turn_speed * delta;
	}
}

int main( int argc, char* argv[] ) {
	// Initialize engine
	octave::Up();

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

		DebugCamera camera;
		camera.position_ = glm::vec3( 0, 0, 10 );
		camera.field_of_view_ = Config::Instance().GetFieldOfView();
		camera.width_ = static_cast<float>( width );
		camera.height_ = static_cast<float>( height );

		auto model_matrix = glm::identity<glm::mat4>();

		const auto shader = ShaderManager::Instance().Get( "basic" );
		if ( !shader ) {
			throw Exception( "Missing required shader " );
		}

		renderer.SetShader( *shader );
		SetDefaultLighting( *shader );

		// Update camera aspect when window size is changed
		window.AddSizeChangedCallback( [&camera, &renderer]( int w, int h ) {
			if ( w > 0 && h > 0 ) {
				renderer.SetViewport( 0, 0, w, h );
				camera.width_ = static_cast<float>( w );
				camera.height_ = static_cast<float>( h );
			}
		} );


		Keyboard keyboard( window );
		StepTimer step_timer;

		Object floor;
		{
			VertexBuffer vbo;
			GeometricPrimitive::CreatePlane( vbo );
			floor.vbo = std::move( vbo );

			auto texture = make_shared<Texture>();
			texture->LoadFromFile( "./resources/textures/wood_diffuse.png" );
			floor.textures.emplace_back( texture );

			texture = make_shared<Texture>();
			texture->LoadFromFile( "./resources/textures/wood_specular.png" );
			floor.textures.emplace_back( texture );
		}
		floor.position = glm::vec3( 0.0f, -3.0f, 0.0f );

		// Main loop
		while ( window.IsOpen() ) {
			renderer.SetShader( *shader );

			step_timer.Tick( [&] {
				if ( keyboard.IsKeyDown( GLFW_KEY_ESCAPE ) ) {
					window.Close();
				}

				const auto delta =
					static_cast<float>( step_timer.GetElapsedSeconds() );

				model_matrix =
					glm::rotate( model_matrix, glm::radians( delta * 25.0f ),
								 glm::vec3( 0.0f, 1.0f, 0.0f ) );

				DebugCameraControls( keyboard, camera, 25.0f, delta );
			} );

			shader->SetMat4( "uMatProjection", camera.GetProjectionMatrix() );
			shader->SetMat4( "uMatView", camera.GetViewMatrix() );
			shader->SetVec3( "uViewPos", camera.position_ );

			// Clear the viewport
			renderer.Clear( true, true, 0.1f, 0.1f, 0.1f );

			// Draw scene
			shader->SetMat4( "uMatModel", model_matrix );
			model.Draw( *shader, renderer );

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
