# Top-level
source = [
	'Application.cpp',
	'Application.hpp',
	'Camera.cpp',
	'Camera.hpp',
	'Config.cpp',
	'Config.hpp',
	'EntryPoint.hpp',
	'Exception.cpp',
	'Exception.hpp',
	'Octave.hpp'
]

# Graphics
source += [
	'graphics/IndexBuffer.cpp',
	'graphics/IndexBuffer.hpp',
	'graphics/Mesh.cpp',
	'graphics/Mesh.hpp',
	'graphics/Model.cpp',
	'graphics/Model.hpp',
	'graphics/Renderer.cpp',
	'graphics/Renderer.hpp',
	'graphics/Shader.cpp',
	'graphics/Shader.hpp',
	'graphics/Texture.cpp',
	'graphics/Texture.hpp',
	'graphics/VertexBuffer.cpp',
	'graphics/VertexBuffer.hpp'
]

# Helpers
source += [
	'helpers/GeometricPrimitive.cpp',
	'helpers/GeometricPrimitive.hpp',
	'helpers/StepTimer.cpp',
	'helpers/StepTimer.hpp',
]

# Platform
source += [
	'platform/Platform.hpp',
	'platform/Window.cpp',
	'platform/Window.hpp',
	'platform/glfw/GLFWError.cpp',
	'platform/glfw/GLFWError.hpp',
	'platform/glfw/PlatformGLFW.cpp',
	'platform/glfw/PlatformGLFW.hpp',
	'platform/glfw/WindowGLFW.cpp',
	'platform/glfw/WindowGLFW.hpp'
]

# Input
source += [
	'input/Gamepad.hpp',
	'input/InputSystem.hpp',
	'input/glfw/GamepadGLFW.cpp',
	'input/glfw/GamepadGLFW.hpp',
	'input/glfw/InputSystemGLFW.cpp',
	'input/glfw/InputSystemGLFW.hpp'
]

# Resources
source += [
	'resources/ResourceManager.hpp',
	'resources/ShaderManager.cpp',
	'resources/ShaderManager.hpp'
]

glad_dep = subproject('glad').get_variable('glad_dep')
simpleini_dep = subproject('simpleini').get_variable('simpleini_dep')
stb_image_dep = subproject('stb_image').get_variable('stb_image_dep')
# imgui = subproject('imgui').get_variable('imgui_dep')

glfw3_dep = dependency('glfw3')
glm_dep = dependency('glm')

octave = static_library(
	'octave',
	source,
	include_directories: include_directories(''),
	dependencies: [
		glad_dep,
		glfw3_dep,
		glm_dep,
		simpleini_dep,
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
		stb_image_dep
	]
)
