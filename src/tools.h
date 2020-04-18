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


#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <vector>

class C_Tools {
public:
	static std::string extractFilename(std::string path);
	static bool checkExtension(std::string path, std::string extension);
	static bool fileExist(const std::string &file);
	static std::vector <std::string> listFiles(std::string path);
};


#endif
