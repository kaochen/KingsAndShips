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

#include "../textureList.h"
#include "../menu/menuItems.h"
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
	t.renderTexture(fileName, m_coord.getXScreen (),m_coord.getYScreen (), true);
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
	C_Settings& settings= C_Locator::getSettings();
	int x = settings.getTileWidth();
	int y = settings.getTileHeight();

	if (m_selected == true) {
		drawEllipse(m_coord.getXScreen (),m_coord.getYScreen (),m_weapon->getFireRange(), true);
		renderTowerStatus("firerate",m_coord.getXScreen (), m_coord.getYScreen () + 2*y);
		renderTowerStatus("firerange",m_coord.getXScreen () -x, m_coord.getYScreen () + y);
		renderTowerStatus("Damage",m_coord.getXScreen () +x , m_coord.getYScreen () + y);
	}
}

void C_Towers::renderTowerStatus(std::string name, int x_screen, int y_screen){
        C_Button *b = new C_Button(name, name, x_screen, y_screen);
        S_Weapon w = m_weapon->getWeaponInfo();

        if(m_upgrade){ //if over mouse over upgrade button
		    C_Grid& grid= C_Locator::getGrid();
			C_UnitFactory factory = grid.getFactory();
			S_UnitModel up;
			if(factory.getSelectedModel(1,up)){ //if upgrade is possible get weapon info from unit + 1 level
			    w = up.weapon;
			}
		}

        string text;
		if(name == "firerange"){
		    text = to_string(w.fireRange);
        } else if (name == "firerate") {
            double f = 0.0;
            if(w.fireRate != 0) {
                f = w.fireRate/1000;
            }
        	text = to_string(f).substr(0,3) + " ms";
        } else if(name == "Damage") {
            text = to_string(w.damage);
        }
        b->setText(text);
		b->renderFlagUnderButton();
        b->render();
        delete b;
}

//---------------------------------------------------


C_ArcherTower::C_ArcherTower(S_UnitModel model):C_Towers(model)
{
    m_max_rank = 3;
	m_targetsTypes.push_back("boat");
	m_canRotate = false;
	m_isBottomAnimated = false;
}

C_Catapult::C_Catapult(S_UnitModel model):C_Towers(model)
{
    m_max_rank = 3;
	m_isBottomAnimated = false;
	m_isTopAnimated = true;
	m_haveABottom = false ;
	m_haveATop = true;
	m_targetsTypes.push_back("boat");
	m_anim.add(new C_Anim("Weapon_Shooting",0,7,120));
	m_anim.add(new C_Anim("Weapon_Reloading",8,15,120));
}

