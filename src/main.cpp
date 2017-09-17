#include <iostream>
#include <string.h>

#include "towers.h"
#include "invaders.h"

using namespace std;


int main()
{
	C_invaders soldier1("SOLDIER","GUN",10,3);
	soldier1.displayStatus();
	C_Towers tower1;
	tower1.shoot(soldier1);
	tower1.shoot(soldier1);
	soldier1.displayStatus();
	return 0;
}
