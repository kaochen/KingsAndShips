#include "level.h"
#include "invaders.h"
#include "grid.h"

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

using namespace std;


C_Level::C_Level():
	m_name("level"),
	m_id(1)
{
	m_count = ++m_id;
}


C_Level::~C_Level()
{
}

void C_Level::status(){
	cout << m_name << " " << m_id << endl;
}

void C_Level::sendNextWave(list<C_GameUnits*>& lB){

	C_Grid& grid=C_Grid::Instances();
		vector <S_boat> l;
		S_boat temp = {1,10,16};
		l.push_back(temp);
		temp.x = 6;
		temp.y = 14;
		l.push_back(temp);
		/*temp.x = 5;
		temp.y = 15;
		l.push_back(temp);
		temp.x = 7;
		l.push_back(temp);
		temp.x = 3;
		l.push_back(temp);*/

		for (size_t i = 0; i < l.size();i++){
		grid.addANewBoat(l[i].x,l[i].y,l[i].rank);
		lB.push_back(grid.getUnits (l[i].x,l[i].y));
	}
	cout << "Next wave" << endl;
}


void C_Level::extractTMXfile(string tmx_File_Path){

	C_Grid& grid=C_Grid::Instances();

	string layerName ="", data ="";
	int width = 30, height = 30;
	cout << "Reading: " << tmx_File_Path << endl;
 xmlpp::TextReader reader(tmx_File_Path);
 while(reader.read())
    {
    		string nodeName = reader.get_name();
	      	//cout << nodeName << "---namespace---\n";

	      	if (reader.has_attributes()){
			reader.move_to_first_attribute();
			do
			{
			  string attributes = reader.get_name();
			  //cout << attributes << "-----"<< endl;

			// <layer name="units" width="30" height="30" visible="0">
			  if (nodeName == "layer" && attributes == "name"){
			  	layerName = reader.get_value();
				}
			  if (nodeName == "layer" && attributes == "width"){
			  	width = stoi(reader.get_value());
				}
			  if (nodeName == "layer" && attributes == "height"){
			  	height = stoi(reader.get_value());
				}
			  if (nodeName == "data" && attributes == "encoding"){
				if (reader.get_value() == "csv" && layerName == "Ground"){
					cout << "found data " << endl;
					data = reader.read_inner_xml();
				    	layerName ="";
						}
					}

			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}
	//cout << "----" << data << "////" << endl;

	//drop all \n
	size_t start = 0;
	string in = "\n", out = "";
	while((start = data.find(in,start)) != std::string::npos){
		data.replace(start,in.length(),out);
		start += out.length();
	}

	//cout  << data << "////" << endl;

	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				//cout << nbr;
				grid.setGround(x,y,nbr);

				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
	//cout << endl << endl;
	//read grid
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			cout << grid.getGround(x,y)<<"-";
		}
		cout << endl;
	}
}



