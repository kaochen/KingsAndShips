project('KingsAndShips','cpp',
	default_options: [
        'c_std=gnu11',
	'cpp_std=c++11',],
	 meson_version: '>=0.37'
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
	'src/coord.cpp',
	'src/time.cpp',
	'src/message.cpp',
	'src/wallet.cpp',
	'src/texture.cpp',
	'src/level/node.cpp',
	'src/level/pathfinding.cpp',
	'src/level/zlayer.cpp',
	'src/level/grid.cpp',
	'src/level/level.cpp',
	'src/level/weapons.cpp',
	'src/level/shooter.cpp',
	'src/level/towers.cpp',
	'src/level/barricade.cpp',
	'src/level/boat.cpp',
	'src/level/town.cpp',
	'src/level/landscape.cpp',
	'src/level/gameUnits.cpp',
	'src/window.cpp',
	'src/menuItems.cpp',
	'src/menu.cpp',
	'src/main.cpp']

exe = executable('kingsandships',
	install : true,
	sources : all_src,
	dependencies : all_dep)

