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


#include "xml.h"
#include "tools.h"
#include "message.h"
#include "locator.h"
#include "textureList.h"
#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

using namespace std;

C_Xml::C_Xml(string const file_Path):
	m_file_path(file_Path)
{
}

//<map version="1.2" tiledversion="1.2.0" orientation="isometric" renderorder="left-up" width="30" height="30" tilewidth="100" tileheight="50" infinite="0" backgroundcolor="#174c89" nextlayerid="9" nextobjectid="1">

//<property name="rank" type="int" value="1"/> tsx files
//<property name="wallet" value="200"/> tmx files

string C_Xml::extractStrValue(string const &node, string const &id, string const &idValue, string const &name)
{
	xmlpp::TextReader reader(m_file_path);
	string value;
	while(reader.read()) {
		string nodeName = reader.get_name();

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attrib = reader.get_name();
				if (nodeName == node && attrib == id) {
					string readName = reader.get_value();
					if(readName == idValue) {
						reader.move_to_first_attribute();
						do {
							string attrib2 = reader.get_name();
							if(attrib2 == name) {
								value = reader.get_value();
							}
						} while(reader.move_to_next_attribute());

					}
				}
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
	}
	string filename = C_Tools::extractFilename(m_file_path);
	C_Message::printV("From: " + filename +" in Node: "+ node + " where "+id+ "="+ idValue +", " +name+ " = "+ value+"\n");
	return value;
}



string C_Xml::extractStrValue(string const &node, string const &name)
{
	xmlpp::TextReader reader(m_file_path);
	string value;
	while(reader.read()) {
		string nodeName = reader.get_name();

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attrib = reader.get_name();
				if (nodeName == node && attrib == name) {
					value = reader.get_value();
				}
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
	}
	string filename = C_Tools::extractFilename(m_file_path);
	C_Message::printV("From: " + filename +" in Node \""+ node  +"\": " + name+ " = "+ value+"\n");
	return value;
}

string C_Xml::extractStrData(string const &node, string const &name)
{
	xmlpp::TextReader reader(m_file_path);
	string data;
	string currentNodeName;
	while(reader.read()) {
		string nodeType = reader.get_name();

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attrib = reader.get_name();
				if (nodeType == node && attrib == "name") {
					currentNodeName = reader.get_value();
				}
				if (nodeType == "data" && currentNodeName == name) {
					data = reader.read_inner_xml();
					currentNodeName ="";

				}
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
	}
	string filename = C_Tools::extractFilename(m_file_path);
	C_Message::printDebug("From: " + filename +" in Node \""+ node  +"\": " + name+ " = "+ data+"\n");
	return data;
}



int C_Xml::countAttributes(string pattern)
{
	string old;
	xmlpp::TextReader reader(m_file_path);
	int c = 0;
	while(reader.read()) {
		string nodeName = reader.get_name();
		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attributes = reader.get_name();
				if (nodeName == "layer" && attributes == "name") {
					string value = reader.get_value();
					//cout << value << endl;
					if(value.compare(0,pattern.size(),pattern)==0 && old != value) {
						c++;
						old = value;
					}
				}
				attributes = "";
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
		nodeName = "";
	}
	string filename = C_Tools::extractFilename(m_file_path);
	C_Message::printV(to_string(c) +" "+ pattern + " in " + filename +"\n");
	return c;
}

int C_Xml::getIntProperty(string const &idValue, int Default)
{
	int ret = Default;
	if(nodeExist("property", idValue)) {
		string text = extractStrValue("property","name",idValue,"value");

		int False = 0;
		if(text.size()>0) {
			for(size_t i = 0; i < text.size(); i++) {
				if(!isdigit(text[i])) {
					C_Message::printV("name="+ idValue+" value= "+ text +" is not a number, It is replaced by \"Default\": "+ to_string(Default) + "\n");
					False++;
				}
			}
			if(False <= 0) {
				ret = stoi(text);
			}
		} else {
			C_Message::printV("name="+ idValue+" value= "+ text +"  is empty It is replaced by \"Default\": "+ to_string(Default) + "\n");
			ret = Default;
		}
	} else {
		C_Message::printV("No property named: "+ idValue + " in " + m_file_path + "-> Apply \"Default\": " + to_string(Default) +"\n");
	}
	return ret;
}


bool C_Xml::nodeExist(string const &node, string const &name)
{
	xmlpp::TextReader reader(m_file_path);
	string value;
	bool ret = false;
	while(reader.read()) {
		string nodeName = reader.get_name();

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attrib = reader.get_name();
				if (nodeName == node && attrib == "name") {
					value = reader.get_value();
					if(value == name) {
						ret = true;
					}
				}
			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
	}

	if(!ret) {
		string filename = C_Tools::extractFilename(m_file_path);
		C_Message::printError("From: " + filename +" the Node: "+ node  +" where " + "name="+ name + " does not exist\n");
	}

	return ret;
}

string C_Xml::getStrProperty(string const &property, string Default)
{
	string ret = extractStrValue("property","name",property,"value");
	if(ret.size()==0) {
		ret = Default;
		string filename = C_Tools::extractFilename(m_file_path);
		C_Message::printV("From: " + filename +" in Node: property where name="+ property + " is empty. -> Apply \"Default\": " + Default +"\n");
	}
	return ret;
}


C_Tileset::C_Tileset(std::string source, int first):
    m_source(source),
    m_first(first)
{
    m_last = 99999;
}

void C_Tileset::show(){
    C_Message::printV("Source: " + m_source + " first= " + to_string(m_first) + " last= " + to_string(m_last) + "\n");
}

bool C_Tileset::find(int tmxNbr){
    bool ret = false;
    if(tmxNbr >= m_first && tmxNbr <= m_last){
        ret = true;
    }
    return ret;
}


C_Tmx::C_Tmx(string const file_Path):C_Xml(file_Path)
{
        m_tilesetList = extractTilesetList();
        calcTilesetLast();
        showAllTileset();
}

S_tmxLayer C_Tmx::extractLayerInTMX(string layerName)
{

	S_tmxLayer layer;
	layer.name = layerName;
	layer.width = stoi(extractStrValue("layer","name",layerName,"width"));
	layer.height = stoi(extractStrValue("layer","name",layerName,"height"));
	layer.data =  extractStrData("layer", layerName);

	//drop all \n
	size_t start = 0;
	string in = "\n", out = "";
	while((start = layer.data.find(in,start)) != std::string::npos) {
		layer.data.replace(start,in.length(),out);
		start += out.length();
	}
	//cout  << data << "////" << endl;
	return layer;
}

std::vector <C_Tileset> C_Tmx::extractTilesetList(){
    //<tileset firstgid="1" source="../img/Ground_01.tsx"/
    vector <C_Tileset> list;

	xmlpp::TextReader reader(m_file_path);
	string value;
	while(reader.read()) {
		string nodeName = reader.get_name();
		int first = 0;
		string source;

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();
			do {
				string attrib = reader.get_name();
				if (nodeName == "tileset") {
				    if(attrib == "firstgid"){
    					first = stoi(reader.get_value());
				    } else if (attrib == "source") {
				        source = C_Tools::extractFilename(reader.get_value());
				    }
				}
			} while(reader.move_to_next_attribute());
		}
		if(!source.empty()){
		    list.push_back(C_Tileset(source, first));
		}
		reader.move_to_element();
	}
	return list;
}

void C_Tmx::showAllTileset(){
    for(auto i: m_tilesetList){
        i.show();
    }
}

void C_Tmx::calcTilesetLast(){
    int l = 99999;
    for(int i = (m_tilesetList.size()-1); i >= 0; i--){
          m_tilesetList[i].setLast(l);
          l = m_tilesetList[i].getFirst() - 1;
    }
}


S_Tile C_Tmx::getTileInfos(int tmxNbr){
    S_Tile ret;
    ret.tmxNbr = tmxNbr;
    for(auto i : m_tilesetList){
            if(i.find(tmxNbr)){
                ret.tsxNbr = tmxNbr - i.getFirst();
                ret.sourcefile = i.getSourceFile();
                C_TextureList& t= C_Locator::getTextureList();
                ret.name = t.getNameFromID(ret.tsxNbr,ret.sourcefile);
            }
    }
    return ret;
}
