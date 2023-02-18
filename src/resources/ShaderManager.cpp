#include "ShaderManager.hpp"

#include <regex>

#include "Config.hpp"
using namespace std;
namespace fs = std::filesystem;

namespace Octave::resources {

using namespace graphics;

static ShaderManager::ResourcePtr LoadShaderProgram(
	const fs::path& shader_directory, const string& shader ) {
	const fs::path vertex_path = shader_directory / ( shader + ".vert" );
	const fs::path fragment_path = shader_directory / ( shader + ".frag" );

	if ( !fs::exists( vertex_path ) ) {
		cout << "Missing vertex shader for " << shader << " program" << endl;
		return nullptr;
	}

	if ( !fs::exists( fragment_path ) ) {
		cout << "Missing fragment shader for " << shader << " program" << endl;
		return nullptr;
	}

	try {
		auto program = make_shared<Shader>();
		program->CompileFromFile( vertex_path, fragment_path );

		return program;
	} catch ( const Shader::CompileError& e ) {
		cerr << "Shader compilation error: " << e.what() << endl;
	}  // try

	return nullptr;
}

ShaderManager::ShaderManager() noexcept {
	// Read shader list
	shader_list_ = Config::Instance().GetShaderList();
	if ( shader_list_.empty() ) {
		cout << "No shaders listed for compilation..." << endl;
	}

	// Read shader directory
	shader_directory_ = Config::Instance().GetShaderDirectory();
}

void ShaderManager::PreloadShaders() {
	for ( auto& shader : shader_list_ ) {
		// Check to see if we already compiled this shader program
		if ( resources_.find( shader ) != resources_.cend() ) {
			// if so, skip it
			continue;
		}

		auto ptr = LoadShaderProgram( shader_directory_, shader );
		if ( ptr != nullptr ) {
			AddResource( shader, ptr );
		}
	}  // for
}

ShaderManager::ResourcePtr ShaderManager::Get( const string& name ) noexcept {
	auto shader = resources_[name];

	if ( !shader ) {
		shader = LoadShaderProgram( shader_directory_, name );
		if ( shader != nullptr ) {
			AddResource( name, shader );
		}
	}

	return shader;
}

}  // namespace octave::resources
