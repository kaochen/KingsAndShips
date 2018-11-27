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
#include "message.h"
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

string C_Xml::extractStrValue(string const &node, string const &id, string const &idValue, string const &name){
     xmlpp::TextReader reader(m_file_path);
     string value;
     while(reader.read())
        {
        		string nodeName = reader.get_name();

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			        string attrib = reader.get_name();
			        if (nodeName == node && attrib == id){
			          	string readName = reader.get_value();
			          	if(readName == idValue){
			                reader.move_to_first_attribute();
			                do{
			              		string attrib2 = reader.get_name();
			                    if(attrib2 == name){
			              	        value = reader.get_value();
			              	    }
			                }while(reader.move_to_next_attribute());

                        }
				    }
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + m_file_path +" in Node: "+ node + " where "+id+ "="+ idValue +", " +name+ " = "+ value+"\n");
    return value;
}



string C_Xml::extractStrValue(string const &node, string const &name){
     xmlpp::TextReader reader(m_file_path);
     string value;
     while(reader.read())
        {
        		string nodeName = reader.get_name();

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			        string attrib = reader.get_name();
			        if (nodeName == node && attrib == name){
                        value = reader.get_value();
                    }
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + m_file_path +" in Node \""+ node  +"\": " + name+ " = "+ value+"\n");
    return value;
}

string C_Xml::extractStrData(string const &node, string const &name){
     xmlpp::TextReader reader(m_file_path);
     string data;
     string currentNodeName;
     while(reader.read())
        {
        		string nodeType = reader.get_name();

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			        string attrib = reader.get_name();
			        if (nodeType == node && attrib == "name"){
			  	        currentNodeName = reader.get_value();
				    }
			        if (nodeType == "data" && currentNodeName == name){
					        data = reader.read_inner_xml();
				            currentNodeName ="";

					}
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + m_file_path +" in Node \""+ node  +"\": " + name+ " = "+ data+"\n");
    return data;
}


S_tmxLayer C_Xml::extractLayerInTMX(string layerName){

	S_tmxLayer layer;
	layer.name = layerName;
	layer.width = stoi(extractStrValue("layer","name",layerName,"width"));
	layer.height = stoi(extractStrValue("layer","name",layerName,"height"));
    layer.data =  extractStrData("layer", layerName);

	//drop all \n
	size_t start = 0;
	string in = "\n", out = "";
	while((start = layer.data.find(in,start)) != std::string::npos){
		layer.data.replace(start,in.length(),out);
		start += out.length();
	}
	//cout  << data << "////" << endl;
    return layer;
}


int C_Xml::countAttributes(string pattern){
	string old;
    xmlpp::TextReader reader(m_file_path);
    int c = 0;
     while(reader.read())
        {
        	string nodeName = reader.get_name();
        	if (reader.has_attributes()){
			reader.move_to_first_attribute();
			do
			{
			  string attributes = reader.get_name();
			  if (nodeName == "layer" && attributes == "name"){
			  	    string value = reader.get_value();
                    //cout << value << endl;
			        if(value.compare(0,pattern.size(),pattern)==0 && old != value){
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
	C_Message m;
	m.printM(c +" "+ pattern + " in " + m_file_path +"\n");
    return c;
}
