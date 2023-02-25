project('stb_image', 'c', version: '2.27.0')

stb_image = static_library(
	'stb_image',
	[
		'stb_image_impl.c',
		'include/stb_image.h'
	],
	include_directories: include_directories('include')
)

stb_image_dep = declare_dependency(
	link_with: stb_image,
	include_directories: include_directories('include')
)
