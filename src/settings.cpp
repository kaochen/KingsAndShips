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
#include "time.h"

using namespace std;

C_Settings C_Settings::m_instance=C_Settings();

C_Settings::C_Settings()
{
	//cout << "Construct C_Settings" << endl;
	m_gridSize = GRID_SIZE;
	m_windowWidth = WINDOW_WIDTH;
	m_windowHeight = WINDOW_WIDTH * ASPECT_RATIO;
	m_gridNbrOfLine = 28;
	m_gridNbrOfRow = 15;
	m_first_tile_x = 13;
	m_first_tile_y = 0;
	m_debugMode = false;
	m_debugPath = false;
	m_imgFolder = "data/img/";
	m_theme ="original";
	m_currentLevel = 0;
}

C_Settings::~C_Settings()
{
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
    if (m_currentLevel > MAX_LEVELS){
        m_currentLevel = 0;
        }
    if (m_currentLevel < 0){
        m_currentLevel = MAX_LEVELS;
        }

    return m_currentLevel;
}




