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

C_Set C_Set::m_instance=C_Set();

C_Set::C_Set()
{
	//cout << "Construct C_Set" << endl;
	m_gridSize = GRID_SIZE;
	m_windowWidth = WINDOW_WIDTH;
	m_windowHeight = WINDOW_WIDTH * ASPECT_RATIO;
	m_gridNbrOfLine = 28;
	m_gridNbrOfRow = 15;
	m_first_tile_x = 13;
	m_first_tile_y = 0;
	m_debugMode = false;
	m_debugPath = false;
}

C_Set::~C_Set()
{
}

C_Set& C_Set::Instances()
{
	return m_instance;
}

int C_Set::getGridSize(){
	return m_gridSize;
}

int C_Set::getWindowWidth(){
	return m_windowWidth;
}

int C_Set::getWindowHeight(){
	return m_windowHeight;
}


int C_Set::getGridWidth(){
	return m_windowWidth / TILE_HALF_WIDTH;
}

int C_Set::getGridHeight(){
	return m_windowHeight / TILE_HALF_HEIGHT;
}

int C_Set::getGridNbrOfLine(){
	return m_gridNbrOfLine;
}

int C_Set::getGridNbrOfRow(){
	return m_gridNbrOfRow;
}

int C_Set::getGridFirstTileX(){
	return m_first_tile_x;
}

int C_Set::getGridFirstTileY(){
	return m_first_tile_y;
}

void C_Set::setDebugMode(){
	if(m_debugMode){
		m_debugMode = false;
		m_debugPath = false;
		}
	else{
		m_debugMode = true;
	}
}

bool C_Set::getDebugMode(){
	return m_debugMode;
}

void C_Set::setDebugPathMode(){
	if(m_debugPath){
		m_debugPath = false;
		}
	else{
		m_debugPath = true;
	}
}

bool C_Set::getDebugPathMode(){
	return m_debugPath;
}

void C_Set::displayDebugMode(){
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


//---------------------------------------------------------------

C_Coord::C_Coord(int x_grid, int y_grid){

	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > GRID_SIZE)
		x_grid = GRID_SIZE;
	if (y_grid > GRID_SIZE)
		y_grid = GRID_SIZE;

	m_this.grid.x = x_grid;
	m_this.grid.y = y_grid;
	m_this.screen = gridToScreen(m_this.grid);
}

C_Coord::C_Coord(S_Coord coord){
	if (coord.x < 0) //to quite gcc
		cout << "";
}


C_Coord::C_Coord(const C_Coord &a){
	m_this.grid.x = a.m_this.grid.x;
	m_this.grid.y = a.m_this.grid.y;
	m_this.screen.x = a.m_this.screen.x;
	m_this.screen.y = a.m_this.screen.y;
}

C_Coord::~C_Coord(){
};


bool operator==(C_Coord const &a, C_Coord const &b){
	return a.isEqual(b);

}

bool C_Coord::isEqual(C_Coord const &b) const{
	//cout << "test equality " << m_this.grid.x << ":" << m_this.grid.y << " ";
	//cout << b.m_this.grid.x << ":" << b.m_this.grid.y << endl;
	return (m_this.grid.x == b.m_this.grid.x && m_this.grid.y == b.m_this.grid.y);
}

void C_Coord::applyOffset(S_Coord offset){
	m_this.screen.x += offset.x;
	m_this.screen.y += offset.y;
}


S_Coord C_Coord::screenToGrid(S_Coord screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() + TILE_HALF_HEIGHT)/2;
		float tempX = 0.0, tempY = 0.0;
		S_Coord coord;
		tempX = ( ((screen.x - xOffset ) / TILE_HALF_WIDTH + (screen.y + yOffset)/TILE_HALF_HEIGHT )/2);
		tempY = ( (screen.y + yOffset )/(TILE_HALF_HEIGHT*2) - (screen.x - xOffset)/(TILE_HALF_WIDTH*2));
		coord.x = tempX;
		coord.y = tempY;
		return coord;
		}

S_Coord C_Coord::gridToScreen(S_Coord grid){
			C_Set& settings=C_Set::Instances();
			S_Coord screen;
			screen.x = settings.getWindowWidth()/2 + (grid.x - grid.y)* TILE_HALF_WIDTH;
			screen.y = (grid.y + grid.x) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
			return screen;
}

void C_Coord::displayStatus(){
 	cout << "\tx_grid:"<< m_this.grid.x << " y_grid:"<< m_this.grid.y;
 	cout << "\tx_screen:"<< m_this.screen.x << " y_screen:"<< m_this.screen.y << endl;
}

S_Coord C_Coord::getGrid(){
	return m_this.grid;
}

int C_Coord::getXGrid() const{
	return m_this.grid.x;
}

int C_Coord::getYGrid() const{
	return m_this.grid.y;
}

S_Coord C_Coord::getScreen(){
	return m_this.screen;
}

void C_Coord::updateScreen(S_Coord new_screen){
	m_this.screen = new_screen;
}

int C_Coord::getXScreen(){
	return m_this.screen.x;
}
int C_Coord::getYScreen(){
	return m_this.screen.y;
}

void C_Coord::centerOnTile(){
	C_CoordGrid tmp(m_this.grid);
	m_this.screen = tmp.getScreen ();
	m_this.screen.y += TILE_HALF_HEIGHT;
	m_this.grid = tmp.getGrid ();
}

int C_Coord::angleToDirection(double angle){
			int  direction = UNKNOWN;
			if (angle > -22.5 && angle <= 22.5){
				direction = NORTH_WEST;
				}
			else if (angle > 22.5 && angle <= 67.5){
				direction = NORTH;
			}
			else if(angle > 67.5 && angle <=112.5){
				direction = NORTH_EAST;
				}
			else if(angle > 112.5 && angle <=157.5){
				direction = EAST;
				}
			else if((angle > 157.5 && angle <= 202.5)){
				direction = SOUTH_EAST;
				}
			else if(angle > 202.5 && angle <= 247.5){
				direction = SOUTH;
				}
			else if(angle > 247.5 && angle <= 292.5){
				direction = SOUTH_WEST;
				}
			else if(angle > 292.5 && angle <= 337.5){
				direction = WEST;
				}
			else if(angle > 337.5 && angle <= 382.5){
				direction = NORTH_WEST;
				}
			else{
				direction = EAST;
				}
			return direction;
}

void C_Coord::move(double angle, int speed){
	C_Time& time=C_Time::Instances();
	//One pixel can't be split, on even frame number use an offset of 0 instead of 1.
	int halfPixel = 1;
	 if(time.getFrameNbr ()%2 == 0){
    			   	 halfPixel =0;
    			   	 }

	angle = angle *180/3.14159265359;
	//cout << "Angle : " << angle << endl;
	if (angle > -22.5 && angle <= 22.5){
			//cout << "SOUTH_EAST" << endl;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.y +=halfPixel;
			    break;
			    	case SLOW:
    			    m_this.screen.y +=1;
			    break;
			    	case NORMAL:
    			    m_this.screen.y +=(1 + halfPixel);
			    break;
			     	case FAST:
    			    m_this.screen.y +=2;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.y +=(2 + halfPixel);
			    break;
			    }
			}
	else if (angle > 22.5 && angle <= 67.5){
			//cout << "EAST" << endl;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x +=1;
    			   	m_this.screen.y +=halfPixel;
			    break;
			    	case SLOW:
    			    m_this.screen.x +=2;
    			    m_this.screen.y +=1;
			    break;
			    	case NORMAL:
    			    m_this.screen.x +=3;
    			    m_this.screen.y +=(1 + halfPixel);
			    break;
			     	case FAST:
    			    m_this.screen.x +=4;
    			    m_this.screen.y +=2;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x +=5;
    			    m_this.screen.y +=(2 + halfPixel);
			    break;
			    }
			}
	else if(angle > 67.5 && angle <=112.5){
			//cout << "NORTH_EAST" << endl;
			m_this.screen.x +=speed;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x +=1;
			    break;
			    	case SLOW:
    			    m_this.screen.x +=2;
			    break;
			    	case NORMAL:
    			    m_this.screen.x +=3;
			    break;
			     	case FAST:
    			    m_this.screen.x +=4;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x -=5;
			    break;
			    }
				}
	else if(angle > 112.5 && angle <=157.5){
			//cout << "NORTH" << endl;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x +=1;
    			   	m_this.screen.y -=halfPixel;
			    break;
			    	case SLOW:
    			    m_this.screen.x +=2;
    			    m_this.screen.y -=1;
			    break;
			    	case NORMAL:
    			    m_this.screen.x +=3;
    			    m_this.screen.y -=(1 + halfPixel);
			    break;
			     	case FAST:
    			    m_this.screen.x +=4;
    			    m_this.screen.y -=2;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x +=5;
    			    m_this.screen.y -=(2 + halfPixel);
			    break;
			    }
				}
	else if((angle > 157.5 && angle <=180) || (angle > -180 && angle <= -157.5)){
			//cout << "NORTH_WEST" << endl;
				    switch(speed){
			        case VERY_SLOW:
        			    m_this.screen.y -=halfPixel;
			        break;
			        	case SLOW:
        			    m_this.screen.y -=1;
			        break;
			        	case NORMAL:
        			    m_this.screen.y -=2;
			        break;
			         	case FAST:
        			    m_this.screen.y -=3;
			        break;
			        	case VERY_FAST:
        			    m_this.screen.y -=4;
			        break;
			        }
				}
	else if(angle > -157.5 && angle <= -112.5){
			//cout << "WEST" << endl;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x -=1;
    			   	m_this.screen.y -=halfPixel;
			    break;
			    	case SLOW:
    			    m_this.screen.x -=2;
    			    m_this.screen.y -=1;
			    break;
			    	case NORMAL:
    			    m_this.screen.x -=3;
    			    m_this.screen.y -=(1 + halfPixel);
			    break;
			     	case FAST:
    			    m_this.screen.x -=4;
    			    m_this.screen.y -=2;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x -=5;
    			    m_this.screen.y -=(2 + halfPixel);
			    break;
			    }
				}
	else if(angle > -112.5 && angle <=-67.5){
			//cout << "SOUTH_WEST" << endl;
				m_this.screen.x -=speed;
				switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x -=1;
			    break;
			    	case SLOW:
    			    m_this.screen.x -=2;
			    break;
			    	case NORMAL:
    			    m_this.screen.x -=3;
			    break;
			     	case FAST:
    			    m_this.screen.x -=4;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x -=5;
			    break;
			    }
				}
	else if(angle > -67.5 && angle <=-22.5){

			//cout << "SOUTH" << endl;
			switch(speed){
			    case VERY_SLOW:
    			    m_this.screen.x -=1;
                    m_this.screen.y +=halfPixel;
			    break;
			    	case SLOW:
    			    m_this.screen.x -=2;
    			    m_this.screen.y +=1;
			    break;
			    	case NORMAL:
    			    m_this.screen.x -=3;
    			    m_this.screen.y +=(1 + halfPixel);
			    break;
			     	case FAST:
    			    m_this.screen.x -=4;
    			    m_this.screen.y +=2;
			    break;
			    	case VERY_FAST:
    			    m_this.screen.x -=5;
    			    m_this.screen.y +=(2 + halfPixel);
			    break;
			    }
				}
	else{
		cout << "Angle is not between 0 and 360 : " << angle << endl;
	}

}

void C_Coord::regenGridCoord(){
		m_this.grid = screenToGrid(m_this.screen);
}


bool C_Coord::closeToCenter(S_Coord grid){
		C_CoordGrid tmp(grid);
		tmp.centerOnTile();
		S_Coord center = tmp.getScreen();
		int l = m_this.screen.x - center.x;
		int h = m_this.screen.y - center.y;
		if (l < 0)
			l *=-1;
		if (h < 0)
			h *=-1;
		if(l < TILE_HALF_WIDTH/4 && h < TILE_HALF_HEIGHT/4){
			//cout << "center true" << endl;
			return true;
			}
		else{
			//cout << "center false" << endl;
			return false;
			}



}


C_CoordGrid::C_CoordGrid(S_Coord coord): C_Coord(coord){
		m_this.grid.x = coord.x;
		m_this.grid.y = coord.y;
		m_this.screen = gridToScreen(m_this.grid);
		//cout << "grid to screen" << endl;
}

C_CoordGrid::C_CoordGrid(int x_grid, int y_grid ): C_Coord(x_grid, y_grid){
		if (x_grid < 0)
			x_grid = 0;
		if (y_grid < 0)
			y_grid = 0;
		if (x_grid > GRID_SIZE)
			x_grid = GRID_SIZE;
		if (y_grid > GRID_SIZE)
			y_grid = GRID_SIZE;

		S_Coord coord;
		coord.x = x_grid;
		coord.y = y_grid;
		C_CoordGrid tmp(coord);
		m_this.screen = tmp.getScreen ();
		m_this.grid = tmp.getGrid ();
}

C_CoordGrid::~C_CoordGrid()
{
};

C_CoordScreen::C_CoordScreen(S_Coord coord): C_Coord(coord){
		m_this.screen.x = coord.x;
		m_this.screen.y = coord.y;
		m_this.grid = screenToGrid(m_this.screen);
		//cout << "screen to grid" << endl;
}

C_CoordScreen::C_CoordScreen(int x_screen, int y_screen ): C_Coord(x_screen, y_screen){

		C_Set& settings=C_Set::Instances();
		if (x_screen < 0)
			x_screen = 0;
		if (y_screen < 0)
			y_screen = 0;
		if (x_screen > settings.getWindowWidth())
			x_screen = settings.getWindowWidth();
		if (y_screen > settings.getWindowHeight())
			y_screen = settings.getWindowHeight();

		S_Coord coord;
		coord.x = x_screen;
		coord.y = y_screen;
		C_CoordScreen tmp(coord);
		m_this.screen = tmp.getScreen ();
		m_this.grid = tmp.getGrid ();
}

C_CoordScreen::~C_CoordScreen()
{
};

