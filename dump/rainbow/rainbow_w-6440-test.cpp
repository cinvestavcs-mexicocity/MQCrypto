/// usage: g++ -O2 rainbow_w-test.cpp -fno-strict-aliasing

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "rainbow_w.h"
#include "conversion.h"

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif


#define RAINBOW6440_TEST_ROUND 500
#define RAINBOW6440_SCHEME rainbow_w_64_40
#define RAINBOW6440_L_MSG   64
#define RAINBOW6440_P	31
#define RAINBOW6440_L_SEC	 40


/// sizes.h
#define RAINBOW6440_SK_BYTES 150512
#define RAINBOW6440_PK_BYTES 57600
#define RAINBOW6440_SIG_BYTES 43
#define RAINBOW6440_HASH_BYTES 24
/// sizes.h



int rainbow6440_keypair( unsigned char sk[RAINBOW6440_SK_BYTES] , unsigned long long * sklen , unsigned char pk[RAINBOW6440_PK_BYTES] , unsigned long long * pklen )
{
	typedef RAINBOW6440_SCHEME<RAINBOW6440_P> pkc;
	pkc::gen_key( pk , sk );
	* sklen = RAINBOW6440_SK_BYTES;
	* pklen = RAINBOW6440_PK_BYTES;
	return 0;
}


int rainbow6440_signature(
  unsigned char sm[RAINBOW6440_SIG_BYTES],unsigned long long *smlen,
  const unsigned char m[RAINBOW6440_HASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[RAINBOW6440_SK_BYTES],const unsigned long long sklen )
{
	if( sklen != RAINBOW6440_SK_BYTES ) return -1;
	if (mlen > RAINBOW6440_HASH_BYTES) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = RAINBOW6440_SIG_BYTES;

	VEC<RAINBOW6440_P,RAINBOW6440_L_SEC> dig;
	VEC<RAINBOW6440_P,RAINBOW6440_L_MSG> sig;

	uint8 b64[64];
	convert_to_31<13>( b64 , *(uint64 *)(&h[0]) );
	convert_to_31<13>( &b64[13] , *(uint64 *)(&h[8]) );
	convert_to_31<13>( &b64[26] , *(uint64 *)(&h[16]) );
	b64[39] = 0;
	for( int i =0;i<40;i++ ) dig[i] = b64[i];


	typedef RAINBOW6440_SCHEME<RAINBOW6440_P> pkc;
	pkc::pri_map( &sig , sk , &dig );

	for(int i=0;i<64;i++) sig[i].v += ((sig[i].v<0)? 31:0);
	for( int i = 0 ; i < 64 ; i++ ) b64[i] = sig[i].v;

	convert_from_31<12>( *(uint64 *)(&sm[0]) , b64 );
	convert_from_31<12>( *(uint64 *)(&sm[8]) , &b64[12] );
	convert_from_31<12>( *(uint64 *)(&sm[16]) , &b64[24] );
	convert_from_31<12>( *(uint64 *)(&sm[24]) , &b64[36] );
	convert_from_31<12>( *(uint64 *)(&sm[32]) , &b64[48] );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	convert_from_31<4>( qq , &b64[60] );
	sm[40] = b8[0]; sm[41] = b8[1]; sm[42] = b8[2];

	return 0;
}

int rainbow6440_verification(
    const unsigned char m[RAINBOW6440_HASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[RAINBOW6440_SIG_BYTES],const unsigned long long smlen,
    const unsigned char pk[RAINBOW6440_PK_BYTES],const unsigned long long pklen )
{
	if (smlen != RAINBOW6440_SIG_BYTES) return -100;
	if (mlen != RAINBOW6440_HASH_BYTES) return -100;
	if( pklen != RAINBOW6440_PK_BYTES ) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	VEC<RAINBOW6440_P,RAINBOW6440_L_SEC> dig;
	VEC<RAINBOW6440_P,RAINBOW6440_L_MSG> sig;

	uint8 b64[64];
	convert_to_31<12>( b64 , *(uint64 *)(&sm[0]) );
	convert_to_31<12>( &b64[12] , *(uint64 *)(&sm[8]) );
	convert_to_31<12>( &b64[24] , *(uint64 *)(&sm[16]) );
	convert_to_31<12>( &b64[36] , *(uint64 *)(&sm[24]) );
	convert_to_31<12>( &b64[48] , *(uint64 *)(&sm[32]) );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	qq = 0;
	b8[0] = sm[40]; b8[1] = sm[41]; b8[2] = sm[42];
	convert_to_31<4>( &b64[60] , qq );

	for(int i=0;i<64;i++) sig[i] = b64[i];

	typedef RAINBOW6440_SCHEME<RAINBOW6440_P> pkc;
	pkc::pub_map( &dig , pk , &sig );

	for(int i=0;i<40;i++) dig[i].v += ((dig[i].v < 0)? 31: 0);
	if( dig[39] ) return -100;

	for( int i = 0 ; i < 40 ; i++ ) b64[i] = dig[i].v;

	uint8 check[24];
	convert_from_31<13>( *(uint64 *)(&check[0]) , b64 );
	convert_from_31<13>( *(uint64 *)(&check[8]) , &b64[13] );
	convert_from_31<13>( *(uint64 *)(&check[16]) , &b64[26] );

	for(int i=0;i<24;i++) if(h[i] != check[i]) return -100;

//	for(int i=0;i<24;i++) printf("%x ",check[i]); printf("\n");
	return 0;

}




#if 1



int main()
{
	srand(time(0));

	uint8 c[RAINBOW6440_SIG_BYTES];
	uint8 m[RAINBOW6440_HASH_BYTES];
	uint8 pk[RAINBOW6440_PK_BYTES];
	uint8 sk[RAINBOW6440_SK_BYTES];

	uint64 lc,lm = RAINBOW6440_HASH_BYTES,lpk,lsk;

	rainbow6440_keypair( sk , &lsk , pk , &lpk );

	for( int i=0;i<RAINBOW6440_HASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<RAINBOW6440_HASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	rainbow6440_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<RAINBOW6440_SIG_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = rainbow6440_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );

}

#endif



#if 0

int main(){
	srand(time(0));

	typedef RAINBOW6440_SCHEME<RAINBOW6440_P> pkc;

	uint8 * pk; pk = new uint8 [pkc::sizeof_pub_key()];
	uint8 * sk; sk = new uint8 [pkc::sizeof_pri_key()];

	printf("pk: %d , sk: %d\n" , (int)pkc::sizeof_pub_key() , (int)pkc::sizeof_pri_key());

	pkc::gen_key( pk , sk );

	VEC<31, RAINBOW6440_L_MSG> vec0;//, vec2;//, vec4;
	VEC<31, RAINBOW6440_L_SEC> vec1, vec3;//, vec5;

	rand_vec( &vec1 );
	//zero_vec( &vec1 );
	vec1.dump( stdout );
	pkc::pri_map( &vec0 , sk , &vec1 );
	//zero_vec( & vec0 );
	vec0.dump( stdout );
	pkc::pub_map( &vec3 , pk , &vec0 );
	vec3.dump( stdout );


	return 0;
}

#endif
