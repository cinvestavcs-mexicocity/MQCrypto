#include <bzlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

void bz2_compress(std::string input, std::string output)
{
	std::ifstream infile;
	// infile.open("rainbow5640_Priv.bin");
	infile.open(input);
	
	// char bz2Filename[] = "file.bz2";
	output += ".bz2";
	FILE *bz2File = fopen(output.c_str(), "wb");
	int bzError;
	const int BLOCK_MULTIPLIER = 7;
	BZFILE *myBZ = BZ2_bzWriteOpen(&bzError, bz2File, BLOCK_MULTIPLIER, 0, 0);
	
	int BUF_SIZE = 1024;    
	char* buf = new char[BUF_SIZE];
	while(infile.tellg() >= 0) {
	    infile.read(buf, BUF_SIZE);
	    BZ2_bzWrite(&bzError, myBZ, buf, infile.gcount());
	}    
	BZ2_bzWriteClose(&bzError, myBZ, 0, NULL, NULL);
	delete[] buf;
	std::cout<<"Keypair saved in file: "<<output<<"\n";
}

void bz2_compress_bin(std::string input, std::string output)
{
	std::ifstream infile(input, std::ios::in | std::ios::binary);
	
	// char bz2Filename[] = "file.bz2";
	output += ".bz2";
	FILE *bz2File = fopen(output.c_str(), "wb");
	int bzError;
	if(!infile)
		std::cout<<"COULD NOT OPEN FILE: " << input << "\n";
	const int BLOCK_MULTIPLIER = 7;
	BZFILE *myBZ = BZ2_bzWriteOpen(&bzError, bz2File, BLOCK_MULTIPLIER, 0, 0);
	
	int BUF_SIZE = 1024;    
	char* buf = new char[BUF_SIZE];
	while(infile.tellg() >= 0) {
	    infile.read(buf, BUF_SIZE);
	    BZ2_bzWrite(&bzError, myBZ, buf, infile.gcount());
	}   
	BZ2_bzWriteClose(&bzError, myBZ, 0, NULL, NULL);
	std::cout<<"Keypair saved in file: "<<output<<"\n";
}