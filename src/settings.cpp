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


#include "settings.h"
#include "message.h"
#include <cmath>
#include <string.h>
#include <fstream>

using namespace std;

C_Settings C_Settings::m_instance=C_Settings();

C_Settings::C_Settings()
{
	//cout << "Construct C_Settings" << endl;
	m_prefFile = "preferences.ini";
	loadPrefFile();

    //centerCameraPosition();
    calcGridSize();
	m_debugMode = false;
	m_debugPath = false;
	m_imgFolder = "data/img/";
	m_theme ="original";
	initTSXfileList();
	m_currentLevel = 1;
	m_levelFolder = "data/levels/";
	m_nbrOfLevels = getNbrOfLevels();

}

C_Settings::~C_Settings()
{
}

void C_Settings::loadPrefFile(){
    extractWindowSize();
}

void C_Settings::extractWindowSize(){
    int w = 1280 ; int h = 720;
    bool testW = extractIntFromINI(w, "window_width",m_prefFile);
    bool testH = extractIntFromINI(h, "window_height",m_prefFile);

    //min and max
    if(w<640){w = 640;}
    else if(w > 4096){ w = 4096;}
    if(h<640){h = 640;}
    else if(h > 4096){ h = 4096;}

	if( testW && testH){
        m_windowWidth = w;
        m_windowHeight = h;
	}
	else if(testW && !testH){
	    m_windowWidth = w;
        m_windowHeight = w * ASPECT_RATIO;
        }
	else if(!testW && testH){
	    m_windowHeight = h;
        m_windowWidth = h * ASPECT_RATIO;
        }
    else{
        m_windowWidth = 1280;
        m_windowHeight = 720;
    }
    C_Message m;
    m.printM( "Windows Size will be :" + to_string(m_windowWidth)  +"x" + to_string(m_windowHeight) + "\n");
}




void C_Settings::calcGridSize(){
    m_gridSize = sqrt(m_windowHeight*m_windowHeight + m_windowWidth*m_windowWidth)/TILE_HALF_WIDTH + 2;
}


void C_Settings::setCameraPosition(const S_Coord &pos){
    m_cameraPos.x = pos.x;
    m_cameraPos.y = pos.y;
}

void C_Settings::centerCameraPosition(){

    m_cameraPos.x = m_windowWidth/2;
    m_cameraPos.y = m_windowHeight/2;
}
void C_Settings::cameraOnAPoint(S_Coord grid){
    S_Coord pos;
	pos.x = getWindowWidth()/2 - (grid.x - grid.y)* TILE_HALF_WIDTH ;
    pos.y = (grid.y + grid.x) * TILE_HALF_HEIGHT - getWindowHeight()/2;
    setCameraPosition(pos);
}

void C_Settings::moveCameraPosition(const int &rigth,const int &left,const int &down,const int &up){
    //cout << "r: " << rigth << " l: " << left << " d: "<< down << " u: "<< up << endl;
    m_cameraPos.x -=rigth;
    m_cameraPos.x +=left;
    m_cameraPos.y -=up;
    m_cameraPos.y +=down;
};

void C_Settings::moveCameraPosition(const int &x,const int &y){
    m_cameraPos.x +=x;
    m_cameraPos.y +=y;
};


  S_Size C_Settings::getNbrOfTilesToDisplay(){
    S_Size size;
    size.w = m_windowWidth / TILE_HALF_WIDTH;
    size.h = m_windowHeight / TILE_HALF_HEIGHT;
    return size;
  }

void C_Settings::initTSXfileList(){
    C_Message m;
    int size = 18;
    string list[size] {"Ground_01.tsx","boat_01.tsx","town_01.tsx",
    "Rocks_00.tsx","Trees_00.tsx","turbine_00.tsx","archerTower_00.tsx",
	"archerTower_01.tsx","buttons.tsx","Water_00.tsx","smoke_01.tsx",
	"charaters.tsx","boat_dead_01.tsx","Weapons.tsx","barricade_01.tsx",
	"gold.tsx","buttons_tabs.tsx","icons_12px.tsx"};

	for(int i = 0; i < size; i++){
    	string filePath = m_imgFolder + list[i];
	    if(fileExist(filePath))
	        m_tsxFileList.push(filePath);
	    else
	        m.printError(filePath + " is missing\n");
	}
}

void C_Settings::setDebugMode(){
	if(m_debugMode){
		m_debugMode = false;
		m_debugPath = false;
		}
	else{
		m_debugMode = true;
	}
}


void C_Settings::setDebugPathMode(){
	if(m_debugPath){
		m_debugPath = false;
		}
	else{
		m_debugPath = true;
	}
}


void C_Settings::displayDebugMode(){
    C_Message m;
    string debug = "off";
    string debugPath = "off";

	if(m_debugMode)
		debug = "on";

	if(m_debugPath)
		debugPath = "on";
	string message = "Debug mode: " + debug + ", debug path mode: " + debugPath + "\n";
	m.printM(message);

}


int C_Settings::setCurrentLevelNbr(int nbr){
    cout << nbr << "->" << m_currentLevel << endl;
    m_currentLevel = nbr;

    if (m_currentLevel > m_nbrOfLevels){
        m_currentLevel = 1;
        }
    if (m_currentLevel < 1){
        m_currentLevel = m_nbrOfLevels;
        }

    return m_currentLevel;
}


int C_Settings::getNbrOfLevels(){
    int i = 1;
    C_Message m;
    string fileName = m_levelFolder + "Level_" + to_string(i) + ".tmx";
    cout << fileName << endl;
    while(fileExist(fileName)){
        i++;
        m.printM("Found level file: " + fileName + "\n");
        fileName = m_levelFolder + "Level_" + to_string(i) + ".tmx";
    }
    return i - 1;
}


bool C_Settings::fileExist(const string &file){
    ifstream tmp(file.c_str());
    return !tmp.fail();
}


bool C_Settings::extractIntFromINI(int &nbr, const string &name, const string &filename){
    bool ret = false;
    ifstream file(filename);
    C_Message m;
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            string extract;
            if(strstr(line.c_str(), name.c_str()) != nullptr){
                     std::size_t pos = line.find("=");      // position of "live" in str
                     extract = line.substr (pos+1);
                     m.printM("From " + filename +": " + name + " -> " + extract +"\n");
                     nbr = stoi(extract);
                     ret = true;
            }
        }
    }
    else{
        m.printM("Can not find " + filename +"\n");
    }
    file.close();
    return ret;
}



