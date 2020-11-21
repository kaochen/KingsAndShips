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
#include "tools.h"
#include "message.h"
#include <cmath>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <config.h>
#include <algorithm>
#include <limits.h>

#if __has_include(<filesystem>)
	#include <filesystem>
	namespace fs = std::filesystem;
	#define FILESYSTEM
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
	#define FILESYSTEM
#else
	#define NOFILESYSTEM
#endif

using namespace std;

C_Settings::C_Settings(std::vector<std::string> args)
{
	C_Message::printM("Constructor C_Settings() : start\n");
	string a_Path = absolutePath(args[0]);
	string share_folder = SHARE_FOLDER;
	string cmake_install_prefix = CMAKE_INSTALL_PREFIX;
	string version = PROJECT_VERSION;
	C_Message::printM("The game "+  version +" is execute from here: " + a_Path + "\n");
	C_Message::printM("Cmake install prefix: "+  cmake_install_prefix + "\n");
	C_Message::printM("Share Folder: "+  share_folder+"\n");
	string pgm_path = getPgmPath();
	C_Message::printM("Pgm Path: "+  pgm_path + "\n");
	string app_folder = pgm_path.substr(0,pgm_path.find("bin"));
	C_Message::printM("App folder: "+  app_folder + "\n");

	string share_path = app_folder + share_folder;
	//determine if the program is installed or if it is execute from a local folder
	if(!fs::equivalent(app_folder,cmake_install_prefix)){
		share_path = a_Path;
	}

	C_Message::printM("Share path: "+  share_path + "\n");

	m_prefFile =  mergePath(share_path,"preferences.ini");
	C_Message::printM("preferences.ini: "+  m_prefFile + "\n");
	loadPrefFile();

	//centerCameraPosition();
	m_zoom = ZOOM_MAX/2;
	updateTileWidth();

	calcGridSize();
	m_verboseMode = getArg("-v",args);
	m_debugMode = getArg("-d",args);
	m_debugPath = getArg("-dp",args);
	m_imgFolder = mergePath(share_path,"data/img/");
	C_Message::printM("images folder: "+  m_imgFolder + "\n");
	m_theme = "original";
	initTSXfileList();
	m_currentLevel = 1;
	m_levelFolder = mergePath(share_path,"data/levels/");
	C_Message::printM("levels folder: "+  m_levelFolder  + "\n");

	setNbrOfLevels();
	m_playing = PLAY;
	C_Message::printM("Constructor C_Settings() : done\n");
}

string C_Settings::absolutePath(string &path){

	char temp[1024];
	string pwd = getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("");

	size_t point = path.find_first_of(".");
	if(point != string::npos){
		string tmp = path.substr(point+2);
		path = tmp;
	}
	string absolutePath = pwd + "/"+ path;
	//avoid repetition
	size_t check = path.find(pwd);
  	if (check!=std::string::npos)
  		absolutePath = path;

	size_t found = absolutePath.find("build");
	size_t found2 = absolutePath.find("/bin/");
	if(found != string::npos && found2 == string::npos ){
		absolutePath = absolutePath.substr(0,found);

	}else if(found == string::npos && found2 != string::npos ) {
		absolutePath = absolutePath.substr(0,found2);
		absolutePath = absolutePath + "/";
	}
	return absolutePath;
}

bool C_Settings::getArg(string pattern, vector<string> args){
	bool ret = false;
	for(int i = 1; i < int(args.size());i++ ){
		if(args[i] == pattern){
			C_Message::printM("Argument "+ args[i] + ": true\n");
			ret = true;
		}
	}
	return ret;
}

void C_Settings::loadPrefFile()
{
	extractWindowSize();
}

void C_Settings::extractWindowSize()
{
	int w = 1280 ;
	int h = 720;
	bool testW = extractIntFromINI(w, "window_width",m_prefFile);
	bool testH = extractIntFromINI(h, "window_height",m_prefFile);

	//min and max
	if(w<640) {
		w = 640;
	} else if(w > 4096) {
		w = 4096;
	}
	if(h<640) {
		h = 640;
	} else if(h > 4096) {
		h = 4096;
	}

	if( testW && testH) {
		m_windowWidth = w;
		m_windowHeight = h;
	} else if(testW && !testH) {
		m_windowWidth = w;
		m_windowHeight = w * ASPECT_RATIO;
	} else if(!testW && testH) {
		m_windowHeight = h;
		m_windowWidth = h * ASPECT_RATIO;
	} else {
		m_windowWidth = 1280;
		m_windowHeight = 720;
	}

	C_Message::printM( "Windows Size will be :" + to_string(m_windowWidth)  +"x" + to_string(m_windowHeight) + "\n");
}




void C_Settings::calcGridSize()
{
	m_gridSize = sqrt(m_windowHeight*m_windowHeight + m_windowWidth*m_windowWidth)/(getTileWidth()/2) + 2;
}


void C_Settings::setCameraPosition(const S_Coord &pos)
{
	m_cameraPos.x = pos.x;
	m_cameraPos.y = pos.y;
}

void C_Settings::centerCameraPosition()
{

	m_cameraPos.x = m_windowWidth/2;
	m_cameraPos.y = m_windowHeight/2;
}
void C_Settings::cameraOnAPoint(S_Coord grid)
{
	S_Coord pos;
	pos.x = getWindowWidth()/2 - (grid.x - grid.y)* (getTileWidth()/2) ;
	pos.y = (grid.y + grid.x) * (getTileHeight()/2) - (getWindowHeight()/2);
	setCameraPosition(pos);
}

void C_Settings::moveCameraPosition(const int &rigth,const int &left,const int &down,const int &up)
{
	//cout << "r: " << rigth << " l: " << left << " d: "<< down << " u: "<< up << endl;
	m_cameraPos.x -=rigth;
	m_cameraPos.x +=left;
	m_cameraPos.y -=up;
	m_cameraPos.y +=down;
};

void C_Settings::moveCameraPosition(const int &x,const int &y)
{
	m_cameraPos.x +=x;
	m_cameraPos.y +=y;
};

void C_Settings::zoomDown(){
	m_zoom +=1;
	int y = -30;
	if(m_zoom%2 == 0){
		y = -60;
	}
	if(m_zoom <= m_zoom_max){
		moveCameraPosition(6,y);
	}

	if(m_zoom > m_zoom_max){
		m_zoom = m_zoom_max;
	}
	//cout << "zoomDown: " << m_zoom << endl;
	updateTileWidth();
}

void C_Settings::zoomUp(){
	m_zoom -=1;
	int y = 60;
	if(m_zoom%2 == 0){
		y = 30;
	}
	if(m_zoom >= m_zoom_min){
		moveCameraPosition(-6,y);
	}

	if(m_zoom <= m_zoom_min){
		m_zoom = m_zoom_min;
	}
	//cout << "zoomUp" << m_zoom  << endl;
	updateTileWidth();
}

void C_Settings::updateTileWidth(){
		int w = IMG_WIDTH;
		int h = IMG_WIDTH/2;
		int i = getZoom() -1;
		if(i >= 0 && i < ZOOM_MAX){
			w = IMG_WIDTH - (i*STEP);
			h = IMG_WIDTH/2 - (i*(STEP/2));
		}
		m_tileWidth = (w - w/4);
		m_tileHeight = (h - h/4);
		//std::cout << i  << "-> " << w << ":" << h << " --> " << m_tileWidth << ":" << m_tileHeight  << std::endl;
}

void C_Settings::initTSXfileList()
{
	vector <string> list = C_Tools::listFiles(m_imgFolder);
	for(auto i : list){
		if(C_Tools::fileExist(i)){
			if(C_Tools::checkExtension(i, ".tsx")){
				C_Message::printM("Found tsx file: "+i + "\n");
				m_tsxFileList.push(i);
			}
		} else {
			C_Message::printError(i + " is missing\n");
		}
	}
}

void C_Settings::setDebugMode()
{
	if(m_debugMode) {
		m_debugMode = false;
		m_debugPath = false;
	} else {
		m_debugMode = true;
	}
}


void C_Settings::setDebugPathMode()
{
	if(m_debugPath) {
		m_debugPath = false;
	} else {
		m_debugPath = true;
	}
}


void C_Settings::displayDebugMode()
{
	string debug = "off";
	string debugPath = "off";

	if(m_debugMode)
		debug = "on";

	if(m_debugPath)
		debugPath = "on";
	string message = "Debug mode: " + debug + ", debug path mode: " + debugPath + "\n";
	C_Message::printM(message);

}


int C_Settings::setCurrentLevelNbr(int nbr)
{
	//cout << nbr << "->" << m_currentLevel << endl;
	m_currentLevel = nbr;

	if (m_currentLevel > m_nbrOfLevels) {
		m_currentLevel = 1;
	}
	if (m_currentLevel < 1) {
		m_currentLevel = m_nbrOfLevels;
	}

	return m_currentLevel;
}


void C_Settings::setNbrOfLevels()
{
	int i = 1;
	string filePath = m_levelFolder + "Level_" + to_string(i) + ".tmx";
	while(C_Tools::fileExist(filePath)) {
		i++;
		string filename = C_Tools::extractFilename(filePath);
		C_Message::printM("Found level file: " + filename + "\n");
		filePath = m_levelFolder + "Level_" + to_string(i) + ".tmx";
	}
	m_nbrOfLevels = i - 1;
}



bool C_Settings::extractIntFromINI(int &nbr, const string &name, const string &filename)
{
	bool ret = false;
	ifstream file(filename);

	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			string extract;
			if(strstr(line.c_str(), name.c_str()) != nullptr) {
				std::size_t pos = line.find("=");      // position of "live" in str
				extract = line.substr (pos+1);
				C_Message::printM("From " + filename +": " + name + " -> " + extract +"\n");
				nbr = stoi(extract);
				ret = true;
			}
		}
	} else {
		C_Message::printM("Can not find " + filename +"\n");
	}
	file.close();
	return ret;
}


void C_Settings::setPlaying()
{

	if(m_playing==PAUSE) {
		m_playing = PLAY;
		C_Message::printM("PLAY\n");
	} else if(m_playing == PLAY) {
		m_playing = PAUSE;
		C_Message::printM("PAUSE\n");
	}
}

void C_Settings::setPlaying(int state)
{
	if(state == PLAY) {
		m_playing = PLAY;
		C_Message::printM("PLAY\n");
	} else if(state == PAUSE) {
		m_playing = PAUSE;
		C_Message::printM("PAUSE\n");
	}
}

std::string C_Settings::getPgmPath(){
	char result[ PATH_MAX ];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	return std::string( result, (count > 0) ? count : 0 );
}

std::string C_Settings::mergePath(const std::string &first, const std::string &last)
{
    std::string ret = first + last;
    #ifdef NOFILESYSTEM
    if(first.back() != '\\' && first.back() != '/' ){
	    #if defined _WIN32 || defined __CYGWIN__
		    ret = first + "\\" + last;
	    #else
		    ret = first +  "/" + last ;
	    #endif
    }
    #else
		fs::path path = first;
	    ret = path / last;
    #endif
    return  ret;
}

