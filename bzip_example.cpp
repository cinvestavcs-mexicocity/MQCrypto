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
	FILE *bz2File = fopen(bz2Filename.c_str(), "wb");
	int bzError;
	const int BLOCK_MULTIPLIER = 7;
	BZFILE *myBZ = BZ2_bzWriteOpen(&bzError, bz2File, BLOCK_MULTIPLIER, 0, 0);
	
	int BUF_SIZE = 1024 * 10;    
	char* buf = new char[BUF_SIZE];
	while(infile.tellg() >= 0) {
	    infile.read(buf, BUF_SIZE);
	    BZ2_bzWrite(&bzError, myBZ, buf, infile.gcount());
	}    
	BZ2_bzWriteClose(&bzError, myBZ, 0, NULL, NULL);
	
	delete[] buf;
}

// int main()
// {
// 	std::string content = "";
// 	uint64_t len = 0;
// 	char *output;
// 	readFile("rainbow5640_Priv.txt", &content, &len);
// 	bz_stream *bz = (bz_stream *)calloc(sizeof(bz_stream), 1);
// 	std::cout<<"Llego 1\n";

// // 	int BZ2_bzCompressInit ( bz_stream *strm, int blockSize100k,
// // 								int verbosity, int workFactor );
// 	BZ2_bzCompressInit(bz, 1, 0, 30);
// 	bz->next_in = (char *)content.c_str();
// 	bz->avail_in = len;
// 	bz->next_out = &(*output);
// 	std::cout<<"Llego 2\n";

// 	// int BZ2_bzCompress ( bz_stream *strm, int action );
// 	BZ2_bzCompress(bz, BZ_FINISH);
// 	std::cout<<"Llego 3\n";

// 	// int BZ2_bzCompressEnd ( bz_stream *strm );
// 	BZ2_bzCompressEnd(bz);
// 	std::cout<<"Llego 4\n";
// 	std::cout << "Output: " << output << "\n";
// 	free(bz);
// 	bz = (bz_stream *)calloc(sizeof(bz_stream), 1);
// 	std::cout<<"Llego 5\n";

// 	// BZ2_bzDecompressInit ( bz_stream *strm, int verbosity, int small );
// 	BZ2_bzDecompressInit(bz, 0, 0);

// 	bz->next_in = (char *)output;
// 	bz->avail_in = len;
// 	std::cout<<"Llego 6\n";

// 	// int BZ2_bzDecompress ( bz_stream *strm );
// 	BZ2_bzDecompress(bz);
// 	std::cout<<"Llego 7\n";

// 	// int BZ2_bzDecompressEnd ( bz_stream *strm );
// 	BZ2_bzDecompressEnd(bz);
// 	std::cout<<"Llego 8\n";
// 	std::cout << "Decompressed: " << bz->next_out << "\n";
// 	free(bz);
// }