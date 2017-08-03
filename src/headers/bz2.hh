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
}

int bz2_decompress(std::string input, std::string output) {
	FILE *f = fopen(input.c_str(), "r");
	FILE *out = fopen(output.c_str(), "w");
  	int bzError;
  	BZFILE *bzf;
  	char buf[1024];
	
	bzf = BZ2_bzReadOpen(&bzError, f, 0, 0, NULL, 0);
	if (bzError != BZ_OK) {
	  fprintf(stderr, "E: BZ2_bzReadOpen: %d\n", bzError);
	  return -1;
	}

  	while (bzError == BZ_OK) {
  	  int nread = BZ2_bzRead(&bzError, bzf, buf, sizeof buf);
  	  if (bzError == BZ_OK || bzError == BZ_STREAM_END) {
  	    size_t nwritten = fwrite(buf, 1, nread, out);
  	    if (nwritten != (size_t) nread) {
  	      fprintf(stderr, "E: short write\n");
  	      return -1;
  	    }
  	  }
  	}
	
  	if (bzError != BZ_STREAM_END) {
  	  fprintf(stderr, "E: bzip error after read: %d\n", bzError);
  	  return -1;
  	}

  	BZ2_bzReadClose(&bzError, bzf);
  	fclose(out);
  	return 0;
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
}