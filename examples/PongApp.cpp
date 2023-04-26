#include <Octave.hpp>
#include <EntryPoint.hpp>
#include "common/SampleApplication.hpp"
#include "common/Helpers.hpp"

using namespace std;
using namespace Octave;
using namespace Octave::Samples;

static constexpr const char* kVertexShaderSource = R"(
#version 410
layout(location=0) in vec2 i_Position;
layout(location=2) in vec2 i_TexCoord;

out vec2 o_TexCoord;

uniform mat4 u_Proj;
uniform mat4 u_Model;

void main() {
	o_TexCoord = i_TexCoord;
	gl_Position = u_Proj * u_Model * vec4( i_Position.xy, 0.0, 1.0 );
}
)";

static constexpr const char* kFragmentShaderSource = R"(
#version 410
in vec2 i_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Texture;
uniform vec3 u_SpriteColor;

void main() {
	//o_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
	o_FragColor = vec4( u_SpriteColor, 1.0 );
	//o_FragColor = vec4( u_SpriteColor, 1.0 ) * texture( u_Texture, i_TexCoord );
}
)";

static const float vertices[] = {
	// pos      // tex
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };

class PongApp final : public SampleApplication {
	std::unique_ptr<Buffer> m_QuadBuffer;
	std::unique_ptr<Program> m_Program;
	VertexLayout m_VertexLayout;
	glm::mat4 m_ProjectionMatrix;
	std::array<int, 4> m_Viewport;

public:
	explicit PongApp( const ApplicationOptions& opts )
		: SampleApplication( opts ) {}

	void OnInitialize() override {
		SampleApplication::OnInitialize();

		// Vertex buffer
		// {
		// 	GeometricPrimitive::VertexCollection vertices;
		// 	GeometricPrimitive::CreateQuad( vertices );

		// 	m_QuadBuffer =
		// 		CreateStaticBuffer( GetGraphicsDevice(), vertices,
		// 							sizeof( GeometricPrimitive::VertexType ),
		// 							BufferType::VertexBuffer );
		// }
		{
			BufferDescription desc{};
			desc.m_Name = "Quad";
			desc.m_Access = ResourceAccess::WriteOnly;
			desc.m_Size = sizeof( vertices );
			desc.m_Stride = sizeof( float ) * 4;
			desc.m_Type = BufferType::VertexBuffer;
			desc.m_Usage = BufferUsage::Static;

			m_QuadBuffer = GetGraphicsDevice().CreateBuffer( desc, vertices );
		}

		// Shader program
		{
			auto vertexShader = GetGraphicsDevice().CreateShader(
				ShaderType::VertexShader, kVertexShaderSource );
			auto fragmentShader = GetGraphicsDevice().CreateShader(
				ShaderType::FragmentShader, kFragmentShaderSource );

			m_Program = GetGraphicsDevice().CreateProgram( *vertexShader,
														   *fragmentShader );
		}

		m_VertexLayout.assign( { { VertexAttributeName::kPosition, 2,
								   VertexAttributeType::kFloat, false },
								 { VertexAttributeName::kTexCoord, 2,
								   VertexAttributeType::kFloat, false } } );

		const auto windowSize = GetWindow().GetSize();
		m_Viewport = { 0, 0, windowSize.first, windowSize.second };
		m_ProjectionMatrix =
			glm::ortho( 0.0f, (float)windowSize.first, (float)windowSize.second,
						0.0f, -1.0f, 1.0f );
	}

	~PongApp() override {
		auto& device = GetGraphicsDevice();
		if ( m_QuadBuffer ) device.DestroyBuffer( std::move( m_QuadBuffer ) );
		if ( m_Program ) device.DestroyProgram( std::move( m_Program ) );
	}

	void Update() override {}

	void Draw() override {
		m_Context->Clear( ContextClearFlags::Color );

		RenderSprite( glm::vec2( 200.0f, 200.0f ), glm::vec2( 300.0f, 400.0f ),
					  0.0f, Colours::Red );

		// Check errors
		auto err = GetGraphicsDevice().TryDequeueError();
		if ( !err.empty() ) {
			Log::Error( "Graphics error: ", err );
		}
	}

	void RenderSprite( const glm::vec2& position, const glm::vec2& size,
					   float rotation, const Colour& colour ) {
		m_Context->Reset();

		auto model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( position, 0.0f ) );

		model = glm::translate(
			model, glm::vec3( 0.5f * size.x, 0.5f * size.y, 0.0f ) );
		model = glm::rotate( model, glm::radians( rotation ),
							 glm::vec3( 0.0f, 0.0f, 1.0f ) );
		model = glm::translate(
			model, glm::vec3( -0.5f * size.x, -0.5f * size.y, 0.0f ) );

		model = glm::scale( model, glm::vec3( size, 1.0f ) );

		m_Program->SetMat4( "u_Proj", m_ProjectionMatrix );
		m_Program->SetMat4( "u_Model", model );
		m_Program->SetVec3(
			"u_SpriteColor",
			glm::vec3( colour.m_Red, colour.m_Green, colour.m_Blue ) );
		m_Context->SetProgram( *m_Program );

		m_Context->SetVertexBuffer( *m_QuadBuffer );
		m_Context->SetVertexLayout( m_VertexLayout );
		m_Context->SetViewport( m_Viewport[0], m_Viewport[1], m_Viewport[2],
								m_Viewport[3] );
		m_Context->Draw( m_QuadBuffer->GetNumElements(), 0 );
	}
};

SAMPLE_MAIN( PongApp )
