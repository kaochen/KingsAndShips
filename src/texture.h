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


#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <map>

#include "window.h"
#include "settings.h"
#include "level/gameUnits.h"

class C_Texture
{
public:
	C_Texture();
	C_Texture(std::string name);

	virtual ~C_Texture();
	virtual SDL_Texture* getTexture();
	virtual void destroyTexture();
	virtual void displayStatus();
	virtual void render(int x, int y, double angle, int align);

	virtual int getId(){return m_id;};
	//Should be virtual :
	virtual void loadTexture(SDL_Texture* fullImage){if(fullImage !=nullptr){}};
	virtual void loadTextAsTextures(std::string &message,SDL_Color color, int fontSize){
		std::cout << "What for ?? " << message << fontSize << color.a << std::endl;};

protected:
	std::string m_name;
	std::vector <SDL_Texture*> m_textures;
	int m_id;
	int m_nbr_of_sub_res;
};


class C_Image: public C_Texture
{
public:
	C_Image(int id,int tileNbr, std::string name,
			SDL_Texture * texture, int tile_width,
			int tile_height, int file_width,
			int file_height, int nbrOfZoom);

	virtual void loadTexture(SDL_Texture* fullImage);
	virtual void displayStatus();

protected:
	int m_tileNbr;
	int m_tile_height;
	int m_tile_width;
	int m_file_width;
	int m_file_height;
	bool m_whiteBgrd;
};

//C_Text
class C_Text: public C_Texture
{
public:
	C_Text(std::string name, std::string message, SDL_Color color, int fontSize);

	virtual void loadTextAsTextures(std::string &message,SDL_Color color, int fontSize);

	//why I need to declare this, no clue at all:
	virtual void loadTexture(SDL_Texture* fullImage){if(fullImage !=nullptr){}};

protected:
	virtual void createNewTexture();
	virtual std::string findFont();
	std::string m_message;
	int m_fontSize;
	SDL_Color m_color;
};

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
	void renderTextureFromId(int id, int x, int y);
	std::map<std::string, C_Texture*>  getTextMap();
	void loadTextAsTexturesIntoMap(std::string name, std::string &message, int fontSize, SDL_Color color);
	void extractTSXfile(std::string tsx_File_Path);
	void displayTexturesList();
	std::string getNameFromID(int id);
	void freeTexture(std::string name);

private:
	SDL_Texture* imageToTexture(std::string &path);
	int m_count;
	std::map<std::string, C_Texture*> m_map_textures;
};


#endif
