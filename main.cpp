#include <iostream>
#include <fstream>
#include <string.h>

#define NOT_ENOUGH_ARGUMENTS 1
#define CANT_OPEN_INPUT 2

using namespace std;

char extractPop(string &line){
	int i = 0;
	int column = 0;
	
	while(1){
		if(line.at(i) == '\t'){
			column++;
		}else{
			if(column == 2){
				return line.at(i);
			}
		}
		i++;
	}
}

void writeLine(string &line, ofstream &stream){
	stream.write(line.c_str(), line.length());
	#ifdef _WIN32
	stream.write("\n",1);
	#endif
}

int main(int argc, char *argv[]) {
	string phenopath;
	string outpath;
	
	//Handle arguments
	if(argc == 5){
		for(int i = 1; i < argc; i++){
			if(!strncmp(argv[i],"-i",2)){
				phenopath = argv[i+1];
			}else if(!strncmp(argv[i],"-o",2)){
				outpath = argv[i+1];
			}
		}
	}else if(argc == 2 && (!strncmp(argv[1],"-h",2) || !strncmp(argv[1],"--help",6))){
		cout << "Usage:\n\tplinkfilter -i [input pheno file] -o [output directory]\n";
		return 0;
	}else{
		cerr << "Not enough arguments\n";
		return NOT_ENOUGH_ARGUMENTS;
	}
	
	ifstream phenofile;
	ofstream outfile1, outfile2;
	
	phenofile.open(phenopath);
	
	//Fix trailing '/' if not present
	if(outpath.back() != '/'){
		outpath.append("/");
	}
	
	outfile1.open(outpath + "PHENO1.txt", ofstream::trunc | ofstream::out);
	outfile2.open(outpath + "PHENO2.txt", ofstream::trunc | ofstream::out);
	
	//Check if input file exists and can be read
	if(phenofile.bad()){
		cerr << "Can't open input pheno file\n";
		return CANT_OPEN_INPUT;
	}
	
	string line;
	
	//Duplicate first line
	getline(phenofile,line);
	
	writeLine(line, outfile1);
	writeLine(line, outfile2);
	
	char p;
	
	while(getline(phenofile,line)){
		p = extractPop(line);
		if(p == '1'){
			writeLine(line, outfile1);
		}else if(p == '2'){
			writeLine(line, outfile2);
		}
	}
	
	phenofile.close();
	outfile1.close();
	outfile2.close();
	
	cout << "Finished\n";
	
	return 0;
}
