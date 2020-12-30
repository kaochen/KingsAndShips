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


#include <sys/stat.h>
#include <sstream>

#include "gameUnits.h"
#include "boat.h"
#include "level.h"
#include "grid.h"
#include "../locator.h"
#include "../wallet.h"
#include "../message.h"
#include "../menu/menu.h"

using namespace std;


C_Level::C_Level(S_LevelModel model):
	m_id(model.nbr),
	m_levelName(model.name),
	m_width(model.width),
	m_height(model.height),
	m_gridSize(model.gridSize),
	m_tilewidth(model.tilewidth),
	m_tileheight(model.tileheight),
	m_backgroundcolor(model.backgroundcolor),
	m_levelStatus(ONGOING)
{
	m_count = ++m_id;
	C_Settings& settings=C_Locator::getSettings();
	m_filename = settings.getLevelFolder() + "Level_" + to_string(model.nbr) + ".tmx";
	struct stat buffer;
	if (stat (m_filename.c_str(),  &buffer) == 0) {
	    m_tmx = new C_Tmx(m_filename);
		m_decorLayer = m_tmx->extractLayerInTMX("Decors");

		m_nbrOfWaves = 0;
		m_currentWaveNbr = -1;
		m_lastWaveTime = SDL_GetTicks();
		m_landscape = nullptr;
	} else {
		C_Message::printM("Can not find " + m_filename+"\n");
	}
}


C_Level::~C_Level()
{
	delete m_landscape;
	delete m_tmx;
}

void C_Level::load(int levelNbr)
{
	//clean before loading
	C_Grid& grid= C_Locator::getGrid();
	grid.reset(m_gridSize);
	m_levelStatus = ONGOING;

	struct stat buffer;
	if (stat (m_filename.c_str(),  &buffer) == 0) {

		loadLayerIntoTheGrid("Ground");
		loadLayerIntoTheGrid("Decors");

		setWallet();
		C_Xml tmx(m_filename);
		m_nbrOfWaves = tmx.countAttributes("Wave");
		for(int i = 0; i < m_nbrOfWaves; i++) {
			loadWave(m_filename.c_str(),i);
		}
		updateMenuInfo();


		createLandscape();
		C_Message::printM("Level " + to_string(levelNbr) +" Loaded\n");
	} else {
		C_Message::printM("Can not find " + m_filename+"\n");
		C_Message::printM("Can not load level " + to_string(levelNbr)+"\n");
	}
}

void C_Level::setWallet()
{
	C_Wallet& wallet= C_Locator::getWallet();
	C_Xml tmx(m_filename);
	int walletCredit =  stoi(tmx.extractStrValue("property","name","wallet","value"));
	if(walletCredit < 1) {
		walletCredit = 500;
	}
	wallet.reset();
	wallet.credit(walletCredit); //add a credit for start
}

void C_Level::createLandscape()
{
	C_Grid& grid= C_Locator::getGrid();
	C_CoordGrid tmp(grid.foundTown());
	S_Coord town = tmp.getScreen();
	m_landscape = new C_Landscape(town);
}

void C_Level::sendNextWave()
{
	m_currentWaveNbr++;
	if(m_currentWaveNbr < m_nbrOfWaves && m_currentWaveNbr >= 0) {
		cliWaveStatus(m_currentWaveNbr);
		loadWaveIntoGrid(m_currentWaveNbr);
		C_Message::printM("Next wave: " + to_string(m_currentWaveNbr)+"\n");
	} else if(m_currentWaveNbr > m_nbrOfWaves) {
		m_currentWaveNbr--;
	}
}


void C_Level::loadLayerIntoTheGrid(std::string type)
{
	C_Grid& grid= C_Locator::getGrid();
	S_tmxLayer	layer = m_tmx->extractLayerInTMX(type);
	string data = layer.data;
	S_Coord start = getFirstTile(layer);
	for (int y = start.y; y < layer.height; y++) {
		for (int x = start.x; x < layer.width; x++) {
			string extract = data;
			int mark = extract.find_first_of(',');
			if (mark > 0)
				extract.resize(mark,'C');
			int nbr = stoi(extract);
			S_Unit unit;
		    unit.coord = {x,y};
			//cout << nbr;
			if(type == "Ground"){
			    if(nbr == 0) {
			        unit.name = "Water";
				    grid.setGround(unit);
			    } else {
			        S_Tile tile = m_tmx->getTileInfos(nbr);
			        unit.name = tile.name;
			        grid.setGround(unit);
			    }
			} else {
			    if(nbr != 0){
			    	S_Tile tile = m_tmx->getTileInfos(nbr);
			    	unit.name = tile.name;
			        grid.setGround(unit);
			    }
			}
    			data = data.substr(mark + 1);
		}
	}
}

S_Coord C_Level::getFirstTile(S_tmxLayer &layer)
{
	C_Grid& grid= C_Locator::getGrid();
	int x = 0;
	int y = 0;
	if(grid.getUsefullSize()-layer.width>1) {
		x = (grid.getUsefullSize() -layer.width)/2;
	}
	if(grid.getUsefullSize()-layer.height>1) {
		y = (grid.getUsefullSize() -layer.height)/2;
	}
	S_Coord first{x,y};
	return first;
}

void C_Level::loadWave(string tmx_File_Path, int waveNbr)
{
	C_Wave wave;
	string name = "Wave" + to_string(waveNbr);
	C_Tmx tmx(tmx_File_Path);
	S_tmxLayer layer = tmx.extractLayerInTMX(name);
	string data = layer.data;
	for (int y = 0; y < layer.height; y++) {
		for (int x = 0; x < layer.width; x++) {
			string extract = data;
			int mark = extract.find_first_of(',');
			if (mark > 0)
				extract.resize(mark,'C');
			int nbr = stoi(extract);
			if (nbr!=0) {
				S_Tile tile = m_tmx->getTileInfos(nbr);
				C_Message::printDebug(to_string(x) + ":" + to_string(y) + "->" + tile.name + "\n") ;
				S_Coord pos = {x,y};
				wave.add(tile.name,pos);
			}
			data = data.substr(mark + 1);
		}
	}
	//cout << endl;
	m_waves.push_back(wave);
}

void C_Level::cliWaveStatus(int i)
{
	int c = 0;
	for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end(); it++) {
		C_Wave wave = *it;
		if(i == c) {
			wave.cliStatus();
		}
		c++;
	}
	C_Message::printM("Number of wave for this level: " + to_string(c) +"\n");
}

void C_Level::loadWaveIntoGrid(int i)
{
	int c = 0;
	for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end(); it++) {
		C_Wave wave = *it;
		if(i == c) {
			C_Message::printM("load wave: " + to_string(c) + "\n");
			wave.loadIntoGrid();
		}
		c++;
	}
	updateMenuInfo();
}


void C_Level::updateMenuInfo()
{
	C_Menu& menu=C_Locator::getMenu();
	menu.updateLevelInfos(m_nbrOfWaves - m_currentWaveNbr, m_nbrOfWaves);
}

void C_Level::render()
{
	C_Grid& grid= C_Locator::getGrid();
	m_landscape->render(grid.getUsefullSize());
	m_landscape->renderTopMask(grid.getUsefullSize());
	grid.renderLayer (GRAVEYARD);
	grid.renderLayer (GROUND);
	grid.renderLayer (FIELD);
	grid.renderLayer (WEAPONS);
	m_landscape->renderBottomMask(grid.getUsefullSize());
	grid.renderLayer (CLOUD);
}

void C_Level::renderSelected(){
	C_Grid& grid= C_Locator::getGrid();
	C_GameUnits *current = grid.getSelected();
	if(current != nullptr){
		current->render(current->getScreen());
		}
}

void C_Level::play()
{
        m_landscape->play();
	long current = SDL_GetTicks();
	C_Settings& settings= C_Locator::getSettings();
	if(settings.getPlaying() == PLAY){
		if((current - m_lastWaveTime)>20000) {
			sendNextWave();
			m_lastWaveTime = current;
		}

	}
	C_Grid& grid= C_Locator::getGrid();
	grid.playAllUnits(GRAVEYARD);
	grid.playAllUnits(FIELD);
	grid.playAllUnits(CLOUD);
}


bool C_Level::selectATower(S_Coord clic)
{
	C_Grid& grid= C_Locator::getGrid();
	C_CoordScreen coord(clic);
	return grid.selectATower(coord);
}

void C_Level::unselectedAll()
{
	C_Grid& grid= C_Locator::getGrid();
	grid.unselectedAll();
}

void C_Level::addUnit(string &type, S_Coord clic)
{
	C_Grid& grid= C_Locator::getGrid();
	C_CoordScreen coord(clic);
	if(grid.addUnit(type,coord.getGrid())) {
		C_GameUnits * tmp = grid.getUnits(coord.getXGrid (),coord.getYGrid ());
		if(tmp != nullptr) {
			C_Wallet& wallet= C_Locator::getWallet();
			wallet.debit(tmp->getCost());
			wallet.cliStatus();
		}
	}
}


S_Coord C_Level::getGridTown()
{
	C_Grid& grid= C_Locator::getGrid();
	return grid.foundTown();
}


void C_Level::endOfALevel(){
	C_Grid& grid= C_Locator::getGrid();
	int playerLife = grid.getAllTownsLifeLevel();
	if(playerLife > 0){
		//check if it is the last wave
		int waveLeft = m_waves.size() - 1 - m_currentWaveNbr;
		if(waveLeft <= 0){
			//check if boats of last wave are all dead
			int boats = grid.nbrOfboatStillAlive();
			if(boats <= 0){
				if(m_levelStatus == ONGOING){
					C_Message::printM("You won this battle\n");
					C_OpenEndLevelMenu end;
					end.setNbr(WIN);
					end.action();
					m_levelStatus = WIN;
				}
			}
		}
	} else {
		if(m_levelStatus == ONGOING){
			C_Message::printM("You lost this battle\n");
			C_OpenEndLevelMenu end;
			end.setNbr(LOSE);
			end.action();
			m_levelStatus = LOSE;
		}
	}
}

//______________________________Waves_____________________________//


C_Wave::C_Wave()
{
	m_count = 0;
}


void C_Wave::add(string name, S_Coord coord)
{
	S_Unit tmp;
	tmp.name = name;
	//extract rank from name boat_1_A_EE_0 -> shrink name to boat_1

	size_t pos = tmp.name.find("_") + 1;
	string sub = tmp.name.substr (pos);
	size_t pos2 = sub.find("_");
	tmp.name = tmp.name.substr(0,pos+pos2);

	tmp.coord = coord;
	m_boatList.push_back(tmp);
	m_count++;
}

void C_Wave::cliStatus()
{
	int c = 0;
	for(vector <S_Unit>::iterator i = m_boatList.begin(); i !=m_boatList.end(); i++) {
		ostringstream message;
		message << "Detect " + (*i).name << " at " << (*i).coord.x << ":" << (*i).coord.y << endl;
		C_Message::printM(message.str());
		c++;
	}
	C_Message::printM("Number of boats in this wave: " + to_string(c) + "\n");
}

void C_Wave::loadIntoGrid()
{
	C_Grid& grid= C_Locator::getGrid();
	for(vector <S_Unit>::iterator i = m_boatList.begin(); i !=m_boatList.end(); i++) {
		S_Unit tmp = *i;
		C_Message::printDebug("Add " + tmp.name + " at " + to_string(tmp.coord.x)
					 + ":" + to_string(tmp.coord.y) +"\n");
		grid.addANewBoat(tmp);
	}
}

