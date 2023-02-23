#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace config {
bool Initialize();
void Reset();
bool TryGetInt( const std::string& section, const std::string& key,
                int& value );
bool TryGetString( const std::string& section, const std::string& key,
                   std::string& value );
} // namespace config

#endif
