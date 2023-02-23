project( 'Octave', ['cpp', 'c'], version: '0.0.0', default_options: ['cpp_std=c++17'] )

# Top-level
source = [
	'src/pch.cpp',
	'src/pch.hpp',
	'src/Application.cpp',
	'src/Application.hpp',
	'src/Camera.cpp',
	'src/Camera.hpp',
	'src/Config.cpp',
	'src/Config.hpp',
	'src/Exception.cpp',
	'src/Exception.hpp',
	'src/Octave.hpp',
	'src/Main.cpp',
]

# Graphics
source += [
	'src/graphics/IndexBuffer.cpp',
	'src/graphics/IndexBuffer.hpp',
	'src/graphics/Mesh.cpp',
	'src/graphics/Mesh.hpp',
	'src/graphics/Model.cpp',
	'src/graphics/Model.hpp',
	'src/graphics/Renderer.cpp',
	'src/graphics/Renderer.hpp',
	'src/graphics/Shader.cpp',
	'src/graphics/Shader.hpp',
	'src/graphics/Texture.cpp',
	'src/graphics/Texture.hpp',
	'src/graphics/VertexBuffer.cpp',
	'src/graphics/VertexBuffer.hpp'
]

# Audio
# source += [
# 	'src/audio/Buffer.cpp',
# 	'src/audio/Listener.cpp',
# 	'src/audio/Source.cpp',
# 	'src/audio/stb_vorbis.cpp',
# ]

# Helpers
source += [
	'src/helpers/GeometricPrimitive.cpp',
	'src/helpers/GeometricPrimitive.hpp',
	'src/helpers/StepTimer.cpp',
	'src/helpers/StepTimer.hpp',
]

# Platform
source += [
	'src/platform/Platform.hpp',
	'src/platform/Window.cpp',
	'src/platform/Window.hpp',
	'src/platform/glfw/GLFWError.cpp',
	'src/platform/glfw/GLFWError.hpp',
	'src/platform/glfw/PlatformGLFW.cpp',
	'src/platform/glfw/PlatformGLFW.hpp',
	'src/platform/glfw/WindowGLFW.cpp',
	'src/platform/glfw/WindowGLFW.hpp'
]

# Input
source += [
	'src/input/Gamepad.hpp',
	'src/input/InputSystem.hpp',
	'src/input/glfw/GamepadGLFW.cpp',
	'src/input/glfw/GamepadGLFW.hpp',
	'src/input/glfw/InputSystemGLFW.cpp',
	'src/input/glfw/InputSystemGLFW.hpp'
]

# Resources
source += [
	'src/resources/ResourceManager.hpp',
	'src/resources/ShaderManager.cpp',
	'src/resources/ShaderManager.hpp'
]

# Samples
source += [
	'src/samples/Sample.cpp',
	'src/samples/Sample.hpp',
	'src/samples/ModelViewerSample.cpp',
	'src/samples/ModelViewerSample.hpp'
]

# Scene

glad_dep = subproject('glad').get_variable('glad_dep')
simpleini_dep = subproject('simpleini').get_variable('simpleini_dep')
stb_image_dep = subproject('stb_image').get_variable('stb_image_dep')
# imgui = subproject('imgui').get_variable('imgui_dep')

glfw3_dep = dependency('glfw3')
glm_dep = dependency('glm')

executable(
	'Octave',
	source,
	include_directories: include_directories('src'),
	dependencies: [
		glad_dep,
		glfw3_dep,
		glm_dep,
		simpleini_dep,
		stb_image_dep
	]
)
