#include "Config.hpp"

#include <SimpleIni.h>

#include <iostream>

using namespace std;

static constexpr const char* kConfigFile = "./resources/config/engine.ini";

namespace config {
static CSimpleIniA ini;

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
    const char* value_str =
        ini.GetValue( section.c_str(), key.c_str(), nullptr );
    if (!value_str) {
        return false;
    }

    try {
        value = stoi( value_str );
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return false;
    }

    return true;
}

bool TryGetString( const string& section, const string& key, string& value ) {
    const char* value_str =
        ini.GetValue( section.c_str(), key.c_str(), nullptr );

    if (!value_str) {
        return false;
    }

    value = value_str;

    return true;
}
} // namespace config
