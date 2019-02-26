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
/*!
 * \file locator.h
 */

#ifndef LOCATOR_H
#define LOCATOR_H

#include <iostream>
#include "message.h"
#include "settings.h"
#include "time.h"
#include "menu/menu.h"
#include "texture.h"
#include "window.h"
#include "wallet.h"
#include "level/grid.h"

/*! \class C_Locator
 * \brief C_Locator find services .
 * http://gameprogrammingpatterns.com/service-locator.html
 */

class C_Menu;

class C_Locator
{
public:
	static C_Time& getTime() { return *m_time; };
	static C_Menu& getMenu() { return *m_menu; };
	static C_TextureList& getTextureList() { return *m_texturelist; };
	static C_Window& getWindow() { return *m_window; };
	static C_Wallet& getWallet() { return *m_wallet; };
	static C_Grid& getGrid() { return *m_grid; };
	static C_Settings& getSettings() { return *m_settings; };

	static void setService(C_Time* service){m_time = service;};
	static void setService(C_Menu* service){m_menu = service;};
	static void setService(C_TextureList* service){m_texturelist = service;};
	static void setService(C_Window* service){m_window = service;};
	static void setService(C_Wallet* service){m_wallet = service;};
	static void setService(C_Grid* service){m_grid = service;};
	static void setService(C_Settings* service){m_settings = service;};

private:
	static C_Time* m_time;
	static C_Menu* m_menu;
	static C_TextureList* m_texturelist;
	static C_Window* m_window;
	static C_Wallet* m_wallet;
	static C_Grid* m_grid;
	static C_Settings* m_settings;
};

#endif

