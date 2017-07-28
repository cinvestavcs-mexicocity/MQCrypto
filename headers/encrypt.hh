int encrypt(char *filename, char *PKfilename, char *output)
{
	MPKCMessage_t *cipher; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	char *version;
  	uint8_t *pk;
	uint64_t pklen;

	uint8_t *cm;
	uint64_t lcm;

  	cipher = (MPKCMessage_t *)calloc(1, sizeof(MPKCMessage_t));
  	if(!cipher) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

	std::string content = "";
	unsigned long long len;

	readFile(filename, &content, &len);

	cm = (uint8_t *)malloc(sizeof(uint8_t)*len+256);

	std::string tmpfile = base64decodeKey(PKfilename);

	MPKCPublicKey_t *PK = decodePublicKey((char *)tmpfile.c_str());

	version = (char *)PK->version.buf;
	pklen = PK->key.size;
	pk = PK->key.buf;

	if(strcmp(version, "hfe") == 0)
	{
		std::cout<<"Encrypting using HFE keys...\n";
	} else
	{
		std::cout<<"This implementation has support for encrypting using the following algorithms:\n-hfe\n";
		return -1;
	}

	remove(tmpfile.c_str());

	if(OCTET_STRING_fromBuf(&cipher->version, &version[0], strlen(version)) < 0) {
    	perror("Error Buffer");
  	}

  	// std::cout<<"Encrypting message: "<<content<<"\n";

  	hfe_encrypt(cm, &lcm, (unsigned char *)content.c_str(), len, pk);

  	if(OCTET_STRING_fromBuf(&cipher->message, (char *)cm, lcm) < 0) {
    	perror("Error Buffer");
  	}

  	char *Cipherfilename;
  	std::string Cipherfile = "/var/tmp/f_XXXXXX";
  	// Cipherfilename = tmpnam(nullptr);
  	Cipherfilename = (char *)Cipherfile.c_str();

  	FILE *fp = fopen(Cipherfilename, "wb");

  	ec = der_encode(&asn_DEF_MPKCMessage, cipher, write_out, fp);

    if(ec.encoded == -1) {
      	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
      	ec.failed_type ? ec.failed_type->name : "unknown");
      	return -1;
    }

    fclose(fp);

    base64encodeCiphertext(filename, Cipherfilename);

    remove(Cipherfilename);

    xer_fprint(stdout, &asn_DEF_MPKCMessage, cipher);

    return 0;
}

int decrypt(const char *filename, const char *SKfilename)
{
	MPKCMessage_t *cipher; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	char *version;
  	uint8_t *sk;
	uint64_t sklen;

	uint8_t *cm;
	uint64_t lcm;

  	cipher = (MPKCMessage_t *)calloc(1, sizeof(MPKCMessage_t));
  	if(!cipher) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

	std::string content = "";
	unsigned long long len;

	std::string tmpcipherfile = base64decodeKey(filename);

	cipher = decodeMessage((char *)tmpcipherfile.c_str());

	std::string tmpskfile = base64decodeKey(SKfilename);

	MPKCPrivateKey_t *SK = decodePrivateKey((char *)tmpskfile.c_str());

	sklen = SK->key.size;
	sk = SK->key.buf;

	if(strcmp((char *)cipher->version.buf, (char *)SK->version.buf) == 0)
	{
		if(strcmp((char *)cipher->version.buf, "hfe") == 0)
		{
			if(hfe_decrypt(cm, &lcm, cipher->message.buf, cipher->message.size, sk) == 0)
			{
				std::cout<<"Decrypt: Success\n";
				// for(int i = 0;i < lcm; i++)
				// 	printf("%x ",cm[i]);
				// printf("\n");
			} else
			{
				std::cout<<"Decrypt: Fail\n";
			}
		} else
		{
			std::cout<<"Decrypt: Fail\nNo supported cryptosystem used\n";
		return -1;
		}
	} else
	{
		std::cout<<"Decrypt: Fail\nERROR: The public key and encrypted document use different cryptosystems\n";
		return -1;
	}
	return 0;
}

int decrypt(const char *filename, const char *SKfilename, const char *output)
{
	MPKCMessage_t *cipher; /* MPKC Signature Structure */
  	asn_enc_rval_t ec;      /* Encoder return value  */

  	char *version;
  	uint8_t *sk;
	uint64_t sklen;

	uint8_t *cm;
	uint64_t lcm;

  	cipher = (MPKCMessage_t *)calloc(1, sizeof(MPKCMessage_t));
  	if(!cipher) {
    	perror("Signature calloc() failed");
    	exit(1);
  	}

	std::string content = "";
	unsigned long long len;

	std::string tmpcipherfile = base64decodeKey(filename);

	cipher = decodeMessage((char *)tmpcipherfile.c_str());

	std::string tmpskfile = base64decodeKey(SKfilename);

	MPKCPrivateKey_t *SK = decodePrivateKey((char *)tmpskfile.c_str());

	sklen = SK->key.size;
	sk = SK->key.buf;

	if(strcmp((char *)cipher->version.buf, (char *)SK->version.buf) == 0)
	{
		if(strcmp((char *)cipher->version.buf, "hfe") == 0)
		{
			if(hfe_decrypt(cm, &lcm, cipher->message.buf, cipher->message.size, sk) == 0)
			{
				std::cout<<"Decrypt: Success\n";
				FILE *fp = fopen(output, "wb");
				fwrite(cm, sizeof(char), lcm, fp);
				fclose(fp);
			} else
			{
				std::cout<<"Decrypt: Fail\n";
			}
		} else
		{
			std::cout<<"Decrypt: Fail\nNo supported cryptosystem used\n";
		return -1;
		}
	} else
	{
		std::cout<<"Decrypt: Fail\nERROR: The public key and encrypted document use different cryptosystems\n";
		return -1;
	}
	return 0;
}