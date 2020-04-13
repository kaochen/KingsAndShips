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
#include <vector>

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
    virtual ~C_Xml(){};

    //values
    virtual std::string extractStrValue(std::string const &node, std::string const &id, std::string const &idValue, std::string const &name); //in the "node" where "id" = "idValue" get value of "name"
    virtual std::string extractStrValue(std::string const &node, std::string const &name); //in the only one "node" get value of "name"
                                                                                           //
    //data
    virtual std::string extractStrData(std::string const &node, std::string const &name);                                                                      //
    virtual int countAttributes(std::string pattern);
    virtual int getIntProperty(std::string const &idValue, int Default);
    virtual std::string getStrProperty(std::string const &property, std::string Default);
    virtual bool nodeExist(std::string const &node, std::string const &name);

protected:
    std::string m_file_path;

};

class C_Tileset{
public :
    C_Tileset(std::string source, int first);
    ~C_Tileset(){};
    void setLast(int last){m_last = last;};
    int getFirst(){return m_first;};
    void show();
private:
    std::string m_source;
    std::string m_type;
    int m_first;
    int m_last;
};

class C_Tmx : public C_Xml
{
public:
    C_Tmx(std::string const file_Path);
    virtual S_tmxLayer extractLayerInTMX(std::string layerName);
protected:
    std::vector <C_Tileset> extractTilesetList();
    void showAllTileset();
    void calcTilesetLast();
    std::vector <C_Tileset> m_tilesetList;
};
#endif
