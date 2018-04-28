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
	m_decorLayer.name="noname";
	m_decorLayer.width=30;
	m_decorLayer.height=30;
	m_decorLayer.data="";
	m_nbrOfWaves = 3;
	m_currentWaveNbr = -1;
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
	C_Message m;
    string levelPath ="data/levels/Level_0";
    string extension =".tmx";
    string filename = levelPath + to_string(levelNbr) + extension;

    struct stat buffer;
    if (stat (filename.c_str(),  &buffer) == 0){

	    loadGroundLayerIntoTheGrid(filename.c_str());
	    loadDecorLayerIntoTheGrid(filename.c_str());
    	for(int i = 0; i < m_nbrOfWaves; i++){
    	    loadWave(filename.c_str(),i);
    	}
    	m.printM("Level " + to_string(levelNbr) +" Loaded");
	}
	else{
    	m.printM("Can not find " + filename);
    	m.printM("Can not load level " + to_string(levelNbr));
	}
}

void C_Level::sendNextWave(){
    C_Message m;
    m_currentWaveNbr++;
    if(m_currentWaveNbr >= m_nbrOfWaves){
        m_currentWaveNbr = 0;
    }

    displayWave(m_currentWaveNbr);
    loadWaveIntoGrid(m_currentWaveNbr);
    m.printM("Next wave: " + to_string(m_currentWaveNbr));
}


S_tmxLayer C_Level::extractTMXfile(string tmx_File_Path, string layerName){

	S_tmxLayer layer;
	layer.name = layerName;
    string currentLayerName ="";
    C_Message m;
	m.printM("Reading: " + tmx_File_Path);
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
				    m.printDebug("found a " + layerName +" layer in the tmx file " + tmx_File_Path);
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

void C_Level::loadWave(string tmx_File_Path, int waveNbr){

  C_TextureList& t=C_TextureList::Instances();

    C_Wave wave;
	string name = "Wave" + to_string(waveNbr);
	S_tmxLayer layer = extractTMXfile(tmx_File_Path,name);
    string data = layer.data;
    C_Message m;
	for (int y = 0; y < layer.height; y++){
		for (int x = 0; x < layer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				if (nbr!=0){
	                string str = t.getNameFromID(nbr);
                    m.printDebug(to_string(x) + ":" + to_string(y) + "->" + str + " // ") ;
                    wave.add(1,x,y);
				    }
				//grid.setGround(x,y,nbr);

				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
	cout << endl;
	m_waves.push_back(wave);
}

void C_Level::displayWave(int i){
        int c = 0;
        for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end();it++){
            C_Wave wave = *it;
            if(i == c){
                wave.display();
            }
            c++;
        }
            C_Message m;
            m.printM("Number of wave for this level: " + to_string(c) +"\n");
}

void C_Level::loadWaveIntoGrid(int i){
        int c = 0;
        C_Message m;
        for(vector <C_Wave>::iterator it = m_waves.begin(); it !=m_waves.end();it++){
            C_Wave wave = *it;
            if(i == c){
                m.printM("load wave: " + to_string(c) + "\n");
                wave.loadIntoGrid();
            }
            c++;
        }
}


void C_Level::loadDecorLayerIntoTheGrid(string tmx_File_Path){
	C_Grid& grid=C_Grid::Instances();
	m_decorLayer = extractTMXfile(tmx_File_Path,"Decors");
    string data = m_decorLayer.data;

	for (int y = 0; y < m_decorLayer.height; y++){
		for (int x = 0; x < m_decorLayer.width; x++){
				string extract = data;
				int mark = extract.find_first_of(',');
				if (mark > 0)
					extract.resize(mark,'C');
				int nbr = stoi(extract);
				if(nbr != 0){
                	grid.setDecors(x,y,nbr);
				    }
				//cout << extract <<":";
				data = data.substr(mark + 1);
		}
	}
}

//______________________________Waves_____________________________//


C_Wave::C_Wave()
{
}
C_Wave::~C_Wave()
{
}


void C_Wave::add(int rank, int x, int y){
    S_boat tmp ={rank,x,y,true};
    m_boatList.push_back(tmp);
}

void C_Wave::display(){
    int c = 0;
    C_Message m;
    string message ="";
    for(vector <S_boat>::iterator i = m_boatList.begin(); i !=m_boatList.end();i++)
    {
        S_boat tmp = *i;
        message = "Rank " + to_string(tmp.rank) + " at " + to_string(tmp.x) + ":" + to_string(tmp.y);
        if (tmp.alive)
            message += " Alive";
        else
            message += "dead";

        c++;
    }
    m.printM(message + "\n");
    m.printM("Number of boats in this wave: " + to_string(c) + "\n");
}

void C_Wave::loadIntoGrid(){
    C_Grid& grid=C_Grid::Instances();
    C_Message m;
    for(vector <S_boat>::iterator i = m_boatList.begin(); i !=m_boatList.end();i++)
    {
        S_boat tmp = *i;
        m.printDebug("Rank " + to_string(tmp.rank) + " at " + to_string(tmp.x) + ":" + to_string(tmp.y) +"\n");
        if (tmp.alive)
            grid.addANewBoat(tmp.x,tmp.y,tmp.rank);

    }
}

