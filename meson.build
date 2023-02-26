project('Octave', ['cpp', 'c'], version: '0.0.0', default_options: ['cpp_std=c++17'])

is_debug = get_option('debug')

if is_debug
	add_project_arguments('-DOGT_DEBUG', language: 'cpp')
endif

sys_os = host_machine.system()

# Platform defines
if sys_os == 'windows'
	add_project_arguments('-DOGT_PLATFORM_WINDOWS', language: 'cpp')
elif sys_os == 'darwin'
	add_project_arguments('-DOGT_PLATFORM_MACOS', language: 'cpp')
elif sys_os == 'linux'
	add_project_arguments('-DOGT_PLATFORM_LINUX', language: 'cpp')
endif

subdir('src')
subdir('examples')
subdir('tests')
