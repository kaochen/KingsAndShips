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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <sys/stat.h>

#include "message.h"
using namespace std;

C_TextureList C_TextureList::m_instance=C_TextureList();

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

C_Texture::~C_Texture(){
}

SDL_Texture* C_Texture::getTexture(){
	return m_texture;
}
void C_Texture::destroyTexture(){
	SDL_DestroyTexture(m_texture);
	m_texture = nullptr;
}

void C_Texture::displayStatus(){
    C_Message m;
    m.printM("Tileset Name: " + m_name +  " " + "\n");
}

void C_Texture::render(int x, int y, double angle, int align){
    C_Window& win=C_Window::Instances();
	C_Settings& settings=C_Settings::Instances();
    if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())){
			SDL_Rect pos;
			SDL_QueryTexture(m_texture, NULL, NULL, &pos.w, &pos.h);
			if(align == CENTER){
			    pos.x = x - pos.w/2;
			    pos.y = y - pos.h/2;
			    }
			else if(align == LEFT){
			pos.x = x;
			pos.y = y - pos.h/2;
			}
			else if(align == RIGHT){
			pos.x = x - pos.w;
			pos.y = y - pos.h/2;
			}
			else if(align == CENTER_TILE){
			pos.x = x - pos.w/2;
			int i = 1;
			if(pos.h > 128){i = 2;}
			pos.y = y - pos.h/2 - i*TILE_HALF_HEIGHT;
			}

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


void C_Image::displayStatus(){
    C_Message m;
    m.printM("Image: " + to_string(m_id) + " "+ m_name + " " + to_string(m_tile_width)
            + ":" + to_string(m_tile_height) + " from: " + m_file_path + " "
             + to_string(m_file_width) + ":" + to_string(m_file_height)+"\n");
}



int C_Image::getId(){
	return m_id;
}

void C_Image::loadTexture(string &path)
{
	C_Message m;
	C_Window& win=C_Window::Instances();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Texture *texture = nullptr;
	SDL_Surface *image = IMG_Load(path.c_str());
	SDL_Rect src;
	int rowCount = m_file_width / m_tile_width;
	int rowNbr = m_tileNbr%rowCount;
	int lineNbr = m_tileNbr/rowCount;

	src.x = m_tile_width * rowNbr;
	src.y = m_tile_width * lineNbr;
	src.w = m_tile_width;
	src.h = m_tile_height;

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = m_tile_width;
	dest.h = m_tile_height;

	//SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND); //usefull ?
	//SDL_SetTextureAlphaMod(texture,0); //usefull ?

	SDL_Texture* clip = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,  src.w, src.h);

	if (image != nullptr && clip != nullptr)
		{
		texture = SDL_CreateTextureFromSurface(renderer,image);
		SDL_FreeSurface(image); //Don't need anymore

		if (texture == nullptr)
			{
			m.printSDLerror("SDL_CreateTextureFromSurface() failed");
			}
		else{
		SDL_SetTextureBlendMode(clip,SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(clip,255);
		//change target to clip
		SDL_SetRenderTarget(renderer, clip);
		//clean new renderer before renderCopy. This is important to avoid image glitch.
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	//fill with background color
		SDL_RenderClear(renderer);

	  	SDL_RenderCopy(renderer, texture, &src, &dest);
	  	// reset target to renderer
	  	SDL_SetRenderTarget(renderer, NULL);
		}
	}
	else
	{
		m.printSDLerror("IMG_LOAD()");
	}

	m_texture = clip;
	if (texture != nullptr){
		SDL_DestroyTexture(texture);
		}
}

void C_Image::loadTextAsTextures(std::string &message,SDL_Color color, int fontSize)
{
	cout << "What for ?? " << message << fontSize << color.a << endl;
}

//C_Text

C_Text::C_Text(string name, string message):
	C_Texture(name)
{
	m_message = message;
}


void C_Text::loadTextAsTextures(std::string &message,SDL_Color color, int fontSize)
{
    C_Message m;
	C_Window& win=C_Window::Instances();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Surface *surf = nullptr;
	TTF_Font *font = TTF_OpenFont(findFont().c_str(),fontSize);

	if (font == nullptr){
	    string error= "TTF_OpenFont open " + findFont() + " failed";
		m.printTTFerror(error);
		m_texture = nullptr;
		}
	else{
		surf = TTF_RenderText_Blended(font, message.c_str(), color);
	}

	if (surf == nullptr){
		TTF_CloseFont(font);
		m.printSDLerror("TTF_RenderText");
		m_texture = nullptr;
		}
	else{
		m_texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (m_texture == nullptr){
    		m.printSDLerror("CreateTexture from this text:" + m_message + " failed ");
		}
	}
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}


string C_Text::findFont()
{
	string font =   "/usr/share/fonts/truetype/roboto/hinted/Roboto-Bold.ttf";
	struct stat buffer;
 		 if(stat (font.c_str(), &buffer) == 0)
			return font;
		 else{
		 	cout << "Roboto-Bold.ttf was not found. You should install the fonts-roboto package\n" << endl;
			return "default";
		 }
}

int C_Text::getId(){
	return 01234567;
}

void C_Text::loadTexture(string &path){
	cout << "empty declaration " << path << endl;
}
//#######################################Texture List##################################################

C_TextureList::C_TextureList():
		m_count (0)
{
}

C_TextureList::~C_TextureList()
{
}


C_TextureList& C_TextureList::Instances()
{
	return m_instance;
}


void C_TextureList::renderTexture(string name, int x, int y){
	renderTextureEx(name, x,y,0.0,CENTER_TILE);
}

void C_TextureList::renderTexture(string name, int x, int y,int align){
	renderTextureEx(name, x,y,0.0,align);
}


void C_TextureList::renderTextureEx(string name, int x, int y, double angle, int align)
{
	if(name != ""){
		map<string, C_Texture*>::iterator search = m_map_textures.find(name);
		if(search == m_map_textures.end()){
		    C_Message m;
			m.printError("\""+ name + "\" not available in the texture map (renderTextureEx)\n");
		}
		else{
			m_map_textures[name]->render(x,y,angle,align);
		}
	}
}

C_Texture* C_TextureList::searchTexture(string name)
{
    C_Texture * texture = nullptr;
	if(name != ""){
		map<string, C_Texture*>::iterator search = m_map_textures.find(name);
		if(search == m_map_textures.end()){
			C_Message m;
			m.printError("\""+ name + "\" not available in the texture map (searchTexture)\n");
		}
		else{
			texture = m_map_textures[name];
		}
	}
	return texture;
}



void C_TextureList::renderTextureFromId(int id, int x, int y){

	string	name = "notFound";
	//cout << "ID: " << id << endl;
	if (id > 0)
	  name = getNameFromID(id);

	if (name !="notFound")
		renderTexture(name, x, y);
}


map<string, C_Texture*>  C_TextureList::getTextMap(){
	return m_map_textures;
}



void C_TextureList::loadTextAsTexturesIntoMap(string name, string &message, int fontSize, SDL_Color color){
		m_map_textures[name] = new C_Text(name,message);
		m_map_textures[name]->loadTextAsTextures(message, color, fontSize);
}

void C_TextureList::freeTexture(string name){

		SDL_DestroyTexture(m_map_textures[name]->getTexture());
		m_map_textures.erase(name);
}



struct indent {
  int depth_;
  indent(int depth): depth_(depth) {};
};

ostream & operator<<(ostream & o, indent const & in)
{
  for(int i = 0; i != in.depth_; ++i)
  {
    o << "  ";
  }
  return o;
}

void C_TextureList::extractTSXfile(string tsx_File_Path)
{

 xmlpp::TextReader reader(tsx_File_Path);
	string filePath = "noFilePath";
	string name = "noName", fullname = name;
	int tile_width = 128 , tile_height = 128, file_width = 1024, file_height = 1024;
	int tileNbr = 0, previousTileNbr = -1;
	bool firstID = false;
	int startCount = m_count + 1;


    //	cout << "Tile Count" << startCount << endl;
    while(reader.read())
    {
    		string nodeName = reader.get_name();
	      	//cout << nodeName << "---namespace---\n";

	      	if (reader.has_attributes()){
			reader.move_to_first_attribute();
			do
			{
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
			  if (nodeName == "image" && attributes == "source"){
			  	C_Settings& settings=C_Settings::Instances();
			    string tmp = reader.get_value();
			    tmp.replace(0,8,""); //drop default theme "original"
			  	filePath = settings.getThemePath() + tmp;
			  	}
			 if (nodeName == "image" && attributes == "width"){
			  	file_width = stoi(reader.get_value());
			  	}
			  if (nodeName == "image" && attributes == "height"){
			  	file_height = stoi(reader.get_value());
			  	}

			  //tile node
			   if (nodeName == "tile" && attributes == "id"){
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
	if(tileNbr != previousTileNbr && firstID == true){
		previousTileNbr = tileNbr;
		int id = tileNbr + startCount;
			map<string, C_Texture*>::iterator search = m_map_textures.find(fullname);
			if(search == m_map_textures.end()){
				m_map_textures[fullname] = new C_Image(id,tileNbr,fullname, filePath, tile_width, tile_height, file_width, file_height );
				m_count++;
				//cout << m_count << ": " << fullname << endl;
			}
	}

	reader.move_to_element();
    }
}

void C_TextureList::displayTexturesList(){

		for (auto const& x : m_map_textures)
		{

		    	string name = x.first;  // string (key)
			map<string, C_Texture*>::iterator search = m_map_textures.find(name);
			if(search == m_map_textures.end()){
			    C_Message m;
			    m.printM("\""+ name + "\" not available in the texture map  (displayTexturesList)\n");
			}
			else{
				m_map_textures[name]->displayStatus();
			}
		}


}


string C_TextureList::getNameFromID(int id){
	string result="error with getNameFromID: \"" + to_string(id) + "\"";
	for (auto const& x : m_map_textures)
		{
			int idTmp = -1;
		    	string n = x.first;  // string (key)
			map<string, C_Texture*>::iterator search = m_map_textures.find(n);
			if(search == m_map_textures.end()){
			    C_Message m;
			    m.printM("\""+ n + "\" not available in the texture map  (getNameFromID)\n");
				result = "notFound";
			}
			else{
				idTmp = m_map_textures[n]->getId();
				if (idTmp == id){
					result = n;
				}
			}

		}
	return result;
}


