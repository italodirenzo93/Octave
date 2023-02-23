#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "CommonInclude.hpp"

#include <SimpleIni.h>

class Config {
   public:
    static Config& Instance() {
        if ( instance_ == nullptr ) {
            instance_ = std::unique_ptr<Config>( new Config() );
        }

        return *instance_;
    }

    ~Config() = default;

    void Reset();

    // Video
    int GetFramebufferWidth() const;
    int GetFramebufferHeight() const;
    bool GetIsFullscreen() const;
    int GetSyncInterval() const;

    // Renderer
    std::vector<std::string> GetShaderList() const;
    std::filesystem::path GetShaderDirectory() const;
    bool GetPreloadShaders() const;

   private:
    Config();
    static std::unique_ptr<Config> instance_;

    CSimpleIniA ini_;

    NON_COPYABLE_OR_MOVABLE_CLASS( Config )
};

#endif
