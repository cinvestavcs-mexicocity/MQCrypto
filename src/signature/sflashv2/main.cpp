// #include "sflashv2-test.c"
#include <pstream.h>
#include <string>
#include <iostream>

int main()
{
	// execl("./sflashv2-text");
	// run a process and create a streambuf that reads its stdout and stderr
  redi::ipstream proc("./sflashv2-test", redi::pstreams::pstdout | redi::pstreams::pstderr);
  std::string line;
  // read child's stdout
  while (std::getline(proc.out(), line))
    std::cout << "stdout: " << line << '\n';
  // read child's stderr
  while (std::getline(proc.err(), line))
    std::cout << "stderr: " << line << '\n';
  // srand(time(NULL));

  // uint8_t c[SIGNATURE_BYTES];
  // uint8_t m[SHORTHASH_BYTES];
  // uint8_t pk[PUBLICKEY_BYTES];
  // uint8_t sk[SECRETKEY_BYTES];

  // uint64_t lc,lm = SHORTHASH_BYTES,lpk,lsk;

  // SflashV2_keypair( pk , sk );

  // // printf("PUBLIC KEY:\n");
  // // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

  // // printf("PRIVATE KEY:\n");
  // // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

  // for( int i=0;i<SHORTHASH_BYTES;i++) 
  //   m[i] = rand() % 256;

  // // printf("PUBLIC KEY:");
  // // for(int i=0;i<PUBLICKEY_BYTES;i++)
  // //   printf("%x ", (unsigned)pk[i]);
  // // printf("\n");

  // for( int i=0;i<SHORTHASH_BYTES;i++) 
  //   printf("%x " , (unsigned) m[i]); 
  // printf("\n");
  
  // SflashV2_signature( c , &lc , m , lm , sk );
  // for( int i=0;i<SIGNATURE_BYTES;i++) 
  //   printf("%x " , (unsigned) c[i]); 
  // printf("\n");


  // int i = SflashV2_verification( m , &lm , c , lc , pk );
  // printf("verify: %d\n" , i );
}