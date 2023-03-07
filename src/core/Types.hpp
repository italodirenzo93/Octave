#ifndef OCTAVE_CORE_TYPES_HPP
#define OCTAVE_CORE_TYPES_HPP

#include <memory>

namespace Octave {

// Smart pointer types

template <typename T>
using Ref = std::unique_ptr<T>;

template <typename T>
using SharedRef = std::shared_ptr<T>;

// Smart pointer factory functions

template <typename T, typename... Args>
inline Ref<T> MakeRef( Args&&... args ) {
	return std::make_unique<T>( args... );
}

template <typename T, typename... Args>
inline SharedRef<T> MakeShared( Args&&... args ) {
	return std::make_shared<T>( args... );
}

}  // namespace Octave

#endif
