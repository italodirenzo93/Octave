#ifndef OCTAVE_LOG_HPP
#define OCTAVE_LOG_HPP

#include <spdlog/spdlog.h>

namespace Octave {

class Log {
public:
	static void Init() noexcept;

	static std::shared_ptr<spdlog::logger>& GetCoreLogger() noexcept {
		return logger_core_;
	}

	static std::shared_ptr<spdlog::logger>& GetClientLogger() noexcept {
		return logger_client_;
	}

	template <typename ...Args>
	static void Critical( const char* format, const Args&... args ) noexcept {
		GetClientLogger()->critical( format, args... );
	}

	template <typename ...Args>
	static void Error( const char* format, const Args&... args ) noexcept {
		GetClientLogger()->error( format, args... );
	}

	template <typename ...Args>
	static void Warn( const char* format, const Args&... args ) noexcept {
		GetClientLogger()->warn( format, args... );
	}

	template <typename ...Args>
	static void Info( const char* format, const Args&... args ) noexcept {
		GetClientLogger()->info( format, args... );
	}

	template <typename ...Args>
	static void Trace( const char* format, const Args&... args ) noexcept {
		GetClientLogger()->trace( format, args... );
	}

private:
	static std::shared_ptr<spdlog::logger> logger_core_;
	static std::shared_ptr<spdlog::logger> logger_client_;

public:
	Log() = delete;
	Log( const Log& other ) = delete;
	Log( Log&& other ) noexcept = delete;
	~Log() noexcept = delete;

	Log& operator=( const Log& other ) = delete;
	Log& operator=( Log&& other ) noexcept = delete;
};

}  // namespace Octave

#endif
