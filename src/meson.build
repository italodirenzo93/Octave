glad_dep = subproject('glad').get_variable('glad_dep')
simpleini_dep = subproject('simpleini').get_variable('simpleini_dep')
stb_image_dep = subproject('stb_image').get_variable('stb_image_dep')

glfw3_dep = dependency('glfw3')
glm_dep = dependency('glm')
spdlog_dep = dependency('spdlog')

octave = static_library(
	'Octave',
	[
		# Top-level
		'Application.cpp',
		'Application.hpp',
		'Camera.cpp',
		'Camera.hpp',
		'Config.cpp',
		'Config.hpp',
		'EntryPoint.hpp',
		'Exception.cpp',
		'Exception.hpp',
		'Layer.cpp',
		'Layer.hpp',
		'LayerStack.cpp',
		'LayerStack.hpp',
		'Octave.hpp',

		# Core
		'core/Log.cpp',
		'core/Log.hpp',
		'core/Platform.hpp',
		'core/Window.cpp',
		'core/Window.hpp',
		'core/Types.hpp',
		'core/glfw/GLFWError.cpp',
		'core/glfw/GLFWError.hpp',
		'core/glfw/PlatformGLFW.cpp',
		'core/glfw/WindowGLFW.cpp',
		'core/glfw/WindowGLFW.hpp',

		# Graphics
		'graphics/Buffer.hpp',
        'graphics/GraphicsDevice.cpp',
		'graphics/GraphicsDevice.hpp',
		# 'graphics/Mesh.cpp',
		# 'graphics/Mesh.hpp',
		# 'graphics/Model.cpp',
		# 'graphics/Model.hpp',
		'graphics/GraphicsContext.hpp',
		'graphics/Program.hpp',
		'graphics/Shader.cpp',
		'graphics/Shader.hpp',
		'graphics/Texture.hpp',
		'graphics/VertexArrayLayout.hpp',
		'graphics/OpenGL/BufferGL.cpp',
		'graphics/OpenGL/BufferGL.hpp',
		'graphics/OpenGL/GraphicsDeviceGL.cpp',
		'graphics/OpenGL/GraphicsDeviceGL.hpp',
		'graphics/OpenGL/GraphicsContextGL.cpp',
		'graphics/OpenGL/GraphicsContextGL.hpp',
		'graphics/OpenGL/ProgramGL.cpp',
		'graphics/OpenGL/ProgramGL.hpp',
		'graphics/OpenGL/TextureGL.cpp',
		'graphics/OpenGL/TextureGL.hpp',
		'graphics/OpenGL/VertexArrayLayoutGL.cpp',
		'graphics/OpenGL/VertexArrayLayoutGL.hpp',

		# Helpers
		'helpers/GeometricPrimitive.cpp',
		'helpers/GeometricPrimitive.hpp',
		'helpers/StepTimer.cpp',
		'helpers/StepTimer.hpp',

		# Input
		'input/Gamepad.hpp',
		'input/InputSystem.hpp',
		'input/glfw/GamepadGLFW.cpp',
		'input/glfw/GamepadGLFW.hpp',
		'input/glfw/InputSystemGLFW.cpp',
		'input/glfw/InputSystemGLFW.hpp',

		# Resources
		'resources/ResourceManager.hpp',
		'resources/ShaderManager.cpp',
		'resources/ShaderManager.hpp',
		'resources/TextureManager.cpp',
		'resources/TextureManager.hpp'
	],
	include_directories: include_directories(''),
	dependencies: [
		glad_dep,
		glfw3_dep,
		glm_dep,
		simpleini_dep,
		spdlog_dep,
		stb_image_dep
	],
	cpp_pch: ['pch/pch.hpp', 'pch/pch.cpp']
)

octave_dep = declare_dependency(
	link_with: octave,
	include_directories: include_directories(''),
	dependencies: [
		glad_dep,
		glfw3_dep,
		glm_dep,
		simpleini_dep,
		spdlog_dep,
		stb_image_dep
	]
)
