// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <iostream>
#include <memory>

using namespace std;
using namespace Octave;

static constexpr const char* kVertexShaderSource = R"(#version 410
layout( location = 0 ) in vec2 position;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
	gl_Position = vec4(position, 0.0, 1.0);
}
)";

static constexpr const char* kFragmentShaderSource = R"(#version 410
out vec4 frag_color;
void main() {
	frag_color = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

struct VertexType {
	float position[2];
};

class SimpleAppLayer final : public Layer {
public:
	explicit SimpleAppLayer( const Application& app )
		: Layer( "Default Layer" ) {
		context_ = app.GetGraphicsDevice().CreateContext();
		context_->SetDepthTestEnabled( false );

		app.GetWindow().AddSizeChangedCallback(
			[this]( int width, int height ) {
				context_->SetViewport( 0, 0, width, height );
			} );

		// Vertex Buffer
		{
			vector<VertexType> vertices{
				{ 0.0f, 1.0f }, { 1.0f, -1.0f }, { -1.0f, -1.0f } };

			BufferDescription desc{};
			desc.size = sizeof( VertexType ) * vertices.size();
			desc.stride = sizeof( VertexType );
			desc.access_flags = BufferAccess::Write;
			desc.bind_flags = BufferBinding::VertexBuffer;

			vbo_ = app.GetGraphicsDevice().CreateBuffer(
				desc, reinterpret_cast<const void*>( vertices.data() ) );
		}

		// Vertex Array
		{
			VertexAttribute attrs[] = { { VertexAttributeName::kPosition, 2,
										  VertexAttributeType::kFloat,
										  false } };

			VertexArrayDescription desc{};
			desc.attributes = attrs;
			desc.count = 1;

			vao_ = app.GetGraphicsDevice().CreateVertexArray( desc );
		}

		context_->SetVertexBuffer( vbo_, vao_ );

		pipeline_ = app.GetGraphicsDevice().CreatePipeline();

		// Vertex Shader
		{
			vertex_shader_ = app.GetGraphicsDevice().CreateShader( ShaderType::VertexShader, kVertexShaderSource );
			pipeline_->SetVertexShader( vertex_shader_ );
		}

		// Fragment Shader
		{
			fragment_shader_ = app.GetGraphicsDevice().CreateShader( ShaderType::FragmentShader, kFragmentShaderSource );
			pipeline_->SetFragmentShader( fragment_shader_ );
		}


		context_->SetPipeline( pipeline_ );
	}

	void OnAttach() override {
		Log::Info( "Attaching SimpleApp layer" );
	}

	void OnDetach() override {
		Log::Info( "Detaching SimpleApp layer" );
	}

	void OnUpdate() override {
		context_->Clear();
		context_->Draw( 3, 0 );
	}

private:
	Ref<GraphicsContext> context_;
	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
	SharedRef<Pipeline> pipeline_;
	SharedRef<Buffer> vbo_;
	SharedRef<VertexArray> vao_;
};

class SimpleApp final : public Application {
public:
	void OnInitialize() override {
		Log::Info( "Hello World!" );

		PushLayer( make_unique<SimpleAppLayer>( *this ) );
	}
};

unique_ptr<Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return make_unique<SimpleApp>();
}
