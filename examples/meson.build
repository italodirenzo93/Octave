subdir('common')

executable('SimpleApp', ['SimpleApp.cpp'], dependencies: [octave_dep])
executable('ModelViewerApp', ['ModelViewerApp.cpp'], dependencies: [octave_dep, samples_common_dep])

sdl2_dep = dependency('sdl2', fallback: ['sdl2', 'sdl2_dep'])
bx_dep = dependency('bx', fallback: ['bx', 'bx_dep'])
bimg_dep = dependency('bimg', fallback: ['bimg', 'bimg_dep'])
bgfx_dep = dependency('bgfx', fallback: ['bgfx', 'bgfx_dep'])

executable(
	'CleanSlate',
	['CleanSlate.cpp'],
	dependencies: [bimg_dep],
	cpp_args: ['-DBX_CONFIG_DEBUG=@0@'.format(get_option('debug') ? 1 : 0)]
)
