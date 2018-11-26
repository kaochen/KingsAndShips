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


#include "factory.h"
#include "grid.h"

using namespace std;

C_UnitFactory::C_UnitFactory(){
    C_Xml tsx("data/img/boat_00.tsx");
    tsx.extractStrValue("property","name","damage","value");
    tsx.extractStrValue("property","name","damage","type");
    C_Xml tmx("data/levels/Level_1.tmx");
    tmx.extractStrValue("map","orientation","isometric","backgroundcolor");

    S_UnitModel boat00;
    boat00.name = "boat";
    boat00.rank = 1;
    boat00.coord.x = 0;
    boat00.coord.y = 0;
    boat00.alive = true;
    m_models[boat00.name]= boat00;
}
C_GameUnits* C_UnitFactory::create(string type, S_Coord pos){
    C_GameUnits* unit = nullptr;
    if(type == "boat" ){
        S_UnitModel current = m_models[type];
        current.coord = pos;
        unit = new C_Boat(current);
        //C_GameUnits *boat = new C_ArcherTower(pos.x,pos.y,current.rank);
    }
    return unit;
}

