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
  void renderWater(int direction);
  int m_waterDrift;
  C_AnimTime* m_animWater;
};

#endif
