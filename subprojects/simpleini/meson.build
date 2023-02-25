project('simpleini', 'c', version: '4.19.0')

simpleini = static_library(
	'simpleini',
	[
		'ConvertUTF.c',
		'include/ConvertUTF.h',
		'include/SimpleIni.h'
	],
	include_directories: include_directories('include')
)

simpleini_dep = declare_dependency(
	link_with: simpleini,
	include_directories: include_directories('include')
)
