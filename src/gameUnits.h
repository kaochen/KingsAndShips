#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
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
	virtual void move() = 0;

	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void render(S_Coord screen);
	virtual void shoot() =0;
	virtual void receiveDamage(S_Weapon weapon);

	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual int getXScreen() const;
	virtual int getYScreen() const;

	virtual bool getSelectedStatus() const;
	virtual void setSelectedStatus(bool status);
	virtual void reverseSelectedStatus();

	virtual void changeDirection(int x_cursor, int y_cursor);
	virtual C_GameUnits * getUnit();

	protected:
	virtual bool alive() const;
	virtual void kill();
	virtual int getDistance(int x, int y) const;

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
