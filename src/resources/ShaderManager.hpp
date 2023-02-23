#ifndef OCTAVE_SHADERMANAGER_HPP
#define OCTAVE_SHADERMANAGER_HPP

#include "ResourceManager.hpp"
#include "graphics/Shader.hpp"

namespace Octave::resources {

class ShaderManager : public ResourceManager<graphics::Shader> {
public:
	ShaderManager() noexcept;

	/**
	 * Pre-compiles and caches all shaders
	 */
	void PreloadShaders();

	/**
	 * Look up shader program by name
	 * @param name Name of the shader program
	 * @returns A shared pointer to the shader or nullptr if it could not be
	 * obtained
	 */
	[[nodiscard]] ResourcePtr Get( const std::string& name ) noexcept override;

private:
	std::filesystem::path shader_directory_;
	std::vector<std::string> shader_list_;
};

}  // namespace octave::resources

#endif
