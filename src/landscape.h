#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <iostream>
#include <string>
#include "gameUnits.h"
#include "surfaces.h"
#include "grid.h"

class C_Landscape
{
	public:
    C_Landscape();
    ~C_Landscape();
    void display();
	protected:
	private:
  //methods
  void renderWater(int direction);
  int waterDirection();

  //attributes
  S_Coord m_waterDrift;
  int m_waterDirection;
  C_AnimTime* m_animWater;
};

class C_Decors : public C_GameUnits
{
	public:
	  C_Decors(std::string name, int x_grid, int y_grid);
  	virtual void play();
	  virtual void move();
  	virtual void shoot(std::string type[MAX_TARGETS], int nbrofTargets);
    virtual void render(S_Coord screen);
  protected:
  private:
};
#endif
