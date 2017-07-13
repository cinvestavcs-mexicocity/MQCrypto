#define RAINBOW6440_TEST_ROUND 500
#define RAINBOW6440_SCHEME rainbow_w_64_40
#define RAINBOW6440_L_MSG   64
#define RAINBOW6440_P	31
#define RAINBOW6440_L_SEC	 40

#define RAINBOW6440_SK_BYTES 150512
#define RAINBOW6440_PK_BYTES 57600
#define RAINBOW6440_SIG_BYTES 43
#define RAINBOW6440_HASH_BYTES 24

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

	for(int i=0;i<24;i++) 
		if(h[i] != check[i]) {
			std::cout<<"Sign verify: Fail\n";
			return -100;
		}

	std::cout<<"Sign verify: OK\n";
//	for(int i=0;i<24;i++) printf("%x ",check[i]); printf("\n");
	return 0;

}