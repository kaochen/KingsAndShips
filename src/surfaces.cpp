#include "surfaces.h"
#include <SDL2_gfxPrimitives.h>

using namespace std;


map<string, SDL_Texture*> C_Texture::map_textures;


C_Texture::C_Texture():
	m_name("texture"),
	m_seqNbr(1)
{
}

C_Texture::C_Texture(string name, int seqNbr):
	m_name(name),
	m_seqNbr(seqNbr)
{
}

C_Texture::~C_Texture(){
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
	for (int y = 0; y < settings.getGridSize(); y++){
		for (int x = 0; x < settings.getGridSize(); x++){
			if (grid[x][y].main != nullptr){
				int x_screen = grid[x][y].main->getXScreen();
				int y_screen = grid[x][y].main->getYScreen();
				grid[x][y].main->render(x_screen, y_screen, renderer);
				}
		}
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
