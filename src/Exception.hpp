#ifndef OCTAVE_EXCEPTION_HPP
#define OCTAVE_EXCEPTION_HPP

#include <exception>
#include <string>

namespace octave {

class Exception : public std::exception {
public:
	Exception() noexcept;
	Exception( const Exception& other ) noexcept;
	Exception( Exception&& other ) noexcept;
	~Exception() override = default;

	explicit Exception( const std::string& message ) noexcept;

	[[nodiscard]] const char* what() const noexcept override;

protected:
	std::string message_;

public:
	Exception& operator=( const Exception& other ) noexcept;
	Exception& operator=( Exception&& other ) noexcept;
};

}  // namespace octave

#endif
