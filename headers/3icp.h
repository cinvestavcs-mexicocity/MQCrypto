#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif

#include <3icp/ThreeIC_p.h>
#include <3icp/conversion.h>
//#include "benchmark.h"

#define TEST_ROUND 500

#define IC3_SCHEME IC3_31_p
#define IC3_POW	18
#define IC3_P 31
#define IC3_L_MSG	 36
#define IC3_L_SEC	 53




/* ========= sizes.h ================ */

#define IC3_SECRETKEY_BYTES 12768
#define IC3_PUBLICKEY_BYTES 35712

#define IC3_SIGNATURE_BYTES 36
#define IC3_SHORTHASH_BYTES 20


/* ========= sizes.h ================ */


int ic3_keypair( unsigned char sk[IC3_SECRETKEY_BYTES] , unsigned long long * sklen , unsigned char pk[IC3_PUBLICKEY_BYTES] , unsigned long long * pklen )
{
	typedef IC3_SCHEME<IC3_POW> pkc;
	pkc::gen_key( pk , sk );
	* sklen = IC3_SECRETKEY_BYTES;
	* pklen = IC3_PUBLICKEY_BYTES;
	return 0;
}


int ic3_signature(
  unsigned char sm[IC3_SIGNATURE_BYTES],unsigned long long *smlen,
  const unsigned char m[IC3_SHORTHASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[IC3_SECRETKEY_BYTES],const unsigned long long sklen )
{
	if( sklen != IC3_SECRETKEY_BYTES ) return -1;
	if (mlen > IC3_SHORTHASH_BYTES) return -1;

	unsigned char h[20];
	for(int i=0;i<20;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = IC3_SIGNATURE_BYTES;

	ic3_VEC<IC3_P,IC3_L_MSG> dig; dig[33] = ic3_gf<IC3_P>::rand(); dig[34] = ic3_gf<IC3_P>::rand(); dig[35] = ic3_gf<IC3_P>::rand();
	ic3_VEC<IC3_P,IC3_L_SEC> sig;

	uint8 b13[13],b7[7];
	ic3_convert_to_31<13>( b13 , *(uint64 *)(&h[0]) ); for(int i=0;i<13;i++) dig[i].v = b13[i];
	ic3_convert_to_31<13>( b13 , *(uint64 *)(&h[8]) ); for(int i=0;i<13;i++) dig[13+i].v = b13[i];
	uint64 qq = (*(uint32 *)(&h[16]));
	ic3_convert_to_31<7>( b7 , qq ); for(int i=0;i<7;i++) dig[26+i].v = b7[i];

	typedef IC3_SCHEME<IC3_POW> pkc;
	pkc::pri_map( &sig , sk , &dig );

	for(int i=0;i<53;i++) sig[i].v += ((sig[i].v<0)? 31:0);
	
	uint8 b54[54] = {0};
	for( int i = 0 ; i < 53 ; i++ ) b54[i] = sig[i].v;

	ic3_convert_from_31<12>( *(uint64 *)(&sm[0]) , b54 );
	ic3_convert_from_31<12>( *(uint64 *)(&sm[8]) , &b54[12] );
	ic3_convert_from_31<12>( *(uint64 *)(&sm[16]) , &b54[24] );
	ic3_convert_from_31<12>( *(uint64 *)(&sm[24]) , &b54[36] );
	ic3_convert_from_31<6>( qq , &b54[48] ); *(uint32 *)(&sm[32]) = (uint32)qq;
	return 0;
}

int ic3_verification(
    const unsigned char m[IC3_SHORTHASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[IC3_SIGNATURE_BYTES],const unsigned long long smlen,
    const unsigned char pk[IC3_PUBLICKEY_BYTES],const unsigned long long pklen )
{
	if (smlen != IC3_SIGNATURE_BYTES) return -100;
	if (mlen != IC3_SHORTHASH_BYTES) return -100;
	if( pklen != IC3_PUBLICKEY_BYTES ) return -1;

	unsigned char h[20];
	for(int i=0;i<20;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	ic3_VEC<IC3_P,IC3_L_MSG> dig;
	ic3_VEC<IC3_P,IC3_L_SEC> sig;

	uint8 b12[12],b6[6];
	ic3_convert_to_31<12>( b12 , *(uint64 *)(&sm[0]) ); for(int i=0;i<12;i++) sig[i].v = b12[i];
	ic3_convert_to_31<12>( b12 , *(uint64 *)(&sm[8]) ); for(int i=0;i<12;i++) sig[12+i].v = b12[i];
	ic3_convert_to_31<12>( b12 , *(uint64 *)(&sm[16]) ); for(int i=0;i<12;i++) sig[24+i].v = b12[i];
	ic3_convert_to_31<12>( b12 , *(uint64 *)(&sm[24]) ); for(int i=0;i<12;i++) sig[36+i].v = b12[i];
	uint64 qq = *(uint32 *)(&sm[32]);
	ic3_convert_to_31<6>( b6 , qq ); for(int i=0;i<5;i++) sig[48+i].v = b6[i];

	typedef IC3_SCHEME<IC3_POW> pkc;
	pkc::pub_map( &dig , pk , &sig );

	for(int i=0;i<36;i++) dig[i].v += ((dig[i].v < 0)? 31: 0);
	uint8 b36[36];
	for( int i = 0 ; i < 36 ; i++ ) b36[i] = dig[i].v;

	uint8 check[24];
	ic3_convert_from_31<13>( *(uint64 *)(&check[0]) , b36 );
	ic3_convert_from_31<13>( *(uint64 *)(&check[8]) , &b36[13] );
	ic3_convert_from_31<7>( *(uint64 *)(&check[16]) , &b36[26] );

	for(int i=0;i<20;i++) 
		if(h[i] != check[i]) 
		{
			std::cout<<"Sign verify: Fail\n";
			return -100;
		}

//	for(int i=0;i<20;i++) printf("%x ",check[i]); printf("\n");

	std::cout<<"Sign verify: OK\n";

	return 0;

}