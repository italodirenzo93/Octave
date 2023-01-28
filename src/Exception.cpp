#include "Exception.hpp"

namespace octave {

Exception::Exception() noexcept : message_( "Unknown Error" ) {
}

Exception::Exception( const Exception& other ) noexcept
    : message_( other.message_ ) {
}

Exception::Exception( Exception&& other ) noexcept
    : message_( std::move( other.message_ ) ) {
}

Exception::Exception( const std::string& message ) noexcept
    : message_( message ) {
}

const char* Exception::what() const noexcept {
    return message_.c_str();
}

Exception& Exception::operator=( const Exception& other ) noexcept {
    if ( &other == this ) {
        return *this;
    }

	message_ = other.message_;

	return *this;
}

Exception& Exception::operator=( Exception&& other ) noexcept {
    message_ = std::move( other.message_ );

	return *this;
}

}  // namespace octave
