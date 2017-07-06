#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <fstream>
#include <pstream.h>

#include <rainbow/rainbow_w.h>
#include <rainbow/conversion.h>
#include <rainbowbin/gf.h>
#include <rainbowbin/blas.h>
#include <rainbowbin/rainbow.h>

#include <pflash/SignScheme.h>

#include "rainbow5640_sizes.h"
#include "rainbow6440_sizes.h"
#include "rainbow16242020_sizes.h"
#include "rainbow256181212_sizes.h"
#include "4HFE.h"

extern "C" {
	#include <ASN1Encode.h>
	#include <MPKCPublicKey.h>
	#include <MPKCPrivateKey.h>
	#include <MPKCSignature.h>
	#include <MPKCMessage.h>
}

#include <b64/encode.h>
#include <b64/decode.h>

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif

#include <sodium.h>

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