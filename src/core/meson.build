subdir('SDL2')

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
		platform_sdl2_dep
	]
)
