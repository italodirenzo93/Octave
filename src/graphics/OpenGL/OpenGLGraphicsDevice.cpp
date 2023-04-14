#include "pch/pch.hpp"
#include "OpenGLGraphicsDevice.hpp"

#include "core/Log.hpp"
#include "Config.hpp"
#include "OpenGLGraphicsContext.hpp"

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

OpenGLGraphicsDevice::OpenGLGraphicsDevice( const Window& window ) {
	SDL_GL_ResetAttributes();

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	int flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

#ifdef OGT_DEBUG
	flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, flags );

	Log::GetCoreLogger()->trace( "Creating OpenGL rendering context" );

	m_Window = static_cast<SDL_Window*>( window.GetNativeWindowHandle() );
	m_Context = SDL_GL_CreateContext( m_Window );
	if ( !m_Context ) {
		Log::GetCoreLogger()->error( "SDL_GL_CreateContext error: {}", SDL_GetError() );
		throw Exception( "Unable to create OpenGL context" );
	}

	// Initialize Open GL extension loader
	if ( !gladLoadGLLoader(
			 reinterpret_cast<GLADloadproc>( SDL_GL_GetProcAddress ) ) ) {
		throw Exception( "Unable to initialize GLAD OpenGL extension loader" );
	}

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

std::string OpenGLGraphicsDevice::TryDequeueError() {
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

std::unique_ptr<GraphicsContext> OpenGLGraphicsDevice::CreateContext() {
	GLuint vao;
	glGenVertexArrays( 1, &vao );

	auto context = std::make_unique<OpenGLGraphicsContext>();
	context->SetApiResource( vao );

	return context;
}

void OpenGLGraphicsDevice::DestroyContext(
	std::unique_ptr<GraphicsContext> context ) {
	assert( context != nullptr );
	const GLuint vao = dynamic_cast<OpenGLGraphicsContext*>( context.get() )
						   ->GetApiResource();
	glDeleteVertexArrays( 1, &vao );
}

std::unique_ptr<Buffer> OpenGLGraphicsDevice::CreateBuffer(
	const BufferDescription& desc, const void* data ) {
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

	auto buffer = std::make_unique<Buffer>( desc );
	buffer->SetApiResource( handle );

	return buffer;
}

void OpenGLGraphicsDevice::DestroyBuffer( std::unique_ptr<Buffer> buffer ) {
	assert( buffer != nullptr );
	const GLuint handle = buffer->GetApiResource();
	glDeleteBuffers( 1, &handle );
}

std::unique_ptr<Program> OpenGLGraphicsDevice::CreateProgram( const Shader& vs,
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

	auto program = std::make_unique<Program>();
	program->SetApiResource( handle );

	return program;
}

void OpenGLGraphicsDevice::DestroyProgram( std::unique_ptr<Program> program ) {
	assert( program != nullptr );
	glDeleteProgram( program->GetApiResource() );
}

std::unique_ptr<Sampler> OpenGLGraphicsDevice::CreateSampler(
	const SamplerDescription& desc ) {
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

	auto sampler = std::make_unique<Sampler>( desc );
	sampler->SetApiResource( handle );

	return sampler;
}

void OpenGLGraphicsDevice::DestroySampler( std::unique_ptr<Sampler> sampler ) {
	assert( sampler != nullptr );
	const GLuint handle = sampler->GetApiResource();
	glDeleteSamplers( 1, &handle );
}

std::unique_ptr<Shader> OpenGLGraphicsDevice::CreateShader( ShaderType type,
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

	auto shader = std::make_unique<Shader>( type );
	shader->SetApiResource( handle );

	return shader;
}

void OpenGLGraphicsDevice::DestroyShader( std::unique_ptr<Shader> shader ) {
	assert( shader != nullptr );
	glDeleteShader( shader->GetApiResource() );
}

std::unique_ptr<Texture2D> OpenGLGraphicsDevice::CreateTexture2D(
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

	auto texture = std::make_unique<Texture2D>( desc );
	texture->SetApiResource( handle );

	return texture;
}

void OpenGLGraphicsDevice::DestroyTexture2D( std::unique_ptr<Texture2D> texture ) {
	assert( texture != nullptr );
	const GLuint handle = texture->GetApiResource();
	glDeleteTextures( 1, &handle );
}

void OpenGLGraphicsDevice::GenerateMipmap( const Texture2D& texture ) {
	assert( glIsTexture( texture.GetApiResource() ) );
	glBindTexture( GL_TEXTURE_2D, texture.GetApiResource() );
	glGenerateMipmap( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void OpenGLGraphicsDevice::SwapBuffers() {
	SDL_GL_SwapWindow( m_Window );
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
