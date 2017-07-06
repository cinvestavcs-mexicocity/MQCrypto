#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib_flash.c"
#include "sizes.h"

int SflashV2_signature( 
		  unsigned char sm[],
		  unsigned long long *smlen,
		  const unsigned char m[],
		  const unsigned long long mlen,
		  const unsigned char sk[SECRETKEY_BYTES]

)
{
  unsigned char signature[37];
  
  Flash_Complete_Key CK = (Flash_Complete_Key) malloc(sizeof(Flash_Complete_Key));
  
  *smlen=SIGNATURE_BYTES+mlen;
  
  
  Flash_load2_private_key(sk,CK);
  
  Flash_sign2(signature,m,8*mlen,CK);
  
  memcpy(sm+SIGNATURE_BYTES,m,mlen);
  memcpy(sm,signature,SIGNATURE_BYTES);

  // free(CK);
  return 0;
}


int main(int argc, char **argv)
{
  if(argc < 3)
  {
    printf("Wrong number of parameters\n");
    return 1;
  }
  srand(time(NULL));

  uint8_t c[SIGNATURE_BYTES];
  uint8_t m[SHORTHASH_BYTES];
  uint8_t pk[PUBLICKEY_BYTES];
  uint8_t sk[SECRETKEY_BYTES];

  uint64_t lc,lm = SHORTHASH_BYTES,lpk,lsk;

  SflashV2_keypair( pk , sk );

  // printf("PUBLIC KEY:\n");
  // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

  // printf("PRIVATE KEY:\n");
  // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

  for( int i=0;i<SHORTHASH_BYTES;i++) 
    m[i] = rand() % 256;

  // printf("PUBLIC KEY:");
  // for(int i=0;i<PUBLICKEY_BYTES;i++)
  //   printf("%x ", (unsigned)pk[i]);
  // printf("\n");

  for( int i=0;i<SHORTHASH_BYTES;i++) 
    printf("%x " , (unsigned) m[i]); 
  printf("\n");

  SflashV2_signature( c , &lc , m , lm , sk );
  for( int i=0;i<SIGNATURE_BYTES;i++) 
    printf("%x " , (unsigned) c[i]); 
  printf("\n");


  int i = SflashV2_verification( m , &lm , c , lc , pk );
  printf("verify: %d\n" , i );
}