#include "ShaderManager.hpp"
#include "Config.hpp"

#include <regex>
using namespace std;
namespace fs = std::filesystem;

static constexpr const char* kDefaultShaderDirectory = "resources/shaders";

namespace graphics {
unique_ptr<ShaderManager> ShaderManager::instance_ = nullptr;

ShaderManager::ShaderManager() {
	// Read shader list
    string shader_list;
    if (config::TryGetString( "Renderer", "ShaderList", shader_list )) {
        regex expr( "," );
        sregex_token_iterator iter( shader_list.begin(), shader_list.end(),
                                    expr, -1 );
        sregex_token_iterator end;

        while (iter != end) {
            shader_list_.emplace_back( *iter++ );
        }
    } else {
        cout << "No shaders listed for compilation..." << endl;
    }

	// Read shader directory
	string dir = kDefaultShaderDirectory;
    config::TryGetString( "Renderer", "ShaderDirectory", dir );

	shader_directory_ = dir;
}

void ShaderManager::PreloadShaders() {
    for (auto& shader : shader_list_) {
        // Check to see if we already compiled this shader program
        if (shaders_.find( shader ) != shaders_.cend()) {
            // if so, skip it
            continue;
        }

        const fs::path vertex_path = shader_directory_ / ( shader + ".vert" );
        const fs::path fragment_path = shader_directory_ / ( shader + ".frag" );

        if (!fs::exists( vertex_path )) {
            cout << "Missing vertex shader for " << shader << " program" <<
                endl;
            continue;
        }

        if (!fs::exists( fragment_path )) {
            cout << "Missing fragment shader for " << shader << " program" <<
                endl;
            continue;
        }

        shaders_.emplace( shader,
                          make_shared<Shader>( vertex_path.string().c_str(),
                                               fragment_path.string().
                                               c_str() ) );
    }
}

void ShaderManager::ClearUnused() {
    vector<string> unused_shaders;

    for (auto& shader : shaders_) {
        // If the use-count is 1, then the ShaderManager is the only owner left
        if (shader.second.use_count() == 1) {
            shader.second.reset();
            unused_shaders.emplace_back( shader.first );
        }
    }

    for (auto& name : unused_shaders) {
        shaders_.erase( name );
    }
}

shared_ptr<Shader> ShaderManager::Get( const string& name ) {
    auto shader = shaders_[name];

    if (!shader) {
        const fs::path vertex_path = shader_directory_ / ( name + ".vert" );
        const fs::path fragment_path = shader_directory_ / ( name + ".frag" );
        shader = make_shared<Shader>( vertex_path.string().c_str(),
                                      fragment_path.string().c_str() );

        if (shader) {
            shaders_[name] = shader;
        } else {
            cout << "No shader found named " << name << endl;
            return nullptr;
        }
    }

    return shader;
}
}
