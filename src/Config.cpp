#include "Config.hpp"

#include <SimpleIni.h>

#include "CommonInclude.hpp"

using namespace std;
namespace fs = std::filesystem;

namespace config {
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

static CSimpleIniA ini;

inline bool EnsureKeyExists( const string& section, const string& key ) {
    if ( !ini.KeyExists( section.c_str(), key.c_str() ) ) {
        cout << "Configuration value [" << section << "]/" << key
             << " not found" << endl;
        return false;
    }

    return true;
}

#define ENSURE_KEY_EXISTS( section, key ) \
    if ( !EnsureKeyExists( section, key ) ) return false

static void SetDefaultConfig( CSimpleIniA& config ) {
    config.SetLongValue( "Video", "FramebufferWidth", kDefaultWidth );
    config.SetLongValue( "Video", "FramebufferHeight", kDefaultHeight );
    config.SetBoolValue( "Video", "IsFullscreen", kDefaultFullscreen );
    config.SetLongValue( "Video", "SyncInterval", kDefaultSyncInterval );

    config.SetValue( "Renderer", "ShaderList", kDefaultShaderList );
    config.SetValue( "Renderer", "ShaderDirectory", kDefaultShaderDirectory );
    config.SetBoolValue( "Renderer", "PreloadShaders", kDefaultPreloadShaders );
}

bool Initialize() {
    SI_Error result;

    static constexpr const char* configFile = "Engine.ini";

    // Check for config file
    if ( fs::exists( configFile ) ) {
        // Try to load it
        result = ini.LoadFile( configFile );
        if ( result < 0 ) {
            cout << "Could not read configuration file... using default "
                    "configuration"
                 << endl;
            SetDefaultConfig( ini );
        }
    } else {
        // Try to copy template
        if ( !fs::copy_file( kConfigTemplate, configFile ) ) {
            // Could not copy template, set built-in defaults
            SetDefaultConfig( ini );
            result = ini.SaveFile( configFile );
            if ( result < 0 ) {
                cout << "Could not write default configuration file..." << endl;
            }
        } else {
            result = ini.LoadFile( configFile );
            if ( result < 0 ) {
                cout << "Could not read configuration file... using default "
                        "configuration"
                     << endl;
                SetDefaultConfig( ini );
            }
        }  // copy file
    }      // config file exists

    assert( result == SI_OK );

    return true;
}

void Reset() { ini.Reset(); }

bool TryGetInt( const string& section, const string& key, int& value ) {
    ENSURE_KEY_EXISTS( section, key );

    value =
        static_cast<int>( ini.GetLongValue( section.c_str(), key.c_str() ) );

    return true;
}

bool TryGetString( const string& section, const string& key, string& value ) {
    ENSURE_KEY_EXISTS( section, key );

    value = ini.GetValue( section.c_str(), key.c_str(), nullptr );

    return true;
}

bool TryGetBool( const std::string& section, const std::string& key,
                 bool& value ) {
    ENSURE_KEY_EXISTS( section, key );

    value = ini.GetBoolValue( section.c_str(), key.c_str() );

    return true;
}
}  // namespace config
