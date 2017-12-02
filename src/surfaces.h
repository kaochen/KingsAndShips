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
#include "gameUnits.h"

class C_Texture
{
public:
	C_Texture();
	C_Texture(int id, std::string name, std::string filePath , int tile_height, int tile_width);
	~C_Texture();


	void displayStatus();
	SDL_Texture* getTexture();
	void loadTexture(const std::string &path);

protected:
	int m_id;
	std::string m_name;
	std::string m_filePath;
	int m_tile_height;
	int m_tile_width;
	SDL_Texture * m_texture;
private:
	static std::map<std::string,SDL_Texture*> map_textures;
};


class C_TextureList
{
public:
	static	C_TextureList& Instances();
	void renderTexture(std::string name, int x, int y);
	std::map<std::string, C_Texture*>  getTextMap();
	void loadTexturesIntoMap();
	void extractTSXfile(std::string tsx_File_Path);
	void displayTexturesList();

private:
	C_TextureList& operator= (const C_TextureList&){return *this;}
	C_TextureList (const C_Texture&){}

	static C_TextureList m_instance;
	C_TextureList();
	~C_TextureList();

	std::map<std::string, C_Texture*> m_map_textures;
};



#endif
