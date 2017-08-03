/// usage: g++ -O2 rainbow_w-test.cpp -fno-strict-aliasing

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "tts6440.h"
#include "tts6440_conversion.h"

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif


#define TEST_ROUND 500
#define TTS6440_SCHEME TTS_g_64_40
#define L_MSG   64
#define P	31
#define L_SEC	 40


/// sizes.h
#define TTS6440_SECRETKEY_BYTES 16608
#define TTS6440_PUBLICKEY_BYTES 57600
#define TTS6440_SIGNATURE_BYTES 43
#define TTS6440_SHORTHASH_BYTES 24
/// sizes.h



int tts6440_keypair( unsigned char sk[TTS6440_SECRETKEY_BYTES] , unsigned long long * sklen , unsigned char pk[TTS6440_PUBLICKEY_BYTES] , unsigned long long * pklen )
{
	typedef TTS6440_SCHEME<P> pkc;
	pkc::gen_key( pk , sk );
	* sklen = TTS6440_SECRETKEY_BYTES;
	* pklen = TTS6440_PUBLICKEY_BYTES;
	return 0;
}


int tts6440_signature(
  unsigned char sm[TTS6440_SIGNATURE_BYTES],unsigned long long *smlen,
  const unsigned char m[TTS6440_SHORTHASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[TTS6440_SECRETKEY_BYTES],const unsigned long long sklen )
{
	if( sklen != TTS6440_SECRETKEY_BYTES ) return -1;
	if (mlen > TTS6440_SHORTHASH_BYTES) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = TTS6440_SIGNATURE_BYTES;

	tts6440_VEC<P,L_SEC> dig;
	tts6440_VEC<P,L_MSG> sig;

	uint8 b64[64];
	tts6440_convert_to_31<13>( b64 , *(uint64 *)(&h[0]) );
	tts6440_convert_to_31<13>( &b64[13] , *(uint64 *)(&h[8]) );
	tts6440_convert_to_31<13>( &b64[26] , *(uint64 *)(&h[16]) );
	b64[39] = 0;
	for( int i =0;i<40;i++ ) dig[i] = b64[i];


	typedef TTS6440_SCHEME<P> pkc;
	pkc::pri_map( &sig , sk , &dig );

	for(int i=0;i<64;i++) sig[i].v += ((sig[i].v<0)? 31:0);
	for( int i = 0 ; i < 64 ; i++ ) b64[i] = sig[i].v;

	tts6440_convert_from_31<12>( *(uint64 *)(&sm[0]) , b64 );
	tts6440_convert_from_31<12>( *(uint64 *)(&sm[8]) , &b64[12] );
	tts6440_convert_from_31<12>( *(uint64 *)(&sm[16]) , &b64[24] );
	tts6440_convert_from_31<12>( *(uint64 *)(&sm[24]) , &b64[36] );
	tts6440_convert_from_31<12>( *(uint64 *)(&sm[32]) , &b64[48] );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	tts6440_convert_from_31<4>( qq , &b64[60] );
	sm[40] = b8[0]; sm[41] = b8[1]; sm[42] = b8[2];

	return 0;
}

int tts6440_verification(
    const unsigned char m[TTS6440_SHORTHASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[TTS6440_SIGNATURE_BYTES],const unsigned long long smlen,
    const unsigned char pk[TTS6440_PUBLICKEY_BYTES],const unsigned long long pklen )
{
	if (smlen != TTS6440_SIGNATURE_BYTES) return -100;
	if (mlen != TTS6440_SHORTHASH_BYTES) return -100;
	if( pklen != TTS6440_PUBLICKEY_BYTES ) return -1;

	unsigned char h[24];
	for(int i=0;i<24;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	tts6440_VEC<P,L_SEC> dig;
	tts6440_VEC<P,L_MSG> sig;

	uint8 b64[64];
	tts6440_convert_to_31<12>( b64 , *(uint64 *)(&sm[0]) );
	tts6440_convert_to_31<12>( &b64[12] , *(uint64 *)(&sm[8]) );
	tts6440_convert_to_31<12>( &b64[24] , *(uint64 *)(&sm[16]) );
	tts6440_convert_to_31<12>( &b64[36] , *(uint64 *)(&sm[24]) );
	tts6440_convert_to_31<12>( &b64[48] , *(uint64 *)(&sm[32]) );
	union {
		uint64 qq;
		uint8 b8[8];
	};
	qq = 0;
	b8[0] = sm[40]; b8[1] = sm[41]; b8[2] = sm[42];
	tts6440_convert_to_31<4>( &b64[60] , qq );

	for(int i=0;i<64;i++) sig[i] = b64[i];

	typedef TTS6440_SCHEME<P> pkc;
	pkc::pub_map( &dig , pk , &sig );

	for(int i=0;i<40;i++) dig[i].v += ((dig[i].v < 0)? 31: 0);
	if( dig[39] ) return -100;

	for( int i = 0 ; i < 40 ; i++ ) b64[i] = dig[i].v;

	uint8 check[24];
	tts6440_convert_from_31<13>( *(uint64 *)(&check[0]) , b64 );
	tts6440_convert_from_31<13>( *(uint64 *)(&check[8]) , &b64[13] );
	tts6440_convert_from_31<13>( *(uint64 *)(&check[16]) , &b64[26] );

	for(int i=0;i<24;i++) if(h[i] != check[i]) return -100;

//	for(int i=0;i<24;i++) printf("%x ",check[i]); printf("\n");
	return 0;

}




#if 1



int main()
{
	srand(time(0));

	uint8 c[TTS6440_SIGNATURE_BYTES];
	uint8 m[TTS6440_SHORTHASH_BYTES];
	uint8 pk[TTS6440_PUBLICKEY_BYTES];
	uint8 sk[TTS6440_SECRETKEY_BYTES];

	uint64 lc,lm = TTS6440_SHORTHASH_BYTES,lpk,lsk;

	tts6440_keypair( sk , &lsk , pk , &lpk );

	for( int i=0;i<TTS6440_SHORTHASH_BYTES;i++) m[i] = rand() % 256;

	for( int i=0;i<TTS6440_SHORTHASH_BYTES;i++) printf("%x " , (unsigned) m[i]); printf("\n");

	tts6440_signature( c , &lc , m , lm , sk , lsk );
	for( int i=0;i<TTS6440_SIGNATURE_BYTES;i++) printf("%x " , (unsigned) c[i]); printf("\n");

	int i = tts6440_verification( m , lm , c , lc , pk , lpk );
	printf("verify: %d\n" , i );

}

#endif



#if 0

int main(){
	srand(time(0));

	typedef TTS6440_SCHEME<P> pkc;

	uint8 * pk; pk = new uint8 [pkc::sizeof_pub_key()];
	uint8 * sk; sk = new uint8 [pkc::sizeof_pri_key()];

	printf("pk: %d , sk: %d\n" , (int)pkc::sizeof_pub_key() , (int)pkc::sizeof_pri_key());

	pkc::gen_key( pk , sk );

	tts6440_VEC<31, L_MSG> vec0;//, vec2;//, vec4;
	tts6440_VEC<31, L_SEC> vec1, vec3;//, vec5;

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
