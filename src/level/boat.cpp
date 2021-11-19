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

#include "boat.h"
#include "grid.h"

#include "../window.h"
#include "../locator.h"
#include "../settings.h"


using namespace std;

C_Boat::C_Boat(S_UnitModel model):C_Shooter(model)
{
	m_speed = model.speed;
	m_coord.centerOnTile();
	//Find a way to town
	C_Grid& grid= C_Locator::getGrid();
	S_Coord town = grid.foundTown();
	m_C_Path = new C_Path(town.x,town.y);
	m_C_Path->calcPath(model.coord.x,model.coord.y,town.x,town.y);
	m_C_Path->showPath();
	m_direction = EAST;
	m_countRegenPath = 0;
	m_targetsTypes.push_back("town");
	m_targetsTypes.push_back("barricade");
	m_targetsTypes.push_back("ArcherTower");
	m_targetsTypes.push_back("Catapult");
	m_state ="Moving";
	m_anim.add(new C_AnimRewind("Moving",1,7,80));
	m_anim.add(new C_Anim("Waiting",1,1,800));
	m_anim.add(new C_Anim("Dying",1,7,100));
	m_anim.add(new C_Anim("Dead",7,7,800));
	m_anim.add(new C_Anim("Gold",1,16,40));

	m_canRotate = true;
	m_isBottomAnimated = true;
	m_isTopAnimated = false;
	m_haveATop = false;
	m_haveABottom = true;
	m_step = {0,0};
}

C_Boat::~C_Boat()
{
	delete m_C_Path;
}

void C_Boat::play()
{
	if(alive()){
		if(m_state == "Waiting"){
			if(m_anim.end(m_state)){
				changeState("Moving");
			}
		}
		if(m_state == "Moving"){
			move();
		}
		shoot();
	} else {
	    if(m_state != "Dying" && m_state != "Dead"){
		    changeState("Dying");
		}
		if(m_state == "Dying"){
			m_anim.get("Gold")->play();
			if(m_anim.end("Dying")){
				changeState("Dead");
				kill();
			}
		}
	}
	m_anim.get(m_state)->play();
};

void C_Boat::kill()
{
	C_Shooter::kill();
	C_Wallet& wallet= C_Locator::getWallet();
	wallet.credit(m_cost); //reward when killing a boat
	wallet.cliStatus();
}

void C_Boat::move()
{
	C_Grid& grid= C_Locator::getGrid();

	if(m_C_Path->closeToDestination(m_coord.getXGrid(),m_coord.getYGrid(),1) || m_C_Path->getPath().size() <= 1) {
			changeState("Waiting");
	} else {
			//old
			C_Coord oldCoord = m_coord;
			//destination
			C_Coord destCoord = m_C_Path->getPath().top()->getCoord();
			destCoord.centerOnTile();

			//orientation
			double angle = calcAngle(destCoord);
			m_direction = destCoord.angleToDirection(angle);

			//simulate next move
			C_Coord next = m_coord;
			next.move(angle, m_speed);
			next.refreshGrid();

		//check if next tile is available
		if(!grid.mainEmpty(next.getXGrid(),next.getYGrid(),this)) {
			if(m_state == "Waiting"){
				changeState("Moving");
			}
			//move
			m_coord = next;
			grid.moveUnit(oldCoord.getXGrid(), oldCoord.getYGrid(),  m_coord.getXGrid (), m_coord.getYGrid ());

			//got next
			if(m_coord.atCenter(destCoord.getGrid())) {
				m_coord.centerOnTile(); //to not deviate too much from the path
				m_countRegenPath++;
				m_C_Path->goNextStep();
			}
			//recalc path anyway
			if(m_countRegenPath > 3) {
				S_Coord finalDestination = grid.foundTown();
				recalcPath(finalDestination);
				m_countRegenPath = 0;
			}
		} else {
			changeState("Waiting");

			if(!m_C_Path->closeToDestination(m_coord.getXGrid(),m_coord.getYGrid(),3)){
				S_Coord finalDestination = grid.foundTown();
				recalcPath(finalDestination);
			}
		}
	}
}


float C_Boat::calcAngle(C_Coord destCoord){
	C_Coord startCoord = m_coord;
	//startCoord.centerOnTile();
	S_Coord start = startCoord.getScreen();
	S_Coord dest = destCoord.getScreen();
	int ab = dest.x - start.x;
	int bc = dest.y - start.y;
	return startCoord.atan2_360(ab,bc);
}



void C_Boat::render(S_Coord screen)
{
	int imageNbr = 0;
	C_TextureList& t= C_Locator::getTextureList();
	if(m_state != "Dying" && m_state != "Dead"){
		if(m_isBottomAnimated){
			imageNbr = m_anim.getImageNbr(m_state);
		}
		if(m_haveABottom){
			string fileName = imageName(ALIVE,m_direction,imageNbr);
			t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE, true);
		}
		if(m_isTopAnimated){
			imageNbr = m_anim.getImageNbr(m_weaponState);
		}
		if( m_haveATop){
			string fileName = imageName(WEAPON,m_direction,imageNbr);
			t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE, true);
		}
		//life bar on top
		if(m_renderLifeBar){
			renderLifeBar(screen.x, screen.y);
		}
	} else {
		int status = DYING;
		if(m_state == "Dead"){
    	    status = DEAD;
		}
		string fileName = imageName(status,m_direction,m_anim.getImageNbr(m_state));
		t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE, true);
		if(m_state == "Dying"){
			fileName = "gold_anim_" + to_string(m_anim.getImageNbr("Gold"));
			t.renderTexture(fileName, screen.x,screen.y - 60,CENTER_TILE, true);
		}
	}

	m_C_Path->displayPath();
}



void C_Boat::recalcPath(S_Coord dest)
{
	C_Coord oldNext = m_C_Path->getPath().top()->getCoord();
	delete m_C_Path;
	m_C_Path = new C_Path(dest.x,dest.y);
	m_C_Path->calcPath(m_coord.getXGrid(),m_coord.getYGrid(),dest.x,dest.y);
	C_Coord newNext = m_C_Path->getPath().top()->getCoord();
	if(oldNext.getXGrid() != newNext.getXGrid() || oldNext.getYGrid() != newNext.getYGrid() ){
			//force passing by the center of the tile before going to the next step
			if(!m_coord.closeToCenter(m_coord.getGrid(),12)) {
				m_C_Path->addANodeAtTheStartOfThePath(m_coord.getGrid());
			} else {
				m_coord.centerOnTile();
			}
	}
	m_C_Path->showPath();

}

