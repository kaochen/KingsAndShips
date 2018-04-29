# Kings & Ships
A defense Tower game (Work In Progess) (C++/SDL2)
The king is going back with his ships, but you have taken his throne during his journey. You don't want him to go back. Organize your defense to push him back away and enjoy your new lifestyle.


The game is under the license: **GPLv3** , and all graphics are under Creative Commons : **CC BY-SA 4.0**

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

This is work in progress, use it carefully.


### Dependencies
```
apt install build-essential git gettext meson libxml++2.6-dev
apt install libsdl2-dev libsdl2-ttf-dev libfreetype6-dev libxml2-dev libsdl2-image-dev libsdl2-gfx-dev fonts-roboto

```

### Download
```
git clone https://github.com/kaochen/DefenseTowerGame
cd downloadfolder/DefenseTowerGame
```
If you have already the project and you want to synchonize from the server :
```
git pull
```

### Compile
```
meson build
ninja -C build/
```

### Play
```
./build/kingsandships
```

Drag & drop Tower or Turbines on to the map.
Push "n" to send next wave.
Push "r" to reset level.
Push "d" for debug mode.
Push "d+p" to get access to the debug path mode.
Push "q" to quit.

### Install

## Dev tips:
```
apt install tiled blender imagemagick
```
http://www.mapeditor.org

Build a tileset from images with imagemagick
```
montage Src*.png -tile 8x8 -geometry +0+0 -background none tilset.png
```

Search where improving the code is more usefull
```
apt install linux-perf
sudo perf record -g ./build/demo -sleep 10
perf report --sort comm,dso
```

### Dev links :

 * https://www.kraxel.org/blog/tag/meson-ninja/
 * http://www.willusher.io/pages/sdl2/
 * http://clintbellanger.net/articles/isometric_math/
 * http://flarerpg.org/tutorials/isometric_tiles/
 * http://doc.mapeditor.org

#### XML : ####

 * https://developer.gnome.org/libxml++-tutorial/stable/
