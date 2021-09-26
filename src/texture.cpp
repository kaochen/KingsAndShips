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


#include "texture.h"
#include "tools.h"
#include "locator.h"
#include "settings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <sys/stat.h>

#include <algorithm>
#include <cctype>

#include "message.h"
using namespace std;

//Textures

C_Texture::C_Texture():
	m_name("texture")
{
	m_tileNbr = 0;
}

C_Texture::C_Texture(string name):
	m_name(name)
{
	m_tileNbr = 0;
}

C_Texture::~C_Texture()
{
    SDL_DestroyTexture(m_texture);
}

SDL_Texture* C_Texture::getTexture()
{
	SDL_Texture* ret = nullptr;
	if(m_texture){
	    ret = m_texture;
	}
	return ret;
}


void C_Texture::displayStatus()
{
	C_Message::printM("Tileset Name: " + m_name);
	SDL_Rect pos;
	SDL_QueryTexture(getTexture(), NULL, NULL, &pos.w, &pos.h);
	cout << "size: " << pos.w << ":" << pos.h << endl;
}

void C_Texture::render(int x, int y, double angle, int align, bool zoom)
{
	C_Settings& settings= C_Locator::getSettings();
	if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())) {
		SDL_Rect pos;
		SDL_Texture* texture = getTexture();
		if(texture){
		    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
            if(zoom){
		        int z = settings.getZoom() - 1;
		        if(z != 0){
		            pos.w -= STEP*z;
		            if(pos.h < pos.w){
		                pos.h -= STEP*z/2;
		            } else {
		                pos.h -= STEP*z;
		            }
		        }
            }

		    if(align == CENTER) {
			    pos.x = x - pos.w/2;
			    pos.y = y - pos.h/2;
		    } else if(align == LEFT) {
			    pos.x = x;
			    pos.y = y - pos.h/2;
		    } else if(align == RIGHT) {
			    pos.x = x - pos.w;
			    pos.y = y - pos.h/2;
		    } else if(align == CENTER_TILE) {
			    pos.x = x - pos.w/2;
			    pos.y = y - (11*pos.h)/16;
		    }

		    C_Window& win=C_Locator::getWindow();
		    SDL_RenderCopyEx(win.getRenderer(),texture, NULL, &pos,angle,NULL,SDL_FLIP_NONE);
		}
	}
}

//C_Image

C_Image::C_Image(int tileNbr, string name,
				 SDL_Texture * texture, int tile_width,
				 int tile_height, int file_width,
				 int file_height,
				 std::string sourcefile):
	C_Texture(name)
{
	m_tileNbr = tileNbr;
	m_tile_height = tile_height;
	m_tile_width = tile_width;
	m_file_width = file_width;
	m_file_height =file_height;
	m_sourcefile = sourcefile;
	size_t found = m_name.find("clouds_Cloud");
	if (found!=std::string::npos){
		m_whiteBgrd =  true;
	} else {
		m_whiteBgrd = false;
	}
	loadTexture (texture);
}


void C_Image::displayStatus()
{
	C_Message::printV("Image: " +  to_string(m_tileNbr) + " " + m_name + " " + to_string(m_tile_width)
			 + ":" + to_string(m_tile_height) + " "
			 + to_string(m_file_width) + ":" + to_string(m_file_height)
			 + " from " + m_sourcefile +"\n");
}




void C_Image::loadTexture(SDL_Texture* fullImage)
{
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer ();
	if (m_whiteBgrd){
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);	//fill with a white background color
	} else {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	//fill with a black background color
	}
	SDL_Rect src;
	int rowCount = m_file_width / m_tile_width;
	int rowNbr = m_tileNbr%rowCount;
	int lineNbr = m_tileNbr/rowCount;

	src.x = m_tile_width * rowNbr;
	src.y = m_tile_height * lineNbr;
	src.w = m_tile_width;
	src.h = m_tile_height;

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = m_tile_width;
	dest.h = m_tile_height;

	//load part of the image into the clips

	SDL_Texture * subClip = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,  dest.w, dest.h);

	if (fullImage != nullptr && subClip != nullptr) {
		SDL_SetTextureBlendMode(subClip,SDL_BLENDMODE_BLEND);
		//change target to clip
		SDL_SetRenderTarget(renderer, subClip);

		//clean new renderer before renderCopy. This is important to avoid image glitch.
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, fullImage, &src, &dest);
			// reset target to renderer
		SDL_SetRenderTarget(renderer, NULL);
		//save the clip
		m_texture = subClip;

		SDL_Rect pos;
		SDL_QueryTexture(subClip, NULL, NULL, &pos.w, &pos.h);
		//cout << "W:" << pos.w <<  " H:" << pos.h << endl;
	}

}


//C_Text

C_Text::C_Text(std::string name, std::string message, SDL_Color color, int fontSize):
	C_Texture(name)
{
	m_message = message;
	m_fontSize = fontSize;
	m_color = color;
	createNewTexture();
}


void C_Text::loadTextAsTextures(std::string &message,SDL_Color color, int fontSize)
{
	bool needUpdate = false;
	if(message != m_message){
		needUpdate = true;
		m_message = message;
	}
	if(fontSize != m_fontSize){
		needUpdate = true;
		m_fontSize = fontSize;
	}
	if(m_color.r != color.r || m_color.g != color.g ||m_color.b != color.b ||
	m_color.a != color.a ){
		needUpdate = true;
		m_color = color;
	}
	if(needUpdate){
		createNewTexture();
	}
}

void C_Text::createNewTexture(){
	C_Window& win= C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Surface *surf = nullptr;
	TTF_Font *font = TTF_OpenFont(findFont().c_str(),m_fontSize);

	if (font == nullptr) {
		string error= "TTF_OpenFont open " + findFont() + " failed";
		C_Message::printTTFerror(error);
	} else {
		surf = TTF_RenderText_Blended(font, m_message.c_str(), m_color);
	}

	if (surf == nullptr) {
		TTF_CloseFont(font);
		C_Message::printSDLerror("TTF_RenderText");
	} else {
		SDL_DestroyTexture(m_texture);
		m_texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (m_texture == nullptr) {
			C_Message::printSDLerror("CreateTexture from this text:" + m_message + " failed ");
		}
	}
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}


string C_Text::findFont()
{
	C_Settings& settings= C_Locator::getSettings();
	string font =  settings.getFontPath();
	struct stat buffer;
	if(stat (font.c_str(), &buffer) == 0){
		return font;
	}  else {
		    cout << font +"\n" << endl;
		    return "default";
	}
}
