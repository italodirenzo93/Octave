#include "Config.hpp"

#include <SimpleIni.h>

#include <regex>

#include "CommonInclude.hpp"

using namespace std;
namespace fs = std::filesystem;

static constexpr const char* kConfigTemplate = "resources/config/Engine.ini";

// Video
static constexpr int kDefaultWidth = 800;
static constexpr int kDefaultHeight = 600;
static constexpr bool kDefaultFullscreen = false;
static constexpr int kDefaultSyncInterval = 1;

// Renderer
static constexpr const char* kDefaultShaderList =
    "basic,mesh,lit,single_color,skybox";
static constexpr const char* kDefaultShaderDirectory = "resources/shaders";
static constexpr bool kDefaultPreloadShaders = false;

static void SetDefaultConfig( CSimpleIniA& config ) {
    config.SetLongValue( "Video", "FramebufferWidth", kDefaultWidth );
    config.SetLongValue( "Video", "FramebufferHeight", kDefaultHeight );
    config.SetBoolValue( "Video", "IsFullscreen", kDefaultFullscreen );
    config.SetLongValue( "Video", "SyncInterval", kDefaultSyncInterval );

    config.SetValue( "Renderer", "ShaderList", kDefaultShaderList );
    config.SetValue( "Renderer", "ShaderDirectory", kDefaultShaderDirectory );
    config.SetBoolValue( "Renderer", "PreloadShaders", kDefaultPreloadShaders );
}

unique_ptr<Config> Config::instance_ = nullptr;

Config::Config() {
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

void Config::Reset() {
    ini_.Reset();
}

int Config::GetFramebufferWidth() const {
    const long width =
        ini_.GetLongValue( "Video", "FramebufferWidth", kDefaultWidth );
    return static_cast<int>( width );
}

int Config::GetFramebufferHeight() const {
    const long height =
        ini_.GetLongValue( "Video", "FramebufferHeight", kDefaultHeight );
    return static_cast<int>( height );
}

bool Config::GetIsFullscreen() const {
    return ini_.GetBoolValue( "Video", "IsFullscreen", kDefaultFullscreen );
}

int Config::GetSyncInterval() const {
    const long sync =
        ini_.GetLongValue( "Video", "SyncInterval", kDefaultSyncInterval );
    return static_cast<int>( sync );
}

std::vector<std::string> Config::GetShaderList() const {
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

std::filesystem::path Config::GetShaderDirectory() const {
    return ini_.GetValue( "Renderer", "ShaderDirectory",
                          kDefaultShaderDirectory );
}

bool Config::GetPreloadShaders() const {
    return ini_.GetBoolValue( "Renderer", "PreloadShaders",
                              kDefaultPreloadShaders );
}
