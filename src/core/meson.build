subdir('glfw')

core_dep = declare_dependency(
	include_directories: include_directories(''),
	sources: [
		'Callback.hpp',
		'Log.cpp',
		'Log.hpp',
		'Platform.hpp',
		'Window.cpp',
		'Window.hpp'
	],
	dependencies: [
		platform_glfw_dep
	]
)
