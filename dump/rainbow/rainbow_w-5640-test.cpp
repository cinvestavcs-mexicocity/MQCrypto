/// usage: g++ -O2 rainbow_w-test.cpp -fno-strict-aliasing

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "rainbow_w.h"
#include "conversion.h"

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif


#define RAINBOW5640_TEST_ROUND 500
#define RAINBOW5640_SCHEME rainbow_w_56_40
#define RAINBOW5640_L_MSG   56
#define RAINBOW5640_P	31
#define RAINBOW5640_L_SEC	 40


/// sizes.h
#define RAINBOW5640_SK_BYTES 86240
#define RAINBOW5640_PK_BYTES 44160
#define RAINBOW5640_SIG_BYTES 37
#define RAINBOW5640_HASH_BYTES 102
/// sizes.h




int rainbow5640_keypair( unsigned char sk[RAINBOW5640_SK_BYTES] , unsigned long long * sklen , unsigned char pk[RAINBOW5640_PK_BYTES] , unsigned long long * pklen )
{
	typedef RAINBOW5640_SCHEME<RAINBOW5640_P> pkc;
	pkc::gen_key( pk , sk );
	* sklen = RAINBOW5640_SK_BYTES;
	* pklen = RAINBOW5640_PK_BYTES;
	return 0;
}


int rainbow5640_signature(
  unsigned char sm[RAINBOW5640_SIG_BYTES],unsigned long long *smlen,
  const unsigned char m[RAINBOW5640_HASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[RAINBOW5640_SK_BYTES],const unsigned long long sklen )
{
	if( sklen != RAINBOW5640_SK_BYTES ) return -1;
	if (mlen > RAINBOW5640_HASH_BYTES) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = RAINBOW5640_SIG_BYTES;

	VEC<RAINBOW5640_P,RAINBOW5640_L_SEC> dig;
	VEC<RAINBOW5640_P,RAINBOW5640_L_MSG> sig;

	uint8 b56[56];
	convert_to_31<13>( b56 , *(uint64 *)(&h[0]) );
	convert_to_31<13>( &b56[13] , *(uint64 *)(&h[8]) );
	convert_to_31<13>( &b56[26] , *(uint64 *)(&h[16]) );
	b56[39] = 0;
	for( int i =0;i<40;i++ ) dig[i] = b56[i];


	typedef RAINBOW5640_SCHEME<RAINBOW5640_P> pkc;
	pkc::pri_map( &sig , sk , &dig );

	for(int i=0;i<56;i++) sig[i].v += ((sig[i].v<0)? 31:0);
	for( int i = 0 ; i < 56 ; i++ ) b56[i] = sig[i].v;

	convert_from_31<12>( *(uint64 *)(&sm[0]) , b56 );
	convert_from_31<12>( *(uint64 *)(&sm[8]) , &b56[12] );
	convert_from_31<12>( *(uint64 *)(&sm[16]) , &b56[24] );
	convert_from_31<12>( *(uint64 *)(&sm[24]) , &b56[36] );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	convert_from_31<8>( qq , &b56[48] );
	sm[32] = b8[0]; sm[33] = b8[1]; sm[34] = b8[2]; sm[35] = b8[3]; sm[36] = b8[4];

	return 0;
}

int rainbow5640_verification(
    const unsigned char m[RAINBOW5640_HASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[RAINBOW5640_SIG_BYTES],const unsigned long long smlen,
    const unsigned char pk[RAINBOW5640_PK_BYTES],const unsigned long long pklen )
{
	if (smlen != RAINBOW5640_SIG_BYTES) return -100;
	if (mlen != RAINBOW5640_HASH_BYTES) return -100;
	if( pklen != RAINBOW5640_PK_BYTES ) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	VEC<RAINBOW5640_P,RAINBOW5640_L_SEC> dig;
	VEC<RAINBOW5640_P,RAINBOW5640_L_MSG> sig;

	uint8 b56[56];
	convert_to_31<12>( b56 , *(uint64 *)(&sm[0]) );
	convert_to_31<12>( &b56[12] , *(uint64 *)(&sm[8]) );
	convert_to_31<12>( &b56[24] , *(uint64 *)(&sm[16]) );
	convert_to_31<12>( &b56[36] , *(uint64 *)(&sm[24]) );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	qq = 0;
	b8[0] = sm[32]; b8[1] = sm[33]; b8[2] = sm[34]; b8[3] = sm[35]; b8[4] = sm[36];
	convert_to_31<8>( &b56[48] , qq );

	for(int i=0;i<56;i++) sig[i] = b56[i];

	typedef RAINBOW5640_SCHEME<RAINBOW5640_P> pkc;
	pkc::pub_map( &dig , pk , &sig );

	for(int i=0;i<40;i++) dig[i].v += ((dig[i].v < 0)? 31: 0);
	if( dig[39] ) return -100;

	for( int i = 0 ; i < 40 ; i++ ) b56[i] = dig[i].v;

	uint8 check[24];
	convert_from_31<13>( *(uint64 *)(&check[0]) , b56 );
	convert_from_31<13>( *(uint64 *)(&check[8]) , &b56[13] );
	convert_from_31<13>( *(uint64 *)(&check[16]) , &b56[26] );

	for(int i=0;i<24;i++) if(h[i] != check[i]) return -100;

	printf("Firma verificada.\n");

	// for(int i=0;i<24;i++) printf("%x ",check[i]); printf("\n");
	return 0;

}



#if 1



int main()
{
	srand(time(0));

	uint8 c[RAINBOW5640_SIG_BYTES];
	uint8 m[RAINBOW5640_HASH_BYTES];
	uint8 pk[RAINBOW5640_PK_BYTES];
	uint8 sk[RAINBOW5640_SK_BYTES];

	uint64 lc,lm = RAINBOW5640_HASH_BYTES,lpk,lsk;

	rainbow5640_keypair( sk , &lsk , pk , &lpk );

	// printf("PUBLIC KEY:\n");
	// for( int i=0;i<RAINBOW5640_PK_BYTES;i++) printf("%x ", (unsigned) pk[i]); printf("\n");

	// printf("PRIVATE KEY:\n");
	// for( int i=0;i<RAINBOW5640_PK_BYTES;i++) printf("%x ", (unsigned) sk[i]); printf("\n");

	for( int i=0;i<RAINBOW5640_HASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<RAINBOW5640_HASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	rainbow5640_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<RAINBOW5640_SIG_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = rainbow5640_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );

}

#endif







#if 0
int main(){
	srand(time(0));

	typedef RAINBOW5640_SCHEME<RAINBOW5640_P> pkc;

	uint8 * pk; pk = new uint8 [pkc::sizeof_pub_key()];
	uint8 * sk; sk = new uint8 [pkc::sizeof_pri_key()];

	printf("pk: %d , sk: %d\n" , (int)pkc::sizeof_pub_key() , (int)pkc::sizeof_pri_key());

	pkc::gen_key( pk , sk );

	VEC<31, RAINBOW5640_L_MSG> vec0;//, vec2;//, vec4;
	VEC<31, RAINBOW5640_L_SEC> vec1, vec3;//, vec5;

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

