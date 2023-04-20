// clang-format off
#include "common/SampleApplication.hpp"
#include "common/Helpers.hpp"
#include <EntryPoint.hpp>
// clang-format on

#include <stb_image.h>

#include <fstream>

#include "common/Camera.hpp"
#include "helpers/GeometricPrimitive.hpp"

using namespace std;
using namespace Octave;
using namespace Octave::Samples;

class ModelViewerSample final : public SampleApplication {
public:
	ModelViewerSample( const ApplicationOptions& opts )
		: SampleApplication( opts ) {}

	void OnInitialize() override {
		SampleApplication::OnInitialize();

		const auto [width, height] = GetWindow().GetSize();
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
				CreateStaticBuffer( GetGraphicsDevice(), vertices,
									sizeof( GeometricPrimitive::VertexType ),
									BufferType::VertexBuffer );

			cube_ibo_ = CreateStaticBuffer( GetGraphicsDevice(), indices,
											sizeof( uint16_t ),
											BufferType::IndexBuffer );

			cube_layout_.assign(
				{ VertexAttribute{ VertexAttributeName::kPosition, 3,
								   VertexAttributeType::kFloat, false },
				  VertexAttribute{ VertexAttributeName::kNormal, 3,
								   VertexAttributeType::kFloat, false },
				  VertexAttribute{ VertexAttributeName::kTexCoord, 2,
								   VertexAttributeType::kFloat, false } } );

			cube_texture_ = CreateTextureFromFile(
				GetGraphicsDevice(), "resources/textures/container.jpg" );

			cube_model_matrix_ = glm::identity<glm::mat4>();
		}

		// Load floor
		{
			GeometricPrimitive::VertexCollection floor_verts;
			GeometricPrimitive::CreatePlane( floor_verts );

			floor_vbo_ =
				CreateStaticBuffer( GetGraphicsDevice(), floor_verts,
									sizeof( GeometricPrimitive::VertexType ),
									BufferType::VertexBuffer );

			floor_layout_.assign(
				{ VertexAttribute{ VertexAttributeName::kPosition, 3,
								   VertexAttributeType::kFloat, false },
				  VertexAttribute{ VertexAttributeName::kNormal, 3,
								   VertexAttributeType::kFloat, false },
				  VertexAttribute{ VertexAttributeName::kTexCoord, 2,
								   VertexAttributeType::kFloat, false } } );

			floor_texture_diffuse_ = CreateTextureFromFile(
				GetGraphicsDevice(), "resources/textures/wood_diffuse.png" );
			floor_texture_specular_ = CreateTextureFromFile(
				GetGraphicsDevice(), "resources/textures/wood_specular.png" );

			floor_model_matrix_ = glm::translate( glm::identity<glm::mat4>(),
												  glm::vec3( 0, -3, 0 ) );
		}

		// Shaders
		{
			auto vs = LoadShaderFromFile( GetGraphicsDevice(),
										  ShaderType::VertexShader,
										  "resources/shaders/basic.vert" );


			auto fs = LoadShaderFromFile( GetGraphicsDevice(),
										  ShaderType::FragmentShader,
										  "resources/shaders/basic.frag" );

			program_ = GetGraphicsDevice().CreateProgram( *vs, *fs );

			GetGraphicsDevice().DestroyShader( std::move( vs ) );
			GetGraphicsDevice().DestroyShader( std::move( fs ) );
		}

		// Samplers
		{
			SamplerDescription desc{};
			desc.m_MaxLod = 0.0f;
			desc.m_MinLod = 0.0f;
			desc.m_MipLodBias = 0.0f;
			desc.m_MaxAnsiotropy = 0;

			sampler_ = GetGraphicsDevice().CreateSampler( desc );
		}
	}

	~ModelViewerSample() noexcept override {
		auto& device = GetGraphicsDevice();

		if ( floor_vbo_ ) device.DestroyBuffer( std::move( floor_vbo_ ) );
		if ( cube_vbo_ ) device.DestroyBuffer( std::move( cube_vbo_ ) );
		if ( cube_ibo_ ) device.DestroyBuffer( std::move( cube_ibo_ ) );

		if ( cube_texture_ )
			device.DestroyTexture2D( std::move( cube_texture_ ) );
		if ( floor_texture_diffuse_ )
			device.DestroyTexture2D( std::move( floor_texture_diffuse_ ) );
		if ( floor_texture_specular_ )
			device.DestroyTexture2D( std::move( floor_texture_specular_ ) );

		if ( sampler_ ) device.DestroySampler( std::move( sampler_ ) );

		if ( program_ ) device.DestroyProgram( std::move( program_ ) );
	}

protected:
	void Update() override {
		const auto delta =
			static_cast<float>( m_StepTimer.GetElapsedSeconds() );

		if ( GetInputSystem().IsKeyDown( GetWindow(), Key::Escape ) ) {
			Exit();
		}

		DebugCameraControls( camera_, 25.0f, delta );

		cube_model_matrix_ =
			glm::rotate( cube_model_matrix_, glm::radians( delta * 25.0f ),
						 glm::vec3( 0, 1, 0 ) );
	}

	void Draw() override {
		m_Context->Clear( ContextClearFlags::Color | ContextClearFlags::Depth );

		program_->SetMat4( "uMatProjection", camera_.GetProjectionMatrix() );
		program_->SetMat4( "uMatView", camera_.GetViewMatrix() );
		program_->SetVec3( "uViewPos", camera_.position_ );

		program_->SetInt( "uMaterial.diffuse", 0 );
		program_->SetInt( "uMaterial.specular", 1 );
		program_->SetFloat( "uMaterial.shininess", 32.0f );

		program_->SetVec3( "uDirectionalLight.direction",
						   glm::vec3( 0.5f, 0.0f, -0.5f ) );
		program_->SetVec3( "uDirectionalLight.ambient", glm::vec3( 0.2f ) );
		program_->SetVec3( "uDirectionalLight.diffuse", glm::vec3( 0.5f ) );
		program_->SetVec3( "uDirectionalLight.specular", glm::vec3( 0.8f ) );

		program_->SetBool( "uPointLights[0].enabled", true );
		program_->SetVec3( "uPointLights[0].position",
						   glm::vec3( 1.0f, 1.5f, -2.1f ) );
		program_->SetVec3( "uPointLights[0].ambient", glm::vec3( 0.2f ) );
		program_->SetVec3( "uPointLights[0].diffuse", glm::vec3( 0.5f ) );
		program_->SetVec3( "uPointLights[0].specular", glm::vec3( 0.8f ) );
		program_->SetFloat( "uPointLights[0].constant", 1.0f );
		program_->SetFloat( "uPointLights[0].linear", 0.09f );
		program_->SetFloat( "uPointLights[0].quadratic", 0.032f );

		// Draw floating box
		{
			program_->SetMat4( "uMatModel", cube_model_matrix_ );
			program_->SetMat4(
				"uMatNormal",
				glm::transpose( glm::inverse( cube_model_matrix_ ) ) );

			m_Context->Reset();
			m_Context->SetVertexBuffer( *cube_vbo_ );
			m_Context->SetIndexBuffer( *cube_ibo_ );
			m_Context->SetTexture( 0, *cube_texture_ );
			m_Context->SetSampler( 0, *sampler_ );
			m_Context->SetProgram( *program_ );
			m_Context->SetVertexLayout( cube_layout_ );
			m_Context->DrawIndexed( cube_ibo_->GetNumElements(), 0, 0 );
		}

		// Draw floor
		{
			program_->SetMat4( "uMatModel", floor_model_matrix_ );
			program_->SetMat4(
				"uMatNormal",
				glm::transpose( glm::inverse( floor_model_matrix_ ) ) );

			m_Context->Reset();
			m_Context->SetVertexBuffer( *floor_vbo_ );
			m_Context->SetTexture( 0, *floor_texture_diffuse_ );
			m_Context->SetSampler( 0, *sampler_ );
			m_Context->SetTexture( 1, *floor_texture_specular_ );
			m_Context->SetSampler( 1, *sampler_ );
			m_Context->SetProgram( *program_ );
			m_Context->SetVertexLayout( floor_layout_ );
			m_Context->Draw( floor_vbo_->GetNumElements(), 0 );
		}
	}

	void DebugCameraControls( DebugCamera& camera, float camera_speed,
							  float delta ) noexcept {
		const auto& window = GetWindow();
		const auto& keyboard = GetInputSystem();

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

private:
	unique_ptr<Program> program_;
	unique_ptr<Sampler> sampler_;

	glm::mat4 cube_model_matrix_;
	glm::mat4 floor_model_matrix_;

	DebugCamera camera_;

	VertexLayout cube_layout_;
	unique_ptr<Buffer> cube_vbo_;
	unique_ptr<Buffer> cube_ibo_;
	unique_ptr<Texture2D> cube_texture_;

	VertexLayout floor_layout_;
	unique_ptr<Buffer> floor_vbo_;
	unique_ptr<Texture2D> floor_texture_diffuse_, floor_texture_specular_;
};

SAMPLE_MAIN( ModelViewerSample )
