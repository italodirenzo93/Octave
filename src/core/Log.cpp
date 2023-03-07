#include "pch/pch.hpp"
#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

using namespace std;

namespace Octave {

SharedRef<spdlog::logger> Log::logger_core_;
SharedRef<spdlog::logger> Log::logger_client_;


void Log::Init() noexcept {
	vector<spdlog::sink_ptr> sinks;
	sinks.emplace_back( MakeShared<spdlog::sinks::stdout_color_sink_mt>() );

	logger_core_ = MakeShared<spdlog::logger>( "OCTAVE", sinks.begin(), sinks.end() );
	spdlog::register_logger( logger_core_ );
	logger_core_->set_level(spdlog::level::trace);
	logger_core_->flush_on(spdlog::level::trace);

	logger_client_ = MakeShared<spdlog::logger>( "CLIENT", sinks.begin(), sinks.end() );
	spdlog::register_logger( logger_client_ );
	logger_client_->set_level(spdlog::level::trace);
	logger_client_->flush_on(spdlog::level::trace);
}

}  // namespace Octave
