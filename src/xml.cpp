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

C_Xml::C_Xml()
{
}

C_Xml::~C_Xml(){
}

string C_Xml::extractStrProperty(string const &File_Path,string const &name){
     xmlpp::TextReader reader(File_Path);
     string value, type;
     while(reader.read())
        {
        		string nodeName = reader.get_name();

	          	if (reader.has_attributes()){
			    reader.move_to_first_attribute();
			    do
			    {
			      string attrib = reader.get_name();
                 //<property name="rank" type="int" value="1"/> tsx files
                 //<property name="wallet" value="200"/> tmx files

			      if (nodeName == "property" && attrib == "name"){
			      	string readName = reader.get_value();
			      	if(readName == name){
			      		reader.move_to_next_attribute();
			      		string attrib2 = reader.get_name();
                        if(attrib2 == "type"){
			      		    type = reader.get_value();
    			      		reader.move_to_next_attribute();
    			      		attrib2 = reader.get_name();
			      		    }
                        if(attrib2 == "value")
			      	        value = reader.get_value();
			      	    }
				    }
				} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
    C_Message m;
	m.printM("From: " + File_Path +" Property: " +name+ " = "+ value + " type: " + type +"\n");
    return value;
}

