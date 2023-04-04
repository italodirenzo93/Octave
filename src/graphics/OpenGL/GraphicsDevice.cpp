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

static GLuint WrapToGLType( TextureWrap wrap ) noexcept {
	switch ( wrap ) {
		default:
		case TextureWrap::Repeat:
			return GL_REPEAT;
		case TextureWrap::Mirror:
			return GL_MIRRORED_REPEAT;
		case TextureWrap::ClampEdge:
			return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampBorder:
			return GL_CLAMP_TO_BORDER;
	}
}

static GLuint FilterToGLType( TextureFilter filter ) noexcept {
	switch ( filter ) {
		default:
		case TextureFilter::Linear:
			return GL_LINEAR;
		case TextureFilter::Nearest:
			return GL_NEAREST;
		case TextureFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
	}
}

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
										  const void* data ) {
	GLenum target = GL_ARRAY_BUFFER;
	switch ( desc.type ) {
		default:
		case BufferType::VertexBuffer:
			target = GL_ARRAY_BUFFER;
			break;
		case BufferType::IndexBuffer:
			target = GL_ELEMENT_ARRAY_BUFFER;
			break;
		case BufferType::UniformBuffer:
			target = GL_UNIFORM_BUFFER;
			break;
	}

	GLuint handle;
	glGenBuffers( 1, &handle );

	glBindBuffer( target, handle );

	if ( GLAD_GL_ARB_buffer_storage ) {
		GLint flags = 0;
		switch ( desc.usage ) {
			default:
			case BufferUsage::Static:
				flags = 0;
				break;
			case BufferUsage::Dynamic:
				flags = GL_DYNAMIC_STORAGE_BIT;
				break;
		}

		glBufferStorage( target, static_cast<GLsizeiptr>( desc.size ), data,
						 flags );
	} else {
		GLenum usage = GL_STATIC_DRAW;
		switch ( desc.usage ) {
			default:
			case BufferUsage::Static:
				usage = GL_STATIC_DRAW;
				break;
			case BufferUsage::Dynamic:
				usage = GL_DYNAMIC_DRAW;
				break;
		}

		glBufferData( target, static_cast<GLsizeiptr>( desc.size ), data,
					  usage );
	}

	glBindBuffer( target, 0 );

	auto buffer = MakeRef<Buffer>( desc );
	buffer->SetApiResource( handle );

	return buffer;
}

void GraphicsDevice::DestroyBuffer( Ref<Buffer> buffer ) {
	const GLuint handle = buffer->GetApiResource();
	glDeleteBuffers( 1, &handle );
}

Ref<VertexArray> GraphicsDevice::CreateVertexArray( const VertexLayout& desc ) {
	GLuint handle;
	glGenVertexArrays( 1, &handle );

	auto vertex_array = MakeRef<VertexArray>( desc );
	vertex_array->SetApiResource( handle );

	return vertex_array;
}

void GraphicsDevice::DestroyVertexArray( Ref<VertexArray> vertex_array ) {
	const GLuint handle = vertex_array->GetApiResource();
	glDeleteVertexArrays( 1, &handle );
}

Ref<Program> GraphicsDevice::CreateProgram( const Shader& vs,
											const Shader& fs ) {
	const GLuint handle = glCreateProgram();

	glAttachShader( handle, vs.GetApiResource() );
	glAttachShader( handle, fs.GetApiResource() );

	glLinkProgram( handle );

	glDetachShader( handle, vs.GetApiResource() );
	glDetachShader( handle, fs.GetApiResource() );

	GLint success = GL_FALSE;

	glGetProgramiv( handle, GL_LINK_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetProgramInfoLog( handle, 512, nullptr, msg );
		throw Exception( msg );
	}

	auto program = MakeRef<Program>();
	program->SetApiResource( handle );

	return program;
}

void GraphicsDevice::DestroyProgram( Ref<Program> program ) {
	assert( glIsProgram( program->GetApiResource() ) );
	glDeleteProgram( program->GetApiResource() );
}

Ref<Sampler> GraphicsDevice::CreateSampler( const SamplerDescription& desc ) {
	GLuint handle;
	glGenSamplers( 1, &handle );

	// Set sampler params
	glSamplerParameterf( handle, GL_TEXTURE_LOD_BIAS, desc.mip_lod_bias );

	glSamplerParameterf( handle, GL_TEXTURE_MAX_LOD, desc.max_lod );
	glSamplerParameterf( handle, GL_TEXTURE_MIN_LOD, desc.min_lod );

	glSamplerParameteri( handle, GL_TEXTURE_MAG_FILTER,
						 FilterToGLType( desc.filter ) );
	glSamplerParameteri( handle, GL_TEXTURE_MIN_FILTER,
						 FilterToGLType( desc.filter ) );

	glSamplerParameteri( handle, GL_TEXTURE_WRAP_S,
						 WrapToGLType( desc.wrap_s ) );
	glSamplerParameteri( handle, GL_TEXTURE_WRAP_T,
						 WrapToGLType( desc.wrap_t ) );

	auto sampler = MakeRef<Sampler>( desc );
	sampler->SetApiResource( handle );

	return sampler;
}

void GraphicsDevice::DestroySampler( Ref<Sampler> sampler ) {
	const GLuint handle = sampler->GetApiResource();
	assert( glIsSampler( handle ) );
	glDeleteSamplers( 1, &handle );
}

Ref<Shader> GraphicsDevice::CreateShader( ShaderType type,
										  const char* source ) {
	const GLenum shader_type = type == ShaderType::VertexShader
								   ? GL_VERTEX_SHADER
								   : GL_FRAGMENT_SHADER;
	const GLuint handle = glCreateShader( shader_type );

	glShaderSource( handle, 1, &source, nullptr );
	glCompileShader( handle );

	GLint success = GL_FALSE;

	glGetShaderiv( handle, GL_COMPILE_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetShaderInfoLog( handle, 512, nullptr, msg );
		throw Exception( msg );
	}

	auto shader = MakeRef<Shader>( shader_type );
	shader->SetApiResource( handle );

	return shader;
}

void GraphicsDevice::DestroyShader( Ref<Shader> shader ) {
	assert( glIsShader( shader->GetApiResource() ) );
	glDeleteShader( shader->GetApiResource() );
}

Ref<Texture2D> GraphicsDevice::CreateTexture2D(
	const TextureDescription2D& desc ) {
	GLuint handle;
	glGenTextures( 1, &handle );

	glBindTexture( GL_TEXTURE_2D, handle );

	// Internal texture format
	GLenum internal_format;
	switch ( desc.format ) {
		case TextureFormat::Rgb:
			internal_format = GL_RGB32F;
			break;
		default:
		case TextureFormat::Rgba:
			internal_format = GL_RGBA32F;
			break;
	}

	// Generic texture format
	GLenum format;
	switch ( desc.format ) {
		case TextureFormat::Rgb:
			format = GL_RGB;
			break;
		default:
		case TextureFormat::Rgba:
			format = GL_RGBA;
			break;
	}

	if ( GLAD_GL_ARB_texture_storage ) {
		glTexStorage2D( GL_TEXTURE_2D, static_cast<GLsizei>( desc.mip_levels ),
						internal_format, static_cast<GLsizei>( desc.width ),
						static_cast<GLsizei>( desc.height ) );
	} else {
		glTexImage2D( GL_TEXTURE_2D, 0, internal_format,
					  static_cast<GLsizei>( desc.width ),
					  static_cast<GLsizei>( desc.height ), 0, format,
					  GL_UNSIGNED_BYTE, nullptr );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );

	auto texture = MakeRef<Texture2D>( desc );
	texture->SetApiResource( handle );

	return texture;
}

void GraphicsDevice::DestroyTexture2D( Ref<Texture2D> texture ) {
	const GLuint handle = texture->GetApiResource();
	glDeleteTextures( 1, &handle );
}

void GraphicsDevice::GenerateMipmap( const Texture2D& texture ) {
	assert( glIsTexture( texture.GetApiResource() ) );
	glBindTexture( GL_TEXTURE_2D, texture.GetApiResource() );
	glGenerateMipmap( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
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
