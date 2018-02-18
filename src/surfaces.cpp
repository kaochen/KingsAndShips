#include "surfaces.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <sys/stat.h>

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
	cout << "Texture Name: " << m_name << endl;
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
	cout << "Id:" << m_id << ": "<< m_name << " " << m_tile_width << ":" << m_tile_height ;
	cout << " " << m_file_path << " " << m_file_width << ":" << m_file_height  << endl;
}



int C_Image::getId(){
	return m_id;
}

void C_Image::loadTexture(string &path)
{
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
			logSDLerror("SDL_CreateTextureFromSurface() failed");
			}
		else{
		SDL_SetTextureBlendMode(clip,SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(clip,255);
		//change target to clip
		SDL_SetRenderTarget(renderer, clip);
	  	SDL_RenderCopy(renderer, texture, &src, &dest);
	  	// reset target to renderer
	  	SDL_SetRenderTarget(renderer, NULL);
		}
	}
	else
	{
		logSDLerror("IMG_LOAD()");
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
	C_Window& win=C_Window::Instances();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Surface *surf = nullptr;
	TTF_Font *font = TTF_OpenFont(findFont().c_str(),fontSize);

	if (font == nullptr){
		logSDLerror("TTF_OpenFont");
		m_texture = nullptr;
		}
	else{
		surf = TTF_RenderText_Blended(font, message.c_str(), color);
	}

	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLerror("TTF_RenderText");
		m_texture = nullptr;
		}
	else{
		m_texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (m_texture == nullptr){
			logSDLerror("CreateTexture");
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
	renderTextureEx(name, x,y,0.0);
}

void C_TextureList::renderTextureEx(string name, int x, int y, double angle)
{
	C_Window& win=C_Window::Instances();
	C_Set& settings=C_Set::Instances();
	SDL_Texture *texture;

        map<string, C_Texture*>::iterator search = m_map_textures.find(name);
	if(search == m_map_textures.end()){
		cout << name << " not available in the texture map" << endl;
		texture = nullptr;
	}
	else{
		texture = m_map_textures[name]->getTexture();
	}

	if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())){
		SDL_Rect pos;
		SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		pos.x = x - pos.w/2;
		pos.y = y - pos.h/2 - (TILE_HALF_HEIGHT*2);
		SDL_RenderCopyEx(win.getRenderer(), texture, NULL, &pos,angle,NULL,SDL_FLIP_NONE);
		}
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



void C_TextureList::loadTexturesIntoMap(){
	int size = 2;
	string file[size] = {"boat_01_Dead.png","Arrow01.png"};

	for (int i = 0; i < size; i++){
		string filePath = "data/img/original/" + file[i];
		m_map_textures[file[i]] = new C_Image((i+1000),0, file[i], filePath, 128, 128, 128, 128);
	}
}

void C_TextureList::loadTextAsTexturesIntoMap(string name, string &message, int fontSize){
		m_map_textures[name] = new C_Text(name,message);
		SDL_Color color = {0,0,0,255};

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


	cout << "Tile Count" << startCount << endl;
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
			  	filePath = reader.get_value();
			  	filePath.replace(0,3,"data/");
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
				cout << name << " not available in the texture map" << endl;
			}
			else{
				m_map_textures[name]->displayStatus();
			}
		}


}


string C_TextureList::getNameFromID(int id){
	string result="error whit getNameFromID";
	for (auto const& x : m_map_textures)
		{
			int idTmp = -1;
		    	string n = x.first;  // string (key)
			map<string, C_Texture*>::iterator search = m_map_textures.find(n);
			if(search == m_map_textures.end()){
				cout << n << " not available in the texture map" << endl;
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


