#include "weapons.h"
#include <cmath>

using namespace std;

C_Weapon::C_Weapon():m_name("CANON"),
		m_x_screen(0),
		m_y_screen(0),
		m_shooting(false),
		m_lastShootTime(0),
		m_dist(80),
		m_direction(UNKNOWN)
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
	m_direction(UNKNOWN)
{
	m_weapon.damage = damage;
	m_weapon.speedImpact = speedImpact;
	m_weapon.fireRate = fireRate;
	m_weapon.fireRange = fireRange;
}

C_Weapon::~C_Weapon()
{
}

void C_Weapon::change(string name, int damage, int fireRate, int fireRange)
{
 	m_name = name;
	m_weapon.damage = damage;
	m_weapon.speedImpact = 0;
	m_weapon.fireRate = fireRate;
	m_weapon.fireRange = fireRange;
}

void C_Weapon::displayStatus() const
{
 	cout << "\tWeapon: " << m_name << " (Damage: "<< m_weapon.damage << ", firerate: " << m_weapon.fireRate << ")" << endl;
}

int C_Weapon::getDamage() const
{
	return m_weapon.damage;
}

int C_Weapon::getSpeedImpact() const
{
	return m_weapon.speedImpact;
}

int C_Weapon::getFireRate() const
{
	return m_weapon.fireRate;
}

int C_Weapon::getFireRange() const
{
	return m_weapon.fireRange;
}

int C_Weapon::getXScreen() const
{
	return m_x_screen;
}

int C_Weapon::getYScreen() const
{
	return m_y_screen;
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

int C_Weapon::getDirection() const
{
	return m_direction;
}
S_Weapon C_Weapon::getWeaponInfo() const
{
	return m_weapon;
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
			angle = angle *180/3.14159265359;
			string direction;
			if (angle > -22.5 && angle <= 22.5){
				m_direction = SOUTH;
				}
			else if (angle > 22.5 && angle <= 67.5){
				m_direction = SOUTH_EAST;
			}
			else if(angle > 67.5 && angle <=112.5){
				m_direction = EAST;
				}
			else if(angle > 112.5 && angle <=157.5){
				m_direction = NORTH_EAST;
				}
			else if((angle > 157.5 && angle <=180) || (angle > -180 && angle <= -157.5)){
				m_direction = NORTH;
				}
			else if(angle > -157.5 && angle <= -112.5){
				m_direction = NORTH_WEST;
				}
			else if(angle > -112.5 && angle <=-67.5){
				m_direction = WEST;
				}
			else if(angle > -67.5 && angle <=-22.5){
				m_direction = SOUTH_WEST;
				}
			else{
				m_direction = UNKNOWN;
				}
			//cout << "Angle:"<< angle << " Direction: " << direction << endl;

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
