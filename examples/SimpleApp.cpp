// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <iostream>
#include <memory>

using namespace std;

static constexpr const char* kVertexShaderSource = R"(#version 410
layout( location = 0 ) in vec2 position;
uniform mat4 uViewProjectionMatrix;
void main() {
	gl_Position = uViewProjectionMatrix * vec4(position, 0.0, 1.0);
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

class SimpleAppLayer final : public Octave::Layer {
public:
	explicit SimpleAppLayer( const Octave::Application& app )
		: Layer( "Default Layer" ) {
		m_renderer_ = app.GetGraphicsSystem().CreateRenderer();
		m_renderer_->SetDepthTestEnabled( false );

		app.GetWindow().AddSizeChangedCallback(
			[this]( int width, int height ) {
				m_renderer_->SetViewport( 0, 0, width, height );
			} );

		m_shader_ = make_unique<Octave::Shader>();
		m_shader_->CompileFromString( kVertexShaderSource,
									  kFragmentShaderSource );

		const Octave::VertexBuffer::VertexLayout layout{
			{ Octave::VertexAttributeName::kPosition, 2,
			  Octave::VertexAttributeType::kFloat, false } };

		vector<VertexType> vertices{
			{ 0.0f, 1.0f }, { 1.0f, -1.0f }, { -1.0f, -1.0f } };

		m_vbo_ = app.GetGraphicsSystem().CreateVertexBuffer();
		m_vbo_->SetData( layout, vertices );
	}

	void OnAttach() override {
		Octave::Log::Info( "Attaching SimpleApp layer" );
	}

	void OnDetach() override {
		Octave::Log::Info( "Detaching SimpleApp layer" );
	}

	void OnUpdate() override {
		m_renderer_->Clear();

		const auto [x, y, width, height] = m_renderer_->GetViewport();
		const auto transform_matrix =
			glm::perspectiveFov( glm::radians( 45.0f ),
								 static_cast<float>( width ),
								 static_cast<float>( height ), 0.01f, 100.0f ) *
			glm::lookAt( glm::vec3( 0.0f, 0.0f, 3.0f ), glm::vec3( 0.0f ),
						 glm::vec3( 0.0f, 1.0f, 0.0f ) );

		m_shader_->SetMat4( "uViewProjectionMatrix", transform_matrix );

		m_renderer_->Draw( *m_shader_, *m_vbo_ );
	}

private:
	unique_ptr<Octave::Renderer> m_renderer_;
	unique_ptr<Octave::Shader> m_shader_;
	unique_ptr<Octave::VertexBuffer> m_vbo_;
};

class SimpleApp final : public Octave::Application {
public:
	void OnInitialize() override {
		Octave::Log::Info( "Hello World!" );

		PushLayer( make_unique<SimpleAppLayer>( *this ) );
	}
};

unique_ptr<Octave::Application> Octave::CreateApplication( int argc,
														   char* argv[] ) {
	return make_unique<SimpleApp>();
}
