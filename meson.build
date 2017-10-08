project('towerdefense','cpp')
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sdl2_ttf = dependency('SDL2_ttf')
sdl2_gfx = dependency('SDL2_gfx')
all_dep = [ sdl2_dep, sdl2_image_dep, sdl2_ttf, sdl2_gfx ]

src00 = ['src/settings.cpp']
src01 = ['src/gameUnits.cpp','src/weapons.cpp','src/shooter.cpp','src/towers.cpp','src/invaders.cpp']
src02 = ['src/surfaces.cpp','src/window.cpp','src/main.cpp']

all_src = [src00, src01, src02]

exe = executable('demo',
	sources : all_src,
	dependencies : all_dep)

test('towerdefense test', exe)
