#include <headers/aes.hh>
#include <headers/system.hh>
#include <headers/encoding.hh>
#include <headers/keypair.hh>
#include <headers/signature.hh>
#include <headers/encrypt.hh>
#include <headers/bz2.hh>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

void SetStdinEcho(bool enable = true)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void keyHeader(const char *filename, char *header, char *footer, std::string b64content, unsigned long long len)
{
  FILE *fp = fopen(filename, "w");

  fprintf(fp, header);

  int l,start = 0;
  l = strlen("pc/sqfu3kOpjNeChthxPgkMsaEfbkudVpNv28J0sAOJhoqCl/duqLx8hk5866pcC+ceXrESl");
  while(start < len)
  {
    fprintf(fp, (char *)(b64content.substr(start, l) + "\n").c_str());
    start += l;
  }

  fprintf(fp, footer);

  fclose(fp);
}

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
    std::cout << "ERROR: File not found "<<filename<<"\n";
    return -1;
  }
  return 0;
}

void writeFile(std::string input, char *output)
{
  std::ofstream outstream(output, std::ios::out);

  outstream.write(input.c_str(), input.length());

  outstream.close();
}

void writeBinaryFile(std::string input, char *output)
{
  std::ofstream outstream(output, std::ios::out | std::ios::binary);

  outstream.write(input.c_str(), input.length());

  outstream.close();
}

void writeEncryptedFile(unsigned char *input, char *output, unsigned long long len)
{
  FILE *out;
  out = fopen(output, "wb");

  if(out)
  {
    fwrite(input, len, 1, out);
    fclose(out);
  }
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
    std::cout << "ERROR: File not found "<<filename<<"\n";
    return -1;
  }
  return 0;
}

int readBinaryFile(const char *filename, std::string *content, unsigned long long *len)
{
  std::string line;
  std::ifstream input(filename, std::ios::in | std::ios::binary);

  if(input.is_open())
  {
    input.seekg(0, std::ios::end);
    line.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(&line[0], line.size());
    input.close();
    *len = (unsigned long long)line.size();
    *content = line;
  } else
  {
    std::cout << "ERROR: File not found "<<filename<<"\n";
    return -1;
  }
  return 0;
}

int validateScheme(char *scheme)
{
  char *listOfSchemes[] = {"rainbow5640", "rainbow6440", "rainbow16242020", 
                          "rainbow256181212", "pflash", "sflashv1", "sflashv2",
                          "hfe", "3icp", "tts6440", "uov"};
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
  printf("sflashv1\n");
  printf("sflashv2\n");
  printf("uov\n");
  printf("3icp\n");
  printf("tts6440\n\n");
  // printf("rainbow5640\t\tRainbow over GF(31) with 56 variables and 40 polynomials.\n");
  // printf("rainbow6440\t\tRainbow over GF(31) with 64 variables and 40 polynomials.\n");
  // printf("rainbow16242020\t\tRainbow over GF(2^4) with 64 variables and 40 polynomials.\n");
  // printf("rainbow256181212\t\tRainbow over GF(2^8) with 42 variables and 24 polynomials.\n");
  // printf("pflash\t\t\tSflash variant over GF(2^4) with 37 variables and 26 polynomials.\n");
  // printf("sflashv1\t\t\tMatsumoto-Imai minus variant over GF(2) with 37 variables and 26 polynomials.\n");
  // printf("sflashv2\t\t\tMatsumoto-Imai minus variant over GF(2^7) with 37 variables and 26 polynomials.\n");
  // printf("uov\t\t\tUOV over GF(2^4) with 120 variables and 40 polynomials.\n");
  // printf("3icp\n");
  // printf("tts6440\n\n");
}

int generateKeypairExec(char **line, uint8_t argc)
{
  char *scheme = "";
  char *output = "";
  char *symencryption = "";
  std::string pass_phrase = "";
  bool ascii = true;
  bool zip = false;

  char Keypairfilename[20];
  std::string asn1bin;

  unsigned char out256[32];
  unsigned char out128[32];

  // secure_string content = "";
  // secure_string cipher_text = "";
  // secure_string plain_text = "";
  unsigned long long len;

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
    } //else if(strcmp(line[i], "-a") == 0)
      //ascii = true;
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
    else if(strcmp(line[i], "-aes_256_ecb") == 0)
      symencryption = "aes_256_ecb";
    else if(strcmp(line[i], "-aes_192_ecb") == 0)
      symencryption = "aes_192_ecb";
    else if(strcmp(line[i], "-aes_128_ecb") == 0)
      symencryption = "aes_128_ecb";
    else
    {
      printf("ERROR: Invalid option: %s\n",line[i]);
      generateKeypairHelp();
      return 1;
    }
  }

  if(strcmp(scheme, "") == 0 || strcmp(output, "") == 0)
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

  if((strcmp(scheme, "sflashv1") == 0 || strcmp(scheme, "sflashv2") == 0 || strcmp(scheme, "uov") == 0) && !SageMath)
  {
    printf("MQCrypto: sage: command not found\n");
    return 1;
  }

  if(pass_phrase == "")
  {
      SetStdinEcho(false);
      std::cout<<"Please enter a pass phrase: ";
      std::cin >> pass_phrase;
      std::cout<<"\n";
      SetStdinEcho(true);
  }

  generateKeypair(scheme, Keypairfilename);

  if(strcmp(Keypairfilename, "") == 0)
  {
    generateKeypairHelp();
    return 1;
  }

  readBinaryFile(Keypairfilename, &asn1bin, &len);
  
  crypto_hash_sha256(out256, (const unsigned char *)pass_phrase.c_str(), strlen(pass_phrase.c_str()));
  crypto_hash_sha256(out128, out256, 32);

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  int ciphertext_len;
  unsigned char cipher_text[len + 100];
  unsigned char plain_text[len + 100];
  if(strcmp(symencryption, "aes_128_cbc") == 0)
  {
    /* Encrypt the plaintext */
    ciphertext_len = c_aes_128_cbc_encrypt((unsigned char *)asn1bin.c_str(), len, &out256[16], &out128[16], cipher_text);
  }
  else if(strcmp(symencryption, "aes_128_ecb") == 0)
  {
    /* Encrypt the plaintext */
    ciphertext_len = c_aes_128_ecb_encrypt((unsigned char *)asn1bin.c_str(), len, &out256[16], &out128[16], cipher_text);
  }
  else if(strcmp(symencryption, "aes_192_cbc") == 0)
  {
    ciphertext_len = c_aes_192_cbc_encrypt((unsigned char *)asn1bin.c_str(), len, &out256[8], &out128[16], cipher_text);
  }
  else if(strcmp(symencryption, "aes_192_ecb") == 0)
  {
    /* Encrypt the plaintext */
    ciphertext_len = c_aes_192_ecb_encrypt((unsigned char *)asn1bin.c_str(), len, &out256[8], &out128[16], cipher_text);
  }
  else if(strcmp(symencryption, "aes_256_ecb") == 0)
  {
    /* Encrypt the plaintext */
    ciphertext_len = c_aes_256_ecb_encrypt((unsigned char *)asn1bin.c_str(), len, out256, &out128[16], cipher_text);
  }
  else
  {
    ciphertext_len = c_aes_256_cbc_encrypt((unsigned char *)asn1bin.c_str(), len, out256, &out128[16], cipher_text);
  }

  char encryptedKey[] = "/var/tmp/enc_XXXXXX";
  mkstemp(encryptedKey);

  writeEncryptedFile(cipher_text, encryptedKey, ciphertext_len);
  
  char b64Key[] = "/var/tmp/b64_XXXXXX";
  mkstemp(b64Key);
  base64encodeKeyPair(encryptedKey, b64Key);
  std::string b64content = "";
  readFile(b64Key, &b64content, &len);
  
  FILE *b64EncFile = fopen(b64Key, "w");

  fprintf(b64EncFile, "-----BEGIN MPKC KEYPAIR-----\n");
  
  if(strcmp(symencryption, "aes_128_cbc") == 0)
    fprintf(b64EncFile, "ENC:AES-128-CBC;\n\n");
  else if(strcmp(symencryption, "aes_128_ecb") == 0)
    fprintf(b64EncFile, "ENC:AES-128-ECB;\n\n");
  else if(strcmp(symencryption, "aes_192_cbc") == 0)
    fprintf(b64EncFile, "ENC:AES-192-CBC;\n\n");
  else if(strcmp(symencryption, "aes_192_ecb") == 0)
    fprintf(b64EncFile, "ENC:AES-192-ECB;\n\n");
  else if(strcmp(symencryption, "aes_256_ecb") == 0)
    fprintf(b64EncFile, "ENC:AES-256-ECB;\n\n");
  else
    fprintf(b64EncFile, "ENC:AES-256-CBC;\n\n");


  int l,start = 0;
  l = strlen("pc/sqfu3kOpjNeChthxPgkMsaEfbkudVpNv28J0sAOJhoqCl/duqLx8hk5866pcC+ceXrESl");
  while(start < len)
  {
    fprintf(b64EncFile, (char *)(b64content.substr(start, l) + "\n").c_str());
    start += l;
  }

  fprintf(b64EncFile, "-----END MPKC KEYPAIR-----");

  fclose(b64EncFile);

  if(zip)
  {
    bz2_compress(b64Key, output);
    remove(b64Key);
    std::cout<<"Keypair saved in file "<<output<<".bzip2\n";
  }
  else {
    std::string move = "mv " + (std::string)b64Key + " " + (std::string)output;
    // rename(b64Key, output);
    system((char *)move.c_str());
    std::cout<<"Keypair saved in file "<<output<<"\n";
  }

  remove(Keypairfilename);
  remove(encryptedKey);

  return 1;
}

int keysExec(char **command, uint8_t argc)
{
  std::string inputKeypair = "";
  bool zip = false;
  bool decrypt = false;

  char *outputPublic = "";
  char *outputPrivate = "";

  char line[256];
  std::string content = "";
  unsigned long long len;
  std::string cipher_text = "";

  std::string b64content = "";
  std::string move = "";

  unsigned char out256[32];
  unsigned char out128[32];

  std::string pass_phrase = "";

  inputKeypair = command[0];

  for(uint8_t i = 1; i < argc; i++)
  {
    if(strcmp(command[i], "-pubout") == 0)
    {
      outputPublic = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-privout") == 0)
    {
      outputPrivate = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-passin") == 0)
    {
      pass_phrase = &command[i+1][5];
      i+=1;
    } else if(strcmp(command[i], "-zip") == 0)
      zip = true;
    else if(strcmp(command[i], "-decrypt") == 0)
      decrypt = true;
    else
    {
      printf("ERROR: Invalid option: %s\n",command[i]);
      keysHelp();
      return 1;
    }
  }

  /*
    CHECK FILE EXTENSION
      - If zipped file is used, decompress it to read the content.
  */
  if(inputKeypair.substr(inputKeypair.length()-4, 4) == ".bz2")
  {
    std::string bz2tmp = "/var/tmp/bz2_XXXXXX";
    mkstemp((char *)bz2tmp.c_str());
    bz2_decompress(inputKeypair, bz2tmp);
    inputKeypair = bz2tmp;
  }

  /*
    CHECK WHETHER IT IS A KEYPAIR FILE
      - Checks if the input key belongs to a keypair.
      - The Encryption algorithm is saved to the line variable.
  */
  FILE *fp = fopen((char *)inputKeypair.c_str(), "rb");

  if(!fp)
  {
    std::cout<<"ERROR: File not found "<<inputKeypair<<"\n";
    return 1;
  }

  fgets(line, sizeof(line), fp);
  if(strcmp(line, "-----BEGIN MPKC KEYPAIR-----\n") != 0)
  {
    std::cout<<"ERROR: Input file \""<<inputKeypair<<"\" does not contain a keypair.\n\n";
    fclose(fp);
    return 1;
  }

  fgets(line, sizeof(line), fp);

  fclose(fp);
  
  /*
    REQUEST PASS PHRASE
      - Pass phrase is required for decryption of the keypair file.
  */
  if(pass_phrase == "")
  {
      SetStdinEcho(false);
      std::cout<<"Please enter your pass phrase: ";
      std::cin >> pass_phrase;
      std::cout<<"\n";
      SetStdinEcho(true);
  }


  if(readFile((char *)inputKeypair.c_str(), &content, &len) != 0)
    return 1;
  std::string tmpinput = "/var/tmp/input_XXXXXX";
  mkstemp((char *)tmpinput.c_str());
  writeFile(content.substr(43, content.length() - 69), (char *)tmpinput.c_str());
  // // std::cout<<content.substr(43, content.length() - 69)<<"\n";

  /*
    BASE64 DECODING
      - The input key file is base64 decoded.
  */
  std::string b64decoded = "/var/tmp/enc_XXXXXX";
  mkstemp((char *)b64decoded.c_str());
  base64decodeKeyPair(tmpinput, b64decoded);
  // std::cout<<"B64 DECODED: "<<b64decoded<<"\n";

  /*
    READ THE CIPHER TEXT OF THE DECODED INPUT FILE
  */
  readBinaryFile((char *)b64decoded.c_str(), &cipher_text, &len);

  crypto_hash_sha256(out256, (const unsigned char *)pass_phrase.c_str(), strlen(pass_phrase.c_str()));
  crypto_hash_sha256(out128, out256, 32);

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  int decryptedtext_len;
  unsigned char plain_text[len + 100];
  // std::cout<<"CIPHER TEXT LENGTH: "<<len<<"\n";
  if(strcmp(&line[4], "AES-256-CBC;\n") == 0)
  {
    decryptedtext_len = c_aes_256_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
  } else if(strcmp(&line[4], "AES-192-CBC;\n") == 0)
  {
    decryptedtext_len = c_aes_192_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
  } else if(strcmp(&line[4], "AES-128-CBC;\n") == 0)
  {
    decryptedtext_len = c_aes_128_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
  } else if(strcmp(&line[4], "AES-256-ECB;\n") == 0)
  {
    decryptedtext_len = c_aes_256_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
  } else if(strcmp(&line[4], "AES-192-ECB;\n") == 0)
  {
    decryptedtext_len = c_aes_192_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
  } else if(strcmp(&line[4], "AES-128-ECB;\n") == 0)
  {
    decryptedtext_len = c_aes_128_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
  }


  // std::cout<<"DECRYPTED LENGTH: "<<decryptedtext_len<<"\n";


  char asn1tmp[] = "/var/tmp/asn1_XXXXXX";
  mkstemp(asn1tmp);
  writeEncryptedFile(plain_text, asn1tmp, decryptedtext_len);

  if(strcmp(outputPublic, "") != 0)
  {
    char pubtmp[] = "/var/tmp/pub_XXXXXX";
    mkstemp(pubtmp);
    char pubb64tmp[] = "/var/tmp/pubb64_XXXXXX";
    mkstemp(pubb64tmp);
    extractPublicKey(plain_text, pubtmp, decryptedtext_len);
    base64encodeKeyPair(pubtmp, pubb64tmp);
    b64content = "";
    readFile(pubb64tmp, &b64content, &len);
    keyHeader(outputPublic, "-----BEGIN MPKC PUBLIC KEY-----\n", "-----END MPKC PUBLIC KEY-----", b64content, len);
    remove(pubtmp);
    remove(pubb64tmp);
    if(zip)
    {
      move = "mv " + (std::string)pubtmp + " " + (std::string)outputPublic;
      // rename(outputPublic, pubtmp);
      system((char *)move.c_str());
      bz2_compress(pubtmp, outputPublic);
      remove(pubtmp);
      std::cout<<"Public key saved in "<<outputPublic<<".bzip2\n";
    } else
      std::cout<<"Public key saved in "<<outputPublic<<"\n";
  }

  if(strcmp(outputPrivate, "") != 0)
  {
    char privtmp[] = "/var/tmp/priv_XXXXXX";
    mkstemp(privtmp);
    char privb64tmp[] = "/var/tmp/privb64_XXXXXX";
    mkstemp(privb64tmp);
    extractPrivateKey(plain_text, privtmp, decryptedtext_len);
    
    int ciphertext_len;
    std::string decrypted_text;
    if(!decrypt)
    {
      readBinaryFile(privtmp, &decrypted_text, &len);
      if(strcmp(&line[4], "AES-256-CBC;\n") == 0)
      {
        ciphertext_len = c_aes_256_cbc_encrypt((unsigned char *)decrypted_text.c_str(), len, out256, &out128[16], (unsigned char *)cipher_text.c_str());
      } else if(strcmp(&line[4], "AES-192-CBC;\n") == 0)
      {
        ciphertext_len = c_aes_192_cbc_encrypt((unsigned char *)decrypted_text.c_str(), len, &out256[8], &out128[16], (unsigned char *)cipher_text.c_str());
      } else if(strcmp(&line[4], "AES-128-CBC;\n") == 0)
      {
        ciphertext_len = c_aes_128_cbc_encrypt((unsigned char *)decrypted_text.c_str(), len, &out256[16], &out128[16], (unsigned char *)cipher_text.c_str());
      } else if(strcmp(&line[4], "AES-256-ECB;\n") == 0)
      {
        ciphertext_len = c_aes_256_ecb_encrypt((unsigned char *)decrypted_text.c_str(), len, out256, &out128[16], (unsigned char *)cipher_text.c_str());
      } else if(strcmp(&line[4], "AES-192-ECB;\n") == 0)
      {
        std::cout<<"DEC\n";
        ciphertext_len = c_aes_192_ecb_encrypt((unsigned char *)decrypted_text.c_str(), len, &out256[8], &out128[16], (unsigned char *)cipher_text.c_str());
        std::cout<<"DEC\n";
      } else if(strcmp(&line[4], "AES-128-ECB;\n") == 0)
      {
        ciphertext_len = c_aes_128_ecb_encrypt((unsigned char *)decrypted_text.c_str(), len, &out256[16], &out128[16], (unsigned char *)cipher_text.c_str());
      }
      writeEncryptedFile((unsigned char *)cipher_text.c_str(), privtmp, ciphertext_len);
    }
    
    base64encodeKeyPair(privtmp, privb64tmp);
    b64content = "";
    readFile(privb64tmp, &b64content, &len);
    if(decrypt)
      keyHeader(outputPrivate, "-----BEGIN MPKC PRIVATE KEY-----\n", "-----END MPKC PRIVATE KEY-----", b64content, len);
    else
    {
      std::string header = "-----BEGIN MPKC PRIVATE KEY-----\n" + (std::string)&line[0] + "\n";
      keyHeader(outputPrivate, (char *)header.c_str(), "-----END MPKC PRIVATE KEY-----", b64content, len);
    }
    remove(privtmp);
    remove(privb64tmp);
    if(zip)
    {
      move = "mv " + (std::string)privtmp + " " + (std::string)outputPrivate;
      // rename(outputPrivate, privtmp);
      system((char *)move.c_str());
      bz2_compress(privtmp, outputPrivate);
      remove(privtmp);
      std::cout<<"Private key saved in "<<outputPrivate<<".bzip2\n";
    } else
      std::cout<<"Private key saved in "<<outputPrivate<<"\n";
  }

  remove(asn1tmp);
  remove((char *)b64decoded.c_str());
  remove((char *)tmpinput.c_str());

  return 1;
}

int signExec(char **command, uint8_t argc)
{
  char *dgst = "";
  char *inputFile = "";
  char *signedFile = "";
  std::string pass_phrase = "";
  bool zip = false;
  bool encrypted = true;

  std::string inputKeypair = command[0];
  std::string move = "";
  char line[256];
  char *keytype;
  
  char encryption[15];

  unsigned char out256[32];
  unsigned char out128[32];

  std::string content = "";
  unsigned long long len;

  for(uint8_t i = 1; i < argc; i++)
  {
    if(strcmp(command[i], "-in") == 0)
    {
      inputFile = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-out") == 0)
    {
      signedFile = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-passin") == 0)
    {
      pass_phrase = &command[i+1][5];
      i+=1;
    } else if(strcmp(command[i], "-sha256") == 0)
      dgst = "sha256";
    else if(strcmp(command[i], "-sha512") == 0)
      dgst = "sha512";
    else if(strcmp(command[i], "-zip") == 0)
      zip = true;
    else
    {
      printf("ERROR: Invalid option: %s\n",command[i]);
      signHelp();
      return 1;
    }
  }

  if(strcmp(dgst, "") == 0)
  {
    printf("The following are valid hash functions that can be used by MQCrypto\n-sha256\n-sha512\n");
    return -1;
  }

  if( access( inputFile, F_OK ) == -1 ) {
    std::cout<<"ERROR: File not found "<<inputFile<<"\n";
    return 1;
  }

  /*
    CHECK FILE EXTENSION
      - If zipped file is used, decompress it to read the content.
  */
  if(inputKeypair.substr(inputKeypair.length()-4, 4) == ".bz2")
  {
    std::string bz2tmp = "/var/tmp/bz2_XXXXXX";
    mkstemp((char *)bz2tmp.c_str());
    bz2_decompress(inputKeypair, bz2tmp);
    inputKeypair = bz2tmp;
  }

  if(readFile((char *)inputKeypair.c_str(), &content, &len) != 0)
    return 1;
  std::string tmpinput = "/var/tmp/input_XXXXXX";
  mkstemp((char *)tmpinput.c_str());

  /*
    CHECK WHETHER IT IS A KEYPAIR FILE
      - Checks if the input key belongs to a keypair.
      - The Encryption algorithm is saved to the line variable.
  */
  FILE *fp = fopen((char *)inputKeypair.c_str(), "rb");

  if(!fp)
  {
    std::cout<<"ERROR: File not found "<<inputKeypair<<"\n";
    return 1;
  }

  fgets(line, sizeof(line), fp);
  if(strcmp(line, "-----BEGIN MPKC KEYPAIR-----\n") == 0)
  {
    keytype = "keypair";
    if(pass_phrase == "")
    {
        SetStdinEcho(false);
        std::cout<<"Please enter your pass phrase: ";
        std::cin >> pass_phrase;
        std::cout<<"\n";
        SetStdinEcho(true);
    }

    fgets(line, sizeof(line), fp);
    strcpy(encryption, &line[4]);

    writeFile(content.substr(43, content.length() - 69), (char *)tmpinput.c_str());
  } else if(strcmp(line, "-----BEGIN MPKC PRIVATE KEY-----\n") == 0)
  {
    keytype = "private";
    fgets(line, sizeof(line), fp);
    if(((std::string)line).length() < 20)
    {
      strcpy(encryption, &line[4]);
      if(pass_phrase == "")
      {
          SetStdinEcho(false);
          std::cout<<"Please enter your pass phrase: ";
          std::cin >> pass_phrase;
          std::cout<<"\n";
          SetStdinEcho(true);
      }
      writeFile(content.substr(32 + ((std::string)line).length() - 1, content.length() - 77), (char *)tmpinput.c_str());
    } else
    {
      encrypted = false;
      writeFile(content.substr(32, content.length() - 62), (char *)tmpinput.c_str());
    }
  }
  else
  {
    std::cout<<"ERROR: Bad key file, a private key is required.\n";
    return 1;
  }

  fclose(fp);

  /*
    BASE64 DECODING
      - The input key file is base64 decoded.
  */
  std::string b64decoded = "/var/tmp/enc_XXXXXX";
  mkstemp((char *)b64decoded.c_str());
  base64decodeKeyPair(tmpinput, b64decoded);

  char signtmp[] = "/var/tmp/sign_XXXXXX";
  mkstemp(signtmp);

  if(encrypted)
  {
    std::string cipher_text;
    /*
      READ THE CIPHER TEXT OF THE DECODED INPUT FILE
    */
    readBinaryFile((char *)b64decoded.c_str(), &cipher_text, &len);
    
    crypto_hash_sha256(out256, (const unsigned char *)pass_phrase.c_str(), strlen(pass_phrase.c_str()));
    crypto_hash_sha256(out128, out256, 32);

    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
  
    int decryptedtext_len;
    unsigned char plain_text[len + 100];
    // std::cout<<"CIPHER TEXT LENGTH: "<<len<<"\n";
    if(strcmp(&line[4], "AES-256-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_256_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-192-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_192_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-128-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_128_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-256-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_256_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-192-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_192_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-128-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_128_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
    }
    
    MPKCPrivateKey_t *Private;
    if(strcmp(keytype, "keypair") == 0)
      Private = getPrivateKey(plain_text, decryptedtext_len);
    else
      Private = getPrivateKey_t(plain_text, decryptedtext_len);

    if(sign(inputFile, Private, dgst, signtmp) != 0)
    {
      remove(signtmp);
      remove((char *)b64decoded.c_str());
      remove((char *)tmpinput.c_str());
      return 1;
    }
  } else {
    if(sign(inputFile, (char *)b64decoded.c_str(), dgst, signtmp) != 0)
    {
      remove(signtmp);
      remove((char *)b64decoded.c_str());
      remove((char *)tmpinput.c_str());
      return 1;
    }  
  }

  content = "";
  readFile(signtmp, &content, &len);

  keyHeader(signedFile, "-----BEGIN MPKC SIGNATURE-----\n", "-----END MPKC SIGNATURE-----", content, len);

  if(zip)
  {
    char asn1tmp[] = "/var/tmp/asn1_XXXXXX";
    mkstemp(asn1tmp);
    move = "mv " + (std::string)signedFile + " " + (std::string)asn1tmp;
    // rename(signedFile, asn1tmp);
    system((char *)move.c_str());
    bz2_compress(asn1tmp, signedFile);
    remove(asn1tmp);
    std::cout<<"Signature saved in: "<<signedFile<<".bz2\n";
  } else
    std::cout<<"Signature saved in: "<<signedFile<<"\n";

  std::cout<<"\n";

  remove(signtmp);
  remove((char *)b64decoded.c_str());
  remove((char *)tmpinput.c_str());

  return 1;
}

int verifyExec(char **command, uint8_t argc)
{
  char *inputFile = "";
  std::string signedFile = "";
  std::string pass_phrase = "";

  std::string inputKeypair = command[0];
  char line[256];
  char *keytype;

  unsigned char out256[32];
  unsigned char out128[32];

  std::string content = "";
  unsigned long long len;

  for(uint8_t i = 1; i < argc; i++)
  {
    if(strcmp(command[i], "-in") == 0)
    {
      inputFile = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-signature") == 0)
    {
      signedFile = command[i+1];
      i+=1;
    } else if(strcmp(command[i], "-passin") == 0)
    {
      pass_phrase = &command[i+1][5];
      i+=1;
    } else
    {
      printf("ERROR: Invalid option: %s\n",command[i]);
      verifyHelp();
      return 1;
    }
  }

  if( access( inputFile, F_OK ) == -1 ) {
    std::cout<<"ERROR: File not found "<<inputFile<<"\n";
    return 1;
  }

  if( access( (char *)signedFile.c_str(), F_OK ) == -1 ) {
    std::cout<<"ERROR: File not found "<<signedFile<<"\n";
    return 1;
  }

  /*
    CHECK FILE EXTENSION
      - If zipped file is used, decompress it to read the content.
  */
  if(inputKeypair.substr(inputKeypair.length()-4, 4) == ".bz2")
  {
    std::string bz2tmp = "/var/tmp/bz2_XXXXXX";
    mkstemp((char *)bz2tmp.c_str());
    bz2_decompress(inputKeypair, bz2tmp);
    inputKeypair = bz2tmp;
  }

  if(readFile((char *)inputKeypair.c_str(), &content, &len) != 0)
    return 1;
  std::string tmpinput = "/var/tmp/input_XXXXXX";
  mkstemp((char *)tmpinput.c_str());

  /*
    CHECK WHETHER IT IS A KEYPAIR FILE
      - Checks if the input key belongs to a keypair.
      - The Encryption algorithm is saved to the line variable.
  */
  FILE *fp = fopen((char *)inputKeypair.c_str(), "rb");

  if(!fp)
  {
    std::cout<<"ERROR: File not found "<<inputKeypair<<"\n";
    return 1;
  }

  fgets(line, sizeof(line), fp);
  if(strcmp(line, "-----BEGIN MPKC KEYPAIR-----\n") == 0)
  {
    if(pass_phrase == "")
    {
        SetStdinEcho(false);
        std::cout<<"Please enter your pass phrase: ";
        std::cin >> pass_phrase;
        std::cout<<"\n";
        SetStdinEcho(true);
    }

    fgets(line, sizeof(line), fp);

    keytype = "keypair";

    writeFile(content.substr(43, content.length() - 69), (char *)tmpinput.c_str());
  } else if(strcmp(line, "-----BEGIN MPKC PUBLIC KEY-----\n") == 0)
  {
    keytype = "public";
    writeFile(content.substr(31, content.length() - 60), (char *)tmpinput.c_str());
  }
  else
  {
    std::cout<<"ERROR: Bad key file, a private key is required.\n";
    return 1;
  }

  fclose(fp);

  /* 
    CHECK SIGNATURE FORMAT

  */
  if(signedFile.substr(signedFile.length()-4, 4) == ".bz2")
  {
    std::string bz2tmp = "/var/tmp/bz2_XXXXXX";
    mkstemp((char *)bz2tmp.c_str());
    bz2_decompress(signedFile, bz2tmp);
    signedFile = bz2tmp;
  }

  content = "";
  readFile((char *)signedFile.c_str(), &content, &len);
  std::string signtmpinput = "/var/tmp/signinput_XXXXXX";
  mkstemp((char *)signtmpinput.c_str());

  /*
    CHECK WHETHER IT IS A SIGNATURE FILE
      - Checks if the input key belongs to a keypair.
      - The Encryption algorithm is saved to the line variable.
  */
  fp = fopen((char *)signedFile.c_str(), "rb");

  if(!fp)
  {
    std::cout<<"ERROR: File not found "<<signedFile<<"\n";
    return 1;
  }

  char aux[256];
  fgets(aux, sizeof(aux), fp);
  if(strcmp(aux, "-----BEGIN MPKC SIGNATURE-----\n") == 0)
  {
    writeFile(content.substr(30, content.length() - 58), (char *)signtmpinput.c_str());
  } else
  {
    std::cout<<"ERROR: Bad signature file format.\n";
    return 1;
  }

  fclose(fp);

  /*
    BASE64 DECODING
      - The input key file is base64 decoded.
  */
  std::string b64decoded = "/var/tmp/enc_XXXXXX";
  mkstemp((char *)b64decoded.c_str());
  base64decodeKeyPair(tmpinput, b64decoded);

  char signtmp[] = "/var/tmp/sign_XXXXXX";
  mkstemp(signtmp);

  if(strcmp(keytype, "keypair") == 0)
  {
    std::string cipher_text;
    /*
      READ THE CIPHER TEXT OF THE DECODED INPUT FILE
    */
    readBinaryFile((char *)b64decoded.c_str(), &cipher_text, &len);

    crypto_hash_sha256(out256, (const unsigned char *)pass_phrase.c_str(), strlen(pass_phrase.c_str()));
    crypto_hash_sha256(out128, out256, 32);

    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
  
    int decryptedtext_len;
    unsigned char plain_text[len + 100];
    // std::cout<<"CIPHER TEXT LENGTH: "<<len<<"\n";
    if(strcmp(&line[4], "AES-256-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_256_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-192-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_192_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-128-CBC;\n") == 0)
    {
      decryptedtext_len = c_aes_128_cbc_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-256-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_256_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, out256, &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-192-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_192_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[8], &out128[16], plain_text);
    } else if(strcmp(&line[4], "AES-128-ECB;\n") == 0)
    {
      decryptedtext_len = c_aes_128_ecb_decrypt((unsigned char *)cipher_text.c_str(), len, &out256[16], &out128[16], plain_text);
    }

    MPKCPublicKey_t *Public = getPublicKey(plain_text, decryptedtext_len);

    verify(inputFile, (char *)signtmpinput.c_str(), Public);
  } else {
    readBinaryFile((char *)b64decoded.c_str(), &content, &len);
    MPKCPublicKey_t *Public = constructPublicKey((unsigned char *)content.c_str(), len);
    verify(inputFile, (char *)signtmpinput.c_str(), Public);
  }

  std::cout<<"\n";
  remove((char *)signtmpinput.c_str());
  remove((char *)b64decoded.c_str());
  remove((char *)tmpinput.c_str());
  remove(signtmp);

  return 1;
}

void keysHelp(void)
{
  printf("keys <keypair.pem> [Options]\n\n");
  printf("Options:\n");
  printf("-pubout <public-key.pem>\tOutput file to save the public key.\n");
  printf("-privout <private-key.pem>\tOutput file to save the private key.\n");
  printf("-passin pass:<phrase>\t\tPass phrase for decryption of the input keypair.\n");
  printf("-decrypt\t\t\tThe private key will be saved in a decrypted file.\n");
  printf("-zip\t\t\t\tThe output file will be compressed using bzip2.\n\n");
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
  printf("\t\t\t\tsflashv1\n");
  printf("\t\t\t\tsflashv2\n");
  printf("\t\t\t\tuov\n");
  printf("\t\t\t\t3icp\n");
  printf("\t\t\t\ttts6440\n");
	printf("-out <private-key.pem>\t\tFile to save the generated set of keys, ");
	printf("the keys will be generate in the files output_PK.bin and output_SK.bin, ");
	printf("if left in blank the keys will output in the files scheme_PK.bin and scheme_SK.bin\n");
  printf("-symencryption\t\t\tSymmetric Encryption to use for the encryption of the private key.\n");
  printf("\t\t\t\tSymmetric Cryptography Algorithms that can be used:\n");
  printf("\t\t\t\t-aes_128_cbc\t-aes_192_cbc\t-aes_256_cbc\n");
  printf("\t\t\t\t-aes_128_ecb\t-aes_192_ecb\t-aes_256_ecb\n");
  printf("-passout pass:<phrase>\t\tPass phrase used to encrypt the private key.\n");
  printf("-zip\t\t\t\tCompress the keypair generated, otherwise the output will be a Base64 encoded file.\n\n");
  // printf("-zip\t\t\tUse a compressor on the output key (using bzip2).\n\n");
}

void signHelp(void)
{
	printf("sign <private-key.pem> [Options]\n");
  printf("Options:\n");
	printf("-in <file>\t\tInput filename to be signed.\n");
	printf("-out <signature>\tOutput file in which to save the signature.\n");
  printf("-passin pass:<phrase>\tPass phrase for decryption of the input keypair.\n");
  printf("-dgst\t\t\tInput filename to be signed.\n");
	printf("\t\t\tThe following digest algorithms can be used:\n");
	printf("\t\t\t-sha256\n");
	printf("\t\t\t-sha512\n");
  printf("-zip\t\t\tCompress the signature using bzip2.\n\n");
}

void verifyHelp(void)
{
	printf("verify <public-key.pem> [Options]\n");
  printf("Options:\n");
	printf("-in\t\t\tInput filename of the document that was signed.\n");
	printf("-signature\t\tSignature of the input document that is going to be verified.\n");
	printf("-passin pass:<phrase>\tPass phrase for decryption of the input keypair.\n\n");
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
  printf("-list_schemes\t\tDisplay a list with the schemes that can be used by MQCrypto\n");
	printf("-genKeys\t\tGenerate a public and private pair of keys of a choosen scheme\n");
  printf("-keys\t\t\tExtracts the public/private key from the keypair file\n");
	printf("-sign\t\t\tSign a document using a previous generated key\n");
	printf("-verify\t\t\tVerify a signature in a document\n");
	// printf("-encrypt\t\tEncrypt a document using a previous generated key\n");
	// printf("-decrypt\t\tDecrypt an encrypted document\n");
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
      signExec(&args[1], argc - 1);
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
  		verifyExec(&args[1], argc - 1);
  		return 1;
  	} else
  	{
  		printf("Incorrect use...\n");
  		verifyHelp();
  		return 1;
  	}
  }
  else if(strcmp(args[0], "keys") == 0)
  {
    if(argc > 1)
    {
      if(strcmp(args[1], "help") == 0)
      {
        keysHelp();
        return 1;
      }
      keysExec(&args[1], argc - 1);
      return 1;
    } else
    {
      printf("Incorrect use...\n");
      keysHelp();
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
  else if(strcmp(args[0], "list_schemes") == 0)
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

int main(int argc, char **argv)
{
  redi::ipstream proc("sage --version", redi::pstreams::pstdout | redi::pstreams::pstderr);

  std::string str;
  std::getline(proc.err(), str);

  if(str == "")
    SageMath = true;

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