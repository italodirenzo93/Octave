// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

using namespace std;
using namespace Octave;

static constexpr const char* kVertexShaderSource = R"(#version 450
layout( location = 0 ) in vec2 position;
layout( location=1 ) in vec3 color;

out gl_PerVertex {
	vec4 gl_Position;
};

out vec3 vertex_color;

layout(std140, binding=0) uniform VertexConstants {
	mat4 projection;
	mat4 view;
	mat4 model;
};

void main() {
	gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
	vertex_color = color;
}
)";

static constexpr const char* kFragmentShaderSource = R"(#version 450
in vec3 vertex_color;

out vec4 frag_color;

void main() {
	frag_color = vec4(vertex_color, 1.0);
}
)";

struct VertexType {
	float position[2];
	float color[3];
};

struct VertexConstants {
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};

class SimpleApp final : public Application {
public:
	void OnInitialize() override {
		Log::Info( "Hello World!" );

		context_ = GetGraphicsDevice().CreateContext();

		GetWindow().OnClose.Add( [&] { Exit(); } );

		GetWindow().OnSizeChanged.Add( [this]( int width, int height ) {
			context_->SetViewport( 0, 0, width, height );
		} );

		// Vertex Buffer
		{
			const vector<VertexType> vertices{
				{ { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
				{ { 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } },
				{ { -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f } } };

			BufferDescription desc{};
			desc.size = sizeof( VertexType ) * vertices.size();
			desc.stride = sizeof( VertexType );
			desc.access_flags = ResourceAccess::Read;

			vbo_ = GetGraphicsDevice().CreateBuffer( desc, vertices.data() );
		}

		// Vertex Array
		{
			const VertexLayout layout{ { VertexAttributeName::kPosition, 2,
										 VertexAttributeType::kFloat, false },
									   { VertexAttributeName::kColor, 3,
										 VertexAttributeType::kFloat, false } };

			vao_ = GetGraphicsDevice().CreateVertexArray( layout );
			vao_->SetVertexBuffer( 0, vbo_ );
		}

		pipeline_ = GetGraphicsDevice().CreatePipeline();

		// Uniform buffer
		{
			BufferDescription desc{};
			desc.size = sizeof( VertexConstants );
			desc.access_flags = ResourceAccess::ReadWrite;

			ubo_ = GetGraphicsDevice().CreateBuffer( desc );

			const auto [width, height] = GetWindow().GetSize();

			VertexConstants constants = {};

			constants.projection = glm::perspectiveFov(
				glm::radians( 45.0f ), static_cast<float>( width ),
				static_cast<float>( height ), 0.1f, 100.0f );

			constants.view = glm::lookAt( glm::vec3( 0, 0, 3 ), glm::vec3( 0 ),
										  glm::vec3( 0, 1, 0 ) );

			constants.model = glm::identity<glm::mat4>();

			ubo_->SetData( &constants, sizeof( constants ) );
		}

		// Vertex Shader
		{
			vertex_shader_ = GetGraphicsDevice().CreateShader(
				ShaderType::VertexShader, kVertexShaderSource );

			vertex_shader_->SetUniformBuffer( 0, ubo_ );

			pipeline_->SetVertexShader( vertex_shader_ );
		}

		// Fragment Shader
		{
			fragment_shader_ = GetGraphicsDevice().CreateShader(
				ShaderType::FragmentShader, kFragmentShaderSource );
			pipeline_->SetFragmentShader( fragment_shader_ );
		}
	}

protected:
	void OnUpdate() override {
		context_->Clear( true, true, 0, 0, 0 );

		context_->SetVertexArray( vao_ );
		context_->SetPipeline( pipeline_ );

		context_->Draw( 3, 0 );
	}

private:
	Ref<GraphicsContext> context_;
	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
	SharedRef<Pipeline> pipeline_;
	SharedRef<Buffer> vbo_;
	SharedRef<VertexArray> vao_;
	SharedRef<Buffer> ubo_;
};

Ref<Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return MakeRef<SimpleApp>();
}
