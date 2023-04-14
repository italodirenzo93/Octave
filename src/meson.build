subdir('core')
subdir('graphics')

glm_dep = dependency('glm')
glad_dep = dependency('glad', fallback: ['glad', 'glad_dep'])
simpleini_dep = dependency('simpleini', fallback: ['simpleini', 'simpleini_dep'])
sdl2_dep = dependency('sdl2')
spdlog_dep = dependency('spdlog')
stb_image_dep = dependency('stb_image', fallback: ['stb_image', 'stb_image_dep'])

octave_lib = static_library(
	'Octave',
	[
		# Top-level
		'Application.cpp',
		'Application.hpp',
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

		# Helpers
		'helpers/GeometricPrimitive.cpp',
		'helpers/GeometricPrimitive.hpp',
		'helpers/StepTimer.cpp',
		'helpers/StepTimer.hpp',

		# Input
		'input/Gamepad.hpp',
		'input/InputSystem.hpp',
		# 'input/glfw/GamepadGLFW.cpp',
		# 'input/glfw/GamepadGLFW.hpp',
		# 'input/glfw/InputSystemGLFW.cpp',
		# 'input/glfw/InputSystemGLFW.hpp',
		'input/Null/InputSystemNull.hpp',

		# Resources
		'resources/ResourceManager.hpp',
		# 'resources/ShaderManager.cpp',
		# 'resources/ShaderManager.hpp',
		# 'resources/TextureManager.cpp',
		# 'resources/TextureManager.hpp'
	],
	include_directories: include_directories(''),
	dependencies: [
		core_dep,
		graphics_dep,

		# Third-party
		glm_dep,
		glad_dep,
		simpleini_dep,
		sdl2_dep,
		spdlog_dep,
		stb_image_dep
	],
	cpp_pch: ['pch/pch.hpp', 'pch/pch.cpp']
)

octave_dep = declare_dependency(
	link_with: octave_lib,
	include_directories: include_directories(''),
	dependencies: [
		# Third-party
		glm_dep,
		glad_dep,
		simpleini_dep,
		sdl2_dep,
		spdlog_dep,
		stb_image_dep
	]
)
