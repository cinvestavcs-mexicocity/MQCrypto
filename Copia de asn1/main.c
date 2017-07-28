// #include <stdio.h>
// #include <sys/types.h>
// #include <MPKCPublicKey.h>   /* Rectangle ASN.1 type */
// #include <time.h>

// /* Write the encoded output into some FILE stream. */
// static int write_out(const void *buffer, size_t size, void *app_key) {
//     FILE *out_fp = app_key;
//     size_t wrote = fwrite(buffer, 1, size, out_fp);
//     return (wrote == size) ? 0 : -1;
// }

// int encodeKeyPair(char cryptosystem[], 
//   const char *pk, const unsigned long long pklen,
//   const char *sk, const unsigned long long sklen) {
//     MPKCPublicKey_t *PK; /* MPKC Public Key Structure */
//     MPKCPrivateKey_t *SK; /* MPKC Private Key Structure */
//     asn_enc_rval_t ec;      /* Encoder return value  */

//     PK = (MPKCPublicKey_t *)calloc(1, sizeof(MPKCPublicKey_t));
//     if(!PK) {
//       perror("Public Key calloc() failed");
//       exit(1);
//     }

//     SK = (MPKCPrivateKey_t *)calloc(1, sizeof(MPKCPrivateKey_t));
//     if(!SK) {
//       perror("Private Key calloc() failed");
//       exit(1);
//     }

//     if(OCTET_STRING_fromBuf(&PK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
//       perror("Error Buffer");
//     }

//     if(OCTET_STRING_fromBuf(&PK->key, pk, pklen) < 0) {
//       perror("Error Buffer");
//     }

//     if(OCTET_STRING_fromBuf(&SK->version, &cryptosystem[0], strlen(cryptosystem)) < 0) {
//       perror("Error Buffer");
//     }

//     if(OCTET_STRING_fromBuf(&SK->key, sk, sklen) < 0) {
//       perror("Error Buffer");
//     }

//     const char *PKfilename = "PublicKey.txt";
//     const char *SKfilename = "PrivateKey.txt";

//     FILE *fp = fopen(PKfilename, "wb");

//     /* for Public Key BER output */
//     if(!fp) {
//       perror(PKfilename);
//       exit(1);
//     }

//     if(ec.encoded == -1) {
//         fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
//         ec.failed_type ? ec.failed_type->name : "unknown");
//         return -1;
//     } else {
//         fprintf(stderr, "Created %s with BER encoded MPKC Key\n", PKfilename);
//     }

//     ec = der_encode(&asn_DEF_MPKCPublicKey, PK, write_out, fp);
//     fclose(fp);

//     fp = fopen(SKfilename, "wb");

//     /* for Private Key BER output */
//     if(!fp) {
//      perror(SKfilename);
//      exit(1);
//     }

//     // ec = der_encode(&asn_DEF_MPKCPrivateKey, SK, write_out, fp);
//     fclose(fp);

//     if(ec.encoded == -1) {
//        fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
//        ec.failed_type ? ec.failed_type->name : "unknown");
//        return -1;
//     } else {
//        fprintf(stderr, "Created %s with BER encoded MPKC Key\n", SKfilename);
//     }

//     return 0;
// }

// #if 0

// int main(int ac, char **av) {
//   MPKCPublicKey_t *PK; /* MPKC Public Key Structure */
//   asn_enc_rval_t ec;      /* Encoder return value  */

//   srand(time(0));
  
//   // Rectangle_t *rectangle; /* Type to encode        */
//   // asn_enc_rval_t ec;      /* Encoder return value  */

//   PK = calloc(1, sizeof(MPKCPublicKey_t));
//   if(!PK) {
//     perror("calloc() failed");
//     exit(1);
//   }

//   const char rainbow[13] = "Rainbow564040";
//   char c[10];
//   for(uint8_t i = 10; i--; )
//     c[i] = (char)(rand() % 1001099302);

//   if(OCTET_STRING_fromBuf(&PK->version, &rainbow[0], 13) < 0) {
//     perror("Error Buffer");
//   }


//   if(OCTET_STRING_fromBuf(&PK->key, c, 10) < 0) {
//     perror("Error Buffer");
//   }

//   if(ac < 2) {
//     fprintf(stderr, "Specify filename for BER output\n");
//   } else {
//     const char *filename = av[1];

//     FILE *fp = fopen(filename, "wb");

//     /* for BER output */
//     if(!fp) {
//       perror(filename);
//       exit(1);
//     }
    
//     /* Encode the Rectangle type as BER (DER) */
//     ec = der_encode(&asn_DEF_MPKCPublicKey, PK, write_out, fp);
//     fclose(fp);
    
//     if(ec.encoded == -1) {
//       fprintf(stderr, "Could not encode MPKC Key (at %s)\n",
//       ec.failed_type ? ec.failed_type->name : "unknown");
//       exit(1);
//     } else {
//       fprintf(stderr, "Created %s with BER encoded MPKC Key\n", filename);
//     }
//   }
//   // /* Also print the constructed Rectangle XER encoded (XML) */
//   xer_fprint(stdout, &asn_DEF_MPKCPublicKey, PK);
  
//   return 0; /* Encoding finished successfully */
// }

// #endif