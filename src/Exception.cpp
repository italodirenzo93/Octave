#include "Exception.hpp"

Exception::Exception() noexcept : message_("Unknown Error") {
}

Exception::Exception(const Exception& other) noexcept : message_(other.message_) {
}

Exception::Exception(Exception&& other) noexcept : message_(std::move(other.message_)) {
}

Exception::~Exception() noexcept {
}

Exception::Exception( const std::string& message ) noexcept : message_(message) {
}

const char* Exception::what() const noexcept {
    return message_.c_str();
}
