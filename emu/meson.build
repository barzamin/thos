project('tutorial', 'c', 'cpp',
	default_options : ['c_std=c11', 'cpp_std=c++23'])

dep_unicorn = dependency('unicorn')
dep_fmt = dependency('fmt')
dep_threads = dependency('threads')
executable('emu', 'main.cpp',
	dependencies : [dep_threads, dep_fmt, dep_unicorn])

