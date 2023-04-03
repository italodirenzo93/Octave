#ifndef OCTAVE_PCH_HPP
#define OCTAVE_PCH_HPP

// clang-format off

// Library headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// clang-format on

// C++ headers
#include <array>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// C headers
#include <cstdint>

// Framework headers
#include "Exception.hpp"
#include "core/Types.hpp"

#define NON_COPYABLE_CLASS( className )     \
	className( const className& ) = delete; \
	className& operator=( const className& ) = delete;

#define NON_MOVABLE_CLASS( className ) \
	className( className&& ) = delete; \
	className& operator=( className&& ) = delete;

#define NON_COPYABLE_OR_MOVABLE_CLASS( className ) \
	NON_COPYABLE_CLASS( className )                \
	NON_MOVABLE_CLASS( className )

#define SELF_REFERENCE_CHECK( other ) \
	if ( &other == this ) return *this

#endif
