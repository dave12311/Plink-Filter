#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

class PedData{
public:
	string data;
	PedData *next = nullptr;
};

class InputData{
public:
	InputData(PedData *first);
	void extract(string &line);
	void closeFiles();
	void openFiles(char *path);
private:
	string id;
	int population;
	PedData *firstPed;
	
	ofstream pop1, pop2;
	
	void search();
};

InputData::InputData(PedData* first){
	firstPed = first;
}

void InputData::extract(string &line){
	int i = 0;
	int column = 0;
	
	id.clear();
	
	while(1){
		if(line.at(i) == '\t'){
			column++;
		}else{
			if(column == 0){
				id.append(1,line.at(i));
			}else if(column == 2){
				if(line.at(i) == '1'){
					population = 1;
					search();
				}else if(line.at(i) == '2'){
					population = 2;
					search();
				}else{
					population = 0;
				}
				break;
			}
		}
		i++;
	}
}

void InputData::search(){
	PedData *pointer = firstPed;
	
	while(pointer->next != nullptr){
		if(pointer->data.compare(0,6,id) == 0){
			if(population == 1){
				pop1.write(pointer->data.c_str(), pointer->data.length());
			}else if(population == 2){
				pop2.write(pointer->data.c_str(), pointer->data.length());
			}
			break;
		}
		pointer = pointer->next;
	}
}

void InputData::openFiles(char* path){
	string outPath(path);
	
	pop1.open(outPath + "POP1.ped", ofstream::trunc | ofstream::out);
	pop2.open(outPath + "POP2.ped", ofstream::trunc | ofstream::out);
}

void InputData::closeFiles(){
	pop1.close();
	pop2.close();
}


int main(int argc, char *argv[]) {
	char *pedpath = nullptr;
	char *inputpath = nullptr;
	char *outpath = nullptr;
	
	PedData *pedData_first = new PedData;
	PedData *pedData_last = pedData_first;
	
	if(argc == 7){
		for(int i = 1; i < argc; i++){
			if(!strncmp(argv[i],"-i",2)){
				inputpath = argv[i+1];
			}else if(!strncmp(argv[i],"-p",2)){
				pedpath = argv[i+1];
			}else if(!strncmp(argv[i],"-o",2)){
				outpath = argv[i+1];
			}
		}
	}else{
		cerr << "Not enough arguments\n";
	}
	
	if(pedpath != nullptr && inputpath != nullptr && outpath != nullptr){
		ifstream pedfile;
		ifstream inputfile;
		
		pedfile.open(pedpath);
		inputfile.open(inputpath);
		
		string line;
		
		//Read PED file to buffer
		while(getline(pedfile,line)){
			pedData_last->data = line;
			pedData_last->next = new PedData;
			pedData_last = pedData_last->next;
		}
		
		pedfile.close();
		
		//Discard first row
		getline(inputfile,line);
		
		InputData inputData(pedData_first);
		
		//Parse input data
		inputData.openFiles(outpath);
		while(getline(inputfile,line)){
			inputData.extract(line);
		}
		
		inputData.closeFiles();
		
		inputfile.close();
	}
	return 0;
}
