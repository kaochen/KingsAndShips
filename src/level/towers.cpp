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


using namespace std;


C_Towers::C_Towers(S_UnitModel model):C_Shooter(model)
{
	m_lastSmokeTime = 0;
	m_smokeNbr = 1;
	m_justAdded = true;
}

void C_Towers::play()
{
	string list[1] = {"boat"};
	this->shoot(list,1);
	if(!this->alive())
		this->kill();
}

void C_Towers::renderSmoke()
{
	int imageNbr = m_animation[JUSTADDED]->getAnimNbr(0,7,100);
	//smoke_01_smoke0
	string fileName = "smoke_01_smoke" + to_string(imageNbr);
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture(fileName, m_coord.getXScreen (),m_coord.getYScreen ());
	if (imageNbr == 7)
		m_justAdded = false;
}

void C_Towers::render(S_Coord screen)
{
	renderSelected();
	C_Shooter::render(screen);
	if (m_justAdded)
		renderSmoke();
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
		string textureName = "Buttons_"+ name + "_Active";
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture(textureName, x_screen,y_screen);
		int value = 0;
		if(name == "firerange"){
			value = (m_weapon->getFireRange()*100)/8;
		} else if(name == "firerate") {
			if(m_weapon->getFireRate() != 0){
				int maxSpeed = 5000;
				value = (((maxSpeed - m_weapon->getFireRate())*100)/maxSpeed);
			}
		} else if(name == "Damage") {
			value = (m_weapon->getDamage()*100)/40;
		}
		int max = 0;
		if(value != 0){
			max = (value*72)/100;
		}
		for(int i = 0; i < max; i++){
		double angle = 5.0 * i;
		t.renderTextureEx("Buttons_Torus_Green", x_screen,y_screen-24, angle, CENTER);
		}

}


//---------------------------------------------------


C_ArcherTower::C_ArcherTower(S_UnitModel model):C_Towers(model)
{
}

void C_ArcherTower::render(S_Coord screen)
{
	if(alive()) {
		renderSelected();
		C_GameUnits::render(screen);
		renderLifeBar(screen.x, screen.y);
		if (m_weapon->getShooting())
			m_weapon->render();

		if (m_justAdded)
			renderSmoke();
	}

}


C_Turbine::C_Turbine(S_UnitModel model):C_Towers(model)
{
}

void C_Turbine::render(S_Coord screen)
{
	if(alive()) {
		renderSelected();
		renderLifeBar(screen.x, screen.y);

		S_Weapon current = m_weapon->getWeaponInfo();
		int rotationSpeed = 200;
		if (m_weapon->getShooting())
			rotationSpeed = 50;
		int imageNbr = m_animation[MAIN_ANIM]->getAnimNbr(0,7,rotationSpeed);

		string fileName = imageName(ALIVE,current.direction,imageNbr);
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);

		if (m_justAdded)
			renderSmoke();
	}
}

