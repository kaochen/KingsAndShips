#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "window.h"

class C_Texture
{
public:
	C_Texture(std::string name, SDL_Texture* image, int seqNbr);
	~C_Texture();

	SDL_Texture* getImage() const;
protected:
	std::string m_name;
	SDL_Texture* m_image;
	int m_seqNbr;
};



SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *renderer);
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);


#endif
