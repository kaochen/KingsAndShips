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

#include "shooter.h"
#include "grid.h"

#include "../texture.h"
#include "../locator.h"
#include "../message.h"

using namespace std;

C_Shooter::C_Shooter(S_UnitModel model):C_GameUnits(model),
	m_lastShootTime(0),
	m_cost(model.cost)
{
	m_weapon = new C_Weapon(model.weapon);
	string message = "Add new shooter: " + m_name +" life: "+ to_string(m_health) + " rank: "+ to_string(m_rank);
	C_Message::printM(message);
	m_coord.displayStatus();
}

C_Shooter::~C_Shooter()
{
	delete m_weapon;
}
C_GameUnits*  C_Shooter::searchNextTarget(string type)
{
	C_Grid& grid= C_Locator::getGrid();
	int gridSize = grid.getFullSize();
	//cout <<"search next target type:" << type << endl;
	int gridDiag = m_weapon->getFireRange();

	int x_grid = m_coord.getXGrid();
	int y_grid = m_coord.getYGrid();
	C_GameUnits* target = nullptr;

	map<int, C_GameUnits*> list;
	for(int y = (y_grid - gridDiag); y <= (y_grid + gridDiag); y++) {
		for(int x = (x_grid - gridDiag); x <= (x_grid + gridDiag); x++) {
			if((x != x_grid || y != y_grid)) {
				if(x >= 0 && x <= gridSize && y >= 0 && y <= gridSize) {
					//cout << "test: "<< x << ":" << y << endl;
					C_GameUnits* tmp = grid.getUnits(x,y);
					if(tmp != nullptr) {
						if(tmp->getType() == type) {
							int dist = getDistance(x,y);
							list[dist] = tmp;
						}
					}
				}
			}
		}
	}
	if(!list.empty()) {
		target = list.begin()->second;
	}

	return target;
}
void C_Shooter::shoot(std::string type[MAX_TARGETS], int nbrofTargets)
{
	for(int i = 0; i < nbrofTargets; i++) {
		C_GameUnits* target = searchNextTarget(type[i]);
		if(target != nullptr) {
			long currentTime = SDL_GetTicks();
			if ((currentTime ) > m_weapon->getLastShootTime() + m_weapon->getFireRate()) {
				m_weapon->setShooting(true);
				shootTarget(*target);
				i = MAX_TARGETS + 1;
				//cout << target.getName() << " has been shot" << endl;
			} else {
				m_weapon->setShooting(false);
			}
		} else {
			m_weapon->setShooting(false);
		}
	}
}

void C_Shooter::kill()
{
	C_Grid& grid= C_Locator::getGrid();
	C_Message::printM("kill " + m_name + " from:"+ to_string(m_coord.getXGrid ())
			 + ":" + to_string(m_coord.getYGrid ()) + "\n");
	grid.moveToDead(m_coord.getXGrid (), m_coord.getYGrid ());
	m_selected = false;
}

void C_Shooter::displayStatus()
{
	C_GameUnits::displayStatus();
	if(m_weapon != nullptr)
		m_weapon->displayStatus();
}


void C_Shooter::move()
{
}

void C_Shooter::upgrade(S_UnitModel model)
{
	m_rank = model.rank;
	m_name = model.name;

	if(m_weapon != nullptr){
		delete m_weapon;
	}
	m_weapon = new C_Weapon(model.weapon);
};


void C_Shooter::shootTarget(C_GameUnits &target)
{
	if(m_weapon->getShooting()) {
		bool test = m_weapon->shoot(*this, target);
		if (test) {
			target.receiveDamage(m_weapon->getWeaponInfo());
		}
	}
}

void C_Shooter::renderLifeBar(int x_screen, int y_screen)
{
    if(m_health < (m_max_health - (m_health/20))){
		C_TextureList& t= C_Locator::getTextureList();
		int x_size = 50;
		int y = y_screen - 80;
		int x = x_screen - (x_size/2);
		t.renderTexture("Menu_details_Progress_Border", x - 1,y,CENTER);
		t.renderTexture("Menu_details_Progress_Border", x + x_size + 1 ,y,CENTER);
		for(int i = 0; i <= x_size; i++){
			string textureName = "Menu_details_Progress_";
			textureName += "Back";
			t.renderTexture(textureName, x + i ,y,CENTER);
		}
		int life = (x_size*m_health/m_max_health);
		for(int i = 0; i <= life; i++){
			string image = "Menu_details_Progress_";
			if(i%5 == 0){
				image += "2_";
			} else {
				image += "1_";
			}
			if (m_health < m_max_health/2 && m_health > m_max_health/3){
				image += "Orange";
			} else if (m_health < m_max_health/3) {
				image += "Red";
			} else {
				image += "Green";
			}
			t.renderTexture(image, x + i ,y,CENTER);
		}
	}
}


void C_Shooter::render(S_Coord screen)
{
	C_GameUnits::render(screen);
	if (m_weapon->getShooting())
		m_weapon->render();
	renderLifeBar(screen.x, screen.y);
}

void C_Shooter::drag(S_Coord screen)
{

	m_justAdded = false;
	C_Grid& grid= C_Locator::getGrid();

	bool water = false;
	if(m_type == "barricade") {
		water = true;
	}
	C_CoordScreen coord(screen);

	int x = coord.getXGrid ();
	int y = coord.getYGrid ();

	bool status = grid.isThisConstructible(coord.getGrid(), water);
	//draw ellipse

	drawEllipse(screen.x,screen.y,m_weapon->getFireRange(), status);
	//draw square
	x -=2;
	y -=2;
	for(int i = 0; i < 3; i++) {
		y++;
		for(int j = 0; j < 3; j++) {
			x++;
			C_CoordGrid tmp(x,y);
			status = grid.isThisConstructible(tmp.getGrid (),water);

			int x_s = tmp.getXScreen ();
			int y_s = tmp.getYScreen ();

			C_TextureList& t= C_Locator::getTextureList();
			string color = "Green";
			string imageNbr = "01";
			if (i == 1 && j == 1) {
				imageNbr = "0" + to_string(m_animation[SELECT]->getLoopAnimNbr(1,8,40));
			}
			if(!status){ color = "Red";};

			t.renderTexture("Select_"+color+"_"+imageNbr, x_s,y_s);

		}
		x = coord.getXGrid () - 2;
	}

	screen.y -= (2*TILE_HALF_HEIGHT);
	render(screen);
}


void C_Shooter::drawEllipse(int x,int y,int size,bool ok)
{
	if(size>0){
		C_TextureList& t= C_Locator::getTextureList();

		string color = "Green";
		if(!ok){
			color= "Red";
			}
		int width = (TILE_HALF_WIDTH);
		int height = (TILE_HALF_HEIGHT);
		t.renderTexture("Select_Corner_"+color+"_EE", x+size*width,y+size*height);
		t.renderTexture("Select_Corner_"+color+"_SE", x,y+(size+1)*height);
		t.renderTexture("Select_Corner_"+color+"_SS", x-size*width,y+size*height);
		t.renderTexture("Select_Corner_"+color+"_SW", x-(size+1)*width,y);
		t.renderTexture("Select_Corner_"+color+"_WW", x-size*width,y-size*height);
		t.renderTexture("Select_Corner_"+color+"_NW", x,y-(size+1)*height);
		t.renderTexture("Select_Corner_"+color+"_NN", x+size*width,y-size*height);
		t.renderTexture("Select_Corner_"+color+"_NE", x+(size+1)*width,y);
	}

}


S_UnitModel C_Shooter::getInfo(){
	S_UnitModel unit;
	unit.name = m_name;
	unit.type = m_type;
	unit.rank = m_rank;
	unit.health = m_health;
	unit.coord = m_coord.getGrid();
	unit.cost = m_cost;
	unit.speed = 0;
	unit.alive = true;
	unit.weapon.type = m_weapon->getWeaponInfo().type;
	unit.weapon.damage = m_weapon->getWeaponInfo().damage;
	unit.weapon.speedImpact =  m_weapon->getWeaponInfo().speedImpact;
	unit.weapon.fireRate =  m_weapon->getWeaponInfo().fireRate;
	unit.weapon.fireRange =  m_weapon->getWeaponInfo().fireRange;
	unit.weapon.direction =  m_weapon->getWeaponInfo().direction;
	return unit;
}
