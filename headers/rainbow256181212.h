#define RAINBOW256181212_SK_BYTES 23408
#define RAINBOW256181212_PK_BYTES 30240
#define RAINBOW256181212_SIG_BYTES 42
#define RAINBOW256181212_HASH_BYTES 24

#define RAINBOW256181212_TEST_ROUND 1000
#define RAINBOW256181212_GEN_KEY_ROUND 50

#define RAINBOW256181212_GF 256
#define RAINBOW256181212_N 42
#define RAINBOW256181212_M 24
#define RAINBOW256181212_P 0
#define RAINBOW256181212_SCHEME "rainbow 18 12 12"

typedef rainbow<RAINBOW256181212_GF,18,12,12> RAINBOW256181212_PKC;

int rainbow256181212_keypair( unsigned char sk[RAINBOW256181212_SK_BYTES] , unsigned long long * 
sklen , unsigned char pk[RAINBOW256181212_PK_BYTES] , unsigned long long * pklen )
{
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
	pkc.gen_key( pk , sk );
	* sklen = RAINBOW256181212_SK_BYTES;
	* pklen = RAINBOW256181212_PK_BYTES;
	return 0;
}


int rainbow256181212_signature(
  unsigned char sm[RAINBOW256181212_SIG_BYTES],unsigned long long *smlen,
  const unsigned char m[RAINBOW256181212_HASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[RAINBOW256181212_SK_BYTES],const unsigned long long sklen 
)
{
	if( sklen != RAINBOW256181212_SK_BYTES ) return -1;
	if (mlen > RAINBOW256181212_HASH_BYTES) return -1;

	unsigned char h[RAINBOW256181212_M];
	for(int i=0;i<RAINBOW256181212_M;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = RAINBOW256181212_SIG_BYTES;

	gfv<RAINBOW256181212_GF,RAINBOW256181212_N> x;
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y;
	memcpy( &y , h ,RAINBOW256181212_M );
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
//	if( 0 != pkc.private_map( x , sk , y )) return -1;
	pkc.private_map( x , sk , y );
	memcpy( sm , &x , RAINBOW256181212_N );

	return 0;
}

int rainbow256181212_verification(
    const unsigned char m[RAINBOW256181212_HASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[RAINBOW256181212_SIG_BYTES],const unsigned long long smlen,
    const unsigned char pk[RAINBOW256181212_PK_BYTES],const unsigned long long pklen )
{
	if (smlen != RAINBOW256181212_SIG_BYTES) return -100;
	if (mlen != RAINBOW256181212_HASH_BYTES) return -100;
	if( pklen != RAINBOW256181212_PK_BYTES ) return -1;

	unsigned char h[RAINBOW256181212_M];
	for(int i=0;i<RAINBOW256181212_M;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif
	gfv<RAINBOW256181212_GF,RAINBOW256181212_N> x;
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y;
	memcpy( &x , sm ,RAINBOW256181212_N );
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
	pkc.public_map( y , pk , x );

	unsigned char check[RAINBOW256181212_M];
	memcpy( check , &y , RAINBOW256181212_M );
	for(int i=0;i<RAINBOW256181212_M;i++) 
		if(h[i] != check[i]) {
			std::cout<<"Sign verify: Fail\n";
			return -100;
		}

	std::cout<<"Sign verify: OK\n";
	
	return 0;

}