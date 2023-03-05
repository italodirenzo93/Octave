#ifndef OCTAVE_CORE_TYPES_HPP
#define OCTAVE_CORE_TYPES_HPP

#include "pch/pch.hpp"

namespace Octave {

template <typename T>
using Ref = std::unique_ptr<T>;

template <typename T>
using SharedRef = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

}

#endif
