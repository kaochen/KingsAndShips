#include "level.h"
#include "invaders.h"
#include "grid.h"

#include <sys/stat.h>
#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

using namespace std;


C_Level::C_Level():
	m_name("level"),
	m_id(1)
{
	m_count = ++m_id;
	m_groundLayer.name="noname";
	m_groundLayer.width=30;
	m_groundLayer.height=30;
	m_groundLayer.data="";
}


C_Level::~C_Level()
{
}

void C_Level::status(){
	cout << m_name << " " << m_id << endl;
}

void C_Level::load(int levelNbr){
    //clean before loading
	C_Grid& grid=C_Grid::Instances();
	grid.reset();

    string levelPath ="data/levels/Level_0";
    string extension =".tmx";
    string filename = levelPath + to_string(levelNbr) + extension;

    struct stat buffer;
    if (stat (filename.c_str(),  &buffer) == 0){

	    loadGroundLayerIntoTheGrid(filename.c_str());
    	cout << "Level "<< levelNbr <<" Loaded" << endl;
    	loadWave(filename.c_str());
	}
	else{
    	cout << "Can not find " << filename << endl;
    	cout << "Can not load level " << levelNbr << endl;
	}
}

void C_Level::sendNextWave(){

	C_Grid& grid=C_Grid::Instances();
		vector <S_boat> l;
		S_boat temp = {1,6,15};
		l.push_back(temp);
		temp.x = 6;
		temp.y = 14;
		l.push_back(temp);
		temp.x = 6;
		temp.y = 16;
		l.push_back(temp);
		/*temp.x = 7;
		l.push_back(temp);
		temp.x = 3;
		l.push_back(temp);*/

		for (size_t i = 0; i < l.size();i++){
		grid.addANewBoat(l[i].x,l[i].y,l[i].rank);
	}
	cout << "Next wave" << endl;
}


S_tmxLayer C_Level::extractTMXfile(string tmx_File_Path, string layerName){

	S_tmxLayer layer;
	layer.name = layerName;
    string currentLayerName ="";

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

			  if (nodeName == "layer" && attributes == "name"){
			  	currentLayerName = reader.get_value();
				}
			  if (nodeName == "layer" && attributes == "width"){
			  	layer.width = stoi(reader.get_value());
				}
			  if (nodeName == "layer" && attributes == "height"){
			  	layer.height = stoi(reader.get_value());
				}
			  if (nodeName == "data" && attributes == "encoding"){
				if (reader.get_value() == "csv" && currentLayerName == layerName){
					cout << "found a " << layerName << " layer in the tmx file " << tmx_File_Path << endl;
					layer.data = reader.read_inner_xml();
				    currentLayerName ="";
						}
					}

			} while(reader.move_to_next_attribute());
		}
		reader.move_to_element();
    	}

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

void C_Level::loadGroundLayerIntoTheGrid(string tmx_File_Path){
	C_Grid& grid=C_Grid::Instances();
	m_groundLayer = extractTMXfile(tmx_File_Path,"Ground");
    string data = m_groundLayer.data;

	for (int y = 0; y < m_groundLayer.height; y++){
		for (int x = 0; x < m_groundLayer.width; x++){
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
}

void C_Level::loadWave(string tmx_File_Path){

  C_TextureList& t=C_TextureList::Instances();
	int nbr = 1;
	string name = "Wave" + to_string(nbr);
	S_tmxLayer layer = extractTMXfile(tmx_File_Path,name);
    string data = layer.data;

	for (int y = 0; y < layer.height; y++){
		for (int x = 0; x < layer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				if (nbr!=0){
	                string str = t.getNameFromID(nbr);
				    cout << x << ":" << y << "->" << str << " // " ;
				    }
				//grid.setGround(x,y,nbr);

				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
	cout << endl;
}


