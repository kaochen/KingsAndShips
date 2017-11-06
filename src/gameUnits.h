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
	virtual void render(int x_screen,int y_screen,SDL_Renderer *renderer);
	virtual void shoot(C_GameUnits &target) =0;
	virtual void stopShooting() =0;
	virtual void receiveDamage(int nbDamage);
	virtual bool alive() const;
	virtual int getRank() const;

	virtual int getXGrid() const;
	virtual int getYGrid() const;
	virtual void setGridXY(int x, int y);
	virtual void move(int direction,
		      S_layer grid[][TABLE_SIZE]) = 0;


	virtual void xyGridToXYScreen();
	virtual int getXScreen() const;
	virtual int getYScreen() const;
	virtual void xyScreenToXYGrid();
	virtual int getYCenterOffset() const;
	virtual int getDistance(int x, int y) const;


	virtual int testFirerange(C_GameUnits &target) = 0;
	virtual void renderMissile(SDL_Renderer *renderer) = 0;

	virtual void del(S_layer grid[][TABLE_SIZE]);


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
};

	void displayGridStatus(C_GameUnits::S_layer grid[][TABLE_SIZE]);
#endif
