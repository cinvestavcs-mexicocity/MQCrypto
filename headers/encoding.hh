/* Write the encoded output into some FILE stream. */
static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = (FILE *)app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}

std::string base64decodeKey(const char *Keyfile)
{
	std::string inputKey = Keyfile;
	base64::decoder D;

	std::ifstream instream(inputKey.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!instream.is_open())
	{
		std::cout<<"Error abrir archivo "<<inputKey<<": ifstream\n";
		exit(-1);
	}

	const char *tmpfile = tmpnam((char *)tmpfile);
	std::string outputKey = tmpfile;
	
	std::ofstream outstream(outputKey.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outstream.is_open())
	{
		std::cout<<"Error abrir archivo "<<outputKey<<": ofstream\n";
		exit(-1);
	}

	D.decode(instream, outstream);

	return outputKey;
}

int base64decodeKeyPair(const char *PKfile, const char *SKfile)
{
	std::string inputPK = PKfile;
	std::string inputSK = SKfile;
	std::string outputPK = "Base64DecodedPublicKey.txt";
	std::string outputSK = "Base64DecodedPrivateKey.txt";
	base64::decoder D;

	std::ifstream inPKstream(inputPK.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inPKstream.is_open())
	{
		// usage("Could not open inputPK file!");
		exit(-1);
	}
	
	std::ofstream outPKstream(outputPK.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outPKstream.is_open())
	{
		// usage("Could not open outputPK file!");
		exit(-1);
	}

	D.decode(inPKstream, outPKstream);

	std::ifstream inSKstream(inputSK.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inSKstream.is_open())
	{
		// usage("Could not open inputSK file!");
		exit(-1);
	}

	std::ofstream outSKstream(outputSK.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outSKstream.is_open())
	{
		// usage("Could not open outputSK file!");
		exit(-1);
	}

	D.decode(inSKstream, outSKstream);

	return 0;
}

int base64encodeKeyPair(const char *Keypairfile, char output[])
{
	std::string inputKP = Keypairfile;
	std::string outputKP = output;
	base64::encoder E;

	std::ifstream inKPstream(inputKP.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inKPstream.is_open())
	{
		std::cout<<"Could not open file: "<<inputKP<<"\n";
		exit(-1);
	}
	
	std::ofstream outKPstream(outputKP.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outKPstream.is_open())
	{
		std::cout<<"Could not open file: "<<outputKP<<"\n";
		exit(-1);
	}

	E.encode(inKPstream, outKPstream);

	// std::cout<<"Keypair saved in file: "<<output<<"\n";

	inKPstream.close();
	outKPstream.close();

	return 0;
}


int base64encodeKeyPair(const char *PKfile, const char *SKfile, char output[])
{
	std::string inputPK = PKfile;
	std::string inputSK = SKfile;
	std::string outputPK = (std::string)"keys/" + output + (std::string)"_Pub.bin";
	std::string outputSK = (std::string)"keys/" + output + (std::string)"_Priv.bin";
	base64::encoder E;

	std::ifstream inPKstream(inputPK.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inPKstream.is_open())
	{
		std::cout<<"Could not open file: "<<inputPK<<"\n";
		exit(-1);
	}
	
	std::ofstream outPKstream(outputPK.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outPKstream.is_open())
	{
		std::cout<<"Could not open file: "<<outputPK<<"\n";
		exit(-1);
	}

	E.encode(inPKstream, outPKstream);

	std::cout<<"Public key saved in file: "<<output<<"_Pub.bin\n";

	std::ifstream inSKstream(inputSK.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!inSKstream.is_open())
	{
		std::cout<<"Could not open file: "<<inputSK<<"\n";
		exit(-1);
	}

	std::ofstream outSKstream(outputSK.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outSKstream.is_open())
	{
		std::cout<<"Could not open file: "<<outputSK<<"\n";
		exit(-1);
	}

	E.encode(inSKstream, outSKstream);

	std::cout<<"Private key saved in file: "<<output<<"_Priv.bin\n";

	return 0;
}

int base64encodeSignature(const char *outputFile, const char *Sigfile)
{
	std::string input = Sigfile;
	std::string output = outputFile;

	base64::encoder E;

	std::ifstream instream(input.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!instream.is_open())
	{
		std::cout<<"Could not open file: "<<input<<"\n";
		exit(-1);
	}
	
	std::ofstream outstream(output.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outstream.is_open())
	{
		std::cout<<"Could not open file: "<<output<<"\n";
		exit(-1);
	}

	E.encode(instream, outstream);

	return 0;
}

int base64encodeCiphertext(const char *inputFile, const char *Ciphertext)
{
	std::string input = Ciphertext;
	std::string output = inputFile;
	output += "_cipher.txt";

	base64::encoder E;

	std::ifstream instream(input.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!instream.is_open())
	{
		exit(-1);
	}
	
	std::ofstream outstream(output.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outstream.is_open())
	{
		exit(-1);
	}

	E.encode(instream, outstream);

	return 0;
}

// int encodeKeyPair(char cryptosystem[], 
//   const char *pk, const unsigned long long pklen,
//   const char *sk, const unsigned long long sklen,
//   char output[])
// {
//   	MPKCPublicKey_t *PK; /* MPKC Public Key Structure */
//   	MPKCPrivateKey_t *SK; /* MPKC Private Key Structure */
//   	asn_enc_rval_t ec;      /* Encoder return value  */

//   	PK = (MPKCPublicKey_t *)calloc(1, sizeof(MPKCPublicKey_t));
//   	if(!PK) {
//     	perror("Public Key calloc() failed");
//     	exit(1);
//   	}

//   	SK = (MPKCPrivateKey_t *)calloc(1, sizeof(MPKCPrivateKey_t));
//   	if(!SK) {
//     	perror("Private Key calloc() failed");
//     	exit(1);
//   	}

//   	if(OCTET_STRING_fromBuf(&PK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
//     	perror("Error Buffer");
//   	}

//   	if(OCTET_STRING_fromBuf(&PK->key, pk, pklen) < 0) {
//     	perror("Error Buffer");
//   	}

//   	if(OCTET_STRING_fromBuf(&SK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
//     	perror("Error Buffer");
//   	}

//   	if(OCTET_STRING_fromBuf(&SK->key, sk, sklen) < 0) {
//     	perror("Error Buffer");
//   	}

//   	char *PKfilename;
//   	char *SKfilename;
//   	PKfilename = tmpnam(PKfilename);
  	
//   	FILE *fp = fopen(PKfilename, "wb");
  	
//     /* for Public Key BER output */
//     if(!fp) {
//     	perror(PKfilename);
//     	exit(1);
//     }

//     ec = der_encode(&asn_DEF_MPKCPublicKey, PK, write_out, fp);

//     if(ec.encoded == -1) {
//       	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
//       	ec.failed_type ? ec.failed_type->name : "unknown");
//       	return -1;
//     }

//     fclose(fp);

//     // xer_fprint(stdout, &asn_DEF_MPKCPublicKey, PK);

//     SKfilename = tmpnam(SKfilename);

//     fp = fopen(SKfilename, "wb");

//     /* for Private Key BER output */
//     if(!fp) {
//     	perror(SKfilename);
//     	exit(1);
//     }

//     ec = der_encode(&asn_DEF_MPKCPrivateKey, SK, write_out, fp);
    
//     if(ec.encoded == -1) {
//       	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
//       	ec.failed_type ? ec.failed_type->name : "unknown");
//       	return -1;
//     }

//     fclose(fp);

//     base64encodeKeyPair(PKfilename, SKfilename, output);

// 	remove(PKfilename);
// 	remove(SKfilename);

// 	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK);

//     return 0;
// }

char *encodeKeyPair(char cryptosystem[], 
  const char *pk, const unsigned long long pklen,
  const char *sk, const unsigned long long sklen)
{
	MPKCKeypair_t *Keypair;
  	asn_enc_rval_t ec;      /* Encoder return value  */
  	char *Keypairfilename = "";

  	Keypair = (MPKCKeypair_t *)calloc(1, sizeof(MPKCKeypair_t));
  	if(!Keypair) {
    	perror("Keypair calloc() failed");
    	exit(1);
  	}

  	if(OCTET_STRING_fromBuf(&Keypair->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&Keypair->pubkey, pk, pklen) < 0) {
    	perror("Error Buffer");
  	}

  	if(OCTET_STRING_fromBuf(&Keypair->privkey, sk, sklen) < 0) {
    	perror("Error Buffer");
  	}
  	Keypairfilename = tmpnam(nullptr);
  	FILE *fp = fopen(Keypairfilename, "wb");
  	/* for Public Key BER output */
    if(!fp) {
    	perror(Keypairfilename);
    	exit(1);
    }
    ec = der_encode(&asn_DEF_MPKCKeypair, Keypair, write_out, fp);
    if(ec.encoded == -1) {
      	fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
      	ec.failed_type ? ec.failed_type->name : "unknown");
    }
    
    fclose(fp);    

    // xer_fprint(stdout, &asn_DEF_MPKCPublicKey, PK);

    return Keypairfilename;
}

MPKCPrivateKey_t *decodePrivateKey(char *tmpfile)
{
	char buf[150512 + 100];      /* Temporary buffer      */
    asn_dec_rval_t rval; /* Decoder return value  */

  	MPKCPrivateKey_t *SK_t = 0; /* MPKC Private Key Structure */
  	
	FILE *fp;	/* Input file handler    */
	size_t size;	/* Number of bytes read  */

	/* Open input file as read-only binary */
    fp = fopen(tmpfile, "rb");
    if(!fp) {
    	perror(tmpfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(tmpfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", tmpfile);
		exit(1);
	}
    
    /* Decode the input buffer as MPKCPublicKey type */
    rval = ber_decode(0, &asn_DEF_MPKCPrivateKey, (void **)&SK_t, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPrivateKey encoding at byte %ld\n", tmpfile,
            (long)rval.consumed);
		exit(1); 
	}

	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK_t);
    
    return SK_t;
}

MPKCPublicKey_t *decodePublicKey(char *tmpfile)
{
	char buf[150512 + 100];      /* Temporary buffer      */
    asn_dec_rval_t rval; /* Decoder return value  */

  	MPKCPublicKey_t *PK_t = 0; /* MPKC Public Key Structure */
  	
	FILE *fp;	/* Input file handler    */
	size_t size;	/* Number of bytes read  */

	/* Open input file as read-only binary */
    fp = fopen(tmpfile, "rb");
    if(!fp) {
    	perror(tmpfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(tmpfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", tmpfile);
		exit(1);
	}
    
    /* Decode the input buffer as MPKCPublicKey type */
    rval = ber_decode(0, &asn_DEF_MPKCPublicKey, (void **)&PK_t, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPublicKey encoding at byte %ld\n", tmpfile,
            (long)rval.consumed);
		exit(1); 
	}

	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK_t);
    
    return PK_t;
}

MPKCSignature_t *decodeSignature(char *tmpfile)
{
	char buf[150512 + 100];      /* Temporary buffer      */
    asn_dec_rval_t rval; /* Decoder return value  */

  	MPKCSignature_t *Sig_t = 0; /* MPKC Public Key Structure */
  	
	FILE *fp;	/* Input file handler    */
	size_t size;	/* Number of bytes read  */

	/* Open input file as read-only binary */
    fp = fopen(tmpfile, "rb");
    if(!fp) {
    	perror(tmpfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(tmpfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", tmpfile);
		exit(1);
	}
    
    /* Decode the input buffer as MPKCPublicKey type */
    rval = ber_decode(0, &asn_DEF_MPKCSignature, (void **)&Sig_t, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPublicKey encoding at byte %ld\n", tmpfile,
            (long)rval.consumed);
		exit(1); 
	}

	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK_t);
    
    return Sig_t;
}

MPKCMessage_t *decodeMessage(char *tmpfile)
{
	char buf[150512 + 100];      /* Temporary buffer      */
    asn_dec_rval_t rval; /* Decoder return value  */

  	MPKCMessage_t *Mes_t = 0; /* MPKC Public Key Structure */
  	
	FILE *fp;	/* Input file handler    */
	size_t size;	/* Number of bytes read  */

	/* Open input file as read-only binary */
    fp = fopen(tmpfile, "rb");
    if(!fp) {
    	perror(tmpfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(tmpfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", tmpfile);
		exit(1);
	}
    
    /* Decode the input buffer as MPKCMessage type */
    rval = ber_decode(0, &asn_DEF_MPKCMessage, (void **)&Mes_t, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPublicKey encoding at byte %ld\n", tmpfile,
            (long)rval.consumed);
		exit(1); 
	}

	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK_t);
    
    return Mes_t;
}

int decodeKeyPair(char cryptosystem[], 
  const char *pk, const unsigned long long pklen,
  const char *sk, const unsigned long long sklen)
{
	char buf[(sklen > pklen) ? sklen + 100 : pklen + 100];      /* Temporary buffer      */
    asn_dec_rval_t rval; /* Decoder return value  */
	
	MPKCPublicKey_t *PK = 0; /* MPKC Public Key Structure */
  	MPKCPrivateKey_t *SK = 0; /* MPKC Private Key Structure */
  	
	FILE *fp;	/* Input file handler    */
	size_t size;	/* Number of bytes read  */
	const char *PKfile = "PublicKey.txt"; /* Input PK name */
	const char *SKfile = "PrivateKey.txt"; /* Input SK name */
	const char *PKfilename = "DecodedPK.txt";	/* PK file output */
	const char *SKfilename = "DecodedSK.txt"; /* SK file output */

	/* Open input file as read-only binary */
    fp = fopen(PKfile, "rb");
    if(!fp) {
    	perror(PKfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(PKfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", PKfile);
		exit(1);
	}
    
    /* Decode the input buffer as MPKCPublicKey type */
    rval = ber_decode(0, &asn_DEF_MPKCPublicKey, (void **)&PK, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPublicKey encoding at byte %ld\n", PKfile,
            (long)rval.consumed);
		exit(1); 
	}
    
    /* Print the decoded MPKCPublicKey type as XML */
    // xer_fprint(stdout, &asn_DEF_MPKCPublicKey, PK);


    /* Open input file as read-only binary */
    fp = fopen(SKfile, "rb");
    if(!fp) {
    	perror(SKfile);
		exit(1);
	}

	/* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    remove(SKfile);
    
    if(!size) {
    	fprintf(stderr, "%s: Empty or broken\n", SKfile);
		exit(1);
	}
    
    // Decode the input buffer as MPKCPrivateKey type 
    rval = ber_decode(0, &asn_DEF_MPKCPrivateKey, (void **)&SK, buf, size);
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken MPKCPrivateKey encoding at byte %ld\n", SKfile,
            (long)rval.consumed);
		exit(1); 
	}

	// xer_fprint(stdout, &asn_DEF_MPKCPrivateKey, SK);
    
    return 0; /* Decoding finished successfully */
}