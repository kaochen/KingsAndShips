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
#include "locator.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <sys/stat.h>

#include "message.h"
using namespace std;

//Textures

C_Texture::C_Texture():
	m_name("texture")
{
	m_texture = nullptr;
}

C_Texture::C_Texture(string name):
	m_name(name)
{
	m_texture = nullptr;
}

C_Texture::~C_Texture()
{
	SDL_DestroyTexture(m_texture);
}

SDL_Texture* C_Texture::getTexture()
{
	return m_texture;
}
void C_Texture::destroyTexture()
{
	SDL_DestroyTexture(m_texture);
	m_texture = nullptr;
}

void C_Texture::displayStatus()
{
	C_Message::printM("Tileset Name: " + m_name +  " " + "\n");
}

void C_Texture::render(int x, int y, double angle, int align)
{
	C_Settings& settings= C_Locator::getSettings();
	if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())) {
		SDL_Rect pos;
		SDL_QueryTexture(m_texture, NULL, NULL, &pos.w, &pos.h);
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
			int i = 1;
			if(pos.h > 128) {
				i = 2;
			}
			pos.y = y - pos.h/2 - i*TILE_HALF_HEIGHT;
		}
		C_Window& win=C_Locator::getWindow();
		SDL_RenderCopyEx(win.getRenderer(),m_texture, NULL, &pos,angle,NULL,SDL_FLIP_NONE);
	}
}

//C_Image

C_Image::C_Image(int id, int tileNbr, string name, string file_path, int tile_width, int tile_height, int file_width, int file_height):
	C_Texture(name)
{
	m_id = id;
	m_tileNbr = tileNbr;
	m_file_path = file_path;
	m_tile_height = tile_height;
	m_tile_width = tile_width;
	m_file_width = file_width;
	m_file_height =file_height;
	loadTexture (m_file_path);
}


void C_Image::displayStatus()
{
	string filename = C_Message::extractFilename(m_file_path);
	C_Message::printV("Image: " + to_string(m_id) + " "+ m_name + " " + to_string(m_tile_width)
			 + ":" + to_string(m_tile_height) + " from: " + filename + " "
			 + to_string(m_file_width) + ":" + to_string(m_file_height)+"\n");
}



int C_Image::getId()
{
	return m_id;
}

void C_Image::loadTexture(string &path)
{
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Surface *image = IMG_Load(path.c_str());
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

	SDL_Texture* clip = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,  src.w, src.h);
	SDL_Texture *texture = nullptr;

	if (image != nullptr && clip != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer,image);
		SDL_FreeSurface(image); //Don't need anymore

		if (texture == nullptr) {
			C_Message::printSDLerror("SDL_CreateTextureFromSurface() failed");
		} else {
			SDL_SetTextureBlendMode(clip,SDL_BLENDMODE_BLEND);
			//change target to clip
			SDL_SetRenderTarget(renderer, clip);
			//clean new renderer before renderCopy. This is important to avoid image glitch.
			size_t found = m_name.find("clouds_Cloud");
  			if (found!=std::string::npos){
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);	//fill with a white background color
			} else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	//fill with a black background color
			}
			SDL_RenderClear(renderer);

			SDL_RenderCopy(renderer, texture, &src, &dest);
			// reset target to renderer
			SDL_SetRenderTarget(renderer, NULL);
		}
	} else {
		C_Message::printSDLerror("IMG_LOAD()");
	}

	m_texture = clip;
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}
}

void C_Image::loadTextAsTextures(std::string &message,SDL_Color color, int fontSize)
{
	cout << "What for ?? " << message << fontSize << color.a << endl;
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
		m_texture = nullptr;
	} else {
		surf = TTF_RenderText_Blended(font, m_message.c_str(), m_color);
	}

	if (surf == nullptr) {
		TTF_CloseFont(font);
		C_Message::printSDLerror("TTF_RenderText");
		m_texture = nullptr;
	} else {
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
	string font =   "/usr/share/fonts/truetype/roboto/hinted/Roboto-Bold.ttf";
	struct stat buffer;
	if(stat (font.c_str(), &buffer) == 0){
		return font;
	} else {
	    font = "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Bold.ttf";
	    if(stat (font.c_str(), &buffer) == 0){
		    return font;
    	} else {
		    cout << "Roboto-Bold.ttf was not found. You should install the fonts-roboto package\n" << endl;
		    return "default";
		}
	}
}

int C_Text::getId()
{
	return 01234567;
}

void C_Text::loadTexture(string &path)
{
	cout << "empty declaration " << path << endl;
}
//#######################################Texture List##################################################

C_TextureList::C_TextureList():
	m_count (0)
{
	C_Message::printM("Constructor C_TextureList() : done\n");
}

C_TextureList::~C_TextureList()
{
	for(map<string, C_Texture*>::iterator it=m_map_textures.begin(); it != m_map_textures.end(); ++it){
		delete it->second;
	}
}


void C_TextureList::renderTexture(string name, int x, int y)
{
	renderTextureEx(name, x,y,0.0,CENTER_TILE);
}

void C_TextureList::renderTexture(string name, int x, int y,int align)
{
	renderTextureEx(name, x,y,0.0,align);
}


void C_TextureList::renderTextureEx(string name, int x, int y, double angle, int align)
{
	if(name != "") {
		map<string, C_Texture*>::iterator search = m_map_textures.find(name);
		if(search == m_map_textures.end()) {
			C_Message::printError("\""+ name + "\" not available in the texture map (renderTextureEx)\n");
		} else {
			m_map_textures[name]->render(x,y,angle,align);
		}
	}
}

C_Texture* C_TextureList::searchTexture(string name)
{
	C_Texture * texture = nullptr;
	if(name != "") {
		map<string, C_Texture*>::iterator search = m_map_textures.find(name);
		if(search == m_map_textures.end()) {
			C_Message::printError("\""+ name + "\" not available in the texture map (searchTexture)\n");
		} else {
			texture = m_map_textures[name];
		}
	}
	return texture;
}



void C_TextureList::renderTextureFromId(int id, int x, int y)
{

	string	name = "notFound";
	//cout << "ID: " << id << endl;
	if (id > 0)
		name = getNameFromID(id);

	if (name !="notFound")
		renderTexture(name, x, y);
}


map<string, C_Texture*>  C_TextureList::getTextMap()
{
	return m_map_textures;
}



void C_TextureList::loadTextAsTexturesIntoMap(string name, string &message, int fontSize, SDL_Color color)
{
	map<string, C_Texture*>::iterator search = m_map_textures.find(name);
	bool exist = false;
	if(search != m_map_textures.end()) {
		if(m_map_textures[name] != nullptr){
			exist = true;
		}
	}
	if(exist)
		m_map_textures[name]->loadTextAsTextures(message, color, fontSize);
	else
		m_map_textures[name] = new C_Text(name,message,color,fontSize);
}

void C_TextureList::freeTexture(string name)
{

	SDL_DestroyTexture(m_map_textures[name]->getTexture());
	m_map_textures.erase(name);
}



struct indent {
	int depth_;
	indent(int depth): depth_(depth) {};
};

ostream & operator<<(ostream & o, indent const & in)
{
	for(int i = 0; i != in.depth_; ++i) {
		o << "  ";
	}
	return o;
}

void C_TextureList::extractTSXfile(string tsx_File_Path)
{

	xmlpp::TextReader reader(tsx_File_Path);
	string filePath = "noFilePath";
	string name = "noName", fullname = name;
	int tile_width = 128, tile_height = 128, file_width = 1024, file_height = 1024;
	int tileNbr = 0, previousTileNbr = -1;
	bool firstID = false;
	int startCount = m_count + 1;


	//	cout << "Tile Count" << startCount << endl;
	while(reader.read()) {
		string nodeName = reader.get_name();
		//cout << nodeName << "---namespace---\n";

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attributes = reader.get_name();
				//cout << attributes << "-----"<< endl;
				//tileset node
				if (nodeName == "tileset" && attributes == "name")
					name = reader.get_value();
				if (nodeName == "tileset" && attributes == "tilewidth")
					tile_width = stoi(reader.get_value());
				if (nodeName == "tileset" && attributes == "tileheight")
					tile_height = stoi(reader.get_value());

				//image node
				if (nodeName == "image" && attributes == "source") {
					C_Settings& settings= C_Locator::getSettings();
					string tmp = reader.get_value();
					tmp.replace(0,8,""); //drop default theme "original"
					filePath = settings.getThemePath() + tmp;
				}
				if (nodeName == "image" && attributes == "width") {
					file_width = stoi(reader.get_value());
				}
				if (nodeName == "image" && attributes == "height") {
					file_height = stoi(reader.get_value());
				}

				//tile node
				if (nodeName == "tile" && attributes == "id") {
					tileNbr = stoi(reader.get_value());
					firstID = true;
					//cout << id << "<--" << endl;
				}
				if (nodeName == "tile" && attributes == "type")
					fullname = name +"_" + reader.get_value();
				//

			} while(reader.move_to_next_attribute());
		}
		//create new texture
		if(tileNbr != previousTileNbr && firstID == true) {
			previousTileNbr = tileNbr;
			int id = tileNbr + startCount;
			map<string, C_Texture*>::iterator search = m_map_textures.find(fullname);
			if(search == m_map_textures.end()) {
				m_map_textures[fullname] = new C_Image(id,tileNbr,fullname, filePath, tile_width, tile_height, file_width, file_height );
				m_count++;
				//cout << m_count << ": " << fullname << "Size: " << tile_width <<":"<< tile_height<< endl;
			}
		}

		reader.move_to_element();
	}
}

void C_TextureList::displayTexturesList()
{

	for (auto const& x : m_map_textures) {
		m_map_textures[x.first]->displayStatus();

	}
}


string C_TextureList::getNameFromID(int id)
{
	string result="error with getNameFromID: \"" + to_string(id) + "\"";
	for (auto const& x : m_map_textures) {
		int idTmp = -1;
		string n = x.first;  // string (key)
		map<string, C_Texture*>::iterator search = m_map_textures.find(n);
		if(search == m_map_textures.end()) {
			C_Message::printM("\""+ n + "\" not available in the texture map  (getNameFromID)\n");
			result = "notFound";
		} else {
			idTmp = m_map_textures[n]->getId();
			if (idTmp == id) {
				result = n;
			}
		}

	}
	return result;
}


