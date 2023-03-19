subdir('OpenGL')

graphics_dep = declare_dependency(
	include_directories: include_directories(''),
	sources: [
		'GraphicsContext.hpp',
		'GraphicsDevice.hpp',
		'Resources.hpp',
		'Structs.hpp',
	],
	dependencies: [rhi_opengl_dep]
)
