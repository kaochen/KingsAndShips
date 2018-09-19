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

	virtual int getId() = 0;
	virtual void loadTexture(std::string &path) = 0;
	virtual void loadTextAsTextures(std::string &message,SDL_Color color, int fontSize) = 0;

protected:
	std::string m_name;
	SDL_Texture * m_texture;
};


class C_Image: public C_Texture
{
public:
	C_Image(int id,int tileNbr, std::string name, std::string file_Path, int tile_width, int tile_height, int file_width, int file_height);

	virtual int getId();
	virtual void loadTexture( std::string &path);
	virtual void displayStatus();
	virtual void loadTextAsTextures(std::string &message,SDL_Color color, int fontSize);

protected:
	int m_id;
	int m_tileNbr;
	std::string m_file_path;
	int m_tile_height;
	int m_tile_width;
	int m_file_width;
	int m_file_height;
};

//C_Text
class C_Text: public C_Texture
{
public:
	C_Text(std::string name, std::string message);

	virtual void loadTextAsTextures(std::string &message,SDL_Color color, int fontSize);

	//why I need to declare this, no clue at all:
	virtual int getId();
	virtual void loadTexture( std::string &path);

protected:
	std::string findFont();
	std::string m_message;
};


//C_TextureList
class C_TextureList
{
public:
	static	C_TextureList& Instances();
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
	C_TextureList& operator= (const C_TextureList&){return *this;}
	C_TextureList (const C_Texture&){}

	static C_TextureList m_instance;
	C_TextureList();
	~C_TextureList();
	int m_count;
	std::map<std::string, C_Texture*> m_map_textures;
};



#endif
