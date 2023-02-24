// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <iostream>
#include <memory>

using namespace std;

static constexpr const char* vertex_shader_source = R"(#version 410
layout( location = 0 ) in vec2 position;
void main() {
	gl_Position = vec4(position, 0.0, 1.0);
}
)";

static constexpr const char* fragment_shader_source = R"(#version 410
out vec4 frag_color;
void main() {
	frag_color = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

struct VertexType {
	float position[2];
};

class SimpleApp : public Octave::Application {
public:
	void OnInitialize() override {
		window_->AddCloseCallback( [this]() { Exit(); } );

		m_renderer = make_unique<Octave::Renderer>();

		Octave::Log::Info( "Hello World!" );

		m_shader = make_unique<Octave::Shader>();
		m_shader->CompileFromString( vertex_shader_source,
									 fragment_shader_source );

		Octave::VertexBuffer::VertexLayout layout{
			{ Octave::LayoutSemantic::kPosition, 2, GL_FLOAT, false } };

		m_vbo = make_unique<Octave::VertexBuffer>();
		m_vbo->SetData<VertexType>( layout, { { 0.0f, 1.0f },
											  { 1.0f, -1.0f },
											  { -1.0f, -1.0f } } );
	}

	void OnUpdate() override {
		m_renderer->Clear( true, true );
		m_renderer->SetShader( *m_shader );
		m_renderer->Draw( *m_vbo );
	}

private:
	unique_ptr<Octave::Renderer> m_renderer;
	unique_ptr<Octave::Shader> m_shader;
	unique_ptr<Octave::VertexBuffer> m_vbo;
};

unique_ptr<Octave::Application> Octave::CreateApplication( int argc,
														   char* argv[] ) {
	return make_unique<SimpleApp>();
}
