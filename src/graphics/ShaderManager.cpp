#include "ShaderManager.hpp"

#include <fstream>
using namespace std;

static const vector<string> ShaderNames{"basic", "lit", "mesh", "single_color",
                                        "skybox"};
static const string shaderDirectory = "./resources/shaders/";

namespace graphics {
ShaderManager* ShaderManager::instance_ = nullptr;


void ShaderManager::PreloadShaders() {
    for (auto& shader : ShaderNames) {
        // Check to see if we already compiled this shader program
        if (shaders_.find( shader ) != shaders_.cend()) {
            // if so, skip it
            continue;
        }

        string vertexPath = shaderDirectory + shader + ".vert";
        string fragmentPath = shaderDirectory + shader + ".frag";

        ifstream ifs;

        ifs.open( vertexPath );
        if (!ifs.is_open()) {
            cout << "Missing vertex shader for " << shader << " program" <<
                endl;
            continue;
        }

        ifs.close();

        ifs.open( fragmentPath );
        if (!ifs.is_open()) {
            cout << "Missing fragment shader for " << shader << " program" <<
                endl;
            continue;
        }

        ifs.close();

        shaders_.emplace(
            shader, make_shared<Shader>( vertexPath.c_str(),
                                         fragmentPath.c_str() ) );
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
        const auto vertex_path = shaderDirectory + name + ".vert";
        const auto fragment_path = shaderDirectory + name + ".frag";
        shader = make_shared<
            Shader>( vertex_path.c_str(), fragment_path.c_str() );

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
