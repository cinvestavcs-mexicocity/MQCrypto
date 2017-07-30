#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <fstream>
#include <headers/pstream.h>

#include <termios.h>
#include <unistd.h>

#include <rainbow/rainbow_w.h>
#include <rainbow/conversion.h>
#include <rainbowbin/gf.h>
#include <rainbowbin/blas.h>
#include <rainbowbin/rainbow.h>

#include <pflash/SignScheme.h>

#include "rainbow5640.h"
#include "rainbow6440.h"
#include "rainbow16242020.h"
#include "rainbow256181212.h"
#include "4HFE.h"
#include "3icp.h"
#include "tts6440.h"

extern "C" {
	#include <ASN1Encode.h>
	#include <MPKCPublicKey.h>
	#include <MPKCPrivateKey.h>
	#include <MPKCSignature.h>
	#include <MPKCMessage.h>
	#include <MPKCKeypair.h>
}

#include <b64/encode.h>
#include <b64/decode.h>

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif

#include <sodium.h>

void SetStdinEcho(bool enable);

int readFile(const char *filename, std::string *content, unsigned long long *len);

void writeBinaryFile(secure_string input, char *output);

int readFile(const char *filename, secure_string *content, unsigned long long *len);

int readBinaryFile(const char *filename, std::string *content, unsigned long long *len);

void writeEncryptedFile(unsigned char *input, char *output, unsigned long long len);

int validateScheme(char *scheme);

void printSchemes(void);

int generateKeypairExec(char **line, uint8_t argc);

int keysExec(char **line, uint8_t argc);

void keysHelp(void);

int readFile(const char *filename, std::string *content, unsigned long long *len);

void generateKeypairHelp(void);

void signHelp(void);

void verifyHelp(void);

void encryptHelp(void);

void decryptHelp(void);

void sh_help(void);

void sh_welcome(void);

int lsh_execute(char **args, uint8_t argc);

char **lsh_split_line(char *line, uint8_t *length);

char *lsh_read_line(void);

void lsh_loop(void);