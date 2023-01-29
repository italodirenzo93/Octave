#ifndef COMMONINCLUDE_HPP
#define COMMONINCLUDE_HPP

// clang-format off

// Library headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// clang-format on

// C++ headers
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// C headers
#include <cstdint>

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
