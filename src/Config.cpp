#include "Config.hpp"

#include <SimpleIni.h>

#include <iostream>

using namespace std;

static constexpr const char* kConfigFile = "./resources/config/engine.ini";

namespace config {
static CSimpleIniA ini;

inline bool EnsureKeyExists( const string& section, const string& key ) {
    if (!ini.KeyExists( section.c_str(), key.c_str() )) {
        cout << "Configuration value [" << section << "]/" << key
            << " not found" << endl;
        return false;
    }

    return true;
}

#define ENSURE_KEY_EXISTS( section, key ) if ( !EnsureKeyExists( section, key ) ) return false

bool Initialize() {
    const SI_Error result = ini.LoadFile( kConfigFile );
    if (result < 0) {
        cout << "Could not find " << kConfigFile << "..." << endl;
        return false;
    }
    assert( result == SI_OK );

    return true;
}

void Reset() { ini.Reset(); }

bool TryGetInt( const string& section, const string& key, int& value ) {
    ENSURE_KEY_EXISTS( section, key );

    value = static_cast<int>(ini.GetLongValue( section.c_str(), key.c_str() ));

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
} // namespace config
