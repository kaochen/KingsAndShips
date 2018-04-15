project('towerdefense','cpp',
	default_options: [
        'c_std=gnu11',
	'cpp_std=c++11',]
	)
##gcc options:
add_global_arguments('-Wall', '-Wextra','-Wno-deprecated-declarations','-g',language:'cpp')

sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_gfx_dep = dependency('SDL2_gfx')
libxml_dep = dependency('libxml++-2.6')

all_dep = [ sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, sdl2_gfx_dep, libxml_dep ]

all_src = ['src/settings.cpp',
	'src/time.cpp',
	'src/surfaces.cpp',
	'src/pathfinding.cpp',
	'src/grid.cpp',
	'src/level.cpp',
	'src/weapons.cpp',
	'src/shooter.cpp',
	'src/towers.cpp',
	'src/invaders.cpp',
	'src/town.cpp',
	'src/landscape.cpp',
	'src/gameUnits.cpp',
	 'src/window.cpp',
	 'src/menu.cpp',
	 'src/main.cpp']

exe = executable('demo',
	install : true,
	sources : all_src,
	dependencies : all_dep)

