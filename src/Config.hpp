#ifndef OCTAVE_CONFIG_HPP
#define OCTAVE_CONFIG_HPP

#include <SimpleIni.h>
#if defined( _WIN32 ) && defined( CreateWindow )
#undef CreateWindow
#endif

#include <filesystem>
#include <vector>
#include <string>

namespace Octave {
class Config {
public:
	static Config& Instance() noexcept {
		if ( instance_ == nullptr ) {
			instance_ = std::unique_ptr<Config>( new Config() );
		}

		return *instance_;
	}

	~Config() = default;

	void Reset() noexcept;

	// Video
	[[nodiscard]] int GetFramebufferWidth() const noexcept;
	[[nodiscard]] int GetFramebufferHeight() const noexcept;
	[[nodiscard]] bool IsFullscreen() const noexcept;
	[[nodiscard]] bool IsBorderless() const noexcept;
	[[nodiscard]] int GetSyncInterval() const noexcept;

	// Renderer
	[[nodiscard]] std::vector<std::string> GetShaderList() const noexcept;
	[[nodiscard]] std::filesystem::path GetShaderDirectory() const noexcept;
	[[nodiscard]] bool GetPreloadShaders() const noexcept;
	[[nodiscard]] float GetFieldOfView() const noexcept;
	[[nodiscard]] bool IsCullFaceEnabled() const noexcept;

private:
	Config() noexcept;
	static std::unique_ptr<Config> instance_;

	CSimpleIniA ini_;

public:
	Config( const Config& ) = delete;
	Config( Config&& ) = delete;
	Config& operator=( const Config& ) = delete;
	Config& operator=( Config&& ) = delete;
};
}  // namespace octave

#endif
