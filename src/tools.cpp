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

#include "tools.h"
#include <fstream>
#include <unistd.h>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace std;

std::string C_Tools::extractFilename(std::string path){
	fs::path p = path;
	return p.filename();
}

bool C_Tools::fileExist(const std::string &file)
{
	return fs::exists(file);
}

std::vector <std::string> C_Tools::listFiles(std::string path){
	vector <string> ret;
	for(const auto & entry : fs::directory_iterator(path)){
		ret.push_back(entry.path());
	}
	return ret;
}


bool C_Tools::checkExtension(std::string path, std::string extension){
	bool ret = false;
	fs::path p = path;
	string ext =  p.extension();
	if(ext == extension){
		ret = true;
	}
	return ret;
}
