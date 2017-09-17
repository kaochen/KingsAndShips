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
	C_Towers tower2(tower1);
	if(tower1 == tower2)
		cout << tower1.getName() << " is equal to " << tower2.getName() << endl;
	else
		cout << tower1.getName() << " is different from " << tower2.getName() << endl;

	tower1.shoot(soldier1);
	tower2.shoot(soldier1);
	soldier1.displayStatus();
	return 0;
}
