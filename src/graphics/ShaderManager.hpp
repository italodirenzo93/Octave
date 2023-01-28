#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"

namespace octave::graphics {
class ShaderManager {
public:
    static ShaderManager& Instance() noexcept {
        if (instance_ == nullptr) {
            instance_ = std::unique_ptr<ShaderManager>( new ShaderManager() );
        }

        return *instance_;
    }

    ~ShaderManager() = default;

    /**
     * Pre-compiles and caches all shaders
     */
    void PreloadShaders() noexcept;

    /**
     * Purges the shader cache of any currently unused shaders
     */
    void ClearUnused() noexcept;

    /**
     * Look up shader program by name
     * @param name Name of the shader program
     * @returns A shared pointer to the shader or nullptr if it could not be obtained
     */
    std::shared_ptr<Shader> Get( const std::string& name ) noexcept;

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
