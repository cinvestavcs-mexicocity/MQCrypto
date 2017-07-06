#include "system.hh"
#include "encoding.hh"
#include "keypair.hh"
#include "signature.hh"
#include "encrypt.hh"

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

void generateKeypairHelp(void)
{
	printf("generateKeypair [scheme] [output]\n");
	printf("-scheme\t\t\t\tScheme to generate keypair of, the following schemes can be used\n");
	printf("-output (Optional)\t\tFile to save the generated set of keys, ");
	printf("the keys will be generate in the files output_PK.bin and output_SK.bin, ");
	printf("if left in blank the keys will output in the files scheme_PK.bin and scheme_SK.bin\n");
	printf("\nSchemes that can be used:\n");
	printf("rainbow5640\n");
	printf("rainbow6440\n");
	printf("rainbow16242020\n");
	printf("rainbow256181212\n");
	printf("pflash\n");
  printf("sflashv2\n");
	printf("hfe\n\n");
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
	printf("-generateKeypair\tGenerate a public and private pair of keys of a choosen scheme\n");
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
  else if(strcmp(args[0], "genKeyPair") == 0)
  {
  	if(argc > 1)
  	{
  		if(strcmp(args[1], "help") == 0)
  		{
  			generateKeypairHelp();
  			return 1;
  		}
  		if(argc > 3)
  		{
  			printf("Incorrect use...\n");
  			generateKeypairHelp();
  			return 1;
  		}
  		switch(argc)
  		{
  			case 2: 
  				generateKeypair(args[1]);
  				break;
  			case 3: 
  				generateKeypair(args[1], args[2]);
  				break;
  			default: 
  				printf("Incorrect use...\n");
  				generateKeypairHelp();
  		}
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
  		printf("MPKC > ");
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

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.
}