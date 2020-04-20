/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef TEXTURESLIST_H
#define TEXTURESLIST_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <map>

#include "window.h"
#include "settings.h"
#include "texture.h"
#include "level/gameUnits.h"


/*! \class C_TextureList
 * \brief C_TextureList is a database for SDL Texture.
 * It store SDL_Texture by name into a map
 */

//C_TextureList
class C_TextureList
{
public:
	C_TextureList();
	virtual ~C_TextureList();
	void renderTexture(std::string name, int x, int y);
	void renderTexture(std::string name, int x, int y,int align);
	void renderTextureEx(std::string name, int x, int y, double angle, int align);
	C_Texture* searchTexture(std::string name);
	std::map<std::string, C_Texture*>  getTextMap();
	void loadTextAsTexturesIntoMap(std::string name, std::string &message, int fontSize, SDL_Color color);
	void extractTSXfile(std::string tsx_File_Path);
	void displayTexturesList();
	std::string getNameFromID(int nbr, std::string tsxName);
	void freeTexture(std::string name);

private:
	int nbrOfZoom(std::string name);
	SDL_Texture* imageToTexture(std::string &path);
	std::map<std::string, C_Texture*> m_map_textures;
};


#endif
