#include "invaders.h"

using namespace std;

C_invaders::C_invaders():C_Shooter("boat",1)
{
}


C_invaders::C_invaders(int rank):C_Shooter("boat", rank)
{
}


void C_invaders::displayStatus() const
{
	C_Shooter::displayStatus();
 	cout << "\tx:"<< m_x << " y:"<< m_y << endl;
}


