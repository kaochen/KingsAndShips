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
	C_Texture(std::string name);

	virtual ~C_Texture();
	virtual SDL_Texture* getTexture();
	virtual void displayStatus();

	virtual int getId() = 0;

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
	C_Text(std::string name);
	virtual void loadTextAsTexture(std::string &message,SDL_Color color, int fontSize);
protected:
	std::string findFont();
};


//C_TextureList
class C_TextureList
{
public:
	static	C_TextureList& Instances();
	void renderTexture(std::string name, int x, int y);
	void renderTextureEx(std::string name, int x, int y, double angle);
	void renderTextureFromId(int id, int x, int y);
	std::map<std::string, C_Texture*>  getTextMap();
	void loadTexturesIntoMap();
	void extractTSXfile(std::string tsx_File_Path);
	void displayTexturesList();
	std::string getNameFromID(int id);

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
