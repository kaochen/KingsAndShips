#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

#include "window.h"
#include "settings.h"
#include "gameUnits.h"

class C_Texture
{
public:
	C_Texture();
	C_Texture(std::string name, int seqNbr);
	~C_Texture();

	static std::map<std::string, SDL_Texture*>  getTextMap();
	static SDL_Texture* getText(std::string name);

	static void loadTexturesIntoMap(SDL_Renderer *renderer);
protected:
	std::string m_name;
	int m_seqNbr;
private:
	static std::map<std::string,SDL_Texture*> map_textures;
};


SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *renderer);
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);
void drawElipse(SDL_Renderer *renderer,int x,int y, int width);

void displayGridContent(SDL_Renderer *renderer,
			C_GameUnits::S_layer grid[][TABLE_SIZE]);
#endif
