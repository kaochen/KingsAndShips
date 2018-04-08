project('towerdefense','cpp',
	default_options: [
        'c_std=gnu11',
	'cpp_std=c++11',]
	)



add_global_arguments('-Wall',language:'cpp')
add_global_arguments('-Wextra', language:'cpp')
add_global_arguments('-Wno-deprecated-declarations',language:'cpp')
add_global_arguments('-g',language:'cpp')

sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_gfx_dep = dependency('SDL2_gfx')
libxml_dep = dependency('libxml++-2.6')

all_dep = [ sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, sdl2_gfx_dep, libxml_dep ]

src00 = ['src/settings.cpp','src/time.cpp','src/surfaces.cpp','src/pathfinding.cpp','src/grid.cpp','src/level.cpp']
src01 = ['src/weapons.cpp','src/shooter.cpp','src/towers.cpp','src/invaders.cpp','src/landscape.cpp']
src02 = ['src/gameUnits.cpp', 'src/window.cpp','src/menu.cpp','src/main.cpp']

all_src = [src00, src01, src02]

exe = executable('demo',
	install : true,
	sources : all_src,
	dependencies : all_dep)

test('towerdefense test', exe)
