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

#include "config.h"
#include "locator.h"
#include "window.h"
#include "menu/menu.h"
#include "level/grid.h"
#include "wallet.h"
#include "settings.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <libintl.h>
#include <locale.h>

using namespace std;

int main(int argc, char** argv)
{

     /* Set up gettext for internationalisation */
  	setlocale (LC_ALL, "");
  	textdomain ("kingsandships");
  	bindtextdomain ("kingsandships", LOCALE_FOLDER);

	vector<string> args;
	for (int i = 0; i < argc; ++i)
		args.push_back(argv[i]);

	//init random
	srand(time(NULL));
	//init services
	C_Locator::setService(new C_Settings(args));
	C_Locator::setService(new C_Time());
	C_Locator::setService(new C_TextureList());
	C_Locator::setService(new C_Grid());
	C_Locator::setService(new C_Wallet());
	C_Locator::setService(new C_Menu());
	C_Locator::setService(new C_Window());



	//start the window
	C_Window& win= C_Locator::getWindow();
	win.createWindow();
	win.loadGame();
	win.listLevels();
	win.gameLoop();
	win.quitProgram();

	C_Locator::deleteAllService();
	return 0;
};
