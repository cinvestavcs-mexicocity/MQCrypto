/*
Usage:  g++ -O3 -msse2 -o HFE-test HFE-test.cpp
*/

/* ============= size.h ================== */
/// 4 HFE
#define HFE_SHORTPLAINTEXT_BYTES 64
#define HFE_ENCRYPTION_BYTES 68
#define HFE_SECRETKEY_BYTES 8480
#define HFE_PUBLICKEY_BYTES 23040
/* ============= size.h ================== */


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "HFE.h"
#include "hfe_conversion.h"
//#include "benchmark.h"

// #include "sodium/crypto_stream_salsa20.h"
#include "sodium.h"
// #include "randombytes.h"
#include "hfe_sizes.h"
// #include "crypto_encrypt.h"

#define TEST_ROUND 500

//#define HFE_SCHEME  HFE_3_31_9
//#define HFE_P 31
//#define HFE_L_MSG  27
//#define HFE_L_SEC  27

#define HFE_SCHEME HFE_4_31_10
#define HFE_P 31
#define HFE_L_MSG  40
#define HFE_L_SEC  40


static const unsigned char nonce[16] ={0}; /* one-time key doesn't need variable nonce */

/* =============  */
//extern "C"
int hfe_keypair(
  unsigned char sk[HFE_SECRETKEY_BYTES],unsigned long long *sklen,
  unsigned char pk[HFE_PUBLICKEY_BYTES],unsigned long long *pklen
)
{
  *sklen = HFE_SECRETKEY_BYTES;
  *pklen = HFE_PUBLICKEY_BYTES;
  typedef HFE_SCHEME<HFE_P> pkc;
  pkc::gen_key( pk , sk );
  return 0;
}

//extern "C"
int hfe_shortdecrypt(
  unsigned char m[HFE_SHORTPLAINTEXT_BYTES],unsigned long long *mlen,
  const unsigned char c[HFE_ENCRYPTION_BYTES],const unsigned long long clen,
  const unsigned char sk[HFE_SECRETKEY_BYTES],const unsigned long long sklen
)
{
  if( HFE_ENCRYPTION_BYTES != clen ||  HFE_SECRETKEY_BYTES != sklen ) return -1;
  *mlen = c[HFE_ENCRYPTION_BYTES-1];
  for(int i=24;i<64;i++) m[i]=c[3+i];
  union {
  uint64 qq;
  uint8 bb[8];
  };
  qq = 0; bb[0]=c[24]; bb[1]=c[25]; bb[2]=c[26];

  uint8 b40[40];
  HFE_VEC<HFE_P,HFE_L_MSG> gf_m;
  HFE_VEC<HFE_P,HFE_L_SEC> gf_c;
  convert_to_31<12>( b40 , *((uint64*)(&c[0])) );
  convert_to_31<12>( &b40[12] , *((uint64*)(&c[8])) );
  convert_to_31<12>( &b40[24] , *((uint64*)(&c[16])) );
  convert_to_31<4>( &b40[36] , qq );
  for(int i=0;i<HFE_L_SEC;i++) gf_c[i]=b40[i];

  typedef HFE_SCHEME<HFE_P> pkc;
  if( 0 != pkc::pri_map( & gf_m , sk , & gf_c )) return -1;

//  gf_m.principle();
  for( int i=0 ; i<HFE_L_MSG ; i++ ) { gf_m[i].v += ((gf_m[i].v<0)?31:0); }

  for( int i=0 ; i<HFE_L_MSG ; i++ ) { b40[i] = gf_m[i].v; }

  convert_from_31<13>( *((uint64*)(&m[0])) , b40 );
  convert_from_31<13>( *((uint64*)(&m[8])) , &b40[13] );
  convert_from_31<13>( *((uint64*)(&m[16])) , &b40[26] );

  // for(int i=0;i<*mlen;i++)
  //   printf("%s ",m[i]);
  // printf("\n");
  // std::cout<<"Message:\n"<<(char *)m<<"\n";

  return 0;
}



extern "C"
int hfe_shortencrypt(
  unsigned char c[HFE_ENCRYPTION_BYTES],unsigned long long *clen,
  const unsigned char m[HFE_SHORTPLAINTEXT_BYTES],const unsigned long long mlen,
  const unsigned char pk[HFE_PUBLICKEY_BYTES],const unsigned long long pklen
)
{
  if( HFE_PUBLICKEY_BYTES != pklen || HFE_SHORTPLAINTEXT_BYTES < mlen ) return -1;
  c[HFE_ENCRYPTION_BYTES-1] = (uint8) mlen;
  *clen = HFE_ENCRYPTION_BYTES;
  for(int i=24;i<64;i++) c[i+3] = m[i];

  uint8 b40[40];

  HFE_VEC<HFE_P,HFE_L_MSG> gf_m;
  HFE_VEC<HFE_P,HFE_L_SEC> gf_c;

  convert_to_31<13>( b40 ,  *((uint64 *)(&m[0])) ); 
  convert_to_31<13>( &b40[13] ,  *((uint64 *)(&m[8])) ); 
  convert_to_31<13>( &b40[26] ,  *((uint64 *)(&m[16])) );
  b40[39] = 0;
  for(unsigned i = 0 ; i < HFE_L_MSG; i++ ) gf_m[i] = b40[i];

  typedef HFE_SCHEME<HFE_P> pkc;
  pkc::pub_map( & gf_c , pk , & gf_m );
//  gf_c.principle();
  for( int i=0 ; i<HFE_L_SEC ; i++ ) { gf_c[i].v += ((gf_c[i].v<0)?31:0); }

  for(int i=0;i<HFE_L_SEC;i++) b40[i] = gf_c[i].v;

  convert_from_31<12>( *((uint64*)(&c[0])) , b40 );
  convert_from_31<12>( *((uint64*)(&c[8])) , &b40[12] );
  convert_from_31<12>( *((uint64*)(&c[16])) , &b40[24] );
  union {
  uint64 qq;
  uint8 bb[8];
  };
  qq = 0;
  convert_from_31<4>( qq , &b40[36] );
  c[24] = bb[0];
  c[25] = bb[1];
  c[26] = bb[2];

  return 0;
}

// int hfe_shortencrypt(
//   unsigned char c[HFE_ENCRYPTION_BYTES],unsigned long long *clen,
//   const unsigned char m[HFE_SHORTPLAINTEXT_BYTES],const unsigned long long mlen,
//   const unsigned char pk[HFE_PUBLICKEY_BYTES],const unsigned long long pklen
// )
// {
//   if( HFE_PUBLICKEY_BYTES != pklen || HFE_SHORTPLAINTEXT_BYTES < mlen ) return -1;
//   c[HFE_ENCRYPTION_BYTES-1] = (uint8) mlen;
//   *clen = HFE_ENCRYPTION_BYTES;
//   for(int i=24;i<mlen;i++) c[i+3] = m[i];

//   uint8 b40[HFE_L_MSG];

//   HFE_VEC<HFE_P,HFE_L_MSG> gf_m;
//   HFE_VEC<HFE_P,HFE_L_SEC> gf_c;

//   for(int i = 0, j = 0; j < HFE_L_MSG - 2; j+=13,i+=8)
//   {
//     convert_to_31<13>( b40 ,  *((uint64 *)(&m[i])) ); 
//     convert_to_31<13>( &b40[j] ,  *((uint64 *)(&m[i])) ); 
//     convert_to_31<13>( &b40[j] ,  *((uint64 *)(&m[i])) );
//   }
//   b40[HFE_L_MSG-2] = 0;
//   b40[HFE_L_MSG-1] = 0;
//   for(unsigned i = 0 ; i < HFE_L_MSG; i++ ) gf_m[i] = b40[i];

//   typedef HFE_SCHEME<HFE_P> pkc;
//   pkc::pub_map( & gf_c , pk , & gf_m );
// //  gf_c.principle();
//   for( int i=0 ; i<HFE_L_SEC ; i++ ) { gf_c[i].v += ((gf_c[i].v<0)?31:0); }

//   for(int i=0;i<HFE_L_SEC;i++) b40[i] = gf_c[i].v;

//   for(int j=0, i=0;j<HFE_L_MSG - 8; i+=8, j+=12)
//   {
//     convert_from_31<12>( *((uint64*)(&c[i])) , b40 );
//     convert_from_31<12>( *((uint64*)(&c[i])) , &b40[j] );
//     convert_from_31<12>( *((uint64*)(&c[i])) , &b40[j] );
//   }
//   union {
//   uint64 qq;
//   uint8 bb[8];
//   };
//   qq = 0;
//   convert_from_31<8>( qq , &b40[72] );
//   c[41] = bb[0];
//   c[42] = bb[1];
//   c[43] = bb[2];

//   return 0;
// }

#ifdef __cplusplus
extern "C"
#endif
int hfe_encrypt(
  unsigned char *c,unsigned long long *clen,
  const unsigned char *m,const unsigned long long mlen,
  const unsigned char *pk
)
{
  unsigned char h[HFE_SHORTPLAINTEXT_BYTES];
  int i;

  if (mlen < HFE_SHORTPLAINTEXT_BYTES) 
    return hfe_shortencrypt(c,clen,m,mlen,pk,HFE_PUBLICKEY_BYTES);
  if (HFE_SHORTPLAINTEXT_BYTES < 32 + 32) 
    return -1;

  randombytes(h,32);
  crypto_stream_salsa20_xor(c + HFE_ENCRYPTION_BYTES - (HFE_SHORTPLAINTEXT_BYTES - 64),m,mlen,nonce,h);
  crypto_hash_sha256(h + 32,c + HFE_ENCRYPTION_BYTES - (HFE_SHORTPLAINTEXT_BYTES - 64),mlen);
  for (i = 0;i < HFE_SHORTPLAINTEXT_BYTES - 64;++i)
    h[i + 64] = c[i + HFE_ENCRYPTION_BYTES - (HFE_SHORTPLAINTEXT_BYTES - 64)];

  if (hfe_shortencrypt(c,clen,h,HFE_SHORTPLAINTEXT_BYTES,pk,HFE_PUBLICKEY_BYTES) < 0)
    return -1;
  if (*clen != HFE_ENCRYPTION_BYTES)
    return -1;
  *clen = mlen + HFE_ENCRYPTION_BYTES - (HFE_SHORTPLAINTEXT_BYTES - 64);

  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
int hfe_decrypt(
  unsigned char *m,unsigned long long *mlen,
  const unsigned char *c,const unsigned long long clen,
  const unsigned char *sk
)
{
  unsigned char h[HFE_SHORTPLAINTEXT_BYTES];
  unsigned long long hlen;
  unsigned char hcheck[32];
  int i;
  unsigned char hchecksum;

  if (clen < HFE_ENCRYPTION_BYTES)
    return -100;
  i = hfe_shortdecrypt(h,&hlen,c,HFE_ENCRYPTION_BYTES,sk,HFE_SECRETKEY_BYTES);
  if (i < 0)
    return i;

  if (hlen < HFE_SHORTPLAINTEXT_BYTES) {
    for (i = 0;i < hlen;++i)
      m[i] = h[i];
    *mlen = hlen;
    for(i = 0; i < *mlen; i++)
      printf("%c",m[i]);
    printf("\n");
    return 0;
  }

  for (i = 64;i < HFE_SHORTPLAINTEXT_BYTES;++i)
    m[i - 64] = h[i];
  for (i = HFE_ENCRYPTION_BYTES;i < clen;++i)
    m[i - HFE_ENCRYPTION_BYTES + HFE_SHORTPLAINTEXT_BYTES - 64] = c[i];

  *mlen = clen - HFE_ENCRYPTION_BYTES + HFE_SHORTPLAINTEXT_BYTES - 64;
  crypto_hash_sha256(hcheck,m,*mlen);
  hchecksum = 0;
  for (i = 0;i < 32;++i)
    hchecksum |= (hcheck[i] ^ h[i + 32]);

  if (hchecksum)
    return -100;

  crypto_stream_salsa20_xor(m,m,*mlen,nonce,h);

  return 0;
}






#if 0

int main()
{
  int messagesize = 5000;
  int ciphersize = 10000;
  srand(time(0));

  uint8 c[ciphersize];
  // uint8 m[HFE_SHORTPLAINTEXT_BYTES] = {0};
  uint8 m[messagesize];
  uint8 cm[messagesize];
  uint64 lcm;
  uint8 pk[HFE_PUBLICKEY_BYTES];
  uint8 sk[HFE_SECRETKEY_BYTES];

  uint64 lc,lm = HFE_SHORTPLAINTEXT_BYTES,lpk,lsk;

  hfe_keypair( sk , &lsk , pk , &lpk );

  for( int i=0;i<messagesize;i++) 
    m[i] = rand() % 256;

  for(int i = 0; i < messagesize; i++) 
    printf("%x ",m[i]);

  hfe_encrypt(c, &lc, m, messagesize, pk);

  // printf("\nCipher Text Length: %d\nCipher Text: ",(int)lc);
  // for(int i = 0; i < lc; i++)
  //  printf("%x ",c[i]);

  hfe_decrypt(cm, &lcm, c, lc, sk);

  printf("\nClear Text Length: %d\nClear text: ",(int)lcm);
  for(int i = 0; i < lcm; i++)
    printf("%x ",cm[i]);

  if(m == cm)
    printf("\nDecrypt: OK");
  else
    printf("\nDecrypt: Fail");

  printf("\n");

  // hfe_shortencrypt( c , & lc , m ,  0 , pk , lpk );
  // for( int i=0;i<HFE_ENCRYPTION_BYTES;i++) 
  //  printf("%x " , (unsigned) c[i]); printf("\n");

  // int q = hfe_shortdecrypt( cm , & lcm , c , lc , sk , lsk );
  // for( int i=0;i<HFE_SHORTPLAINTEXT_BYTES;i++) 
  //  printf("%x " , (unsigned) cm[i]); 
  // printf(" : %d\n" , q);
  // if( 0 != q ) printf("fail!\n");

  // for( int i=0;i<HFE_SHORTPLAINTEXT_BYTES;i++) 
  //  m[i] = rand() % 256;
  
  // for( int i=0;i<HFE_SHORTPLAINTEXT_BYTES;i++) 
  //  printf("%x " , (unsigned) m[i]); 
  // printf("\n");
  // hfe_shortencrypt( c , & lc , m ,  lm , pk , lpk );
  
  // for( int i=0;i<HFE_ENCRYPTION_BYTES;i++) 
  //  printf("%x " , (unsigned) c[i]); 
  // printf("\n");
  // q = hfe_shortdecrypt( cm , & lcm , c , lc , sk , lsk );
  
  // for( int i=0;i<HFE_SHORTPLAINTEXT_BYTES;i++) 
  //  printf("%x " , (unsigned) cm[i]); 
  // printf(": %d\n", q );

  // int err = 0;
  // int _check_err = 0;
  // for( int j = 0 ; j < TEST_ROUND ; j++ ) {
  //  for( int i=0;i<HFE_SHORTPLAINTEXT_BYTES;i++) m[i] = rand() % 256;
  //  hfe_shortencrypt( c , & lc , m ,  lm , pk , lpk );
  //  int q = hfe_shortdecrypt( cm , & lcm , c , lc , sk , lsk );
  //  if( 0 != q ) err ++;
  //  uint8 _check_sum = 0;
  //  for( uint64 k=0;k<lm;k++) _check_sum |= m[k] ^ cm[k];
  //  if( _check_sum ) _check_err ++;
  // }

  // printf("err: %d : %d / %d\n" , err , _check_err , TEST_ROUND );

}

#endif


#if 0
int main(){
  srand(time(0));

  benchmark b_gen;
  benchmark b_pub;
  benchmark b_pri;
  bm_init(&b_gen);
  bm_init(&b_pub);
  bm_init(&b_pri);

  typedef HFE_SCHEME<HFE_P> pkc;

  std::cout << "pub_key:" << pkc::sizeof_pub_key() << "\n" << "pri_key:" << pkc::sizeof_pri_key() << "\n";
  uint8 * pub_key; pub_key = new uint8 [pkc::sizeof_pub_key()];
  uint8 * pri_key; pri_key = new uint8 [pkc::sizeof_pri_key()];

  pkc::gen_key( pub_key , pri_key );

  HFE_VEC<HFE_P, HFE_L_MSG> vec0, vec2;//, vec4;
  HFE_VEC<HFE_P, HFE_L_SEC> vec1;//, vec3;//, vec5;

  uint8 bb1[HFE_L_MSG];
  uint8 bb2[HFE_L_MSG];
  for(unsigned i=0;i<HFE_L_MSG;i++) { bb1[i]=rand()%HFE_P; vec0[i].v=0; }
//  vec0[26] = gf<HFE_P>(0);
  vec0[39] = gf<HFE_P>(0);
  //zero_vec( &vec0 );
  vec0.dump( stdout );

  pkc::pub_map( & vec1 , pub_key , & vec0 );
  vec1.dump( stdout );
  
  int q = pkc::pri_map( & vec2 , pri_key , & vec1 );
  //zero_vec( & vec2 );
  for(unsigned i=0;i<HFE_L_MSG;i++) {
    vec2[i].v += (vec2[i].v<0)?31:0;
    bb2[i]=vec2[i].v;
  }
  vec2.dump( stdout );
  printf("return :%d\n" , q );

  for( int i = 0 ; i < TEST_ROUND ; i++ ) {
//    BENCHMARK(b_gen, pkc::gen_key( pub_key , pri_key ) );
//    BENCHMARK(b_pub, pkc::pub_map( & vec1 , pub_key , & vec0 ) );
//    BENCHMARK(b_pri, pkc::pri_map( & vec2 , pri_key , & vec1 ) );
  }

  char buf[256];
  bm_dump(buf, sizeof(buf), &b_gen); printf ("gen key   : %s\n", buf);
  bm_dump(buf, sizeof(buf), &b_pub); printf ("pub map   : %s\n", buf);
  bm_dump(buf, sizeof(buf), &b_pri); printf ("pri map   : %s\n", buf);

  delete [] pub_key;
  delete [] pri_key;

  return 0;
}

#endif


