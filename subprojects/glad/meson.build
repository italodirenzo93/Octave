project('glad', 'c', version: '0.1.36')

glad = static_library(
	'glad',
	[
		'src/glad.c',
		'include/glad/glad.h',
		'include/KHR/khrplatform.h'
	],
	include_directories: include_directories('include')
)

glad_dep = declare_dependency(
	link_with: glad,
	include_directories: include_directories('include')
)
