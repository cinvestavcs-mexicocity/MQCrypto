int sign(const char *filename, const char *SKfilename, char *digest) 
{
	MPKCSignature_t *signature; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	char *version;
  	uint8_t *sk;
	unsigned long long sklen;
	unsigned char out[crypto_hash_sha512_BYTES];

	uint8 s[64];
	unsigned long long ls;
	std::string s_aux = "";
	std::string move = "";

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
		printf("The following are valid hash functions that can be used by MQCrypto\n-sha256\n-sha512\n");
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
	} else if(strcmp(version, "sflashv1") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[264];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 256);
			memcpy(&hashvalue[256], &out[0], 8);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 264);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv1 " + (std::string)converted + " " + privtmp + " " + sgntmp;;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
        std::cout << str;

        readBinaryFile((char *)sgntmp.c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "sflashv2") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[264];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 256);
			memcpy(&hashvalue[256], &out[0], 8);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 264);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv2 " + (std::string)converted + " " + privtmp + " " + sgntmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
        std::cout << str;

        readBinaryFile((char *)sgntmp.c_str(), &s_aux, &ls);
        strcpy((char *)s,(char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "uov") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[264];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 256);
			memcpy(&hashvalue[256], &out[0], 8);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 264);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py UOV 16 40 80 " + (std::string)converted + " " + privtmp + " " + sgntmp;;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
        // std::cout << str;

        readBinaryFile((char *)sgntmp.c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "pflash") == 0)
	{
		unsigned char hashvalue[SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], SHORTHASH_BYTES);
		pflash_signature((unsigned char *)s, &ls, hashvalue, SHORTHASH_BYTES, sk, sklen);
	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n-sflashv1\n-sflashv2\n-uov\n-3icp\n-tts6440\n");
	}

	char *Sigfilename;
	std::string Sigfile = "/var/tmp/f_XXXXXX";
  	// Sigfilename = tmpnam(nullptr);
  	Sigfilename = (char *)Sigfile.c_str();

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
	unsigned long long sklen;
	unsigned char out[crypto_hash_sha512_BYTES];

	uint8 s[64]; //s[43]
	unsigned long long ls;
	std::string s_aux = "";
	std::string move = "";

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
		printf("The following are valid hash functions that can be used by MQCrypto\n-sha256\n-sha512\n");
		return -1;
	}
	
	// std::string tmpfile = base64decodeKey(SKfilename);
	std::string tmpfile = SKfilename;

	MPKCPrivateKey_t *SK = decodePrivateKey((char *)tmpfile.c_str());

	// remove(tmpfile.c_str());

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
	} else if(strcmp(version, "3icp") == 0)
	{
		unsigned char hashvalue[IC3_SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], IC3_SHORTHASH_BYTES);
		ic3_signature((unsigned char *)s, &ls, hashvalue, IC3_SHORTHASH_BYTES, sk, sklen);
	} else if(strcmp(version, "tts6440") == 0)
	{
		unsigned char hashvalue[TTS6440_SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], TTS6440_SHORTHASH_BYTES);
		tts6440_signature((unsigned char *)s, &ls, hashvalue, TTS6440_SHORTHASH_BYTES, sk, sklen);
	} else if(strcmp(version, "sflashv1") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[33];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 32);
			memcpy(&hashvalue[32], &out[0], 1);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 33);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv1 " + (std::string)converted + " " + privtmp + " " + sgntmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;

        readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "sflashv2") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[33];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 32);
			memcpy(&hashvalue[32], &out[0], 1);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 33);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);
		
		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}
		
		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv2 " + (std::string)converted + " " + privtmp + " " + sgntmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);
		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;
		readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s,(char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "uov") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[20];
		int conv[crypto_hash_sha512_BYTES];
		memcpy(hashvalue, &out[0], 20);
		
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py UOV 16 40 80 " + (std::string)converted + " " + privtmp + " " + sgntmp;;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;

        readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n");
	}

	char *Sigfilename;
  	std::string Sigfile = "/var/tmp/f_XXXXXX";
  	// Sigfilename = tmpnam(nullptr);
  	Sigfilename = (char *)Sigfile.c_str();

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

    // std::cout<<"Sign saved in file: "<<output<<"\n";

    free(signature);
    free(SK);

    remove(Sigfilename);

    return 0;
}

int sign(const char *filename, MPKCPrivateKey_t *SK, char *digest, const char *output) 
{
	MPKCSignature_t *signature; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	std::cout<<"Generating signature...\n";

  	char *version;
  	uint8_t *sk;
	unsigned long long sklen;
	unsigned char out[crypto_hash_sha512_BYTES];

	uint8 s[64]; //s[43]
	unsigned long long ls;
	std::string s_aux = "";
	std::string move = "";

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
		printf("The following are valid hash functions that can be used by MQCrypto\n-sha256\n-sha512\n");
		return -1;
	}

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
	} else if(strcmp(version, "3icp") == 0)
	{
		unsigned char hashvalue[IC3_SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], IC3_SHORTHASH_BYTES);
		ic3_signature((unsigned char *)s, &ls, hashvalue, IC3_SHORTHASH_BYTES, sk, sklen);
	} else if(strcmp(version, "tts6440") == 0)
	{
		unsigned char hashvalue[TTS6440_SHORTHASH_BYTES];
		memcpy(hashvalue, &out[0], TTS6440_SHORTHASH_BYTES);
		tts6440_signature((unsigned char *)s, &ls, hashvalue, TTS6440_SHORTHASH_BYTES, sk, sklen);
	} else if(strcmp(version, "sflashv1") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[33];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 32);
			memcpy(&hashvalue[32], &out[0], 1);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 33);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv1 " + (std::string)converted + " " + privtmp + " " + sgntmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;

        readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "sflashv2") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[33];
		int conv[crypto_hash_sha512_BYTES];
		if(strcmp(digest, "sha256") == 0)
		{
			memcpy(hashvalue, &out[0], 32);
			memcpy(&hashvalue[32], &out[0], 1);
		} else if(strcmp(digest, "sha512") == 0)
		{
			memcpy(hashvalue, &out[0], 33);
		}
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);
		
		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}
		
		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv2 " + (std::string)converted + " " + privtmp + " " + sgntmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);
		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;

		readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s,(char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else if(strcmp(version, "uov") == 0)
	{
		if(!SageMath)
		{
    		printf("MQCrypto: sage: command not found\n");
    		return 1;
		}
		unsigned char hashvalue[20];
		int conv[crypto_hash_sha512_BYTES];
		memcpy(hashvalue, &out[0], 20);
		
		std::string sgntmp = "/var/tmp/sgn_XXXXXX";
		mkstemp((char *)sgntmp.c_str());
		std::string privtmp = "/var/tmp/priv_XXXXXX";
		mkstemp((char *)privtmp.c_str());
		move = "mv " + privtmp + " " + privtmp + ".priv";
		system((char *)move.c_str());
		// rename((char *)privtmp.c_str(), (char *)(privtmp + ".priv").c_str());
		writeEncryptedFile(sk, (char *)(privtmp + ".priv").c_str(), sklen);

		char converted[33 * 2 + 1];
		for(int i = 0; i < 64; i++)
		{
			conv[i] = (int)out[i];
    		sprintf(&converted[i*2], "%02X", conv[i]);
		}

		std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py UOV 16 40 80 " + (std::string)converted + " " + privtmp + " " + sgntmp;;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');
   //      std::cout << str;

        readBinaryFile((char *)(sgntmp + ".sgn").c_str(), &s_aux, &ls);
        strcpy((char *)s, (char *)s_aux.c_str());
        
        remove((char *)(privtmp + ".priv").c_str());
        remove((char *)(sgntmp + ".sgn").c_str());
        remove((char *)sgntmp.c_str());
	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n");
	}

	char Sigfilename[] = "/var/tmp/f_XXXXXX";
	mkstemp(Sigfilename);

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

    // std::cout<<"Sign saved in file: "<<output<<"\n";

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
  	std::string s_aux = "";

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
			crypto_hash_sha256(out, (const unsigned char *)content.c_str(), len);
		} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
		{
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
		} else if(strcmp((char *)signature->version.buf, "3icp") == 0)
		{
			unsigned char hashvalue[IC3_SHORTHASH_BYTES];
			memcpy(hashvalue, &out[0], IC3_SHORTHASH_BYTES);
			ic3_verification(hashvalue, IC3_SHORTHASH_BYTES, signature->signature.buf, IC3_SIGNATURE_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "tts6440") == 0)
		{
			unsigned char hashvalue[TTS6440_SHORTHASH_BYTES];
			memcpy(hashvalue, &out[0], TTS6440_SHORTHASH_BYTES);
			tts6440_verification(hashvalue, TTS6440_SHORTHASH_BYTES, signature->signature.buf, TTS6440_SIGNATURE_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "sflashv1") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[33];
			int conv[crypto_hash_sha512_BYTES];
			if(strcmp((char *)signature->digest.buf, "sha256") == 0)
			{
				memcpy(hashvalue, &out[0], 32);
				memcpy(&hashvalue[32], &out[0], 1);
			} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
			{
				memcpy(hashvalue, &out[0], 33);
			}
			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			std::string pubtmp = "/var/tmp/pub_XXXXXX.pub";
			mkstemp((char *)pubtmp.c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);
	
			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}
	
			std::string sage = "sage -python /usr/local/mqcrypto/sage/verify.py Sflashv1 " + (std::string)converted + " " + pubtmp + " " + sgntmp;;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;
		} else if(strcmp((char *)signature->version.buf, "sflashv2") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[33];
			int conv[crypto_hash_sha512_BYTES];
			if(strcmp((char *)signature->digest.buf, "sha256") == 0)
			{
				memcpy(hashvalue, &out[0], 32);
				memcpy(&hashvalue[32], &out[0], 1);
			} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
			{
				memcpy(hashvalue, &out[0], 33);
			}
			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			std::string pubtmp = "/var/tmp/pub_XXXXXX.pub";
			mkstemp((char *)pubtmp.c_str());
			// rename((char *)pubtmp.c_str(), (char *)(pubtmp + ".pub").c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);

			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}

			std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py Sflashv2 " + (std::string)converted + " " + pubtmp + " " + sgntmp;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;
		} else if(strcmp((char *)signature->version.buf, "uov") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[20];
			int conv[crypto_hash_sha512_BYTES];
			memcpy(hashvalue, &out[0], 20);

			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			std::string pubtmp = "/var/tmp/pub_XXXXXX.pub";
			mkstemp((char *)pubtmp.c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);

			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}

			std::string sage = "sage -python /usr/local/mqcrypto/sage/sign.py UOV 16 40 80 " + (std::string)converted + " " + pubtmp + " " + sgntmp;;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;
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

    return 0;
}

int verify(const char *filename, const char *sign, MPKCPublicKey_t *PK)
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

	std::string move = "";

	readFile(sign, &content, &len);

	std::string tmpsignfile = base64decodeKey(sign);
	readBinaryFile((char *)tmpsignfile.c_str(), &content, &len);

	signature = decodeSignature((unsigned char *)content.c_str(), len);

	remove((char *)tmpsignfile.c_str());

	if(strcmp((char *)signature->version.buf, (char *)PK->version.buf) == 0)
	{
		unsigned char out[crypto_hash_sha512_BYTES];
		std::cout<<"Verifying signature...\n";
		content = "";
		readFile(filename, &content, &len);
		if(strcmp((char *)signature->digest.buf, "sha256") == 0)
		{
			crypto_hash_sha256(out, (const unsigned char *)content.c_str(), len);
		} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
		{
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
		} else if(strcmp((char *)signature->version.buf, "3icp") == 0)
		{
			unsigned char hashvalue[IC3_SHORTHASH_BYTES];
			memcpy(hashvalue, &out[0], IC3_SHORTHASH_BYTES);
			ic3_verification(hashvalue, IC3_SHORTHASH_BYTES, signature->signature.buf, IC3_SIGNATURE_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "tts6440") == 0)
		{
			unsigned char hashvalue[TTS6440_SHORTHASH_BYTES];
			memcpy(hashvalue, &out[0], TTS6440_SHORTHASH_BYTES);
			tts6440_verification(hashvalue, TTS6440_SHORTHASH_BYTES, signature->signature.buf, TTS6440_SIGNATURE_BYTES, PK->key.buf, PK->key.size);
		} else if(strcmp((char *)signature->version.buf, "sflashv1") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[33];
			int conv[crypto_hash_sha512_BYTES];
			if(strcmp((char *)signature->digest.buf, "sha256") == 0)
			{
				memcpy(hashvalue, &out[0], 32);
				memcpy(&hashvalue[32], &out[0], 1);
			} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
			{
				memcpy(hashvalue, &out[0], 33);
			}
			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			move = "mv " + sgntmp + " " + sgntmp + ".sgn";
			system((char *)move.c_str());
			// rename((char *)sgntmp.c_str(), (char *)(sgntmp + ".sgn").c_str());

			std::string pubtmp = "/var/tmp/pub_XXXXXX";
			mkstemp((char *)pubtmp.c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);
			move = "mv " + pubtmp + " " + pubtmp + ".pub";
			system((char *)move.c_str());
			// rename((char *)pubtmp.c_str(), (char *)(pubtmp + ".pub").c_str());
			
	
			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}
			std::string sage = "sage -python /usr/local/mqcrypto/sage/verify.py Sflashv1 " + (std::string)converted + " " + pubtmp + " " + sgntmp;;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;

        	remove((char *)(pubtmp + ".pub").c_str());
        	remove((char *)(sgntmp + ".sgn").c_str());
		} else if(strcmp((char *)signature->version.buf, "sflashv2") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[33];
			int conv[crypto_hash_sha512_BYTES];
			if(strcmp((char *)signature->digest.buf, "sha256") == 0)
			{
				memcpy(hashvalue, &out[0], 32);
				memcpy(&hashvalue[32], &out[0], 1);
			} else if(strcmp((char *)signature->digest.buf, "sha512") == 0)
			{
				memcpy(hashvalue, &out[0], 33);
			}
			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			move = "mv " + sgntmp + " " + sgntmp + ".sgn";
			system((char *)move.c_str());
			// rename((char *)sgntmp.c_str(), (char *)(sgntmp + ".sgn").c_str());

			std::string pubtmp = "/var/tmp/pub_XXXXXX";
			mkstemp((char *)pubtmp.c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);
			move = "mv " + pubtmp + " " + pubtmp + ".pub";
			system((char *)move.c_str());
			// rename((char *)pubtmp.c_str(), (char *)(pubtmp + ".pub").c_str());

			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}

			std::string sage = "sage -python /usr/local/mqcrypto/sage/verify.py Sflashv2 " + (std::string)converted + " " + pubtmp + " " + sgntmp;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;

        	remove((char *)(pubtmp + ".pub").c_str());
        	remove((char *)(sgntmp + ".sgn").c_str());
		} else if(strcmp((char *)signature->version.buf, "uov") == 0)
		{
			if(!SageMath)
			{
    			printf("MQCrypto: sage: command not found\n");
    			return 1;
			}
			unsigned char hashvalue[20];
			int conv[crypto_hash_sha512_BYTES];
			memcpy(hashvalue, &out[0], 20);

			std::string sgntmp = "/var/tmp/sgn_XXXXXX";
			mkstemp((char *)sgntmp.c_str());
			writeEncryptedFile(signature->signature.buf, (char *)sgntmp.c_str(), signature->signature.size);
			move = "mv " + sgntmp + " " + sgntmp + ".sgn";
			system((char *)move.c_str());
			// rename((char *)sgntmp.c_str(), (char *)(sgntmp + ".sgn").c_str());

			std::string pubtmp = "/var/tmp/pub_XXXXXX";
			mkstemp((char *)pubtmp.c_str());
			writeEncryptedFile(PK->key.buf, (char *)pubtmp.c_str(), PK->key.size);
			move = "mv " + pubtmp + " " + pubtmp + ".pub";
			system((char *)move.c_str());
			// rename((char *)pubtmp.c_str(), (char *)(pubtmp + ".pub").c_str());

			char converted[33 * 2 + 1];
			for(int i = 0; i < 64; i++)
			{
				conv[i] = (int)out[i];
    			sprintf(&converted[i*2], "%02X", conv[i]);
			}

			std::string sage = "sage -python /usr/local/mqcrypto/sage/verify.py UOV 16 40 80 " + (std::string)converted + " " + pubtmp + " " + sgntmp;;
			redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 			std::string str;
        	std::getline(proc, str, '\0');
        	std::cout << str;

        	remove((char *)(pubtmp + ".pub").c_str());
        	remove((char *)(sgntmp + ".sgn").c_str());
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

    return 0;
}