void firmaRainbow5640() {
	srand(time(0));

	uint8 c[RAINBOW5640_SIG_BYTES];
	uint8 m[RAINBOW5640_HASH_BYTES];
	uint8 pk[RAINBOW5640_PK_BYTES];
	uint8 sk[RAINBOW5640_SK_BYTES];

	uint64 lc,lm = RAINBOW5640_HASH_BYTES,lpk,lsk;

	rainbow5640_keypair( sk , &lsk , pk , &lpk );

	// printf("PUBLIC KEY:\n");
	// for( int i=0;i<RAINBOW5640_PK_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

	// printf("PRIVATE KEY:\n");
	// for( int i=0;i<RAINBOW5640_PK_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

	for( int i=0;i<RAINBOW5640_HASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<RAINBOW5640_HASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	rainbow5640_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<RAINBOW5640_SIG_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = rainbow5640_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );

	// encodeKeyPair("Rainbow5640", (char *)pk, RAINBOW5640_PK_BYTES, (char *)sk, RAINBOW5640_SK_BYTES);
}

void firmaRainbow6440() {
	srand(time(0));

	uint8 c[RAINBOW6440_SIG_BYTES];
	uint8 m[RAINBOW6440_HASH_BYTES];
	uint8 pk[RAINBOW6440_PK_BYTES];
	uint8 sk[RAINBOW6440_SK_BYTES];

	uint64 lc,lm = RAINBOW6440_HASH_BYTES,lpk,lsk;

	rainbow6440_keypair( sk , &lsk , pk , &lpk );

	for( int i=0;i<RAINBOW6440_HASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<RAINBOW6440_HASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	rainbow6440_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<RAINBOW6440_SIG_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = rainbow6440_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );
}

void firmaRainbow16242020() {
	srand(time(0));
	unsigned char m[RAINBOW16242020_HASH_BYTES];
	unsigned long long mlen = RAINBOW16242020_HASH_BYTES;
	unsigned char sm[RAINBOW16242020_SIG_BYTES];
	unsigned long long smlen;
	unsigned char pk[RAINBOW16242020_PK_BYTES];
	unsigned long long pklen;
	unsigned char sk[RAINBOW16242020_SK_BYTES];
	unsigned long long sklen;

	for(int i=0;i<RAINBOW16242020_HASH_BYTES;i++) 
		m[i]=rand()%256;
	rainbow16242020_keypair( sk , &sklen , pk , &pklen );
	for( int i=0;i<RAINBOW16242020_HASH_BYTES;i++) 
		printf("%x " , (unsigned) m[i]); printf("\n");
	rainbow16242020_signature(sm,&smlen,m,mlen,sk,sklen);
	for( int i=0;i<RAINBOW16242020_SIG_BYTES;i++) 
		printf("%x " , (unsigned) sm[i]); printf("\n");
	int i = rainbow16242020_verification(m,mlen,sm,smlen,pk,pklen);
	printf("verify: %d\n" , i );
	// printf("%d\n",i);
}

void firmaRainbow256181212() {
	srand(time(0));
	unsigned char m[RAINBOW256181212_HASH_BYTES];
	unsigned long long mlen = RAINBOW256181212_HASH_BYTES;
	unsigned char sm[RAINBOW256181212_SIG_BYTES];
	unsigned long long smlen;
	unsigned char pk[RAINBOW256181212_PK_BYTES];
	unsigned long long pklen;
	unsigned char sk[RAINBOW256181212_SK_BYTES];
	unsigned long long sklen;

	for(int i=0;i<RAINBOW256181212_HASH_BYTES;i++) 
		m[i]=rand()%256;
	rainbow256181212_keypair( sk , &sklen , pk , &pklen );
	for( int i=0;i<RAINBOW256181212_HASH_BYTES;i++) 
		printf("%x " , (unsigned) m[i]); printf("\n");
	rainbow256181212_signature(sm,&smlen,m,mlen,sk,sklen);
	for( int i=0;i<RAINBOW256181212_SIG_BYTES;i++) 
		printf("%x " , (unsigned) sm[i]); printf("\n");
	int i = rainbow256181212_verification(m,mlen,sm,smlen,pk,pklen);
	printf("verify: %d\n" , i );
	// printf("%d\n",i);
}

void firmaPFlash()
{
	srand(time(0));

	uint8_t c[SIGNATURE_BYTES];
	uint8_t m[SHORTHASH_BYTES];
	uint8_t pk[PUBLICKEY_BYTES];
	uint8_t sk[SECRETKEY_BYTES];

	uint64_t lc,lm = SHORTHASH_BYTES,lpk,lsk;

	pflash_keypair( sk , &lsk , pk , &lpk );

	// printf("PUBLIC KEY:\n");
	// for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

	// printf("PRIVATE KEY:\n");
	// for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

	for( int i=0;i<SHORTHASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<SHORTHASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	pflash_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<SIGNATURE_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = pflash_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );

}

int sign(const char *filename, const char *SKfilename, char *digest) 
{
	MPKCSignature_t *signature; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	char *version;
  	uint8_t *sk;
	uint64_t sklen;
	unsigned char out[crypto_hash_sha512_BYTES];

	uint8 s[43];
	uint64_t ls;

  	signature = (MPKCSignature_t *)calloc(1, sizeof(MPKCSignature_t));
  	if(!signature) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

  	std::string content = "";
	unsigned long long len;

	readFile(filename, &content, &len);

	if(strcmp(digest, "sha256") == 0)
	{
		crypto_hash_sha256(out, (const unsigned char *)content.c_str(), len);
	} else if(strcmp(digest, "sha512") == 0)
	{
		crypto_hash_sha512(out, (const unsigned char *)content.c_str(), len);
	} else
	{
		printf("The following are valid hash functions to be used by this implementation\n-sha256\n-sha512\n");
		return -1;
	}

	std::string tmpfile = base64decodeKey(SKfilename);

	MPKCPrivateKey_t *SK = decodePrivateKey((char *)tmpfile.c_str());

	remove(tmpfile.c_str());

	version = (char *)SK->version.buf;
	sklen = SK->key.size;
	sk = SK->key.buf;

	if(strcmp(version, "rainbow5640") == 0)
	{
		unsigned char hashvalue[RAINBOW5640_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW5640_HASH_BYTES);
		rainbow5640_signature((unsigned char *)s, &ls, hashvalue, RAINBOW5640_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow6440") == 0)
	{
		unsigned char hashvalue[RAINBOW6440_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW6440_HASH_BYTES);
		rainbow6440_signature((unsigned char *)s, &ls, hashvalue, RAINBOW6440_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow16242020") == 0)
	{
		unsigned char hashvalue[RAINBOW16242020_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW16242020_HASH_BYTES);
		rainbow16242020_signature((unsigned char *)s, &ls, hashvalue, RAINBOW16242020_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow256181212") == 0)
	{
		unsigned char hashvalue[RAINBOW256181212_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW256181212_HASH_BYTES);
		rainbow256181212_signature((unsigned char *)s, &ls, hashvalue, RAINBOW256181212_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "pflash") == 0)
	{
		unsigned char hashvalue[SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], SHORTHASH_BYTES);
		pflash_signature((unsigned char *)s, &ls, hashvalue, SHORTHASH_BYTES, sk, sklen);
	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n");
	}

	char *Sigfilename;
  	Sigfilename = tmpnam(nullptr);

  	FILE *fp = fopen(Sigfilename, "wb");

  	/* for Public Key BER output */
    if(!fp) {
    	perror(Sigfilename);
    	exit(1);
    }

    if(OCTET_STRING_fromBuf(&signature->version, &version[0], strlen(version)) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&signature->digest, &digest[0], strlen(digest)) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&signature->signature, (char *)s, ls) < 0) {
    	perror("Error Buffer");
  	}

  	ec = der_encode(&asn_DEF_MPKCSignature, signature, write_out, fp);

    if(ec.encoded == -1) {
      	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
      	ec.failed_type ? ec.failed_type->name : "unknown");
      	return -1;
    } 

    fclose(fp);

    std::string outputFile = filename + (std::string)"_sign.txt";
    base64encodeSignature((char *)outputFile.c_str(), Sigfilename);

    free(signature);
    free(SK);

    content = "";
    readFile((char *)outputFile.c_str(), &content, &len);

    std::cout<<"Signature:\n"<<content<<"\n";

    remove((char *)outputFile.c_str());
    remove(Sigfilename);

    return 0;
}

int sign(const char *filename, const char *SKfilename, char *digest, const char *output) 
{
	MPKCSignature_t *signature; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	std::cout<<"Generating signature...\n";

  	char *version;
  	uint8_t *sk;
	uint64_t sklen;
	unsigned char out[crypto_hash_sha512_BYTES];

	uint8 s[43];
	uint64_t ls;

  	signature = (MPKCSignature_t *)calloc(1, sizeof(MPKCSignature_t));
  	if(!signature) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

	std::string content = "";
	unsigned long long len;

	readFile(filename, &content, &len);

	if(strcmp(digest, "sha256") == 0)
	{
		crypto_hash_sha256(out, (const unsigned char *)content.c_str(), len);
	} else if(strcmp(digest, "sha512") == 0)
	{
		crypto_hash_sha512(out, (const unsigned char *)content.c_str(), len);
	} else
	{
		printf("The following are valid hash functions to be used by this implementation\n-sha256\n-sha512\n");
		return -1;
	}
	
	std::string tmpfile = base64decodeKey(SKfilename);

	MPKCPrivateKey_t *SK = decodePrivateKey((char *)tmpfile.c_str());

	remove(tmpfile.c_str());

	version = (char *)SK->version.buf;
	sklen = SK->key.size;
	sk = SK->key.buf;

	if(strcmp(version, "rainbow5640") == 0)
	{
		unsigned char hashvalue[RAINBOW5640_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW5640_HASH_BYTES);
		rainbow5640_signature((unsigned char *)s, &ls, hashvalue, RAINBOW5640_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow6440") == 0)
	{
		unsigned char hashvalue[RAINBOW6440_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW6440_HASH_BYTES);
		rainbow6440_signature((unsigned char *)s, &ls, hashvalue, RAINBOW6440_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow16242020") == 0)
	{
		unsigned char hashvalue[RAINBOW16242020_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW16242020_HASH_BYTES);
		rainbow16242020_signature((unsigned char *)s, &ls, hashvalue, RAINBOW16242020_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "rainbow256181212") == 0)
	{
		unsigned char hashvalue[RAINBOW256181212_HASH_BYTES];
		memcpy(hashvalue, &out[0], RAINBOW256181212_HASH_BYTES);
		rainbow256181212_signature((unsigned char *)s, &ls, hashvalue, RAINBOW256181212_HASH_BYTES, sk, sklen);
	} else if(strcmp(version, "pflash") == 0)
	{
		unsigned char hashvalue[SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], SHORTHASH_BYTES);
		pflash_signature((unsigned char *)s, &ls, hashvalue, SHORTHASH_BYTES, sk, sklen);
	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n");
	}

	char *Sigfilename;
  	Sigfilename = tmpnam(nullptr);

  	FILE *fp = fopen(Sigfilename, "wb");

  	/* for Public Key BER output */
    if(!fp) {
    	perror(Sigfilename);
    	exit(1);
    }

  	if(OCTET_STRING_fromBuf(&signature->version, &version[0], strlen(version)) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&signature->digest, &digest[0], strlen(digest)) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&signature->signature, (char *)s, ls) < 0) {
    	perror("Error Buffer");
  	}

  	ec = der_encode(&asn_DEF_MPKCSignature, signature, write_out, fp);

    if(ec.encoded == -1) {
      	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
      	ec.failed_type ? ec.failed_type->name : "unknown");
      	return -1;
    } 
    fclose(fp);

    base64encodeSignature(output, Sigfilename);

    std::cout<<"Sign saved in file: "<<output<<"\n";

    free(signature);
    free(SK);

    remove(Sigfilename);

    return 0;
}

int verify(const char *filename, const char *sign, const char *PKfilename)
{
	MPKCSignature_t *signature; /* MPKC Signature Structure */
  	asn_enc_rval_t ec; 

  	char *version;

  	signature = (MPKCSignature_t *)calloc(1, sizeof(MPKCSignature_t));
  	if(!signature) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

	std::string content = "";
	unsigned long long len;

	readFile(sign, &content, &len);

	std::string tmpsignfile = base64decodeKey(sign);

	signature = decodeSignature((char *)tmpsignfile.c_str());
	// std::cout<<"\nVERIFYING:\n";
	// xer_fprint(stdout, &asn_DEF_MPKCSignature, signature);

	std::string tmppkfile = base64decodeKey(PKfilename);

	MPKCPublicKey_t *PK = decodePublicKey((char *)tmppkfile.c_str());

	remove((char *)tmppkfile.c_str());

	if(strcmp((char *)signature->version.buf, (char *)PK->version.buf) == 0)
	{
		unsigned char out[crypto_hash_sha512_BYTES];
		std::cout<<"Verifying signature...\n";
		content = "";
		readFile(filename, &content, &len);
		if(strcmp((char *)signature->digest.buf, "sha256") == 0)
		{
			// unsigned char out[crypto_hash_sha256_BYTES];
			crypto_hash_sha256(out, (const unsigned char *)content.c_str(), len);
		} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
		{
			// unsigned char out[crypto_hash_sha512_BYTES];
			crypto_hash_sha512(out, (const unsigned char *)content.c_str(), len);
		} else
		{
			printf("The following are valid hash functions to be used by this implementation\n-sha256\n-sha512\n");
			return -1;
		}
		//Proceed to signature verification
		if(strcmp((char *)signature->version.buf, "rainbow5640") == 0)
		{
			unsigned char hashvalue[RAINBOW5640_HASH_BYTES];
			memcpy(hashvalue, &out[0], RAINBOW5640_HASH_BYTES);
			rainbow5640_verification(hashvalue, RAINBOW5640_HASH_BYTES, signature->signature.buf, RAINBOW5640_SIG_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "rainbow6440") == 0)
		{
			unsigned char hashvalue[RAINBOW6440_HASH_BYTES];
			memcpy(hashvalue, &out[0], RAINBOW6440_HASH_BYTES);
			rainbow6440_verification(hashvalue, RAINBOW6440_HASH_BYTES, signature->signature.buf, RAINBOW6440_SIG_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "rainbow16242020") == 0)
		{
			unsigned char hashvalue[RAINBOW16242020_HASH_BYTES];
			memcpy(hashvalue, &out[0], RAINBOW16242020_HASH_BYTES);
			rainbow16242020_verification(hashvalue, RAINBOW16242020_HASH_BYTES, signature->signature.buf, RAINBOW16242020_SIG_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "rainbow256181212") == 0)
		{
			unsigned char hashvalue[RAINBOW256181212_HASH_BYTES];
			memcpy(hashvalue, &out[0], RAINBOW256181212_HASH_BYTES);
			rainbow256181212_verification(hashvalue, RAINBOW256181212_HASH_BYTES, signature->signature.buf, RAINBOW256181212_SIG_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "pflash") == 0)
		{
			unsigned char hashvalue[SHORTHASH_BYTES];
			memcpy(hashvalue, &out[0], SHORTHASH_BYTES);
			pflash_verification(hashvalue, SHORTHASH_BYTES, signature->signature.buf, SIGNATURE_BYTES, PK->key.buf, PK->key.size);
		} else
		{
			printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n");
		}
	}
	else
	{
		std::cout<<"Sign verify: Fail\nERROR: The public key and signed document use different cryptosystems\n";
		return -1;
	}

	free(signature);
    free(PK);
}