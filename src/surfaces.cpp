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
	m_filePath("default"),
	m_tile_height(128),
	m_tile_width(128)
{
}

C_Texture::C_Texture(int id, string name, string filePath, int tile_height, int tile_width):
	m_id(id),
	m_name(name),
	m_filePath(filePath),
	m_tile_height(tile_height),
	m_tile_width(tile_width)
{
}

C_Texture::~C_Texture(){
}

void C_Texture::displayStatus(){
	cout << "Id:" << m_id << ": "<< m_name << " " << m_filePath;
	cout << " " << m_tile_width << ":" << m_tile_height << endl;
}


SDL_Texture* C_Texture::loadTexture(const string &path)
{
	C_Window& win=C_Window::Instances();
	SDL_Texture *texture = nullptr;
	SDL_Surface *image = IMG_Load(path.c_str());
	if (image != nullptr)
		{
		texture = SDL_CreateTextureFromSurface(win.getRenderer(),image);
		SDL_FreeSurface(image); //Don't need anymore

		if (texture == nullptr)
			{
			logSDLerror("SDL_CreateTextureFromSurface() failed");
			}
		}
	else
	{
		logSDLerror("IMG_LOAD()");
	}
	//Clipping
	SDL_Rect src;
  	src.x = 0;
  	src.y = 0;
  	src.w = m_tile_width;
  	src.h = m_tile_height;

	SDL_Rect dest;
	src.x = 0;
  	src.y = 0;
  	src.w = src.w;
  	src.h = src.h;

	SDL_RenderCopy(win.getRenderer(), texture, &src, &dest);
	return texture;
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

        map<string, SDL_Texture*>::iterator search = m_map_textures.find(name);

	if(search == m_map_textures.end()){
		cout << name << " not available in the texture map" << endl;
		texture = nullptr;
	}
	else{
		texture = m_map_textures[name];
	}

	if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())){
		SDL_Rect pos;
		SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		pos.x = x - pos.w/2;
		pos.y = y;
		SDL_RenderCopy(win.getRenderer(), texture, NULL, &pos);
		}
}


map<string, SDL_Texture*>  C_TextureList::getTextMap(){
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

	C_Texture t;
	for (int i = 0; i < size; i++){
		string path = "data/img/original/" + file[i];
		m_map_textures[file[i]] = t.loadTexture(path);
	}
}


void drawElipse(int x,
		int y,
		int width){
		C_Window& win=C_Window::Instances();
		int height = width/2;
		ellipseRGBA(win.getRenderer(),x,y,width+1,height+1,0,200,0,128);
		filledEllipseRGBA(win.getRenderer(),x,y,100,50,0,200,0,32);
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
	int tilewidth= 0;
	int tileheight= 0;
	int id =0, previousID = 0;
	C_Texture t;
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
		  		tilewidth = stoi(reader.get_value());
		  	  if (nodeName == "tileset" && attributes == "tileheight")
		  		tileheight = stoi(reader.get_value());

			  //image node
			  if (nodeName == "image" && attributes == "source"){
			  	filePath = reader.get_value();
			  	filePath.replace(0,3,"data/");
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
		m_map_textures[fullname] = t.loadTexture(filePath);
	}

	reader.move_to_element();
    }
}

void C_TextureList::displayTexturesList(){
		cout << "Image names :" << endl;
		int i = 0;
		for (auto const& x : m_map_textures)
		{
		    i++;
		    std::cout << x.first  // string (key)
			      << " // ";
		    if (i%4 == 0)
			cout << endl ;
		}
		cout << endl ;
}



