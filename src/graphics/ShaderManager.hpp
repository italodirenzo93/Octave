#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"

namespace graphics {
class ShaderManager {
public:
    static ShaderManager& Instance() {
        if (instance_ == nullptr) {
            instance_ = std::unique_ptr<ShaderManager>( new ShaderManager() );
        }

        return *instance_;
    }

    ~ShaderManager() = default;

    /**
     * Pre-compiles and caches all shaders
     */
    void PreloadShaders();

    /**
     * Purges the shader cache of any currently unused shaders
     */
    void ClearUnused();

    /**
     * Look up shader program by name
     */
    std::shared_ptr<Shader> Get( const std::string& name );

private:
    ShaderManager();

    static std::unique_ptr<ShaderManager> instance_;

	std::filesystem::path shader_directory_;
	std::vector<std::string> shader_list_;
    std::map<std::string, std::shared_ptr<Shader>> shaders_;

    NON_COPYABLE_OR_MOVABLE_CLASS( ShaderManager )
};
}

#endif
