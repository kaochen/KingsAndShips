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


#include "textureList.h"
#include "tools.h"
#include "locator.h"
#include "settings.h"


#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <sys/stat.h>

#include <algorithm>
#include <cctype>

#include "message.h"
using namespace std;

//#######################################Texture List##################################################

C_TextureList::C_TextureList()
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
	renderTextureEx(name, x,y,0.0,CENTER_TILE,false);
}

void C_TextureList::renderTexture(string name, int x, int y,int align)
{
	renderTextureEx(name, x,y,0.0,align,false);
}

void C_TextureList::renderTexture(string name, int x, int y,int align, bool zoom)
{
	renderTextureEx(name, x,y,0.0,align,zoom);
}


void C_TextureList::renderTextureEx(string name, int x, int y, double angle, int align, bool zoom)
{
	if(name != "") {
		map<string, C_Texture*>::iterator search = m_map_textures.find(name);
		if(search == m_map_textures.end()) {
			C_Message::printError("\""+ name + "\" not available in the texture map (renderTextureEx)\n");
		} else {
			m_map_textures[name]->render(x,y,angle,align,zoom);
		}
	}
}

void C_TextureList::renderText(std::string name, int x, int y,int align){
    renderTexture(name, x, y,align);
    renderTexture(name, x+1, y,align); //the promocyja-webfont is too thin, this simulate a bold version
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
	std::string str = message;
	if(str.empty()){
	    str = " "; //SDL cannot render text without width
	}
	if(exist)
		m_map_textures[name]->loadTextAsTextures(str, color, fontSize);
	else
		m_map_textures[name] = new C_TextAsTexture(name,str,color,fontSize);
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

	//Get general values :
	while(reader.read()) {
		string nodeName = reader.get_name();
		//cout << nodeName << "---namespace---\n";

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attributes = reader.get_name();
				//cout << attributes << "-----"<< endl;
				//tileset node
				if (nodeName == "tileset" && attributes == "name"){
					name = reader.get_value();
					}
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
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
	}

	SDL_Texture* texture = imageToTexture(filePath);

	xmlpp::TextReader reader2(tsx_File_Path);
	while(reader2.read()) {
		string nodeName = reader2.get_name();
		//cout << nodeName << "---namespace---\n";

		if (reader2.has_attributes()) {
			reader2.move_to_first_attribute();
			do {
				string attributes = reader2.get_name();
				//cout << attributes << "-----"<< endl;

				//tile node
				if (nodeName == "tile" && attributes == "id") {
					tileNbr = stoi(reader2.get_value());
					firstID = true;
					//cout << id << "<--" << endl;
				}
				if (nodeName == "tile" && attributes == "type")
					fullname = name +"_" + reader2.get_value();
				//

			} while(reader2.move_to_next_attribute());
		}
		//create new texture
		if(tileNbr != previousTileNbr && firstID == true) {
			previousTileNbr = tileNbr;
			map<string, C_Texture*>::iterator search = m_map_textures.find(fullname);
			if(search == m_map_textures.end()) {
			    string filename = C_Tools::extractFilename(tsx_File_Path);
				m_map_textures[fullname] = new C_Image(tileNbr,fullname, texture, tile_width, tile_height, file_width, file_height, filename );
			}
		}

		reader2.move_to_element();
	}

	//erase the original textures FIXME should be done outside this function in order to load image just one time
	if (texture == nullptr) {
		SDL_DestroyTexture(texture); //Don't need anymore
	}
}

void C_TextureList::displayTexturesList()
{

	for (auto const& x : m_map_textures) {
		m_map_textures[x.first]->displayStatus();

	}
}

std::string C_TextureList::getNameFromID(int nbr, std::string tsxName)
{
	string result="error with getNameFromID: \"" + to_string(nbr) + " from " + tsxName + "\"";
	for (auto const& x : m_map_textures) {
		string n = x.first;  // string (key)
		map<string, C_Texture*>::iterator search = m_map_textures.find(n);
		if(search == m_map_textures.end()) {
			C_Message::printM("\""+ n + "\" not available in the texture map  (getNameFromID)\n");
			result = "notFound";
		} else {
			int tileNbr = m_map_textures[n]->getTileNbr();
			string sourcefile = m_map_textures[n]->getSourceFileName();
			if (tileNbr  == nbr && sourcefile == tsxName ) {
				result = n;
			}
		}

	}
	return result;
}

SDL_Texture* C_TextureList::imageToTexture(std::string &path){
	SDL_Texture *ret = nullptr;
	C_Window& win=C_Locator::getWindow();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Surface *image = IMG_Load(path.c_str());
	if(image != nullptr){
		ret = SDL_CreateTextureFromSurface(renderer,image);
		if (ret == nullptr) {
			C_Message::printSDLerror("SDL_CreateTextureFromSurface() failed for:" + path);
		}
		SDL_FreeSurface(image); //Don't need anymore
	} else {
		C_Message::printSDLerror("IMG_LOAD()");
	}
	return ret;
}
