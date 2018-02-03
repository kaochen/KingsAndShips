#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "game.h"
#include "time.h"
#include "settings.h"

#include "pathfinding.h"

class C_GameUnits
{
	public:
	struct S_layer{
		C_GameUnits * main;
		C_GameUnits * dead;
		bool path;
		int ground;
		bool plot;
		bool water;

	};
	//methods
	C_GameUnits(std::string name,
		     int x_grid,
		     int y_grid,
		     int rank);

	virtual ~C_GameUnits();
	virtual void play() = 0;
	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void render(S_Coord screen);
	virtual void shoot(C_GameUnits &target) =0;
	virtual void stopShooting() =0;
	virtual void receiveDamage(S_Weapon weapon);
	virtual bool alive() const;
	virtual int getRank() const;

	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual void setGridXY(int x, int y);
	virtual void move() = 0;


	virtual int getXScreen() const;
	virtual int getYScreen() const;

	virtual int getYCenterOffset() const;
	virtual int getDistance(int x, int y) const;


	virtual int testFirerange(C_GameUnits &target) = 0;
	virtual void renderMissile() = 0;

	virtual void kill();

	virtual bool getSelectedStatus() const;
	virtual void setSelectedStatus(bool status);
	virtual void reverseSelectedStatus();

	virtual void setDirection(std::string strDirection);
	virtual std::string intDirectionToStr(int direction);

	virtual void changeDirection(int x_cursor, int y_cursor);
	virtual C_GameUnits * getUnit();

	protected:
	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;

	C_Coord* m_coord;
	int m_y_center_offset;
	std::string m_strDirection;
	//Store time for animation
	C_AnimTime* m_animation[5];

	int m_direction;
	int m_lastDirection;
	bool m_selected;
};

#endif
