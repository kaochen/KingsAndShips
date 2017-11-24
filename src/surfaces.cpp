#include "surfaces.h"
#include <SDL2_gfxPrimitives.h>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

using namespace std;


map<string, SDL_Texture*> C_Texture::map_textures;


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


SDL_Texture* loadTexture(const string &path, SDL_Renderer *renderer)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *image = IMG_Load(path.c_str());
	if (image != nullptr)
		{
		texture = SDL_CreateTextureFromSurface(renderer,image);
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
	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
	C_Set& settings=C_Set::Instances();
	if((x >= 0 || x <= settings.getWindowWidth()) && ( y >= 0  || y <= settings.getWindowHeight())){
		SDL_Rect pos;
		SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		pos.x = x - pos.w/2;
		pos.y = y;
		SDL_RenderCopy(renderer, texture, NULL, &pos);
		}
}

map<string, SDL_Texture*>  C_Texture::getTextMap(){
	return map_textures;
}

SDL_Texture* C_Texture::getText(string name){
	map<string, SDL_Texture*> t = C_Texture::getTextMap();
	map<string, SDL_Texture*>::iterator search = t.find(name);
	if(search == t.end()){
		cout << name << " not available in the texture map" << endl;
		return nullptr;
	}
	else{
		return t[name];
	}
}

void C_Texture::loadTexturesIntoMap(SDL_Renderer *renderer){
	C_Texture::map_textures = C_Texture::getTextMap();
	map_textures["SetupBackground.png"] = loadTexture("src/img/SetupBackground.png", renderer);
	map_textures["Tower_00_00.png"] = loadTexture("data/img/original/Tower_00_00.png", renderer);
	map_textures["Tower_01_00.png"] = loadTexture("data/img/original/Tower_01_00.png", renderer);
	map_textures["boat_01_00.png"] = loadTexture("data/img/original/boat_01_00.png", renderer);
	map_textures["boatMoving_01_00.png"] = loadTexture("data/img/original/boatMoving_01_00.png", renderer);
	map_textures["boatMoving_01_10.png"] = loadTexture("data/img/original/boatMoving_01_10.png", renderer);
	map_textures["boat_01_Dead.png"] = loadTexture("data/img/original/boat_01_Dead.png", renderer);
	map_textures["SimpleTile.png"] = loadTexture("data/img/original/SimpleTile.png", renderer);
	map_textures["Grass_01.png"] = loadTexture("data/img/original/Grass_01.png", renderer);
	map_textures["Grass_02.png"] = loadTexture("data/img/original/Grass_02.png", renderer);
	map_textures["SimpleWaterTile.png"] = loadTexture("data/img/original/SimpleWaterTile.png", renderer);
	map_textures["Tile_Highlight_Green.png"] = loadTexture("data/img/original/Tile_Highlight_Green.png", renderer);
	map_textures["CrossBow_01.png"] = loadTexture("data/img/original/CrossBow_01.png", renderer);
	map_textures["Arrow01_South.png"] = loadTexture("data/img/original/Arrow01_South.png", renderer);
	map_textures["Arrow01_East.png"] = loadTexture("data/img/original/Arrow01_East.png", renderer);
	map_textures["Arrow01_North.png"] = loadTexture("data/img/original/Arrow01_North.png", renderer);
	map_textures["Arrow01_West.png"] = loadTexture("data/img/original/Arrow01_West.png", renderer);
	map_textures["Arrow01_NorthEast.png"] = loadTexture("data/img/original/Arrow01_NorthEast.png", renderer);
	map_textures["Arrow01_NorthWest.png"] = loadTexture("data/img/original/Arrow01_NorthWest.png", renderer);
	map_textures["Arrow01_SouthEast.png"] = loadTexture("data/img/original/Arrow01_SouthEast.png", renderer);
	map_textures["Arrow01_SouthWest.png"] = loadTexture("data/img/original/Arrow01_SouthWest.png", renderer);
	// smoke
	map_textures["smoke_01.png"] = loadTexture("data/img/original/smoke_01.png", renderer);
	map_textures["smoke_02.png"] = loadTexture("data/img/original/smoke_02.png", renderer);
	map_textures["smoke_03.png"] = loadTexture("data/img/original/smoke_03.png", renderer);
	map_textures["smoke_04.png"] = loadTexture("data/img/original/smoke_04.png", renderer);
	map_textures["smoke_05.png"] = loadTexture("data/img/original/smoke_05.png", renderer);
	map_textures["smoke_06.png"] = loadTexture("data/img/original/smoke_06.png", renderer);
	map_textures["smoke_07.png"] = loadTexture("data/img/original/smoke_07.png", renderer);
	map_textures["smoke_08.png"] = loadTexture("data/img/original/smoke_08.png", renderer);
}

//displayContent of the grid
void displayGridContent(SDL_Renderer *renderer,
			C_GameUnits::S_layer grid[][GRID_SIZE]){
	C_Set& settings=C_Set::Instances();
	int x_start = 0, y_start = 13;
	//int x_end = settings.getGridSize();
	int lineCount = 0, tileCount = 0;
	for (int l = 0; l < 25; l++){
		int y = y_start;
		int x = x_start;
		for (int r = 0 ; r < 17; r++){
				cout << "|" << x << ":"<< y;
				if (grid[x][y].main != nullptr){
					int x_screen = grid[x][y].main->getXScreen();
					int y_screen = grid[x][y].main->getYScreen();
					grid[x][y].main->render(x_screen, y_screen, renderer);
				}
				y++;
				x++;
				tileCount++;
		}
		lineCount++;
		cout << endl;
		if (lineCount %2 == 0)
			y_start--;
		else
			x_start++;

		cout << "line: "<< lineCount <<" tile: " << tileCount << endl;
	}
}

void drawElipse(SDL_Renderer *renderer,
		int x,
		int y,
		int width){
		int height = width/2;
		ellipseRGBA(renderer,x,y,width+1,height+1,0,200,0,128);
		filledEllipseRGBA(renderer,x,y,100,50,0,200,0,32);
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

void C_Texture::extractTSXfile(vector <C_Texture*>& list)
{

 xmlpp::TextReader reader("data/levels/boat_01.tsx");
	string filePath = "noFilePath";
	string name = "noName", fullname = name;
	int tilewidth= 0;
	int tileheight= 0;
	int id =0, previousID = 0;
    while(reader.read())
    {
    		string nodeName = reader.get_name();
	      	cout << nodeName << "---namespace---\n";

	      	if (reader.has_attributes()){
			reader.move_to_first_attribute();
			do
			{
			  string attributes = reader.get_name();
			  cout << attributes << "-----"<< endl;
			  //tileset node
			  if (nodeName == "tileset" && attributes == "name")
			  	name = reader.get_value();
			  if (nodeName == "tileset" && attributes == "tilewidth")
		  		tilewidth = stoi(reader.get_value());
		  	  if (nodeName == "tileset" && attributes == "tileheight")
		  		tileheight = stoi(reader.get_value());

			  //image node
			  if (nodeName == "image" && attributes == "source")
			  	filePath = reader.get_value();

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
		list.push_back(new C_Texture(id, fullname ,filePath, tilewidth, tileheight));
	}

	reader.move_to_element();
    }
}


void C_Texture::displayTexturesList(vector <C_Texture*>& list){
	for (size_t i = 0; i < list.size(); i++){
		list[i]->displayStatus();
	}
}
