#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <MPKCPublicKey.h>
#include <MPKCPrivateKey.h>

#include "lib_flash.c"
#include "sizes.h"

void ASN1encodeKeyPair(char cryptosystem[], 
  const char *pk, const unsigned long long pklen,
  const char *sk, const unsigned long long sklen,
  char output[]);

/* Write the encoded output into some FILE stream. */
static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = (FILE *)app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}

void ASN1encodeKeyPair(char cryptosystem[], 
  const char *pk, const unsigned long long pklen,
  const char *sk, const unsigned long long sklen,
  char output[])
{
    MPKCPublicKey_t *PK; /* MPKC Public Key Structure */
    MPKCPrivateKey_t *SK; /* MPKC Private Key Structure */
    asn_enc_rval_t ec;      /* Encoder return value  */

    PK = (MPKCPublicKey_t *)calloc(1, sizeof(MPKCPublicKey_t));
    if(!PK) {
      perror("Public Key calloc() failed");
      exit(1);
    }

    SK = (MPKCPrivateKey_t *)calloc(1, sizeof(MPKCPrivateKey_t));
    if(!SK) {
      perror("Private Key calloc() failed");
      exit(1);
    }

    if(OCTET_STRING_fromBuf(&PK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
      perror("Error Buffer");
    }

    if(OCTET_STRING_fromBuf(&PK->key, pk, pklen) < 0) {
      perror("Error Buffer");
    }

    if(OCTET_STRING_fromBuf(&SK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
      perror("Error Buffer");
    }

    if(OCTET_STRING_fromBuf(&SK->key, sk, sklen) < 0) {
      perror("Error Buffer");
    }

    char *PKfilename;
    char *SKfilename;
    PKfilename = tmpnam(PKfilename);
    
    FILE *fp = fopen(PKfilename, "wb");
    
    /* for Public Key BER output */
    if(!fp) {
      perror(PKfilename);
      exit(1);
    }

    ec = der_encode(&asn_DEF_MPKCPublicKey, PK, write_out, fp);

    if(ec.encoded == -1) {
        fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
        ec.failed_type ? ec.failed_type->name : "unknown");
        // return -1;
    }

    fclose(fp);

    printf("%s\n",PKfilename);

    SKfilename = tmpnam(SKfilename);

    fp = fopen(SKfilename, "wb");

    /* for Private Key BER output */
    if(!fp) {
      perror(SKfilename);
      exit(1);
    }

    ec = der_encode(&asn_DEF_MPKCPrivateKey, SK, write_out, fp);
    
    if(ec.encoded == -1) {
        fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
        ec.failed_type ? ec.failed_type->name : "unknown");
        // return -1;
    }

    fclose(fp);

    printf("%s\n",SKfilename);
}

int SflashV2_keypair(unsigned char *pk,unsigned char *sk)
{
  unsigned char *seed;
  int length;

  Flash_Complete_Key CK = (Flash_Complete_Key) malloc(sizeof(Flash_Complete_Key));
  
  Flash_build_complete_key(CK);     
  
  Flash_store2_public_key (pk,CK);

  Flash_store2_private_key(sk,CK);

  free(CK);
  
  return 0;
}


int main(int argc, char **argv)
{
  srand(time(NULL));

  uint8_t pk[PUBLICKEY_BYTES];
  uint8_t sk[SECRETKEY_BYTES];
  uint64_t pklen = PUBLICKEY_BYTES;
  uint64_t sklen = SECRETKEY_BYTES;

  SflashV2_keypair( pk , sk );

  char *filename = "hola";
  ASN1encodeKeyPair("sflashv2",pk,pklen,sk,sklen,"clave");

  return 0;
}