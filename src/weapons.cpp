#include "weapons.h"
#include "surfaces.h"
#include <cmath>

using namespace std;

C_Weapon::C_Weapon():m_name("CANON"),
		m_x_screen(0),
		m_y_screen(0),
		m_shooting(false),
		m_lastShootTime(0),
		m_dist(80)
{
	m_weapon.damage = 10;
	m_weapon.speedImpact = 0;
	m_weapon.fireRate = 500;
	m_weapon.fireRange = 2;

}

C_Weapon::C_Weapon(std::string name, int damage,int speedImpact, int fireRate, int fireRange):
	m_name(name),
	m_x_screen(0),
	m_y_screen(0),
	m_shooting(false),
	m_lastShootTime(0),
	m_dist(80),
	m_angle(0.0)
{
	m_weapon.damage = damage;
	m_weapon.speedImpact = speedImpact;
	m_weapon.fireRate = fireRate;
	m_weapon.fireRange = fireRange;
	m_weapon.moveImpact.x = 0;
	m_weapon.moveImpact.y = 0;
}

C_Weapon::~C_Weapon()
{
}

void C_Weapon::change(string name, int damage, int fireRate, int fireRange)
{
 	m_name = name;
	m_weapon.damage = damage;
	m_weapon.fireRate = fireRate;
	m_weapon.fireRange = fireRange;
	m_weapon.moveImpact.x = 0;
	m_weapon.moveImpact.y = 0;
	m_angle = 0.0;
}

void C_Weapon::displayStatus() const
{
 	cout << "\tWeapon: " << m_name << " (Damage: "<< m_weapon.damage << ", firerate: " << m_weapon.fireRate << ")" << endl;
}

int C_Weapon::getDamage() const
{
	return m_weapon.damage;
}

int C_Weapon::getFireRate() const
{
	return m_weapon.fireRate;
}

int C_Weapon::getFireRange() const
{
	return m_weapon.fireRange;
}


bool C_Weapon::getShooting() const
{
	return m_shooting;
}

long C_Weapon::getLastShootTime() const
{
	return m_lastShootTime;
}

void C_Weapon::setShooting(bool status)
{
	m_shooting = status;
}


S_Weapon C_Weapon::getWeaponInfo() const
{
	//cout <<"weapon offset "<< m_weapon.moveImpact.x  << ":" << m_weapon.moveImpact.y << endl;  ;
	return m_weapon;
}


S_Coord C_Weapon::getMoveImpact() const
{
	return m_weapon.moveImpact;
}

bool C_Weapon::shoot(C_GameUnits &shooter, C_GameUnits &target){
			int x_s_target = target.getXScreen();
			int y_s_target = target.getYScreen();
			int x_s_shooter = shooter.getXScreen();
			int y_s_shooter = shooter.getYScreen() + 30;
			int ab = x_s_target - x_s_shooter;
			int bc = y_s_target - y_s_shooter;
			int hyp = sqrt((ab*ab + bc*bc));
			hyp -= hyp*m_dist/100;
			double angle = atan2(ab,bc);
			int newA = hyp*sin(angle);
			int newB = hyp*cos(angle);
			m_angle = 180 - (angle *180/3.14159265359);
			if(m_angle < 0)
				m_angle +=360;
			//cout << "angle: " << m_angle << endl;

			m_x_screen = x_s_shooter + newA;
			m_y_screen = y_s_shooter + newB;
			m_dist -= 10;
			if (m_dist < 20){
				m_dist = 80;
				m_lastShootTime = SDL_GetTicks();
				return true;
				}
			return false;
}


void C_Weapon::render(){
		C_TextureList& t=C_TextureList::Instances();
		t.renderTextureEx("Arrow01.png", m_x_screen,m_y_screen,m_angle);
}


void C_Weapon::changeDirection(string direction){

		if  (direction == "NE"){
			m_weapon.moveImpact.x = 2;
			m_weapon.moveImpact.y = 0;
			}
		else if(direction == "SW"){
			m_weapon.moveImpact.x = -2;
			m_weapon.moveImpact.y = 0;
			}
		else if(direction == "NW"){
			m_weapon.moveImpact.x = 0;
			m_weapon.moveImpact.y = -2;
			}
		else if(direction == "SE"){
			m_weapon.moveImpact.x = 0;
			m_weapon.moveImpact.y = 2;
			}
		else if(direction == "EE"){
			m_weapon.moveImpact.x = 1;
			m_weapon.moveImpact.y = 1;
			}
		else if(direction == "SS"){
			m_weapon.moveImpact.x = -1;
			m_weapon.moveImpact.y = 1;
			}
		else if(direction == "WW"){
			m_weapon.moveImpact.x = -1;
			m_weapon.moveImpact.y = -1;
			}
		else if(direction == "NN"){
			m_weapon.moveImpact.x = 1;
			m_weapon.moveImpact.y = -1;;
			}
		else{
			cout <<"\""<<direction << "\" unknow weapon direction";
		}
			//cout <<"weapon offset "<< m_weapon.moveImpact.x  << ":" << m_weapon.moveImpact.y << endl;  ;
}

