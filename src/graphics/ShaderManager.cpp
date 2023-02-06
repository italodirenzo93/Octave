#include "ShaderManager.hpp"

#include <regex>

#include "Config.hpp"
using namespace std;
namespace fs = std::filesystem;

namespace octave::graphics {
unique_ptr<ShaderManager> ShaderManager::instance_ = nullptr;

ShaderManager::ShaderManager() {
	// Read shader list
	shader_list_ = Config::Instance().GetShaderList();
	if ( shader_list_.empty() ) {
		cout << "No shaders listed for compilation..." << endl;
	}

	// Read shader directory
	shader_directory_ = Config::Instance().GetShaderDirectory();
}

void ShaderManager::PreloadShaders() noexcept {
	for ( auto& shader : shader_list_ ) {
		// Check to see if we already compiled this shader program
		if ( shaders_.find( shader ) != shaders_.cend() ) {
			// if so, skip it
			continue;
		}

		const fs::path vertex_path = shader_directory_ / ( shader + ".vert" );
		const fs::path fragment_path = shader_directory_ / ( shader + ".frag" );

		if ( !fs::exists( vertex_path ) ) {
			cout << "Missing vertex shader for " << shader << " program"
				 << endl;
			continue;
		}

		if ( !fs::exists( fragment_path ) ) {
			cout << "Missing fragment shader for " << shader << " program"
				 << endl;
			continue;
		}

		try {
			auto program = make_shared<Shader>();
			program->CompileFromFile( vertex_path, fragment_path );

			shaders_.emplace( shader, std::move( program ) );
		} catch ( const Shader::CompileError& e ) {
			cerr << "Shader compilation error: " << e.what() << endl;
		}  // try
	}      // for
}

void ShaderManager::ClearUnused() noexcept {
	vector<string> unused_shaders;

	for ( auto& shader : shaders_ ) {
		// If the use-count is 1, then the ShaderManager is the only owner left
		if ( shader.second.use_count() == 1 ) {
			shader.second.reset();
			unused_shaders.emplace_back( shader.first );
		}
	}

	for ( auto& name : unused_shaders ) {
		shaders_.erase( name );
	}
}

shared_ptr<Shader> ShaderManager::Get( const string& name ) noexcept {
	auto shader = shaders_[name];

	if ( !shader ) {
		const fs::path vertex_path = shader_directory_ / ( name + ".vert" );
		const fs::path fragment_path = shader_directory_ / ( name + ".frag" );

		if ( !fs::exists( vertex_path ) ) {
			cout << "Missing vertex shader for " << shader << " program"
				 << endl;
			return nullptr;
		}

		if ( !fs::exists( fragment_path ) ) {
			cout << "Missing fragment shader for " << shader << " program"
				 << endl;
			return nullptr;
		}

		try {
			shader = make_shared<Shader>();
			shader->CompileFromFile( vertex_path, fragment_path );
			shaders_[name] = shader;
		} catch ( const Shader::CompileError& e ) {
			cerr << "Shader compilation error: " << e.what() << endl;
			return nullptr;
		}
	}

	return shader;
}
}  // namespace octave::graphics
