#include "system.hh"
#include "encoding.hh"
#include "keypair.hh"
#include "signature.hh"
#include "encrypt.hh"
#include "aes.hh"
#include "bz2.hh"

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int readFile(const char *filename, std::string *content, unsigned long long *len)
{
  std::string line;
  std::ifstream input(filename);

  if(input.is_open())
  {
    while(getline(input, line))
    {
      *content += line;
    }
    *len = (unsigned long long)content->size();
    input.close();
  } else
  {
    std::cout << "ERROR: File not found";
    return -1;
  }
  return 0;
}

int writeBinaryFile(secure_string input, char *output)
{
  std::ofstream outstream(output, std::ios::out | std::ios::binary);

  outstream.write(input, input.length());

  outstream.close();
}

int readFile(const char *filename, secure_string *content, unsigned long long *len)
{
  secure_string line;
  std::ifstream input(filename);

  if(input.is_open())
  {
    while(getline(input, line))
    {
      *content += line;
    }
    *len = (unsigned long long)content->size();
    input.close();
  } else
  {
    std::cout << "ERROR: File not found";
    return -1;
  }
  return 0;
}

int readBinaryFile(const char *filename, secure_string *content, unsigned long long *len)
{
  secure_string line;
  std::ifstream input(filename, std::ios::in | std::ios::binary);

  if(input.is_open())
  {
    while(getline(input, line))
    {
      *content += line;
    }
    *len = (unsigned long long)content->size();
    input.close();
  } else
  {
    std::cout << "ERROR: File not found";
    return -1;
  }
  return 0;
}

int validateScheme(char *scheme)
{
  char *listOfSchemes[] = {"rainbow5640", "rainbow6440", "rainbow16242020", 
                          "rainbow256181212", "pflash", "sflashv2", "hfe"};
  size_t n = sizeof(listOfSchemes) / sizeof(listOfSchemes[0]);
  for(uint8_t i = 0; i < n; i++)
    if(strcmp(scheme, listOfSchemes[i]) == 0)
      return 1;
  return 0;
}

void printSchemes(void)
{
  printf("List of schemes that can be used:\n");
  printf("rainbow5640\n");
  printf("rainbow6440\n");
  printf("rainbow16242020\n");
  printf("rainbow256181212\n");
  printf("pflash\n");
  printf("sflashv2\n");
  printf("hfe\n\n");
}

int generateKeypairExec(char **line, uint8_t argc)
{
  char *scheme = "";
  char *output = "";
  char *symencryption = "";
  char *pass_phrase = "";
  bool ascii = false;
  bool zip = false;

  char *Keypairfilename = "";
  char b64tmp[L_tmpnam];

  unsigned char out256[32];
  unsigned char out128[32];

  secure_string content = "";
  secure_string cipher_text = "";
  secure_string plain_text = "";
  uint64_t len;

  for(uint8_t i = 0; i < argc; i++)
  {
    if(strcmp(line[i], "-scheme") == 0)
    {
      scheme = line[i+1];
      i+=1;
    } else if(strcmp(line[i], "-out") == 0)
    {
      output = line[i+1];
      i+=1;
    } else if(strcmp(line[i], "-a") == 0)
      ascii = true;
    else if(strcmp(line[i], "-zip") == 0)
      zip = true;
    else if(strcmp(line[i], "-passout") == 0)
    {
      pass_phrase = &line[i+1][5];
      i+=1;
    } else if(strcmp(line[i], "-aes_256_cbc") == 0)
      symencryption = "aes_256_cbc";
    else if(strcmp(line[i], "-aes_192_cbc") == 0)
      symencryption = "aes_192_cbc";
    else if(strcmp(line[i], "-aes_128_cbc") == 0)
      symencryption = "aes_128_cbc";
    else
    {
      printf("ERROR: Invalid option: %s\n",line[i]);
      generateKeypairHelp();
      return 1;
    }
  }

  // std::cout<<"Scheme: " << scheme << "\n";
  // std::cout<<"Output: " << output << "\n";
  // std::cout<<"Symmetric Encryption: " << symencryption << "\n";
  // std::cout<<"Password: " << pass_phrase << "\n";
  // std::cout<<"ASCII: " << ascii << "\n";

  if(strcmp(scheme, "") == 0 || strcmp(output, "") == 0 || strcmp(pass_phrase, "") == 0)
  {
    printf("ERROR: Invalid input format\n");
    generateKeypairHelp();
    return 1;
  }

  if(!validateScheme(scheme))
  {
    printf("ERROR: Invalid Scheme \"%s\"\n", scheme);
    generateKeypairHelp();
    return 1;
  }

  Keypairfilename = generateKeypair(scheme);

  if(ascii && zip)
  {
    tmpnam(b64tmp);
    base64encodeKeyPair(Keypairfilename,b64tmp);
    bz2_compress(b64tmp, output);
    readBinaryFile(output, &content, &len);
    remove(b64tmp);
  } else if(ascii)
  {
    base64encodeKeyPair(Keypairfilename,output);
    readFile(output, &content, &len);
    std::cout<<"Keypair saved in file: "<<output<<"\n";
  } else if(zip)
  {
    bz2_compress_bin(Keypairfilename, output);
    readBinaryFile(output, &content, &len);
  } else
  {
    rename(Keypairfilename, output);
    readBinaryFile(output, &content, &len);
    std::cout<<"Keypair saved in file: "<<output<<"\n";
    return 1;
  }

  crypto_hash_sha256(out256, (const unsigned char *)pass_phrase, strlen(pass_phrase));
  crypto_hash_sha256(out128, out256, 32);

  if(strcmp(symencryption, "aes_128_cbc") == 0)
  {
    EVP_add_cipher(EVP_aes_128_cbc());
    aes_128_cbc_encrypt(out256, &out128[15], content, cipher_text);
    // aes_128_cbc_decrypt(out256, &out128[15], cipher_text, plain_text);
  }
  else if(strcmp(symencryption, "aes_192_cbc") == 0)
  {
    EVP_add_cipher(EVP_aes_192_cbc());
    aes_192_cbc_encrypt(out256, &out128[15], content, cipher_text);
    // aes_192_cbc_decrypt(out256, &out128[15], cipher_text, plain_text);
  }
  else if(strcmp(symencryption, "aes_256_cbc") == 0)
  {
    EVP_add_cipher(EVP_aes_256_cbc());
    aes_256_cbc_encrypt(out256, &out128[15], content, cipher_text);
    // aes_256_cbc_decrypt(out256, &out128[15], cipher_text, plain_text);
  }

  writeBinaryFile(cipher_text, output);

  // std::cout<<"Content: "<<content<<"\n";
  // std::cout<<"cipher_text: "<<cipher_text.length()<<"\n";
  // std::cout<<"SIZEOF: "<<sizeof((uint8_t *)cipher_text.c_str())<<"\n";
  // std::cout<<"STRLEN: "<<strlen((uint8_t *)cipher_text.c_str())<<"\n";
  // std::cout<<"STRLEN PText: "<<strlen((char *)plain_text.c_str())<<"\n";
  // std::cout<<"IGUALES: "<<strcmp((char *)plain_text.c_str(), (char *)content.c_str())<<"\n";


  remove(Keypairfilename);

  return 1;


  // tmpnam(bz2Out);

  // bz2_compress_bin(Keypairfilename, bz2Out);

  // char *b64tmp = tmpnam(bz2Out);

  // base64encodeKeyPair(Keypairfilename, b64tmp);

  // bz2_compress(b64tmp, "keys/private-key.pem");
  // remove(Keypairfilename);
}

void generateKeypairHelp(void)
{
	printf("genKeys [Options]\n\n");
  printf("Options:\n");
	printf("-scheme <Scheme>\t\tScheme to generate keypair of, the following schemes can be used\n");
  printf("\t\t\t\tSchemes that can be used:\n");
  printf("\t\t\t\trainbow5640\n");
  printf("\t\t\t\trainbow6440\n");
  printf("\t\t\t\trainbow16242020\n");
  printf("\t\t\t\trainbow256181212\n");
  printf("\t\t\t\tpflash\n");
  printf("\t\t\t\tsflashv2\n");
  printf("\t\t\t\thfe\n");
	printf("-out <private-key.pem>\t\tFile to save the generated set of keys, ");
	printf("the keys will be generate in the files output_PK.bin and output_SK.bin, ");
	printf("if left in blank the keys will output in the files scheme_PK.bin and scheme_SK.bin\n");
  printf("-symencryption\t\t\tSymmetric Encryption to use for the encryption of the private key.\n");
  printf("\t\t\t\tSymmetric Cryptography Algorithms that can be used:\n");
  printf("\t\t\t\taes_128_cbc\taes_192_cbc\taes_256_cbc\n");
  printf("-passout pass:<phrase>\t\tPass phrase used to encrypt the private key.\n");
  printf("-a\t\t\t\tOutput the keypair generated in ASCII mode encoded as Base64, otherwise the output will be output in a binary file.\n\n");
  printf("-zip\t\t\tUse a compressor on the output key (using bzip2).");
}

void signHelp(void)
{
	printf("sign [input] [output] [secret key] [digest]\n");
	printf("-input\t\t\tInput filename to be signed.\n");
	printf("-output (Optional)\tOutput file in which to save the signature.\n");
	printf("-secret key\t\tSecret key to be used for the signing of the input document.\n");
	printf("-digest\t\t\tDigest algorithm to be used for the signing of the input document.\n");
	printf("\nThe following digest algorithms can be used:\n");
	printf("sha256\n");
	printf("sha512\n\n");
}

void verifyHelp(void)
{
	printf("verify [input] [sign] [public key]\n");
	printf("-input\t\t\tInput filename of the document that was signed.\n");
	printf("-sign\t\t\tSignature of the input document that is going to be verifyied.\n");
	printf("-public key\t\tPublic key to be used for the verification of the signature\n\n");
}

void encryptHelp(void)
{
	printf("encrypt [clear text] [cipher text] [public key]\n");
	printf("-clear text\t\tDocument to be encrypted.\n");
	printf("-cipher_text\t\tOutput document to save the encrypted text.\n");
	printf("-public key\t\tPublic key to encrypt the clear text.\n");
	printf("\nThe following schemes can be used to encrypt:\n");
	printf("-hfe\n\n");
}

void decryptHelp(void)
{
	printf("decrypt [cipher text] [clear text] [private key]\n");
	printf("-cipher_text\t\tInput document to be decrypted.\n");
	printf("-clear text(Optional)\tDocument to save the decrypted text.\n");
	printf("-private key\t\tPrivate key to decrypt the cipher text.\n");
	printf("\nThe following schemes can be used to encrypt:\n");
	printf("-hfe\n\n");
}

void sh_help(void)
{
	printf("The following instructions can be used\n");
	printf("-help\t\t\tDisplay this menu with instructions of how to use the implemented MPKC schemes\n");
	printf("-genKeys\tGenerate a public and private pair of keys of a choosen scheme\n");
	printf("-sign\t\t\tSign a document using a previous generated key\n");
	printf("-verify\t\t\tVerify a signature in a document\n");
	printf("-encrypt\t\tEncrypt a document using a previous generated key\n");
	printf("-decrypt\t\tDecrypt an encrypted document\n");
	printf("exit()\t\t\tExit from the execution of the present program\n\n");
}

void sh_welcome(void)
{
	system("clear");
	printf("------------------------------------------------------------------\n");
	printf("| Multivariate Public Key Cryptography schemes implementation    |\n");
	printf("| Type help to display information about the execution           |\n");
	printf("------------------------------------------------------------------\n");
}

int lsh_execute(char **args, uint8_t argc)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  if(strcmp(args[0], "clear") == 0)
  	system("clear");
  else if(strcmp(args[0], "exit()") == 0 || strcmp(args[0], "exit") == 0)
  {
  	printf("Exiting system...\n");
  	exit(0);
  }
  else if(strcmp(args[0], "help") == 0)
  	sh_help();
  else if(strcmp(args[0], "genKeys") == 0)
  {
  	if(argc > 1)
  	{
  		if(strcmp(args[1], "help") == 0)
  		{
  			generateKeypairHelp();
  			return 1;
  		}
      generateKeypairExec(&args[1], argc - 1);
  		return 1;
  	} else
  	{
  		printf("Incorrect use...\n");
  		generateKeypairHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "sign") == 0)
  {
  	if(argc > 1)
  	{
		if(strcmp(args[1], "help") == 0)
		{
  			signHelp();
  			return 1;
		}
		if(argc < 4 || argc > 5)
		{
			printf("Incorrect use...\n");
  			signHelp();
  			return 1;
		}
		switch(argc)
		{
			case 4:
				sign(args[1], args[2], args[3]);
				break;
			case 5:
				sign(args[1], args[3], args[4], args[2]);
				break;
			default:
				printf("Incorrect use...\n");
  				signHelp();
		}
		return 1;
  	} else
  	{
  		printf("Incorrect use...\n");
  		signHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "verify") == 0)
  {
  	if(argc > 1)
  	{
  		if(strcmp(args[1], "help") == 0)
  		{
  			verifyHelp();
  			return 1;
  		}
  		if(argc != 4)
  		{
  			printf("Incorrect use...\n");
  			verifyHelp();
  			return 1;
  		}
  		verify(args[1],args[2],args[3]);
  		return 1;
  	} else
  	{
  		printf("Incorrect use...\n");
  		verifyHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "encrypt") == 0)
  {
  	if(argc > 1)
  	{
  		if(strcmp(args[1], "help") == 0)
  		{
  			encryptHelp();
  			return 1;
  		}
  		if(argc != 4)
  		{
  			printf("Incorrect use...\n");
  			encryptHelp();
  			return 1;
  		}
  		encrypt(args[1],args[3],args[2]);
  		return 1;
  	} else
  	{
  		printf("Incorrect use...\n");
  		encryptHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "decrypt") == 0)
  {
  	if(argc > 1)
  	{
  		if(strcmp(args[1], "help") == 0)
  		{
  			decryptHelp();
  			return 1;
  		}
  		if(argc < 3 || argc > 4)
  		{
  			printf("Incorrect use...\n");
  			decryptHelp();
  			return 1;
  		}
  		switch(argc)
  		{
  			case 3:
  				decrypt(args[1], args[2]);
  				break;
  			case 4:
  				decrypt(args[1], args[3], args[2]);
  				break;
  			default:
  				printf("Incorrect use...\n");
  				decryptHelp();
  				return 1;
  		}
  	} else
  	{
  		printf("Incorrect use...\n");
  		decryptHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "-list_schemes") == 0)
  {
    printSchemes();
  }
  else
  	printf("%s: command not found\n",args[0]);
  return 1;

  // for (i = 0; i < lsh_num_builtins(); i++) {
  //   if (strcmp(args[0], builtin_str[i]) == 0) {
  //     return (*builtin_func[i])(args);
  //   }
  // }

  // return lsh_launch(args);
}

char **lsh_split_line(char *line, uint8_t *length)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = (char **)malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = (char **)realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  *length = position;
  tokens[position] = NULL;
  return tokens;
}

char *lsh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

void lsh_loop(void)
{
  char *line;
  char **args;
  uint8_t length;
  int status = 1;

  sh_welcome();

  do {
  		printf("MQCrypto > ");
  		line = lsh_read_line();
  		args = lsh_split_line(line, &length);
  		status = lsh_execute(args, length);  		
  		free(line);
  		free(args);
  } while (status);
}

#if 0

int main()
{
	srand(time(0));

	// // printf("\n***Iniciando prueba de firma del esquema Rainbow5640***\n");
	// // firmaRainbow5640();
	// generateKeypair("rainbow5640", "rainbow5640");
	// sign("archivo_prueba.txt", "rainbow5640_SK.txt", "sha256");
	// verify("archivo_prueba.txt", "archivo_prueba.txt_signature.txt", "rainbow5640_PK.txt");

	// // printf("\n***Iniciando prueba de firma del esquema Rainbow6440***\n");
	// // firmaRainbow6440();
	// generateKeypair("rainbow6440", "rainbow6440");
	// sign("archivo_prueba.txt", "rainbow6440_SK.txt", "sha256");
	// verify("archivo_prueba.txt", "archivo_prueba.txt_signature.txt", "rainbow6440_PK.txt");

	// // printf("\n***Iniciando prueba de firma del esquema Rainbow16242020***\n");
	// // firmaRainbow16242020();
	// generateKeypair("rainbow16242020", "rainbow16242020");
	// sign("archivo_prueba.txt", "rainbow16242020_SK.txt", "sha256");
	// verify("archivo_prueba.txt", "archivo_prueba.txt_signature.txt", "rainbow16242020_PK.txt");

	// // printf("\n***Iniciando prueba de firma del esquema Rainbow256181212***\n");
	// // firmaRainbow256181212();
	// generateKeypair("rainbow256181212", "rainbow256181212");
	// sign("archivo_prueba.txt", "rainbow256181212_SK.txt", "sha256");
	// verify("archivo_prueba.txt", "archivo_prueba.txt_signature.txt", "rainbow256181212_PK.txt");

	// // printf("\n***Iniciando prueba de firma del esquema PFlash***\n");
	// // firmaPFlash();
	// generateKeypair("pflash", "pflash");
	// sign("archivo_prueba.txt", "pflash_SK.txt", "sha256");
	// verify("archivo_prueba.txt", "archivo_prueba.txt_signature.txt", "pflash_PK.txt");

	// generateKeypair("hfe", "hfe");
	// encrypt("archivo_prueba.txt", "hfe_PK.txt", "cipher_text.txt");
	// decrypt("archivo_prueba.txt_cipher.txt", "hfe_SK.txt", "decrypted_text.txt");
}

#endif

int main(int argc, char **argv)
{
	// Load config files, if any.
  if(argc > 1)
  {
    lsh_execute(&argv[1], argc-1);
    return 0;
  }

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.
}