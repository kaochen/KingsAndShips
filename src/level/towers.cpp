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

#include "towers.h"
#include "weapons.h"

#include "../texture.h"
#include "../locator.h"
#include "../anim.h"



using namespace std;


C_Towers::C_Towers(S_UnitModel model):C_Shooter(model)
{
	m_targetsTypes.push_back("boat");
	m_renderDead = false;
}


void C_Towers::renderSmoke()
{
	int imageNbr = m_anim.getImageNbr("JustAdded");
	m_anim.get("JustAdded")->play();

	//smoke_01_smoke0
	string fileName = "smoke_01_smoke" + to_string(imageNbr);
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, m_coord.getXScreen (),m_coord.getYScreen ());
}

void C_Towers::render(S_Coord screen)
{
	if (!m_anim.end("JustAdded")){
		renderSmoke();
	}
	if(alive()){
		renderSelected();
	}
	C_Shooter::render(screen);
}


void C_Towers::renderSelected()
{
	if (m_selected == true) {
		drawEllipse(m_coord.getXScreen (),m_coord.getYScreen (),m_weapon->getFireRange(), true);
		renderTowerStatus("firerate",m_coord.getXScreen (), m_coord.getYScreen () + 90);
		renderTowerStatus("firerange",m_coord.getXScreen () -50, m_coord.getYScreen () + 80);
		renderTowerStatus("Damage",m_coord.getXScreen () +50, m_coord.getYScreen () + 80);
	}
}

void C_Towers::renderTowerStatus(std::string name, int x_screen, int y_screen){
		C_TextureList& t= C_Locator::getTextureList();
		string textureName = "Buttons_"+ name + "_Active";
		t.renderTexture(textureName, x_screen,y_screen);
		int fireRange = m_weapon->getFireRange();
		int fireRate = m_weapon->getFireRate();
		int damage = m_weapon->getDamage();
		if(m_upgrade){
		C_Grid& grid= C_Locator::getGrid();
				C_UnitFactory factory = grid.getFactory();
				S_UnitModel up;
				bool check = factory.getSelectedModel(1,up);
				if(check){
					renderTowerStatusCircle(name, x_screen, y_screen, up.weapon.fireRate,up.weapon.fireRange, up.weapon.damage, "Dark");
				}
		}
		renderTowerStatusCircle(name, x_screen, y_screen, fireRate,fireRange, damage, "Green");

}
void C_Towers::renderTowerStatusCircle(std::string name, int x_screen, int y_screen, int fireRate, int fireRange, int damage, std::string color){
		C_TextureList& t= C_Locator::getTextureList();
		int value = 0;
		if(name == "firerange"){
			value = (fireRange*100)/8;
		} else if(name == "firerate") {
			if(m_weapon->getFireRate() != 0){
				int maxSpeed = 5000;
				value = (((maxSpeed - fireRate)*100)/maxSpeed);
			}
		} else if(name == "Damage") {
			value = (damage*100)/40;
		}
		int max = 0;
		if(value != 0){
			max = (value*72)/100;
		}
		for(int i = 0; i < max; i++){
		double angle = 5.0 * i;
		t.renderTextureEx("Buttons_Torus_"+ color, x_screen,y_screen-24, angle, CENTER);
		}
}



//---------------------------------------------------


C_ArcherTower::C_ArcherTower(S_UnitModel model):C_Towers(model)
{
	m_targetsTypes.push_back("boat");
	m_canRotate = false;
	m_isBottomAnimated = false;
}

C_Catapult::C_Catapult(S_UnitModel model):C_Towers(model)
{
	m_isBottomAnimated = false;
	m_isTopAnimated = true;
	m_haveABottom = false ;
	m_haveATop = true;
	m_targetsTypes.push_back("boat");
	m_anim.add(new C_Anim("Weapon_Shooting",0,5,100));
	m_anim.add(new C_Anim("Weapon_Reloading",6,11,120));
}

