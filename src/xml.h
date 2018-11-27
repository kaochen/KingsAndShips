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
 *Functions to extract data from tmx and tsx files
 */

#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>

/*! \class C_Xml
 * \brief functions to extract data from tmx and tsx files
 */

struct S_tmxLayer{
  std::string name;
  int width;
  int height;
  std::string data;
};

class C_Xml
{
	public:
    C_Xml(std::string const file_Path);
    ~C_Xml(){};
    //values
    std::string extractStrValue(std::string const &node, std::string const &id, std::string const &idValue, std::string const &name); //in the "node" where "id" = "idValue" get value of "name"
    std::string extractStrValue(std::string const &node, std::string const &name); //in the only one "node" get value of "name"
    //data
    std::string extractStrData(std::string const &node, std::string const &name);                                                                      //
    S_tmxLayer extractLayerInTMX(std::string layerName);
    int countAttributes(std::string pattern);
  protected:

	private:
  std::string m_file_path;

};
#endif
