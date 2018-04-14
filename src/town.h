#ifndef TOWN_H
#define TOWN_H

#include <iostream>
#include <string>
#include "shooter.h"

class C_Town: public C_Shooter
{
  public:
    C_Town(int x_grid, int y_grid);
  protected:
  	virtual void play();
  	virtual void render(S_Coord screen);
  private:
};

#endif
