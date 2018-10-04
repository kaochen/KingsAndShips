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
#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "gameUnits.h"
#include "boat.h"
#include "level.h"
#include "grid.h"

#include "../wallet.h"
#include "../message.h"
#include "../menu/menu.h"
#include "../coord.h"

using namespace std;


C_Level::C_Level():
	m_name("level"),
	m_id(1)
{
	m_count = ++m_id;
	m_groundLayer.name="noname";
	m_groundLayer.width=30;
	m_groundLayer.height=30;
	m_groundLayer.data="";
	m_decorLayer.name="noname";
	m_decorLayer.width=30;
	m_decorLayer.height=30;
	m_decorLayer.data="";
	m_nbrOfWaves = 0;
	m_currentWaveNbr = -1;

	m_landscape = nullptr;
}


C_Level::~C_Level()
{
    delete m_landscape;
}

void C_Level::cliStatus(){
	cout << m_name << " " << m_id << endl;
}

void C_Level::extractInfosFromTmx(int levelNbr){
	C_Settings& settings=C_Settings::Instances();
    m_filename = settings.getLevelFolder() + "Level_" + to_string(levelNbr) + ".tmx";
	C_Message m;
    struct stat buffer;
    if (stat (m_filename.c_str(),  &buffer) == 0){
        m_width = stoi(extractValueFromTmxFile(m_filename.c_str(), "map", "width"));
        m_height = stoi(extractValueFromTmxFile(m_filename.c_str(), "map", "height"));
        m_gridSize = calcGridSize();
        m.printM("Grid size should be " + to_string(m_gridSize) + "\n");
        m_tilewidth = stoi(extractValueFromTmxFile(m_filename.c_str(), "map", "tilewidth"));
        m_tileheight = stoi(extractValueFromTmxFile(m_filename.c_str(), "map", "tileheight"));
        m_backgroundcolor = extractValueFromTmxFile(m_filename.c_str(), "map", "backgroundcolor");
        extractPropertyFromTmxFile(m_filename.c_str(), "subname");
	}
	else{
	    C_Message m;
    	m.printM("Can not find " + m_filename+"\n");
	}
}

int C_Level::calcGridSize(){
    if(m_width > m_height)
        return m_width + 2;
    else
        return m_height + 2;
}

void C_Level::load(int levelNbr){
    extractInfosFromTmx(levelNbr);
    //clean before loading
	C_Grid& grid=C_Grid::Instances();
	grid.reset();

	C_Message m;
    struct stat buffer;
    if (stat (m_filename.c_str(),  &buffer) == 0){

	    loadGroundLayerIntoTheGrid(m_filename.c_str());
	    loadDecorLayerIntoTheGrid(m_filename.c_str());
	    m_nbrOfWaves = countAttributes(m_filename.c_str(),"Wave");
    	for(int i = 0; i < m_nbrOfWaves; i++){
    	    loadWave(m_filename.c_str(),i);
    	}
        updateMenuInfo();
        C_Wallet& wallet=C_Wallet::Instances();
        int walletCredit =  stoi(extractPropertyFromTmxFile(m_filename.c_str(), "wallet"));
        if(walletCredit < 1){
            walletCredit = 500;
        }
        wallet.reset();
        wallet.credit(walletCredit); //add a credit for start

        createLandscape();
    	m.printM("Level " + to_string(levelNbr) +" Loaded\n");
	}
	else{
    	m.printM("Can not find " + m_filename+"\n");
    	m.printM("Can not load level " + to_string(levelNbr)+"\n");
	}
}

void C_Level::createLandscape(){
	C_Grid& grid=C_Grid::Instances();
    C_CoordGrid tmp(grid.foundTown());
	S_Coord town = tmp.getScreen();
	m_landscape = new C_Landscape(town);
	}

void C_Level::sendNextWave(){
    C_Message m;
    m_currentWaveNbr++;
    if(m_currentWaveNbr >= m_nbrOfWaves){
        m_currentWaveNbr = 0;
    }

    cliWaveStatus(m_currentWaveNbr);
    loadWaveIntoGrid(m_currentWaveNbr);
    m.printM("Next wave: " + to_string(m_currentWaveNbr)+"\n");
}

string C_Level::extractValueFromTmxFile(string tmx_File_Path, const string &node, const string &attribute){
     xmlpp::TextReader reader(tmx_File_Path);
     string value;
     while(reader.read())
        {
        		string nodeName = reader.get_name();
	          	//cout << nodeName << "---namespace---\n";

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			      string attrib = reader.get_name();
			      //cout << attributes << "-----"<< endl;

			      if (nodeName == node && attrib == attribute){
			      	value = reader.get_value();
				    }
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + tmx_File_Path +" Node: \""+ node + "\" attribute: \"" + attribute + "\" get this value: "+ value + "\n");
    return value;
}

string C_Level::extractPropertyFromTmxFile(string tmx_File_Path, const string &name){
     xmlpp::TextReader reader(tmx_File_Path);
     string value;
     while(reader.read())
        {
        		string nodeName = reader.get_name();
	          	//cout << nodeName << "---namespace---\n";

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			      string attrib = reader.get_name();
			      //cout << attributes << "-----"<< endl;

			      if (nodeName == "property" && attrib == "name"){
			      	string readName = reader.get_value();
			      	if(readName == name){
			      	    reader.move_to_next_attribute();
			      	    value = reader.get_value();
			      	    }
				    }
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + tmx_File_Path +" Property: " +name+ " = "+ value + "\n");
    return value;
}


S_tmxLayer C_Level::extractTMXfile(string tmx_File_Path, string layerName){

	S_tmxLayer layer;
	layer.name = layerName;
    string currentLayerName ="";
    C_Message m;
	m.printM("Reading: " + tmx_File_Path +"\n");
 xmlpp::TextReader reader(tmx_File_Path);
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

			  if (nodeName == "layer" && attributes == "name"){
			  	currentLayerName = reader.get_value();
				}
			  if (nodeName == "layer" && attributes == "width"){
			  	layer.width = stoi(reader.get_value());
				}
			  if (nodeName == "layer" && attributes == "height"){
			  	layer.height = stoi(reader.get_value());
				}
			  if (nodeName == "data" && attributes == "encoding"){
				if (reader.get_value() == "csv" && currentLayerName == layerName){
				    m.printDebug("found a " + layerName +" layer in the tmx file " + tmx_File_Path+"\n");
					layer.data = reader.read_inner_xml();
				    currentLayerName ="";
						}
					}

			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}

	//drop all \n
	size_t start = 0;
	string in = "\n", out = "";
	while((start = layer.data.find(in,start)) != std::string::npos){
		layer.data.replace(start,in.length(),out);
		start += out.length();
	}

	//cout  << data << "////" << endl;
    return layer;
}

void C_Level::loadGroundLayerIntoTheGrid(string tmx_File_Path){
	C_Grid& grid=C_Grid::Instances();
	m_groundLayer = extractTMXfile(tmx_File_Path,"Ground");
    string data = m_groundLayer.data;
    S_Coord start = getFirstTile(m_groundLayer);
	for (int y = start.y; y < m_groundLayer.height; y++){
		for (int x = start.x; x < m_groundLayer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				//cout << nbr;
				if(nbr == 0){nbr = 27;}; //FIXME water is not the 0 but the 27 in the tileset
				grid.setGround(x,y,nbr);

				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
}

S_Coord C_Level::getFirstTile(S_tmxLayer &layer){
    C_Grid& grid=C_Grid::Instances();
    int x = 0; int y = 0;
    if(grid.size()-layer.width>1){
        x = (grid.size()-layer.width)/2;
        }
    if(grid.size()-layer.height>1){
        y = (grid.size()-layer.height)/2;
        }
    S_Coord first{x,y};
    cout << "first " << x << ":" << y << "-----------------------------------------"<< endl;
    return first;
}

void C_Level::loadWave(string tmx_File_Path, int waveNbr){

  C_TextureList& t=C_TextureList::Instances();

    C_Wave wave;
	string name = "Wave" + to_string(waveNbr);
	S_tmxLayer layer = extractTMXfile(tmx_File_Path,name);
    string data = layer.data;
    C_Message m;
	for (int y = 0; y < layer.height; y++){
		for (int x = 0; x < layer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				if (nbr!=0){
	                string str = t.getNameFromID(nbr);
                    m.printDebug(to_string(x) + ":" + to_string(y) + "->" + str + " // ") ;
                    wave.add(1,x,y);
				    }
				//grid.setGround(x,y,nbr);

				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
	//cout << endl;
	m_waves.push_back(wave);
}

void C_Level::cliWaveStatus(int i){
        int c = 0;
        for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end();it++){
            C_Wave wave = *it;
            if(i == c){
                wave.cliStatus();
            }
            c++;
        }
            C_Message m;
            m.printM("Number of wave for this level: " + to_string(c) +"\n");
}

void C_Level::loadWaveIntoGrid(int i){
        int c = 0;
        C_Message m;
        for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end();it++){
            C_Wave wave = *it;
            if(i == c){
                m.printM("load wave: " + to_string(c) + "\n");
                wave.loadIntoGrid();
            }
            c++;
        }
        updateMenuInfo();
}


void C_Level::loadDecorLayerIntoTheGrid(string tmx_File_Path){
	C_Grid& grid=C_Grid::Instances();
	m_decorLayer = extractTMXfile(tmx_File_Path,"Decors");
    string data = m_decorLayer.data;
    S_Coord start = getFirstTile(m_decorLayer);
	for (int y = start.y; y < m_decorLayer.height; y++){
		for (int x = start.x; x < m_decorLayer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				if(nbr != 0){
                	grid.setDecors(x,y,nbr);
				    }
				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
}


void C_Level::updateMenuInfo(){
    C_Menu& menu=C_Menu::Instances();
    menu.updateLevelInfos(m_nbrOfWaves - m_currentWaveNbr, m_nbrOfWaves);
}

int C_Level::countAttributes(string tmx_File_Path, string pattern){
	string old = "";
    C_Message m;
    xmlpp::TextReader reader(tmx_File_Path);
    int c = 0;
     while(reader.read())
        {
        	string nodeName = reader.get_name();
        	if (reader.has_attributes()){
			reader.move_to_first_attribute();
			do
			{
			  string attributes = reader.get_name();
			  if (nodeName == "layer" && attributes == "name"){
                	//cout << "NodeName: " << nodeName << endl;
			  	    //cout << "Attributes: " << attributes << endl;
			  	    string value = reader.get_value();
                    //cout << value << endl;
			        if(value.compare(0,pattern.size(),pattern)==0 && old != value){
			  	        c++;
    			  	    //cout << "Found " << reader.get_value() << " + " << c << endl;
    			  	    old = value;
			  	    }
				}
				attributes = "";
			} while(reader.move_to_next_attribute());
		}
	    reader.move_to_element();
	    nodeName = "";
	}
	m.printM(c +" "+ pattern + " in " + tmx_File_Path +"\n");
    return c;
}

void C_Level::render(){
    C_Grid& grid=C_Grid::Instances();
    m_landscape->render();
	grid.renderLayer (GRAVEYARD);
	grid.renderLayer (GROUND);
	grid.renderLayer (FIELD);
}

void C_Level::playAllUnits(){
    C_Grid& grid=C_Grid::Instances();
    grid.playAllUnits();
}

bool C_Level::selectATower(S_Coord clic){
    C_Grid& grid=C_Grid::Instances();
    C_CoordScreen coord(clic);
    return grid.selectATower(coord);
}

void C_Level::addUnit(string &type, S_Coord clic){
    C_Grid& grid=C_Grid::Instances();
    C_CoordScreen coord(clic);
    if(grid.addUnit(type,coord.getXGrid (),coord.getYGrid (),0) == EXIT_SUCCESS){
	   C_GameUnits * tmp = grid.getUnits(coord.getXGrid (),coord.getYGrid ());
	   if(tmp != nullptr){
	       C_Wallet& wallet=C_Wallet::Instances();
	       wallet.debit(tmp->getCost());
	       wallet.cliStatus();
	       }
	    }
}


//______________________________Waves_____________________________//


C_Wave::C_Wave()
{
}
C_Wave::~C_Wave()
{
    m_count = 0;
    m_count_dead = 0;
}


void C_Wave::add(int rank, int x, int y){
    S_boat tmp ={rank,x,y,true};
    m_boatList.push_back(tmp);
    m_count++;
}

void C_Wave::cliStatus(){
    int c = 0;
    C_Message m;
    for(vector <S_boat>::iterator i = m_boatList.begin(); i !=m_boatList.end();i++)
    {
        ostringstream message;
        message << "Rank " + to_string((*i).rank) << " at " << (*i).x << ":" << (*i).y;
        if ((*i).alive)
            message << " Alive\n";
        else
            message << "dead\n";
        m.printM(message.str());
        c++;
    }
    m.printM("Number of boats in this wave: " + to_string(c) + "\n");
}

void C_Wave::loadIntoGrid(){
    C_Grid& grid=C_Grid::Instances();
    C_Message m;
    for(vector <S_boat>::iterator i = m_boatList.begin(); i !=m_boatList.end();i++)
    {
        S_boat tmp = *i;
        m.printDebug("Rank " + to_string(tmp.rank) + " at " + to_string(tmp.x) + ":" + to_string(tmp.y) +"\n");
        if (tmp.alive)
            grid.addANewBoat(tmp.x,tmp.y,tmp.rank,this);

    }
}

void C_Wave::addToDeadCounter(int nbr){
    m_count_dead += nbr;
}
