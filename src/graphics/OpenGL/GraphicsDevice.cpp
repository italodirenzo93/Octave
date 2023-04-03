#include "pch/pch.hpp"
#include "graphics/GraphicsDevice.hpp"

#include "core/Log.hpp"
#include "Config.hpp"
#include "graphics/GraphicsContext.hpp"
#include "core/glfw/GLFWError.hpp"
#include <GLFW/glfw3.h>

using namespace std;

namespace Octave {

static void APIENTRY DebugCallback( GLenum source, GLenum type, unsigned int id,
									GLenum severity, GLsizei length,
									const char* message,
									const void* userParam );

GraphicsDevice::GraphicsDevice( const Window& window ) {
	const auto p_window =
		static_cast<GLFWwindow*>( window.GetNativeWindowHandle() );

	if ( !p_window ) {
		throw Exception( "Cannot accept a null window pointer" );
	}

	// Validate GLFW handle
	{
		const auto client = glfwGetWindowAttrib( p_window, GLFW_CLIENT_API );
		if ( client == GLFW_PLATFORM_ERROR || client == GLFW_NOT_INITIALIZED ) {
			throw GLFWError();
		}
	}

	// Initialize Open GL extension loader
	if ( !gladLoadGLLoader(
			 reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) ) {
		throw Exception( "Unable to initialize GLAD OpenGL extension loader" );
	}

	Log::GetCoreLogger()->trace( "Creating OpenGL rendering context" );

	// Get context information
	int context_flags;
	glGetIntegerv( GL_CONTEXT_FLAGS, &context_flags );

	// Configure debug callback if we have a debug context
	if ( GLAD_GL_KHR_debug && ( context_flags & GL_CONTEXT_FLAG_DEBUG_BIT ) ) {
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( DebugCallback, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
							   nullptr, GL_TRUE );
	}
}

GraphicsDevice::~GraphicsDevice() noexcept {
	Log::GetCoreLogger()->trace( "Deleting OpenGL rendering context" );
}

std::string GraphicsDevice::TryDequeueError() noexcept {
	const GLenum error = glGetError();

	switch ( error ) {
		case GL_NO_ERROR:
			return "";
		case GL_INVALID_ENUM:
			return "Invalid Enum";
		case GL_INVALID_VALUE:
			return "Invalid Value";
		case GL_INVALID_OPERATION:
			return "Invalid Operation";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "Invalid Framebuffer Operation";
		case GL_OUT_OF_MEMORY:
			return "Out Of Memory";
		case GL_STACK_OVERFLOW:
			return "Stack Overflow";
		case GL_STACK_UNDERFLOW:
			return "Stack Undeflow";
		default:
			return "Unknown Error";
	}
}

Ref<GraphicsContext> GraphicsDevice::CreateContext() noexcept {
	return unique_ptr<GraphicsContext>( new GraphicsContext() );
}

Ref<Buffer> GraphicsDevice::CreateBuffer( const BufferDescription& desc,
										  const void* initial_data ) {
	return MakeRef<Buffer>( desc, initial_data );
}

Ref<Fence> GraphicsDevice::CreateFence() noexcept {
	return MakeRef<Fence>();
}

Ref<VertexArray> GraphicsDevice::CreateVertexArray( const VertexLayout& desc ) {
	return MakeRef<VertexArray>( desc );
}

Ref<Program> GraphicsDevice::CreateProgram( const Shader& vs,
											const Shader& fs ) {
	return MakeRef<Program>( vs, fs );
}

Ref<Sampler> GraphicsDevice::CreateSampler( const SamplerDescription& desc ) {
	return MakeRef<Sampler>( desc );
}

Ref<Shader> GraphicsDevice::CreateShader( ShaderType type,
										  const char* source ) {
	return MakeRef<Shader>( type, source );
}

Ref<Texture2D> GraphicsDevice::CreateTexture2D(
	const TextureDescription2D& desc ) {
	return MakeRef<Texture2D>( desc );
}

static void APIENTRY DebugCallback( GLenum source, GLenum type, unsigned int id,
									GLenum severity, GLsizei length,
									const char* message,
									const void* userParam ) {
	// ignore non-significant error/warning codes
	if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

	string str_source;
	switch ( source ) {
		case GL_DEBUG_SOURCE_API:
			str_source = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			str_source = "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			str_source = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			str_source = "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			str_source = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			str_source = "Other";
			break;
	}

	string str_type;
	switch ( type ) {
		case GL_DEBUG_TYPE_ERROR:
			str_type = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			str_type = "Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			str_type = "Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			str_type = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			str_type = "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			str_type = "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			str_type = "Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			str_type = "Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER:
			str_type = "Other";
			break;
	}

	string str_severity;
	spdlog::level::level_enum log_level = spdlog::level::trace;
	switch ( severity ) {
		case GL_DEBUG_SEVERITY_HIGH:
			str_severity = "high";
			log_level = spdlog::level::critical;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			str_severity = "medium";
			log_level = spdlog::level::err;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			str_severity = "low";
			log_level = spdlog::level::warn;
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			str_severity = "notification";
			log_level = spdlog::level::info;
			break;
	}

	Log::GetCoreLogger()->log(
		log_level, "OpenGL Error {} | Source: {} Type: {} Severity: {} | {}",
		id, str_source, str_type, str_severity, message );
}

}  // namespace Octave
