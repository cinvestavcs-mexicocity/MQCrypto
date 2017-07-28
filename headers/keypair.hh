// void generateKeypair(char cryptosystem[], char output[])
// {
// 	if(strcmp(cryptosystem, "rainbow5640") == 0)
// 	{
// 		uint8 pk[RAINBOW5640_PK_BYTES];
// 		uint8 sk[RAINBOW5640_SK_BYTES];

// 		uint64 lpk,lsk;

// 		std::cout<<"Generating Rainbow5640 Keypair...\n";
// 		rainbow5640_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("rainbow5640", (char *)pk, RAINBOW5640_PK_BYTES, (char *)sk, RAINBOW5640_SK_BYTES, output);
// 	} else if(strcmp(cryptosystem, "rainbow6440") == 0)
// 	{
// 		uint8 pk[RAINBOW6440_PK_BYTES];
// 		uint8 sk[RAINBOW6440_SK_BYTES];

// 		uint64 lpk,lsk;

// 		std::cout<<"Generating Rainbow6440 Keypair...\n";
// 		rainbow6440_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("rainbow6440", (char *)pk, RAINBOW6440_PK_BYTES, (char *)sk, RAINBOW6440_SK_BYTES, output);
// 	} else if(strcmp(cryptosystem, "rainbow16242020") == 0)
// 	{
// 		unsigned char pk[RAINBOW16242020_PK_BYTES];
// 		unsigned long long pklen;
// 		unsigned char sk[RAINBOW16242020_SK_BYTES];
// 		unsigned long long sklen;

// 		std::cout<<"Generating Rainbow16242020 Keypair...\n";
// 		rainbow16242020_keypair( sk , &sklen , pk , &pklen );

// 		encodeKeyPair("rainbow16242020", (char *)pk, RAINBOW16242020_PK_BYTES, (char *)sk, RAINBOW16242020_SK_BYTES, output);
// 	} else if(strcmp(cryptosystem, "rainbow256181212") == 0)
// 	{
// 		unsigned char pk[RAINBOW256181212_PK_BYTES];
// 		unsigned long long pklen;
// 		unsigned char sk[RAINBOW256181212_SK_BYTES];
// 		unsigned long long sklen;

// 		std::cout<<"Generating Rainbow256181212 Keypair...\n";
// 		rainbow256181212_keypair( sk , &sklen , pk , &pklen );

// 		encodeKeyPair("rainbow256181212", (char *)pk, RAINBOW256181212_PK_BYTES, (char *)sk, RAINBOW256181212_SK_BYTES, output);
// 	} else if(strcmp(cryptosystem, "pflash") == 0)
// 	{
// 		uint8_t pk[PUBLICKEY_BYTES];
// 		uint8_t sk[SECRETKEY_BYTES];

// 		uint64_t lpk,lsk;

// 		std::cout<<"Generating PFlash Keypair...\n";
// 		pflash_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("pflash", (char *)pk, PUBLICKEY_BYTES, (char *)sk, SECRETKEY_BYTES, output);
// 	} else if(strcmp(cryptosystem, "hfe") == 0)
// 	{
// 		uint8_t pk[HFE_PUBLICKEY_BYTES];
// 		uint8_t sk[HFE_SECRETKEY_BYTES];

// 		uint64_t lpk,lsk;

// 		std::cout<<"Generating HFE Keypair...\n";
// 		hfe_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("hfe", (char *)pk, HFE_PUBLICKEY_BYTES, (char *)sk, HFE_SECRETKEY_BYTES, "hfe");
// 	} else if(strcmp(cryptosystem, "sflashv2") == 0)
// 	{
// 		std::string line;
// 		std::cout <<"asjklfajsk";
// 		// redi::ipstream proc("./sflashv2Keypair", redi::pstreams::pstdout | redi::pstreams::pstderr);
//  		redi::ipstream proc("sage -python sflashv2_test.py", redi::pstreams::pstdout | redi::pstreams::pstderr);
//  		// std::string pk,sk;
//  		// std::getline(proc.out(), pk);
//  		// std::getline(proc.out(), sk);
//  		// base64encodeKeyPair((char *)pk.c_str(), (char *)sk.c_str(), output);
//  		// remove((char *)pk.c_str());
//  		// remove((char *)sk.c_str());

//  		while (std::getline(proc.out(), line))
//  		  std::cout << "stdout: " << line << '\n';
//  		// read child's stderr
//  		// while (std::getline(proc.err(), line))
//  		//   std::cout << "stderr: " << line << '\n';
// 	} else
// 	{
// 		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\nsflashv2\n-pflash\n");
// 	}
// }

// void generateKeypair(char cryptosystem[]) {
// 	if(strcmp(cryptosystem, "rainbow5640") == 0)
// 	{
// 		uint8 pk[RAINBOW5640_PK_BYTES];
// 		uint8 sk[RAINBOW5640_SK_BYTES];

// 		uint64 lpk,lsk;

// 		std::cout<<"Generating Rainbow5640 Keypair...\n";
// 		rainbow5640_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("rainbow5640", (char *)pk, RAINBOW5640_PK_BYTES, (char *)sk, RAINBOW5640_SK_BYTES, "rainbow5640");
// 	} else if(strcmp(cryptosystem, "rainbow6440") == 0)
// 	{
// 		uint8 pk[RAINBOW6440_PK_BYTES];
// 		uint8 sk[RAINBOW6440_SK_BYTES];

// 		uint64 lpk,lsk;

// 		std::cout<<"Generating Rainbow6440 Keypair...\n";
// 		rainbow6440_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("rainbow6440", (char *)pk, RAINBOW6440_PK_BYTES, (char *)sk, RAINBOW6440_SK_BYTES, "rainbow6440");
// 	} else if(strcmp(cryptosystem, "rainbow16242020") == 0)
// 	{
// 		unsigned char pk[RAINBOW16242020_PK_BYTES];
// 		unsigned long long pklen;
// 		unsigned char sk[RAINBOW16242020_SK_BYTES];
// 		unsigned long long sklen;

// 		std::cout<<"Generating Rainbow16242020 Keypair...\n";
// 		rainbow16242020_keypair( sk , &sklen , pk , &pklen );

// 		encodeKeyPair("rainbow16242020", (char *)pk, RAINBOW16242020_PK_BYTES, (char *)sk, RAINBOW16242020_SK_BYTES, "rainbow16242020");
// 	} else if(strcmp(cryptosystem, "rainbow256181212") == 0)
// 	{
// 		unsigned char pk[RAINBOW256181212_PK_BYTES];
// 		unsigned long long pklen;
// 		unsigned char sk[RAINBOW256181212_SK_BYTES];
// 		unsigned long long sklen;

// 		std::cout<<"Generating Rainbow256181212 Keypair...\n";
// 		rainbow256181212_keypair( sk , &sklen , pk , &pklen );

// 		encodeKeyPair("rainbow256181212", (char *)pk, RAINBOW256181212_PK_BYTES, (char *)sk, RAINBOW256181212_SK_BYTES, "rainbow256181212");
// 	} else if(strcmp(cryptosystem, "pflash") == 0)
// 	{
// 		uint8_t pk[PUBLICKEY_BYTES];
// 		uint8_t sk[SECRETKEY_BYTES];

// 		uint64_t lpk,lsk;

// 		std::cout<<"Generating PFlash Keypair...\n";
// 		pflash_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("pflash", (char *)pk, PUBLICKEY_BYTES, (char *)sk, SECRETKEY_BYTES, "pflash");
// 	} else if(strcmp(cryptosystem, "hfe") == 0)
// 	{
// 		uint8_t pk[HFE_PUBLICKEY_BYTES];
// 		uint8_t sk[HFE_SECRETKEY_BYTES];

// 		uint64_t lpk,lsk;

// 		std::cout<<"Generating HFE Keypair...\n";
// 		hfe_keypair( sk , &lsk , pk , &lpk );

// 		encodeKeyPair("hfe", (char *)pk, HFE_PUBLICKEY_BYTES, (char *)sk, HFE_SECRETKEY_BYTES, "hfe");
// 	} else if(strcmp(cryptosystem, "sflashv2") == 0)
// 	{
// 		std::string line;
// 		std::string pk, sk;
// 		// redi::ipstream proc("./sflashv2Keypair", redi::pstreams::pstdout | redi::pstreams::pstderr);
//  		redi::ipstream proc("sage -python sflashv2_test.py", redi::pstreams::pstdout | redi::pstreams::pstderr);
//  		// std::string pk,sk;
//  		std::getline(proc.out(), pk);
//  		std::getline(proc.out(), sk);

//  		std::cout<<pk<<"\n";
//  		std::cout<<sk<<"\n";
//  		// base64encodeKeyPair((char *)pk.c_str(), (char *)sk.c_str(), output);
//  		// remove((char *)pk.c_str());
//  		// remove((char *)sk.c_str());

//  		while (std::getline(proc.out(), line))
//  		  std::cout << "stdout: " << line << '\n';
// 	} else
// 	{
// 		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n-sflashv2\n-4hfe\n");
// 	}
// }

char *generateKeypair(char cryptosystem[]) {
	char *Keypairfilename = "";
	if(strcmp(cryptosystem, "rainbow5640") == 0)
	{
		uint8 pk[RAINBOW5640_PK_BYTES];
		uint8 sk[RAINBOW5640_SK_BYTES];

		uint64 lpk,lsk;

		std::cout<<"Generating Rainbow5640 Keypair...\n";
		rainbow5640_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("rainbow5640", (char *)pk, RAINBOW5640_PK_BYTES, (char *)sk, RAINBOW5640_SK_BYTES);
	} else if(strcmp(cryptosystem, "rainbow6440") == 0)
	{
		uint8 pk[RAINBOW6440_PK_BYTES];
		uint8 sk[RAINBOW6440_SK_BYTES];

		uint64 lpk,lsk;

		std::cout<<"Generating Rainbow6440 Keypair...\n";
		rainbow6440_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("rainbow6440", (char *)pk, RAINBOW6440_PK_BYTES, (char *)sk, RAINBOW6440_SK_BYTES);
	} else if(strcmp(cryptosystem, "rainbow16242020") == 0)
	{
		unsigned char pk[RAINBOW16242020_PK_BYTES];
		unsigned long long pklen;
		unsigned char sk[RAINBOW16242020_SK_BYTES];
		unsigned long long sklen;

		std::cout<<"Generating Rainbow16242020 Keypair...\n";
		rainbow16242020_keypair( sk , &sklen , pk , &pklen );

		Keypairfilename = encodeKeyPair("rainbow16242020", (char *)pk, RAINBOW16242020_PK_BYTES, (char *)sk, RAINBOW16242020_SK_BYTES);
	} else if(strcmp(cryptosystem, "rainbow256181212") == 0)
	{
		unsigned char pk[RAINBOW256181212_PK_BYTES];
		unsigned long long pklen;
		unsigned char sk[RAINBOW256181212_SK_BYTES];
		unsigned long long sklen;

		std::cout<<"Generating Rainbow256181212 Keypair...\n";
		rainbow256181212_keypair( sk , &sklen , pk , &pklen );

		Keypairfilename = encodeKeyPair("rainbow256181212", (char *)pk, RAINBOW256181212_PK_BYTES, (char *)sk, RAINBOW256181212_SK_BYTES);
	} else if(strcmp(cryptosystem, "pflash") == 0)
	{
		uint8_t pk[PUBLICKEY_BYTES];
		uint8_t sk[SECRETKEY_BYTES];

		uint64_t lpk,lsk;

		std::cout<<"Generating PFlash Keypair...\n";
		pflash_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("pflash", (char *)pk, PUBLICKEY_BYTES, (char *)sk, SECRETKEY_BYTES);
	} else if(strcmp(cryptosystem, "hfe") == 0)
	{
		uint8_t pk[HFE_PUBLICKEY_BYTES];
		uint8_t sk[HFE_SECRETKEY_BYTES];

		uint64_t lpk,lsk;

		std::cout<<"Generating HFE Keypair...\n";
		hfe_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("hfe", (char *)pk, HFE_PUBLICKEY_BYTES, (char *)sk, HFE_SECRETKEY_BYTES);
	} else if(strcmp(cryptosystem, "3icp") == 0)
	{
		uint8_t pk[IC3_PUBLICKEY_BYTES];
		uint8_t sk[IC3_SECRETKEY_BYTES];

		uint64_t lpk,lsk;

		std::cout<<"Generating 3ICP Keypair...\n";
		ic3_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("3icp", (char *)pk, IC3_PUBLICKEY_BYTES, (char *)sk, IC3_SECRETKEY_BYTES);
	} else if(strcmp(cryptosystem, "tts6440") == 0)
	{
		uint8_t pk[TTS6440_PUBLICKEY_BYTES];
		uint8_t sk[TTS6440_SECRETKEY_BYTES];

		uint64_t lpk,lsk;

		std::cout<<"Generating TTS6440 Keypair...\n";
		tts6440_keypair( sk , &lsk , pk , &lpk );

		Keypairfilename = encodeKeyPair("tts6440", (char *)pk, TTS6440_PUBLICKEY_BYTES, (char *)sk, TTS6440_SECRETKEY_BYTES);
	} else if(strcmp(cryptosystem, "sflashv1") == 0)
	{
		// uint8_t pk[100000];
		// uint8_t sk[100000];
		std::string pk = "";
		std::string sk = "";

		uint64_t lpk,lsk;

		std::cout<<"Generating SFlashV1 Keypair...\n";
		std::string keytmp = "/var/tmp/s1_XXXXXX";
		mkstemp((char *)keytmp.c_str());
		std::string sage = "sage -python ~/Documents/MAESTRIA/MQCrypto/sage/keygen.py Sflashv1 " + keytmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');

        if(str == "Key pair generated.\n")
        {
			readBinaryFile((char *)(keytmp + ".pub").c_str(), &pk, &lpk);
        	readBinaryFile((char *)(keytmp + ".priv").c_str(), &sk, &lsk);

			char keypairfile[] = "/var/tmp/ASN1_XXXXXX";
			mkstemp(keypairfile);
			Keypairfilename = (char *)keypairfile;
        	encodeKeyPair("sflashv1", (char *)pk.c_str(), lpk, (char *)sk.c_str(), lsk, Keypairfilename);
        } else {
        	std::cout<<"ERROR: Key generation failed.\n";
        }
	} else if(strcmp(cryptosystem, "sflashv2") == 0)
	{
		// uint8_t pk[100000];
		// uint8_t sk[100000];
		std::string pk = "";
		std::string sk = "";

		uint64_t lpk,lsk;

		std::cout<<"Generating SFlashV2 Keypair...\n";
		std::string keytmp = "/var/tmp/s2_XXXXXX";
		mkstemp((char *)keytmp.c_str());
		std::string sage = "sage -python ~/Documents/MAESTRIA/MQCrypto/sage/keygen.py Sflashv2 " + keytmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');

        if(str == "Key pair generated.\n")
        {
        	readBinaryFile((char *)(keytmp + ".pub").c_str(), &pk, &lpk);
        	readBinaryFile((char *)(keytmp + ".priv").c_str(), &sk, &lsk);

        	char keypairfile[] = "/var/tmp/ASN1_XXXXXX";
			mkstemp(keypairfile);
			Keypairfilename = (char *)keypairfile;
        	encodeKeyPair("sflashv2", (char *)pk.c_str(), lpk, (char *)sk.c_str(), lsk, Keypairfilename);
        } else {
        	std::cout<<"ERROR: Key generation failed.\n";
        }

	} else if(strcmp(cryptosystem, "uov") == 0)
	{
		std::string pk = "";
		std::string sk = "";

		uint64_t lpk,lsk;

		std::cout<<"Generating UOV Keypair...\n";
		std::string keytmp = "/var/tmp/uov_XXXXXX";
		mkstemp((char *)keytmp.c_str());
		std::string sage = "sage -python ~/Documents/MAESTRIA/MQCrypto/sage/keygen.py UOV 16 40 80 " + keytmp;
		redi::ipstream proc(sage, redi::pstreams::pstdout | redi::pstreams::pstderr);

 		std::string str;
        std::getline(proc, str, '\0');

        if(str == "Key pair generated.\n")
        {
        	readBinaryFile((char *)(keytmp + ".pub").c_str(), &pk, &lpk);
        	readBinaryFile((char *)(keytmp + ".priv").c_str(), &sk, &lsk);

        	char keypairfile[] = "/var/tmp/ASN1_XXXXXX";
			mkstemp(keypairfile);
			Keypairfilename = (char *)keypairfile;
        	encodeKeyPair("uov", (char *)pk.c_str(), lpk, (char *)sk.c_str(), lsk, Keypairfilename);
        } else {
        	std::cout<<"ERROR: Key generation failed.\n";
        }

	} else
	{
		printf("This system has support for generating key pair of the following MPKC schemes\n-rainbow5640\n-rainbow6440\n-rainbow16242020\n-rainbow256181212\n-pflash\n-sflashv1\n-sflashv2\n-3icp\n-tts6440\n-uov");
	}
	return Keypairfilename;
}