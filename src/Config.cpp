#include "Config.hpp"

#include <SimpleIni.h>

#include <regex>

#include "CommonInclude.hpp"

using namespace std;
namespace fs = std::filesystem;

namespace octave {

static constexpr const char* kConfigTemplate = "resources/config/Engine.ini";

// Video
static constexpr int kDefaultWidth = 1280;
static constexpr int kDefaultHeight = 720;
static constexpr bool kDefaultIsFullscreen = false;
static constexpr bool kDefaultIsBorderless = false;
static constexpr int kDefaultSyncInterval = 1;

// Renderer
static constexpr const char* kDefaultShaderList = "basic";
static constexpr const char* kDefaultShaderDirectory = "resources/shaders";
static constexpr bool kDefaultPreloadShaders = false;
static constexpr float kDefaultFieldOfView = 90.0f;

static void SetDefaultConfig( CSimpleIniA& config ) {
    config.SetLongValue( "Video", "FramebufferWidth", kDefaultWidth );
    config.SetLongValue( "Video", "FramebufferHeight", kDefaultHeight );
    config.SetBoolValue( "Video", "IsFullscreen", kDefaultIsFullscreen );
    config.SetBoolValue( "Video", "IsBorderless", kDefaultIsBorderless );
    config.SetLongValue( "Video", "SyncInterval", kDefaultSyncInterval );

    config.SetValue( "Renderer", "ShaderList", kDefaultShaderList );
    config.SetValue( "Renderer", "ShaderDirectory", kDefaultShaderDirectory );
    config.SetBoolValue( "Renderer", "PreloadShaders", kDefaultPreloadShaders );
}

unique_ptr<Config> Config::instance_ = nullptr;

Config::Config() noexcept {
    SI_Error result;

    static constexpr const char* configFile = "Engine.ini";

    // Check for config file
    if ( fs::exists( configFile ) ) {
        // Try to load it
        result = ini_.LoadFile( configFile );
        if ( result < 0 ) {
            cout << "Could not read configuration file... using default "
                    "configuration"
                 << endl;
            SetDefaultConfig( ini_ );
        }
    } else {
        // Try to copy template
        if ( !fs::copy_file( kConfigTemplate, configFile ) ) {
            // Could not copy template, set built-in defaults
            SetDefaultConfig( ini_ );
            result = ini_.SaveFile( configFile );
            if ( result < 0 ) {
                cout << "Could not write default configuration file..." << endl;
            }
        } else {
            result = ini_.LoadFile( configFile );
            if ( result < 0 ) {
                cout << "Could not read configuration file... using default "
                        "configuration"
                     << endl;
                SetDefaultConfig( ini_ );
            }
        }  // copy file
    }      // config file exists

    assert( result == SI_OK );
}

void Config::Reset() noexcept {
    ini_.Reset();
}

int Config::GetFramebufferWidth() const noexcept {
    const long width =
        ini_.GetLongValue( "Video", "FramebufferWidth", kDefaultWidth );
    return static_cast<int>( width );
}

int Config::GetFramebufferHeight() const noexcept {
    const long height =
        ini_.GetLongValue( "Video", "FramebufferHeight", kDefaultHeight );
    return static_cast<int>( height );
}

bool Config::IsFullscreen() const noexcept {
    return ini_.GetBoolValue( "Video", "IsFullscreen", kDefaultIsFullscreen );
}

bool Config::IsBorderless() const noexcept {
    return ini_.GetBoolValue( "Video", "IsBorderless", kDefaultIsBorderless );
}

int Config::GetSyncInterval() const noexcept {
    const long sync =
        ini_.GetLongValue( "Video", "SyncInterval", kDefaultSyncInterval );
    return static_cast<int>( sync );
}

std::vector<std::string> Config::GetShaderList() const noexcept {
    vector<string> shader_list;

    const string val =
        ini_.GetValue( "Renderer", "ShaderList", kDefaultShaderList );

    const regex expr( "," );
    sregex_token_iterator iter( val.begin(), val.end(), expr, -1 );
    const sregex_token_iterator end;

    while ( iter != end ) {
        shader_list.emplace_back( *iter++ );
    }

    return shader_list;
}

std::filesystem::path Config::GetShaderDirectory() const noexcept {
    return ini_.GetValue( "Renderer", "ShaderDirectory",
                          kDefaultShaderDirectory );
}

bool Config::GetPreloadShaders() const noexcept {
    return ini_.GetBoolValue( "Renderer", "PreloadShaders",
                              kDefaultPreloadShaders );
}

float Config::GetFieldOfView() const noexcept {
    const auto fov =
        ini_.GetDoubleValue( "Renderer", "FieldOfView", kDefaultFieldOfView );
    return static_cast<float>( fov );
}

}  // namespace octave
