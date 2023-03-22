// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on

#include "common/Camera.hpp"
#include "helpers/GeometricPrimitive.hpp"

#include <stb_image.h>
#include <fstream>

using namespace std;
using namespace Octave;
using namespace Octave::Samples;

template <class T>
Ref<Buffer> CreateStaticBuffer( GraphicsDevice& device,
								const std::vector<T>& data, size_t stride,
								BufferBinding binding ) {
	BufferDescription desc{};
	desc.size = static_cast<uint32_t>( sizeof( T ) * data.size() );
	desc.stride = static_cast<uint32_t>( stride );
	desc.access_flags = ResourceAccess::ReadWrite;
	desc.bind_flags = binding;

	return device.CreateBuffer( desc, data.data() );
}

inline Ref<Texture2D> CreateTextureFromFile( GraphicsDevice& device,
											 const std::string& filename ) {
	stbi_set_flip_vertically_on_load( true );

	int n_channels, width, height;
	const stbi_uc* image = stbi_load( filename.c_str(), &width, &height,
									  &n_channels, STBI_default );

	if ( !image ) {
		throw Octave::Exception( "Could not load texture from file " +
								 filename );
	}

	TextureDescription2D desc{};
	desc.width = width;
	desc.height = height;
	desc.access_flags = ResourceAccess::ReadWrite;
	desc.mip_levels = 1;

	if ( n_channels == 4 ) {
		desc.format = TextureFormat::Rgba;
	} else {
		desc.format = TextureFormat::Rgb;
	}

	auto texture = device.CreateTexture2D( desc );

	texture->SetData( desc.format, 0, 0, 0, width, height, image );
	texture->GenerateMipmap();

	return texture;
}

inline Ref<Shader> LoadShaderFromFile( GraphicsDevice& device, ShaderType type,
									   const std::string& path ) {
	std::ifstream ifs( path );
	if ( !ifs.is_open() ) {
		throw Octave::Exception( "Unable to open file " + path );
	}

	std::string code( ( std::istreambuf_iterator<char>( ifs ) ),
					  ( std::istreambuf_iterator<char>() ) );

	return device.CreateShader( type, code.c_str() );
}


struct Matrices {
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 model_matrix;
	glm::mat4 normal_matrix;
	glm::vec3 view_position;
	glm::float32 pad;

	Matrices() noexcept = default;
	Matrices( glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::vec3 view_pos ) noexcept
		: projection_matrix( std::move( proj ) ),
		  view_matrix( std::move( view ) ),
		  model_matrix( std::move( model ) ),
		  view_position( std::move(view_pos) ) {
		normal_matrix = glm::transpose( glm::inverse( model_matrix ) );
	}
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::float32 pad_direction;
	glm::vec3 ambient;
	glm::float32 pad_ambient;
	glm::vec3 diffuse;
	glm::float32 pad_diffuse;
	glm::vec3 specular;
	glm::float32 pad_specular;
};

template <class T>
class UniformBuffer {
public:
	UniformBuffer() = default;

	explicit UniformBuffer( GraphicsDevice& device ) {
		Create( device );
	}

	void Create( GraphicsDevice& device ) {
		if ( buffer_ != nullptr ) {
			return;
		}

		BufferDescription desc{};
		desc.size = sizeof( T );
		desc.stride = 0;
		desc.access_flags = ResourceAccess::ReadWrite;
		desc.bind_flags = BufferBinding::UniformBuffer;

		buffer_ = device.CreateBuffer( desc );
	}

	[[nodiscard]] SharedRef<Buffer> GetBuffer() const noexcept {
		return buffer_;
	}

	void SetData( const T& data ) noexcept {
		assert( buffer_ != nullptr );
		buffer_->SetData( &data, sizeof( data ) );
	}

private:
	SharedRef<Buffer> buffer_;
};

class Material {
public:
	SharedRef<Texture2D> diffuse_;
	SharedRef<Texture2D> specular_;
	SharedRef<Sampler> sampler_;
	float shininess_;
};

class Mesh {
public:
	// WHAT to draw
	SharedRef<Buffer> vbo_;
	SharedRef<Buffer> ibo_;
	SharedRef<VertexArray> vao_;

	// HOW to draw
	SharedRef<Material> material_;

	Mesh* parent_;
	std::list<Mesh> sub_meshes_;
};

void RenderMesh( GraphicsContext* context, Mesh* mesh ) {
	context->SetVertexBuffer( mesh->vbo_, mesh->vao_ );
	context->SetIndexBuffer( mesh->ibo_ );
	context->SetSampler( 0, mesh->material_->sampler_ );
	context->SetTextureUnit( 0, mesh->material_->diffuse_ );
	context->SetTextureUnit( 1, mesh->material_->specular_ );
}

class ApplicationLayer : public Layer {
public:
	explicit ApplicationLayer( Application& app )
		: app_( app ) {

		context_ = app.GetGraphicsDevice().CreateContext();

		pad_ = app.GetInputSystem().GetGamepad( 0 );
		if ( pad_ != nullptr ) {
			cout << "Gamepad: " << pad_->GetName() << endl;
		}

		const auto [width, height] = app.GetWindow().GetSize();
		camera_.width_ = static_cast<float>( width );
		camera_.height_ = static_cast<float>( height );
		camera_.field_of_view_ = Config::Instance().GetFieldOfView();
		camera_.position_ = glm::vec3( 0, 0, 7 );

		// Load GPU resources for objects
		{
			GeometricPrimitive::VertexCollection vertices;
			GeometricPrimitive::IndexCollection indices;

			GeometricPrimitive::CreateCube( vertices, indices );

			// Create cube GPU resources
			cube_vbo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), vertices,
									sizeof( GeometricPrimitive::VertexType ),
									BufferBinding::VertexBuffer );

			const VertexLayout layout{
				VertexAttribute{ VertexAttributeName::kPosition, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kNormal, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kTexCoord, 2,
								 VertexAttributeType::kFloat, false } };

			cube_vao_ = app.GetGraphicsDevice().CreateVertexArray( layout );

			cube_ibo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), indices, sizeof( uint16_t ),
									BufferBinding::IndexBuffer );

			cube_texture_ = CreateTextureFromFile(
				app.GetGraphicsDevice(), "resources/textures/container.jpg" );

			cube_model_matrix_ = glm::identity<glm::mat4>();
		}

		// Load floor
		{
			GeometricPrimitive::VertexCollection floor_verts;
			GeometricPrimitive::CreatePlane( floor_verts );

			floor_vbo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), floor_verts,
									sizeof( GeometricPrimitive::VertexType ),
									BufferBinding::VertexBuffer );

			const VertexLayout layout{
				VertexAttribute{ VertexAttributeName::kPosition, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kNormal, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kTexCoord, 2,
								 VertexAttributeType::kFloat, false } };

			floor_vao_ = app.GetGraphicsDevice().CreateVertexArray( layout );

			floor_texture_diffuse_ =
				CreateTextureFromFile( app.GetGraphicsDevice(),
									   "resources/textures/wood_diffuse.png" );
			floor_texture_specular_ =
				CreateTextureFromFile( app.GetGraphicsDevice(),
									   "resources/textures/wood_specular.png" );

			floor_model_matrix_ = glm::translate( glm::identity<glm::mat4>(),
												  glm::vec3( 0, -3, 0 ) );
		}

		// Matrix Uniform buffer
		{
			BufferDescription desc{};
			desc.size = sizeof( Matrices );
			desc.access_flags = ResourceAccess::ReadWrite;

			const Matrices matrices(
				camera_.GetProjectionMatrix(), camera_.GetViewMatrix(), glm::identity<glm::mat4>(), camera_.position_ );
			
			ub_matrices_ =
				app.GetGraphicsDevice().CreateBuffer( desc );
		}

		// Directional light uniform buffer
		{
			BufferDescription desc{};
			desc.size = sizeof( DirectionalLight );
			desc.access_flags = ResourceAccess::ReadWrite;

			DirectionalLight light{};
			light.direction = glm::vec3( 0.5f, 0.0f, -0.5f );
			light.ambient = glm::vec3( 0.2f );
			light.diffuse = glm::vec3( 0.5f );
			light.specular = glm::vec3( 0.8f );

			ub_directional_light_ =
				app.GetGraphicsDevice().CreateBuffer( desc, &light );
		}

		// Shaders
		{
			vertex_shader_ = LoadShaderFromFile(
				app.GetGraphicsDevice(), ShaderType::VertexShader,
				"resources/shaders/basic.vert" );

			vertex_shader_->SetUniformBuffer( 0, ub_matrices_ );

			fragment_shader_ = LoadShaderFromFile(
				app.GetGraphicsDevice(), ShaderType::FragmentShader,
				"resources/shaders/basic.frag" );

			fragment_shader_->SetUniformBuffer( 2, ub_directional_light_ );

			pipeline_ = app.GetGraphicsDevice().CreatePipeline();
			pipeline_->SetVertexShader( vertex_shader_ );
			pipeline_->SetFragmentShader( fragment_shader_ );
		}

		// Samplers
		{
			SamplerDescription desc{};
			desc.max_lod = 0.0f;
			desc.min_lod = 0.0f;
			desc.mip_lod_bias = 0.0f;
			desc.max_ansiotropy = 0;

			sampler_ = app.GetGraphicsDevice().CreateSampler( desc );
		}
	}

protected:
	void OnUpdate() override {
		step_timer_.Tick( [this]() {
			const auto delta =
				static_cast<float>( step_timer_.GetElapsedSeconds() );

			if ( app_.GetInputSystem().IsKeyDown( app_.GetWindow(),
												  Key::Escape ) ||
				 ( pad_ && pad_->IsButtonDown( GamepadButton::Select ) ) ) {
				app_.Exit();
			}

			DebugCameraControls( camera_, 25.0f, delta );

			if ( pad_ ) {
				DebugCameraControls( *pad_, camera_, 25.0f, delta );
			}

			cube_model_matrix_ =
				glm::rotate( cube_model_matrix_, glm::radians( delta * 25.0f ),
							 glm::vec3( 0, 1, 0 ) );
		} );

		context_->Clear( true, true, 0.1f, 0.1f, 0.1f );

		context_->SetPipeline( pipeline_ );
		context_->SetSampler( 0, sampler_ );

		// Draw floating box
		{
			const Matrices matrices( camera_.GetProjectionMatrix(),
									 camera_.GetViewMatrix(),
									 cube_model_matrix_, camera_.position_ );
			ub_matrices_->SetData( &matrices, sizeof( matrices ) );

			context_->SetVertexBuffer( cube_vbo_, cube_vao_ );
			context_->SetIndexBuffer( cube_ibo_ );
			context_->SetTextureUnit( 0, cube_texture_ );
			context_->DrawIndexed( cube_ibo_->GetNumElements(), 0, 0 );
		}

		// Draw floor
		{
			const Matrices matrices( camera_.GetProjectionMatrix(),
									 camera_.GetViewMatrix(),
									 floor_model_matrix_, camera_.position_ );
			ub_matrices_->SetData( &matrices, sizeof( matrices ) );

			context_->SetVertexBuffer( floor_vbo_, floor_vao_ );
			context_->SetTextureUnit( 0, floor_texture_diffuse_ );
			context_->SetTextureUnit( 1, floor_texture_specular_ );
			context_->Draw( floor_vbo_->GetNumElements(), 0 );
		}
	}

	void DebugCameraControls( DebugCamera& camera, float camera_speed,
							  float delta ) noexcept {
		const auto& window = app_.GetWindow();
		const auto& keyboard = app_.GetInputSystem();

		// Strafe Left
		if ( keyboard.IsKeyDown( window, Key::A ) ) {
			camera.position_ -=
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
				camera_speed * delta;
		}
		// Strafe right
		if ( keyboard.IsKeyDown( window, Key::D ) ) {
			camera.position_ +=
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
				camera_speed * delta;
		}
		// Forward
		if ( keyboard.IsKeyDown( window, Key::W ) ) {
			camera.position_ += camera_speed * delta * camera.front_;
		}
		// Backward
		if ( keyboard.IsKeyDown( window, Key::S ) ) {
			camera.position_ -= camera_speed * delta * camera.front_;
		}
		// Up
		if ( keyboard.IsKeyDown( window, Key::E ) ) {
			camera.position_ += camera_speed * delta * camera.up_;
		}
		// Down
		if ( keyboard.IsKeyDown( window, Key::Q ) ) {
			camera.position_ -= camera_speed * delta * camera.up_;
		}

		// Turn left
		const float turn_speed = camera_speed * 3.0f;
		if ( keyboard.IsKeyDown( window, Key::Left ) ) {
			camera.yaw_ -= turn_speed * delta;
		}
		// Turn right
		if ( keyboard.IsKeyDown( window, Key::Right ) ) {
			camera.yaw_ += turn_speed * delta;
		}
		// Look up
		if ( keyboard.IsKeyDown( window, Key::Up ) ) {
			camera.pitch_ += turn_speed * delta;
		}
		// Look down
		if ( keyboard.IsKeyDown( window, Key::Down ) ) {
			camera.pitch_ -= turn_speed * delta;
		}
	}

	void DebugCameraControls( const Octave::Gamepad& gamepad,
							  DebugCamera& camera, float camera_speed,
							  float delta ) noexcept {
		const auto [left_x, left_y] = gamepad.GetLeftStick();

		// Move
		if ( left_x > 0.0f ) {
			camera.position_ +=
				left_x *
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
				camera_speed * delta;
		}

		if ( left_y > 0.0f ) {
			camera.position_ += left_y * camera_speed * delta * camera.front_;
		}

		// Look
		const auto [right_x, right_y] = gamepad.GetRightStick();
		const float turn_speed = camera_speed * 3.0f;

		if ( right_x > 0.0f ) {
			camera.yaw_ += right_x * turn_speed * delta;
		}

		if ( right_y > 0.0f ) {
			camera.pitch_ += right_y * turn_speed * delta;
		}
	}

private:
	Application& app_;

	Ref<GraphicsContext> context_;

	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
	SharedRef<Pipeline> pipeline_;
	SharedRef<Sampler> sampler_;

	SharedRef<Buffer> ub_matrices_;
	SharedRef<Buffer> ub_directional_light_;

	Ref<Gamepad> pad_;

	glm::mat4 cube_model_matrix_;
	glm::mat4 floor_model_matrix_;

	StepTimer step_timer_;
	DebugCamera camera_;

	SharedRef<Buffer> cube_vbo_;
	SharedRef<VertexArray> cube_vao_;
	SharedRef<Buffer> cube_ibo_;
	SharedRef<Texture2D> cube_texture_;

	SharedRef<Buffer> floor_vbo_;
	SharedRef<VertexArray> floor_vao_;
	SharedRef<Texture2D> floor_texture_diffuse_,
		floor_texture_specular_;
};

class ModelViewerSample : public Application {
public:
	void OnInitialize() override {
		GetWindow().OnClose.Add( [this] { Exit(); } );

		PushLayer( MakeRef<ApplicationLayer>( *this ) );
	}
};

Ref<Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return MakeRef<ModelViewerSample>();
}
