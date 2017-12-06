#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "game.h"
#include "settings.h"



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
	C_GameUnits(C_GameUnits const& original);
	virtual ~C_GameUnits();

	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void render(int x_screen,int y_screen);
	virtual void shoot(C_GameUnits &target) =0;
	virtual void stopShooting() =0;
	virtual void receiveDamage(int nbDamage);
	virtual bool alive() const;
	virtual int getRank() const;

	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual void setGridXY(int x, int y);
	virtual void move(int direction) = 0;


	virtual int getXScreen() const;
	virtual int getYScreen() const;

	virtual int getYCenterOffset() const;
	virtual int getDistance(int x, int y) const;


	virtual int testFirerange(C_GameUnits &target) = 0;
	virtual void renderMissile() = 0;

	virtual void kill();

	int getAnimTileNbr(int firstTileNbr, int lastTileNbr, long delay);
	int getAnim2TileNbr(int firstTileNbr, int lastTileNbr, long delay);

	protected:
	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;
	int m_x_grid;
	int m_y_grid;
	int m_x_screen;
	int m_y_screen;
	int m_y_center_offset;
	//Store time for animation
	int m_animNbr;
	long m_lastAnimTime;
	int m_anim2Nbr;
	int m_lastAnim2Time;
	int m_direction;
};

#endif
