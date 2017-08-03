#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib_flash.c"
#include "sizes.h"

int SflashV2_keypair(unsigned char *pk,unsigned char *sk)
{
  unsigned char *seed;
  int length;

  //FILE *RAND;
  //RAND = fopen("sizes.h","rb");

  Flash_Complete_Key CK = (Flash_Complete_Key) malloc(sizeof(Flash_Complete_Key));
  
  
  //fseek(RAND,0,SEEK_END);
  //length = ftell(RAND)>55 ? 55 : ftell(RAND);
  //seed = (unsigned char *) malloc(length*sizeof(unsigned char));
  //fseek(RAND,0,SEEK_SET);
  //fread(seed,1,length,RAND);
  //fclose(RAND);
  
  //init_rando(seed, length);
  Flash_build_complete_key(CK);     
  
  Flash_store2_public_key (pk,CK);

  Flash_store2_private_key(sk,CK);

  // free(CK);
  
  return 0;
  
}

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

int SflashV2_verification(unsigned char m[],
      unsigned long long *mlen,
      const unsigned char sm[],
      const unsigned long long smlen,
      const unsigned char pk[PUBLICKEY_BYTES]
)
{
  Flash_Complete_Key CK = (Flash_Complete_Key) malloc(sizeof(Flash_Complete_Key));
  unsigned char signature[SIGNATURE_BYTES];
  if (smlen<SIGNATURE_BYTES)
    {
     // free(CK);
      return -100;
    }
  *mlen=smlen-SIGNATURE_BYTES;
  
  Flash_load2_public_key(pk,CK);
  memcpy(signature,sm,SIGNATURE_BYTES);
  memcpy(m,sm+SIGNATURE_BYTES,*mlen);
  if (Flash_verify2(m,8*(*mlen),signature,CK))
    {
     // free(CK);
      return 0;
    }
  else
    {
     // free(CK);
      return -100;
    }
}


// int main()
// {
//   srand(time(NULL));

//   uint8_t c[SIGNATURE_BYTES];
//   uint8_t m[SHORTHASH_BYTES];
//   uint8_t pk[PUBLICKEY_BYTES];
//   uint8_t sk[SECRETKEY_BYTES];

//   uint64_t lc,lm = SHORTHASH_BYTES,lpk,lsk;

//   SflashV2_keypair( pk , sk );

//   // printf("PUBLIC KEY:\n");
//   // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

//   // printf("PRIVATE KEY:\n");
//   // for( int i=0;i<PUBLICKEY_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

//   for( int i=0;i<SHORTHASH_BYTES;i++) 
//     m[i] = rand() % 256;

//   // printf("PUBLIC KEY:");
//   // for(int i=0;i<PUBLICKEY_BYTES;i++)
//   //   printf("%x ", (unsigned)pk[i]);
//   // printf("\n");

//   for( int i=0;i<SHORTHASH_BYTES;i++) 
//     printf("%x " , (unsigned) m[i]); 
//   printf("\n");

//   SflashV2_signature( c , &lc , m , lm , sk );
//   for( int i=0;i<SIGNATURE_BYTES;i++) 
//     printf("%x " , (unsigned) c[i]); 
//   printf("\n");


//   int i = SflashV2_verification( m , &lm , c , lc , pk );
//   printf("verify: %d\n" , i );
// }