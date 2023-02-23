#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class Exception : public std::exception {
public:
    Exception() noexcept;
    Exception(const Exception& other) noexcept;
    Exception(Exception&& other) noexcept;
    ~Exception() noexcept override;

    explicit Exception( const std::string& message ) noexcept;

    const char* what() const noexcept override;

protected:
    std::string message_;
};

#endif
