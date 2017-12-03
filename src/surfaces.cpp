#include "surfaces.h"
#include <SDL2_gfxPrimitives.h>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

using namespace std;

C_TextureList C_TextureList::m_instance=C_TextureList();

//Textures

C_Texture::C_Texture():
	m_id(0),
	m_name("texture"),
	m_file_path("default"),
	m_tile_height(128),
	m_tile_width(128),
	m_file_width(1024),
	m_file_height(1024)
{
	m_texture = nullptr;
}

C_Texture::C_Texture(int id, string name, string file_path, int tile_width, int tile_height, int file_width, int file_height):
	m_id(id),
	m_name(name),
	m_file_path(file_path),
	m_tile_height(tile_height),
	m_tile_width(tile_width),
	m_file_width(file_width),
	m_file_height(file_height)
{
	loadTexture (m_file_path);
}

C_Texture::~C_Texture(){
}

void C_Texture::displayStatus(){
	cout << "Id:" << m_id << ": "<< m_name << " " << m_tile_width << ":" << m_tile_height ;
	cout << " " << m_file_path << " " << m_file_width << ":" << m_file_height  << endl;
}

SDL_Texture* C_Texture::getTexture(){
	return m_texture;
}

void C_Texture::loadTexture(const string &path)
{
	C_Window& win=C_Window::Instances();
	SDL_Renderer* renderer = win.getRenderer ();
	SDL_Texture *texture = nullptr;
	SDL_Surface *image = IMG_Load(path.c_str());
	SDL_Rect src;
	int rowCount = m_file_width / m_tile_width;
	int rowNbr = m_id%rowCount;
	int lineNbr = m_id/rowCount;

	src.x = m_tile_width * rowNbr;
	src.y = m_tile_width * lineNbr;
	src.w = SPRITE_SIZE;
	src.h = SPRITE_SIZE;

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = SPRITE_SIZE;
	dest.h = SPRITE_SIZE;

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
}




//#######################################Texture List##################################################

C_TextureList::C_TextureList()
{
}

C_TextureList::~C_TextureList()
{
}


C_TextureList& C_TextureList::Instances()
{
	return m_instance;
}


void C_TextureList::renderTexture(string name, int x, int y)
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
		pos.y = y;
		SDL_RenderCopy(win.getRenderer(), texture, NULL, &pos);
		}
}


map<string, C_Texture*>  C_TextureList::getTextMap(){
	return m_map_textures;
}



void C_TextureList::loadTexturesIntoMap(){
	int size = 28;
	string file[size] = {"Tower_00_00.png","Tower_01_00.png","boat_01_00.png","boatMoving_01_00.png",
	"boatMoving_01_10.png","boat_01_Dead.png","SimpleTile.png","Grass_01.png","Grass_02.png",
	"SimpleWaterTile.png","Tile_Highlight_Green.png","CrossBow_01.png","Arrow01_South.png",
	"Arrow01_East.png","Arrow01_North.png","Arrow01_West.png","Arrow01_NorthEast.png",
	"Arrow01_NorthWest.png","Arrow01_SouthEast.png","Arrow01_SouthWest.png",
	"smoke_01.png","smoke_02.png","smoke_03.png","smoke_04.png","smoke_05.png","smoke_06.png","smoke_07.png","smoke_08.png",};


	for (int i = 0; i < size; i++){
		string filePath = "data/img/original/" + file[i];
		m_map_textures[file[i]] = new C_Texture(0, file[i], filePath, 128, 128, 128, 128);
	}
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
	int id =0, previousID = 0;
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
		  		id = stoi(reader.get_value());
		  		}
		  	   if (nodeName == "tile" && attributes == "type")
				fullname = name +"_" + reader.get_value();
			  //
			  if (nodeName == "frame" && attributes == "tileid"){
			  	int tmpId = stoi(reader.get_value());
			  	if(tmpId != previousID){
			  		id = tmpId;
					fullname += "_a";
					}
				}
		  	} while(reader.move_to_next_attribute());
		}
	//create new texture
	if(id != previousID){
		previousID = id;
		m_map_textures[fullname] = new C_Texture(id, fullname, filePath, tile_width, tile_height, file_width, file_height );
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



