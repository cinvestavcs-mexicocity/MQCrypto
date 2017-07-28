#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <tts6440/tts6440.h>
#include <tts6440/tts6440_conversion.h>

#ifdef _USE_SHA1_
#include <openssl/sha.h>
#endif


#define TEST_ROUND 500
#define TTS6440_SCHEME TTS_g_64_40
#define TTS6440_L_MSG   64
#define TTS6440_P	31
#define TTS6440_L_SEC	 40


/// sizes.h
#define TTS6440_SECRETKEY_BYTES 16608
#define TTS6440_PUBLICKEY_BYTES 57600
#define TTS6440_SIGNATURE_BYTES 43
#define TTS6440_SHORTHASH_BYTES 24
/// sizes.h



int tts6440_keypair( unsigned char sk[TTS6440_SECRETKEY_BYTES] , unsigned long long * sklen , unsigned char pk[TTS6440_PUBLICKEY_BYTES] , unsigned long long * pklen )
{
	typedef TTS6440_SCHEME<TTS6440_P> pkc;
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

	tts6440_VEC<TTS6440_P,TTS6440_L_SEC> dig;
	tts6440_VEC<TTS6440_P,TTS6440_L_MSG> sig;

	uint8 b64[64];
	tts6440_convert_to_31<13>( b64 , *(uint64 *)(&h[0]) );
	tts6440_convert_to_31<13>( &b64[13] , *(uint64 *)(&h[8]) );
	tts6440_convert_to_31<13>( &b64[26] , *(uint64 *)(&h[16]) );
	b64[39] = 0;
	for( int i =0;i<40;i++ ) dig[i] = b64[i];


	typedef TTS6440_SCHEME<TTS6440_P> pkc;
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

	tts6440_VEC<TTS6440_P,TTS6440_L_SEC> dig;
	tts6440_VEC<TTS6440_P,TTS6440_L_MSG> sig;

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

	typedef TTS6440_SCHEME<TTS6440_P> pkc;
	pkc::pub_map( &dig , pk , &sig );

	for(int i=0;i<40;i++) dig[i].v += ((dig[i].v < 0)? 31: 0);
	if( dig[39] ) return -100;

	for( int i = 0 ; i < 40 ; i++ ) b64[i] = dig[i].v;

	uint8 check[24];
	tts6440_convert_from_31<13>( *(uint64 *)(&check[0]) , b64 );
	tts6440_convert_from_31<13>( *(uint64 *)(&check[8]) , &b64[13] );
	tts6440_convert_from_31<13>( *(uint64 *)(&check[16]) , &b64[26] );

	for(int i=0;i<24;i++) 
		if(h[i] != check[i]) 
		{
			std::cout<<"Sign verify: Fail\n";	
			return -100;
		}

//	for(int i=0;i<24;i++) printf("%x ",check[i]); printf("\n");

	std::cout<<"Sign verify: OK\n";

	return 0;

}