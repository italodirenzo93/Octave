#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"

namespace graphics {
class ShaderManager {
public:
    static ShaderManager& instance() {
        if (instance_ == nullptr) {
            instance_ = new ShaderManager;
        }

        return *instance_;
    }

    ~ShaderManager() {
        delete instance_;
    }

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
    ShaderManager() = default;
    static ShaderManager* instance_;

    std::map<std::string, std::shared_ptr<Shader>> shaders_;

    NON_COPYABLE_OR_MOVABLE_CLASS( ShaderManager )
};
}

#endif
