// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <memory>

using namespace std;
using namespace Octave;

static constexpr const char* kVertexShaderSource = R"(#version 410
layout( location = 0 ) in vec2 position;
layout( location=1 ) in vec3 color;

out gl_PerVertex {
	vec4 gl_Position;
};

out vec3 vertex_color;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	vertex_color = color;
}
)";

static constexpr const char* kFragmentShaderSource = R"(#version 410
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

class SimpleAppLayer final : public Layer {
public:
	explicit SimpleAppLayer( Application& app )
		: Layer( "Default Layer" ) {
		context_ = app.GetGraphicsDevice().CreateContext();
		context_->SetDepthTestEnabled( false );

		app.GetWindow().OnClose.Add( [&] { app.Exit(); } );

		app.GetWindow().OnSizeChanged.Add(
			[this]( int width, int height ) {
				context_->SetViewport( 0, 0, width, height );
			} );

		// Vertex Buffer
		{
			const vector<VertexType> vertices{ { {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} },
										 { {1.0f, -1.0f}, {0.0f, 1.0f, 0.0f} },
										 { {-1.0f, -1.0f}, {0.0f, 0.0f, 1.0f} } };

			BufferDescription desc{};
			desc.size = sizeof( VertexType ) * vertices.size();
			desc.stride = sizeof( VertexType );
			desc.access_flags = ResourceAccess::Write;
			desc.bind_flags = BufferBinding::VertexBuffer;

			vbo_ =
				app.GetGraphicsDevice().CreateBuffer( desc, vertices.data() );
		}

		// Vertex Array
		{
			const VertexLayout layout{
				{ VertexAttributeName::kPosition, 2,
				  VertexAttributeType::kFloat, false },
				{ VertexAttributeName::kColor, 3, VertexAttributeType::kFloat,
				  false } };

			vao_ = app.GetGraphicsDevice().CreateVertexArray( layout );
		}

		pipeline_ = app.GetGraphicsDevice().CreatePipeline();

		// Vertex Shader
		{
			vertex_shader_ = app.GetGraphicsDevice().CreateShader(
				ShaderType::VertexShader, kVertexShaderSource );
			pipeline_->SetVertexShader( vertex_shader_ );
		}

		// Fragment Shader
		{
			fragment_shader_ = app.GetGraphicsDevice().CreateShader(
				ShaderType::FragmentShader, kFragmentShaderSource );
			pipeline_->SetFragmentShader( fragment_shader_ );
		}
	}

	void OnAttach() override { Log::Info( "Attaching SimpleApp layer" ); }

	void OnDetach() override { Log::Info( "Detaching SimpleApp layer" ); }

	void OnUpdate() override {
		context_->Clear();

		context_->SetVertexBuffer( vbo_, vao_ );
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

class SimpleApp final : public Application {
public:
	void OnInitialize() override {
		Log::Info( "Hello World!" );

		PushLayer( MakeRef<SimpleAppLayer>( *this ) );
	}
};

Ref<Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return MakeRef<SimpleApp>();
}
